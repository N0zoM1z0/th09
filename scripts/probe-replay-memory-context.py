#!/usr/bin/env python3
"""Compare complete SaveReplay with actual Alloc/Free definitions visible.

Diagnostic only: no maintained source, profile or exact ledger is changed.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import re
import struct
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
CASES = ('baseline', 'before', 'after')


def load(name):
    spec = importlib.util.spec_from_file_location(name.replace('-', '_'), ROOT/'scripts'/name)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def source(base, memory, case):
    if case not in CASES:
        raise ValueError('unknown context')
    first = 'void *ZunMemory::Alloc('
    last = 'void *ZunMemory::AddToRegistry('
    if memory.count(first) != 1 or memory.count(last) != 1:
        raise ValueError('memory definition boundaries changed')
    a, b = memory.index(first), memory.index(last)
    if a >= b or memory[a:b].count('void ZunMemory::Free(') != 1:
        raise ValueError('memory definition order changed')
    native = '#include "ZunMemory.hpp"\n#include <stdlib.h>\n' + memory[a:b]
    return native+'\n'+base if case == 'before' else base+'\n'+native if case == 'after' else base


def run(case):
    sys.path.insert(0, str(ROOT/'scripts'))
    coff, audit = load('compare-coff-function.py'), load('inspect-replay-save.py')
    image = coff.verified_target()
    audit.require_original(coff.pe_bytes_at(image, audit.BASE, audit.SIZE))
    inputs = {}
    def collect(name):
        if name in inputs:
            return
        data = (ROOT/name).read_bytes()
        inputs[name] = data
        for inc in re.findall(rb'^\s*#include\s+"([^"\r\n]+)"', data, re.M):
            collect(str(Path(name).parent/inc.decode()))
    collect('src/ReplayInput.cpp')
    collect('src/ZunMemoryRelease.cpp')
    for name in ('config/match-units.toml', 'config/tools.lock.toml',
                 'scripts/probe-replay-memory-context.py', 'scripts/compile-probe.sh',
                 'scripts/compare-coff-function.py', 'scripts/inspect-replay-save.py',
                 'scripts/diagnostic_branch_alignment.py', 'scripts/target_identity.py'):
        inputs[name] = (ROOT/name).read_bytes()
    sha = lambda data: hashlib.sha256(data).hexdigest()
    units = tomllib.loads(inputs['config/match-units.toml'].decode())['units']
    carrier = units['replay-manager-begin-recording-stage']
    if carrier['source'] != 'src/ReplayInput.cpp' or '/EHsc' in carrier['profile']:
        raise ValueError('reviewed no-EH carrier changed')
    candidate = source(inputs['src/ReplayInput.cpp'].decode(), inputs['src/ZunMemoryRelease.cpp'].decode(), case)
    report = dict(case=case, diagnostic_only=True, exactness_credit='none',
                  target_sha256=sha(image), profile=carrier['profile'],
                  source_sha256=sha(candidate.encode()), inputs={n:sha(b) for n,b in inputs.items()})
    build = ROOT/'build/diagnostics'
    build.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='replay-memory-', dir=build) as temporary:
        folder = Path(temporary)
        for name, data in inputs.items():
            if name.startswith('src/'):
                path = folder/name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(data)
        src, obj = folder/'src/ReplayInput.cpp', folder/'Replay.obj'
        src.write_text(candidate)
        compiled = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                   str(src), str(obj), *carrier['profile']], cwd=ROOT,
                                  capture_output=True, text=True)
        if compiled.returncode:
            raise ValueError('compiler failed: '+(compiled.stdout+compiled.stderr)[-2000:])
        raw, rows = coff.object_function(obj, audit.SYMBOL, include_symbol_locations=True)
        report.update(raw_hex=raw.hex(), coff=rows, native=[])
        for name in ('zun-memory-alloc', 'zun-memory-free'):
            unit = units[name]
            references = [row for row in rows if row['symbol'] == unit['symbol']]
            if len(references) != 1 or references[0]['addend'] != 0:
                raise ValueError('missing unique genuine memory call')
            ref = references[0]
            defined = ref['symbol_section'] > 0
            if defined != (case != 'baseline'):
                raise ValueError('actual memory definition visibility differs')
            if case == 'baseline':
                continue
            body, fields = coff.object_function(obj, unit['symbol'], include_symbol_locations=True)
            if len(body) != 19 or len(fields) != 1 or len(unit['relocations']) != 1:
                raise ValueError('native memory extent/census differs')
            row, expected = fields[0], unit['relocations'][0]
            if any(row[k] != expected[k] for k in ('offset','type','symbol')) or row['addend'] != 0:
                raise ValueError('native memory field differs')
            if ref['symbol_section'] != row['owner_section'] or ref['symbol_value'] != 0:
                raise ValueError('caller does not reference actual native body')
            linked = bytearray(body)
            struct.pack_into('<I', linked, row['offset'],
                             (expected['target']-unit['target_address']-row['offset']-4)&0xffffffff)
            target = coff.pe_bytes_at(image, unit['target_address'], unit['size'])
            if linked != target:
                raise ValueError('complete native memory bytes differ')
            report['native'].append(dict(unit=name, bytes=len(body), raw_hex=body.hex(), coff=fields, original_equal=True))
        report['diagnostic'] = audit.diagnose(obj)
        try:
            audit.guard(report['diagnostic'])
            report['frontier_passed'] = True
        except ValueError as error:
            report.update(frontier_passed=False, frontier_error=str(error))
        for name, data in inputs.items():
            if name.startswith('src/') and name != 'src/ReplayInput.cpp' and (folder/name).read_bytes() != data:
                raise ValueError('staged source changed')
        if src.read_text() != candidate:
            raise ValueError('candidate changed')
    if any((ROOT/name).read_bytes() != data for name,data in inputs.items()):
        raise ValueError('live inputs changed')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    output = args.output.resolve()
    if output.exists() or not output.is_relative_to((ROOT/'.analysis').resolve()):
        parser.error('output must be a new file below .analysis')
    report = run(args.case)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(dict(case=args.case, native_bodies=len(report['native']),
                         frontier_passed=report['frontier_passed'], diagnostic=report['diagnostic'])))

if __name__ == '__main__':
    main()
