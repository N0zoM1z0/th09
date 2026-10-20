#!/usr/bin/env python3
"""Cold-reproduce Type3's complete 15 -> 52 -> 4 byte diagnostic candidates.

The private models qualify the actual middle-Z store, transport the tail-X
float through one real volatile word read, and forward the three real collision
queries through a fully defined inline utility. They do not establish retail
types, original source spelling, aliasing intent or portable semantics. The
four-byte candidate changes previously matching bytes and is NOT eligible for
production adoption. This script never changes maintained source or ledgers.

Each serial build uses the existing VC7.1 profile at lowered priority. Only a
compressed report survives; temporary source, object and PDB files self-clean.
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
BASELINE_SHA256 = 'c57cf9b9277f967832c5ab85031a4610716a88cb03fe02e96efa6389cac030a2'
PROFILE = ['/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF', '/Oi',
           '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src']
CASE_NAMES = ('baseline', 'ordered-word', 'forwarded-collision')
sys.path.insert(0, str(ROOT / 'scripts'))

WORD_READER = '''union Type3FloatWord
{
    float value;
    unsigned int bits;
};
typedef char Type3FloatWordSizeIs04[
    (sizeof(Type3FloatWord) == 4 && sizeof(float) == 4) ? 1 : -1];
static __forceinline float ReadType3TailWord(const Float3 &tailHistory)
{
    Type3FloatWord value;
    value.bits = *reinterpret_cast<const volatile unsigned int *>(&tailHistory.x);
    return value.value;
}

'''
COLLISION_FORWARD = '''static __forceinline int CheckType3Position(
    PlayerLifecycleView *player, PlayerPositionView *position,
    PlayerPositionView *halfSize)
{
    return player->CheckBulletCollision(position, halfSize, 0);
}

'''


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def replace_once(text, before, after):
    if text.count(before) != 1:
        raise ValueError('Type3 source shape changed; review recipe: ' + before[:100])
    return text.replace(before, after, 1)


def variants(source):
    entry = 'int __fastcall ExAttackUpdateCallbackType3'
    ordered = replace_once(source, entry, WORD_READER + entry)
    ordered = replace_once(ordered, '    middle.z = 0.0f;',
                           '    static_cast<volatile float &>(middle.z) = 0.0f;')
    ordered = replace_once(
        ordered, 'g_GameManager.InverseTransformPopupX(extra->history10[24].x)',
        'g_GameManager.InverseTransformPopupX(ReadType3TailWord(extra->history10[24]))')
    forwarded = replace_once(ordered, entry, COLLISION_FORWARD + entry)
    receiver = ('reinterpret_cast<PlayerLifecycleView *>'
                '(g_GameManager.sides[record->opponentSide04].player)')
    for position in ('current', 'middle', 'tail'):
        before = (receiver + '->CheckBulletCollision(\n                    &' +
                  position + ',\n                    &collisionSize,\n                    0)')
        after = ('CheckType3Position(' + receiver + ', &' + position +
                 ', &collisionSize)')
        forwarded = replace_once(forwarded, before, after)
    return (('baseline', source), ('ordered-word', ordered),
            ('forwarded-collision', forwarded))


def source_inputs():
    """Copy the real recursive quoted-header closure, never synthetic headers."""
    result = {}

    def collect(name):
        if name in result:
            return
        path = (ROOT / 'src' / name).resolve()
        if not path.is_relative_to((ROOT / 'src').resolve()):
            raise ValueError('project include escapes source tree')
        raw = path.read_bytes()
        result[name] = raw
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', raw, re.M):
            collect((Path(name).parent / include.decode()).as_posix())

    collect(SOURCE)
    if hashlib.sha256(result[SOURCE]).hexdigest() != BASELINE_SHA256:
        raise ValueError('maintained Type3 baseline changed; review this bounded experiment')
    return result


def run(output, selected_case='all'):
    if selected_case not in ('all', *CASE_NAMES):
        raise ValueError('unknown Type3 experiment case')
    coff = module('word_order_coff', 'compare-coff-function.py')
    audit = module('word_order_audit', 'inspect-exattack-type3-schedule.py')
    guard = module('word_order_guard', 'check-boss-frontiers.py')
    target = coff.verified_target()
    # Once the target is attested, invalidate an earlier successful report
    # before source validation or a compiler can fail. A failed run must not
    # leave a stale success at the requested output path.
    report_path = output / 'type3-word-order.json.gz'
    if report_path.is_symlink():
        raise ValueError('refusing symlinked experiment report')
    report_path.unlink(missing_ok=True)
    inputs = source_inputs()
    config = (ROOT / 'config/match-units.toml').read_bytes()
    unit = tomllib.loads(config.decode())['units']['ex-attack-initialize-type3']
    if (unit['source'] != 'src/ExAttackInitType3.cpp' or
            unit['target_address'] != 0x00442580 or unit['size'] != 454 or
            unit['profile'] != PROFILE):
        raise ValueError('reviewed Type3 compiler profile/owner changed')
    tool_paths = ('config/tools.lock.toml', 'scripts/compile-probe.sh',
                  'scripts/compare-coff-function.py',
                  'scripts/inspect-exattack-type3-schedule.py',
                  'scripts/inspect-exattack-type3.py',
                  'scripts/inspect-exattack-type01.py',
                  'scripts/diagnostic_branch_alignment.py',
                  'scripts/check-boss-frontiers.py', Path(__file__).name)
    tools = {}
    for path in tool_paths:
        if path == Path(__file__).name:
            path = 'scripts/' + path
        tools[path] = (ROOT / path).read_bytes()
    report = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'source_commit': subprocess.check_output(
            ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
        'source_inputs': {name: hashlib.sha256(raw).hexdigest()
                          for name, raw in inputs.items()},
        'tool_hashes': {name: hashlib.sha256(raw).hexdigest()
                        for name, raw in tools.items()},
        'manifest_sha256': hashlib.sha256(config).hexdigest(),
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'profile': PROFILE, 'requested_case': selected_case, 'rows': [],
    }
    for name, source in variants(inputs[SOURCE].decode().replace('\r\n', '\n')):
        if selected_case not in ('all', name):
            continue
        with tempfile.TemporaryDirectory(prefix=name + '-', dir=output) as temporary:
            directory = Path(temporary)
            for filename, raw in inputs.items():
                path = directory / filename
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(raw)
            path, obj = directory / SOURCE, directory / 'unit.obj'
            path.write_text(source)
            compiled = subprocess.run(
                ['nice', '-n', '10', str(ROOT / 'scripts/compile-probe.sh'),
                 str(path), str(obj), *PROFILE], cwd=ROOT, capture_output=True, text=True)
            if compiled.returncode or not obj.is_file():
                raise RuntimeError('cold Type3 compiler failed: ' +
                                   (compiled.stdout + compiled.stderr)[-2000:])
            diagnostic = audit.analyze(obj)
            raw, fields = coff.object_function(obj, audit.SYMBOL,
                                               include_symbol_locations=True)
            offsets = {int(x, 16) for x in diagnostic['difference_offsets']}
            introduced = sorted(offsets - guard.TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS)
            try:
                guard.guard_exattack_type3_update(diagnostic)
                verdict = {'passed': True}
            except RuntimeError as error:
                verdict = {'passed': False, 'reason': str(error)}
            if name == 'baseline' and not verdict['passed']:
                raise ValueError('maintained source fails unchanged production frontier')
            row = {
                'name': name, 'source_sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                'raw_hex': raw.hex(), 'coff': fields, 'diagnostic': diagnostic,
                'production_frontier': verdict,
                'previously_equal_bytes_changed': [hex(x) for x in introduced],
            }
            report['rows'].append(row)
        print(name, diagnostic['candidate_bytes'], 'bytes;',
              diagnostic['ordinary_differences'], 'differences;',
              len(introduced), 'previously equal bytes changed;',
              'production frontier', verdict['passed'], flush=True)
    if (inputs != source_inputs() or (ROOT / 'config/match-units.toml').read_bytes() != config
            or any((ROOT / name).read_bytes() != raw for name, raw in tools.items())):
        raise ValueError('experiment inputs changed during cold builds')
    report['compiler_artifacts_retained'] = False
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True,
                        help='directory below .analysis/ for the compact compressed result')
    parser.add_argument('--case', choices=('all', *CASE_NAMES), default='all',
                        help='cold-build only the selected candidate (default: all three)')
    args = parser.parse_args()
    try:
        output = args.output.resolve()
        analysis = (ROOT / '.analysis').resolve()
        if (not output.is_relative_to(analysis) or output == analysis or
                args.output.is_symlink()):
            raise ValueError('output must be a nonsymlink directory below .analysis/')
        output.mkdir(parents=True, exist_ok=True)
        path = output / 'type3-word-order.json.gz'
        if path.is_symlink():
            raise ValueError('refusing symlinked experiment report')
        report = run(output, args.case)
        with gzip.open(path, 'wt') as stream:
            json.dump(report, stream, indent=2)
        return 0
    except (OSError, ValueError, KeyError, ImportError, RuntimeError) as error:
        print('Type3 word-order probe rejected input:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
