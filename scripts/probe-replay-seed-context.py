#!/usr/bin/env python3
"""Cold-test real Replay RNG definition visibility, with no exactness credit.

Use the maintained setter alone or the complete ReplayRuntimeLeaves source,
before/after the actual ReplayInput carrier. A separate typed configuration
copy tests the adjacent copy/seed boundary. No signature, compiler profile or
game source is changed. Every invocation builds one case serially at nice10;
temporary source/OBJ/PDB files are removed even when the comparison rejects it.
"""
from __future__ import annotations

import argparse
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
CASES = ('baseline', 'seed-before', 'seed-after', 'leaves-before',
         'leaves-after', 'config-assignment', 'config-assignment-seed',
         'config-assignment-leaves')
NATIVE_UNITS = (
    ('replay-rng-get-seed', 0x0041A89E, 4),
    ('replay-rng-set-seed', 0x00420160, 14),
    ('replay-rng-reset-generation-count', 0x00420170, 8),
    ('supervisor-replay-is-speedhack-detected', 0x00420180, 13),
)
COPY = 'memcpy(g_ReplayConfigSnapshot, &replayManager->replayData->configSnapshot, 0xCC);'
TYPED_COPY = ('*reinterpret_cast<GameConfiguration *>(g_ReplayConfigSnapshot) = '
              'replayManager->replayData->configSnapshot;')
PRODUCER_INPUTS = (
    'scripts/probe-replay-seed-context.py', 'scripts/compile-probe.sh',
    'scripts/compare-coff-function.py', 'scripts/target_identity.py',
    'scripts/inspect-replay-begin-playback.py',
    'scripts/diagnostic_field_alignment.py',
    'scripts/diagnostic_branch_alignment.py', 'config/tools.lock.toml',
)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def module(filename: str):
    spec = importlib.util.spec_from_file_location(
        'replay_seed_' + filename.replace('-', '_'), ROOT/'scripts'/filename)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def source_inputs(root: Path = ROOT) -> dict[str, bytes]:
    """Freeze the real first-include paths, retaining their exact raw bytes."""
    result = {}

    def visit(name):
        if name in result:
            return
        path = root/'src'/name
        if (path.is_symlink() or not path.is_file()
                or not path.resolve().is_relative_to((root/'src').resolve())):
            raise ValueError('unreviewed project include: ' + name)
        data = path.read_bytes()
        result[name] = data
        for include in re.findall(rb'^\s*#include\s+"([^"\r\n]+)"', data, re.M):
            visit(str(Path(name).parent/include.decode()))

    visit('ReplayInput.cpp')
    visit('ReplayRuntimeLeaves.cpp')
    return result


def case_source(case: str, inputs: dict[str, bytes]) -> tuple[bytes, tuple]:
    if case not in CASES:
        raise ValueError('unknown Replay seed context')
    base = inputs['ReplayInput.cpp'].decode().replace('\r\n', '\n')
    leaves = inputs['ReplayRuntimeLeaves.cpp'].decode().replace('\r\n', '\n')
    start = 'void ReplayRngView::SetSeed('
    end = 'void ReplayRngView::ResetGenerationCount('
    if leaves.count(start) != 1 or leaves.count(end) != 1:
        raise ValueError('maintained RNG definition boundaries changed')
    a, b = leaves.index(start), leaves.index(end)
    if a >= b:
        raise ValueError('maintained RNG definition order changed')
    setter = '#include "ReplayRuntimeLeaves.hpp"\n\n' + leaves[a:b]
    if case.startswith('config-assignment'):
        if base.count(COPY) != 1:
            raise ValueError('reviewed configuration copy changed')
        base = base.replace(COPY, TYPED_COPY)
    native = (NATIVE_UNITS if 'leaves' in case else
              (NATIVE_UNITS[1],) if 'seed' in case else ())
    extra = leaves if 'leaves' in case else setter if 'seed' in case else ''
    if case.endswith('-after'):
        base += '\n' + extra
    elif extra:
        base = extra + '\n' + base
    return base.encode(), native


def native_definition(raw: bytes, rows: list, target: bytes, unit: dict,
                      expected_address: int, expected_size: int) -> dict:
    if (unit.get('target_address') != expected_address
            or unit.get('size') != expected_size or unit.get('relocations') != []
            or len(target) != expected_size or len(raw) != expected_size
            or rows or raw != target):
        raise ValueError('complete original RNG/runtime definition differs')
    return dict(physical_bytes=len(raw), fields=0, original_equal=True,
                target_address=expected_address, original_sha256=digest(target),
                raw_sha256=digest(raw))


