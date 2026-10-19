#!/usr/bin/env python3
"""Source-cold TH09 RunEcl variant replay with the *first* lexical include.

When EclManager.cpp starts, EclRunLate.inl includes EclRunState.inl before
RunEcl's later shared-switch BODY include. Replacing only a BODY copy silently
keeps the canonical earlier declaration and invalidates signature experiments.
This tool stages the genuine lexical chain, compiles with its pinned exact ECL
sibling's VC7.1 profile, then executes strictly non-crediting PE/COFF audits.
No modification of maintained game source, target, match ledgers or compiler flags.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / 'src'
MANIFEST = ROOT / 'config/match-units.toml'
FRAGMENTS = ('EclManager.cpp', 'EclRunLate.inl', 'EclRunState.inl',
             'EclRunBullet.inl', 'EclRunRemote.inl',
             'EclRunMovement.inl', 'EclRunControl.inl')


def attested_include_chain(data: dict[str, bytes]) -> None:
    """Require the first DECLARATIONS include and the BODY include to share path."""
    # Normalize only this textual predicate; stage_variants preserves and
    # hashes the original bytes that the compiler will actually consume.
    cpp = data['EclManager.cpp'].decode().replace('\r\n', '\n')
    late = data['EclRunLate.inl'].decode()
    early = '#include "EclRunLate.inl"'
    body = '#include "EclRunState.inl"'
    from_late = '#include "EclRunState.inl"'
    if (cpp.count(early) != 2 or cpp.count(body) != 1 or
            cpp.index(early) >= cpp.index(body) or late.count(from_late) != 1):
        raise ValueError('RunEcl declaration-first include chain changed; probe not valid')
    if ('#define TH09_ECL_RUN_STATE_BODY\n' + body) not in cpp:
        raise ValueError('RunEcl state BODY is no longer a shared-switch include')
    if '#define TH09_ECL_RUN_SHARED_SWITCH' not in cpp:
        raise ValueError('RunEcl multi-fragment switch is not the expected owner')
    # Every quoted include in this declaration path must resolve inside the
    # staged carrier.  A single missing intermediate file silently makes the
    # first declaration pick the old src/ fragment instead of a probe variant.
    for parent, child in (
            ('EclRunLate.inl', 'EclRunState.inl'),
            ('EclRunState.inl', 'EclRunBullet.inl'),
            ('EclRunBullet.inl', 'EclRunRemote.inl'),
            ('EclRunRemote.inl', 'EclRunMovement.inl'),
            ('EclRunMovement.inl', 'EclRunControl.inl')):
        if data[parent].decode().count(f'#include "{child}"') < 1:
            raise ValueError(f'RunEcl first declaration include drifted: {parent} -> {child}')


def stage_variants(destination: Path, *, state: Path | None = None,
                   control: Path | None = None) -> dict[str, str]:
    """Copy the entire first-include path, even when only a helper body changes."""
    sources = {name: SRC / name for name in FRAGMENTS}
    if state is not None:
        sources['EclRunState.inl'] = state
    if control is not None:
        sources['EclRunControl.inl'] = control
    if any(path.is_symlink() for path in sources.values()):
        raise ValueError('refusing symlinked ECL variant source input')
    data = {name: path.read_bytes() for name, path in sources.items()}
    attested_include_chain(data)
    destination.mkdir(parents=True, exist_ok=True)
    manifest = {}
    for name, content in data.items():
        output = destination / name
        if output.is_symlink():
            raise ValueError('refusing to replace symlink in ECL probe carrier')
        output.write_bytes(content)
        if output.read_bytes() != content:
            raise ValueError('ECL variant source carrier copy failed')
        manifest[name] = hashlib.sha256(content).hexdigest()
    # A second, explicit read proves the compiler's *first* relative include
    # resolves locally, rather than to a stale src/EclRunState.inl.
    if any(not (destination / name).is_file() for name in FRAGMENTS):
        raise ValueError('first-include ECL chain was not fully staged')
    return manifest


def pinned_profile() -> list[str]:
    with MANIFEST.open('rb') as stream:
        units = tomllib.load(stream)['units']
    unit = units['ecl-pop-context']
    if unit['source'] != 'src/EclManager.cpp':
        raise ValueError('ECL exact callee has changed its actual carrier')
    profile = unit['profile']
    if not profile or any(not isinstance(flag, str) for flag in profile):
        raise ValueError('ECL original compiler profile unavailable')
    return profile


def command(args: list[str], *, acceptable: tuple[int, ...] = (0,)) -> str:
    out = subprocess.run(args, cwd=ROOT, text=True, capture_output=True)
    if out.returncode not in acceptable:
        raise RuntimeError(f'{args[0]} exit={out.returncode}: '
                           f'{(out.stdout + out.stderr)[-1500:]}')
    return out.stdout


def clear_replay_outputs(destination: Path) -> None:
    outputs = [destination / name for name in (
        'EclManager.obj', 'complete.json', 'replay-diagnostic.json', 'probe-result.json')]
    for output in outputs:
        if output.is_symlink():
            raise ValueError('refusing stale/symlink ECL replay output')
    # A failed compiler must not leave an earlier trial's successful report.
    # Check all destinations before removing any of the previous outputs.
    for output in outputs:
        output.unlink(missing_ok=True)


def replay(destination: Path) -> dict:
    clear_replay_outputs(destination)
    obj = destination / 'EclManager.obj'
    json_path = destination / 'complete.json'
    diagnostic_path = destination / 'replay-diagnostic.json'
    command([str(ROOT/'scripts/compile-probe.sh'),
             str(destination/'EclManager.cpp'), str(obj), *pinned_profile()])
    if not obj.is_file():
        raise ValueError('cold ECL object missing')
    # inspect-ecl-complete intentionally exits 1 on any valid nonexact result.
    audit = subprocess.run([sys.executable, str(ROOT/'scripts/inspect-ecl-complete.py'),
                            str(obj), '--output', str(json_path)],
                           cwd=ROOT, capture_output=True, text=True)
    if audit.returncode not in (0, 1) or not json_path.is_file():
        raise ValueError('original-bound complete ECL owner rejected altered layout: '
                         + (audit.stdout+audit.stderr)[-700:])
    complete = json.loads(json_path.read_text())
    if (not complete.get('diagnostic_only') or
            complete.get('identity_audit_passed') is not True or
            complete.get('physical_bytes') != 15564 or
            complete.get('relocations') != 598):
        raise ValueError('unreviewed RunEcl input identity or physical range')
    # A shortened handler moves later fields while leaving many handler shapes
    # unchanged. Preserve the independently audited shape report even when the
    # strict field-position comparison below will reject this candidate.
    shapes = json.loads(command([sys.executable,
                                 str(ROOT/'scripts/report-ecl-handler-shapes.py'),
                                 '--json', str(obj)]))
    if shapes.get('identity_audit_passed') is not True:
        raise ValueError('ECL handler identity audit did not pass')
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'physical_bytes': complete['physical_bytes'],
        'original_field_count': complete['relocations'],
        'ordinary_differences': complete['ordinary_different_bytes'],
        'field_differences': complete['field_different_bytes'],
        'table_differences': complete['table_different_bytes'],
        'matching_handler_shapes': shapes['matching_handler_shapes'],
        'unique_handlers': shapes['unique_handlers'],
        'mismatched_handlers': [e['opcode'] for e in shapes['mismatches']],
        'handler_mismatches': shapes['mismatches'],
        'candidate_code_bytes': shapes['candidate_code_bytes'],
        'target_code_bytes': shapes['target_code_bytes'],
        'raw_sha256': complete['raw_sha256'],
        'target_owner_sha256': complete['target_owner_sha256'],
        'object_source_binding': complete['object_source_binding'],
    }
    result['field_positions_agree'] = (
        result['field_differences'] == 0 and result['table_differences'] == 0)
    diagnostic_path.write_text(json.dumps(result, indent=2) + '\n')
    if not result['field_positions_agree']:
        raise ValueError(
            'RunEcl field positions changed: '
            f"{result['field_differences']} field bytes, "
            f"{result['table_differences']} table bytes; "
            f"{result['matching_handler_shapes']}/{result['unique_handlers']} "
            f'handler shapes agree; diagnostic saved to {diagnostic_path}')
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--state', type=Path, help='complete variant EclRunState.inl')
    parser.add_argument('--control', type=Path, help='complete variant EclRunControl.inl')
    parser.add_argument('--output', type=Path, required=True,
                        help='an ignored .analysis/ subdirectory for the staged cold compiler TU')
    parser.add_argument('--stage-only', action='store_true',
                        help='check the lexical include tree without invoking the compiler')
    args = parser.parse_args()
    try:
        root = (ROOT / '.analysis').resolve()
        out = args.output.resolve()
        if not out.is_relative_to(root) or out == root:
            raise ValueError('probe output must be inside repository .analysis/')
        if args.output.is_symlink():
            raise ValueError('probe output must not be a symlink')
        clear_replay_outputs(out)
        manifest = stage_variants(out, state=args.state, control=args.control)
        report: dict[str, object] = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'staged_sources_sha256': manifest,
            'first_include': 'EclManager -> Late -> State -> Bullet -> Remote -> Movement -> Control',
        }
        if not args.stage_only:
            report.update(replay(out))
        (out / 'probe-result.json').write_text(json.dumps(report, indent=2)+'\n')
        print(json.dumps(report, indent=2))
        return 0
    except (OSError, KeyError, TypeError, ValueError, RuntimeError, tomllib.TOMLDecodeError) as e:
        print('ECL first-include probe rejected input:', e, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
