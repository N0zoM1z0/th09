#!/usr/bin/env python3
"""Cold Type3 callee-visibility controls; no source adoption or exact credit.

Run one named case to avoid recompiling unchanged experiments. Reports retain
source/tool hashes, actual COFF records and complete-owner comparisons. Both
temporary source carriers and compiler objects clean themselves on failure.
"""
from __future__ import annotations

import argparse
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
SOURCE = 'ExAttackUpdateType3.cpp'
CASES = {
    'baseline': (),
    'coordinates': ('GameManagerCoordinates.cpp',),
    'collision': ('PlayerCollisionHelpers.cpp',),
    'float-core': ('Float3Core.inl',),
    'float-ops': ('Float3Core.inl', 'Float3.cpp'),
}


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def inputs_for(case):
    if case not in CASES:
        raise ValueError('unknown Type3 native context')
    # This existing producer also enforces the reviewed maintained-source SHA.
    word = module('context_word_inputs', 'probe-exattack-type3-word-order.py')
    inputs = word.source_inputs()
    def collect(name):
        if name in inputs:
            return
        path = ROOT / 'src' / name
        if path.is_symlink() or not path.resolve().is_relative_to((ROOT / 'src').resolve()):
            raise ValueError('native context source escapes or aliases src')
        data = path.read_bytes()
        inputs[name] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent / include.decode()).as_posix())
    for filename in CASES[case]:
        collect(filename)
    return inputs, word.PROFILE


def run(output, case):
    if case not in CASES:
        raise ValueError('unknown Type3 native context')
    coff = module('context_coff', 'compare-coff-function.py')
    audit = module('context_audit', 'inspect-exattack-type3-schedule.py')
    guard = module('context_guard', 'check-boss-frontiers.py')
    target = coff.verified_target()
    report_path = output / ('type3-context-' + case + '.json.gz')
    if report_path.is_symlink():
        raise ValueError('refusing symlinked context report')
    report_path.unlink(missing_ok=True)
    inputs, profile = inputs_for(case)
    manifest = (ROOT / 'config/match-units.toml').read_bytes()
    unit = tomllib.loads(manifest.decode())['units']['ex-attack-initialize-type3']
    if (unit['source'], unit['target_address'], unit['size'], unit['profile']) != (
            'src/ExAttackInitType3.cpp', 0x00442580, 454, profile):
        raise ValueError('Type3 original-profile reference changed')
    tool_names = (
        'config/tools.lock.toml', 'config/target.toml', 'scripts/compile-probe.sh',
        'scripts/compare-coff-function.py', 'scripts/inspect-exattack-type3-schedule.py',
        'scripts/inspect-exattack-type3.py', 'scripts/inspect-exattack-type01.py',
        'scripts/diagnostic_branch_alignment.py', 'scripts/check-boss-frontiers.py',
        'scripts/probe-exattack-type3-word-order.py', 'scripts/probe-type3-native-context.py')
    tools = {name: (ROOT / name).read_bytes() for name in tool_names}
    build_root = ROOT / 'build' / 'diagnostics'
    build_root.mkdir(parents=True, exist_ok=True)
    with (tempfile.TemporaryDirectory(prefix='type3-' + case + '-', dir=output) as scratch,
          tempfile.TemporaryDirectory(prefix='type3-context-', dir=build_root) as binary):
        source_root = Path(scratch)
        for filename, content in inputs.items():
            path = source_root / filename
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(content)
        source = source_root / SOURCE
        prefix = ''.join('#include "' + name + '"\n' for name in CASES[case])
        source.write_bytes(prefix.encode() + inputs[SOURCE])
        source_digest = hashlib.sha256(source.read_bytes()).hexdigest()
        obj = Path(binary) / 'unit.obj'
        compiled = subprocess.run(
            ['nice', '-n', '10', str(ROOT / 'scripts/compile-probe.sh'),
             str(source), str(obj), *profile], cwd=ROOT, capture_output=True, text=True)
        if compiled.returncode or not obj.is_file():
            raise RuntimeError('Type3 context compiler failed: ' +
                               (compiled.stdout + compiled.stderr)[-1600:])
        diagnostic = audit.context_report(obj)
        raw, rows = coff.object_function(obj, audit.SYMBOL, include_symbol_locations=True)
        try:
            strict = audit.analyze(obj)
            guard.guard_exattack_type3_update(strict)
            frontier = {'passed': True}
        except (ValueError, RuntimeError) as error:
            frontier = {'passed': False, 'reason': str(error)}
        if case == 'baseline' and not frontier['passed']:
            raise ValueError('maintained Type3 source no longer passes its frontier')
        report = {
            'diagnostic_only': True, 'exactness_credit': 'none', 'case': case,
            'source_commit': subprocess.check_output(
                ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
            'target_sha256': hashlib.sha256(target).hexdigest(), 'profile': profile,
            'source_inputs': {name: hashlib.sha256(data).hexdigest()
                              for name, data in inputs.items()},
            'compiled_source_sha256': source_digest,
            'manifest_sha256': hashlib.sha256(manifest).hexdigest(),
            'tool_hashes': {name: hashlib.sha256(data).hexdigest()
                            for name, data in tools.items()},
            'diagnostic': diagnostic, 'production_frontier': frontier,
            'raw_hex': raw.hex(), 'coff': rows, 'compiler_artifacts_retained': False,
        }
    if (inputs_for(case)[0] != inputs or
            (ROOT / 'config/match-units.toml').read_bytes() != manifest or
            any((ROOT / name).read_bytes() != data for name, data in tools.items())):
        raise ValueError('Type3 context inputs changed during compilation')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=tuple(CASES), default='baseline')
    parser.add_argument('--output', type=Path,
                        default=ROOT / '.analysis' / 'type3-native-context')
    args = parser.parse_args()
    output = args.output.resolve()
    if args.output.is_symlink() or not output.is_relative_to((ROOT / '.analysis').resolve()):
        parser.error('output must be below repository .analysis without a symlink')
    output.mkdir(parents=True, exist_ok=True)
    report = run(output, args.case)
    path = output / ('type3-context-' + args.case + '.json.gz')
    with gzip.open(path, 'wt', encoding='utf-8') as stream:
        json.dump(report, stream, indent=2)
        stream.write('\n')
    d = report['diagnostic']
    print(args.case, d['candidate_bytes'], 'bytes;', d['bound_fields'], 'fields;',
          d['overlap_differences'], 'overlap differences;',
          d['missing_target_bytes'], 'missing;', 'frontier', report['production_frontier']['passed'])
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