def seed_call(rows: list[dict], symbol: str, *, defined: bool) -> dict:
    calls = [row for row in rows if row.get('symbol') == symbol]
    if len(calls) != 1:
        raise ValueError('playback lacks one genuine SetSeed reference')
    row = calls[0]
    section, owner = row.get('symbol_section'), row.get('owner_section')
    if (row.get('type') != 'REL32' or row.get('addend') != 0
            or type(section) is not int or type(owner) is not int
            or row.get('symbol_value') != 0 or owner <= 0
            or (defined and (section <= 0 or section == owner))
            or (not defined and section != 0)):
        raise ValueError('SetSeed call lacks its actual expected definition')
    return dict(row, same_object_definition=defined)


def run(case: str, output: Path) -> dict:
    if case not in CASES:
        raise ValueError('unknown Replay seed context')
    output = output.resolve()
    if not output.is_relative_to((ROOT/'.analysis').resolve()):
        raise ValueError('diagnostic output must be below .analysis')
    output.mkdir(parents=True, exist_ok=True)
    report_path = output/(case+'.json')
    if report_path.is_symlink():
        raise ValueError('refusing symlinked Replay diagnostic')
    report_path.unlink(missing_ok=True)
    sys.path.insert(0, str(ROOT/'scripts'))
    coff = module('compare-coff-function.py')
    audit = module('inspect-replay-begin-playback.py')
    image = coff.verified_target()
    audit.validate_original(coff.pe_bytes_at(image, audit.BASE, audit.SIZE))
    manifest_bytes = (ROOT/'config/match-units.toml').read_bytes()
    units = tomllib.loads(manifest_bytes.decode())['units']
    carrier = units['replay-manager-begin-recording-stage']
    if carrier['source'] != 'src/ReplayInput.cpp' or '/EHsc' in carrier['profile']:
        raise ValueError('original no-EH Replay carrier changed')
    inputs = source_inputs()
    source, native = case_source(case, inputs)
    hashes = {name: digest(data) for name, data in inputs.items()}
    producer_hashes = {name: digest((ROOT/name).read_bytes()) for name in PRODUCER_INPUTS}
    report = dict(diagnostic_only=True, exactness_credit='none', case=case,
                  source_head=subprocess.check_output(
                      ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
                  target_sha256=digest(image), source_inputs=hashes,
                  source_sha256=digest(source), profile=carrier['profile'],
                  match_manifest_sha256=digest(manifest_bytes),
                  producer_inputs=producer_hashes)
    with tempfile.TemporaryDirectory(prefix='replay-seed-', dir=output) as temporary:
        folder = Path(temporary)
        for name, data in inputs.items():
            path = folder/name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        (folder/'ReplayInput.cpp').write_bytes(source)
        obj = folder/'ReplayInput.obj'
        compiled = subprocess.run(
            ['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
             str(folder/'ReplayInput.cpp'), str(obj), *carrier['profile']],
            cwd=ROOT, capture_output=True, text=True)
        report['compile_returncode'] = compiled.returncode
        (output/(case+'-compile.log')).write_text(compiled.stdout+compiled.stderr)
        if compiled.returncode or not obj.is_file():
            raise ValueError('native Replay context compilation failed')
        raw, rows = coff.object_function(obj, audit.SYMBOL, include_symbol_locations=True)
        report['playback'] = audit.diagnose(obj)
        report['caller_raw_hex'] = raw.hex()
        report['caller_coff'] = rows
        report['native_definitions'] = {}
        for name, address, size in native:
            unit = units[name]
            body, fields = coff.object_function(obj, unit['symbol'])
            report['native_definitions'][name] = native_definition(
                body, fields, coff.pe_bytes_at(image, address, size),
                unit, address, size)
        report['actual_seed_call'] = seed_call(
            rows, units['replay-rng-set-seed']['symbol'], defined=bool(native))
    if (source_inputs() != inputs
            or (ROOT/'config/match-units.toml').read_bytes() != manifest_bytes
            or {name: digest((ROOT/name).read_bytes()) for name in PRODUCER_INPUTS}
            != producer_hashes):
        raise ValueError('Replay source/profile changed during comparison')
    report['temporary_outputs_removed'] = not folder.exists()
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='seed-before')
    parser.add_argument('--output', type=Path,
                        default=ROOT/'.analysis/replay-seed-context')
    args = parser.parse_args()
    try:
        report = run(args.case, args.output)
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        print('Replay seed context rejected:', error, file=sys.stderr)
        return 1
    current = report['playback']
    print(json.dumps(dict(case=args.case, exactness_credit='none',
                          candidate_bytes=current['candidate_bytes'],
                          overlap_differences=current['complete_overlap_differences'],
                          fields=current['bound_fields'],
                          native_definitions=report['native_definitions'],
                          temporary_outputs_removed=report['temporary_outputs_removed']),
                     indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
