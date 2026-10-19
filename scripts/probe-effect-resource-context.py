#!/usr/bin/env python3
"""Cold-compare Effect AddedCallback with the actual GetAnm definition visible.

Run under nice -n 10. All three builds are serial and disposable. This is a
non-crediting source-context experiment: the complete original callback, every
source field and the real callee are checked independently. A shorter callback
is reported with missing bytes, never treated as an exact or partial match.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
GET_ANM_SYMBOL = '?GetAnm@AnmManager@@QAEPAVAnmLoaded@@H@Z'


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def validate_units(units):
    callee = units.get('anm-manager-get-anm', {})
    caller = units.get('effect-manager-update', {})
    if (callee.get('source') != 'src/AnmManagerBackground.cpp' or
            callee.get('symbol') != GET_ANM_SYMBOL or
            callee.get('target_address') != 0x00401660 or
            callee.get('size') != 20 or callee.get('relocations') != [] or
            caller.get('source') != 'src/EffectManager.cpp' or
            not caller.get('profile') or not callee.get('profile')):
        raise ValueError('actual GetAnm source, ABI, extent or compiler profile changed')
    return callee, caller['profile']


def variants(callback_source, background_source):
    entry = 'AnmLoaded *AnmManager::GetAnm(int anmIdx)'
    if background_source.count(entry) != 1:
        raise ValueError('actual GetAnm definition needs review')
    start = background_source.index(entry)
    opening = background_source.index('{', start)
    closing = background_source.index('}', opening)
    definition = background_source[start:closing + 1] + '\n'
    if '{' in background_source[opening + 1:closing]:
        raise ValueError('actual GetAnm body shape needs review')
    callback = 'int EffectManager::AddedCallback('
    if callback_source.count(callback) != 1 or entry in callback_source:
        raise ValueError('Effect callback source partition changed')
    point = callback_source.index(callback)
    return (
        ('baseline', callback_source),
        ('callee-before', callback_source[:point] + definition + '\n' + callback_source[point:]),
        ('callee-after', callback_source + '\n' + definition),
    )


def compare_callback(coff, audit, obj, image):
    from diagnostic_branch_alignment import direct_control_flow
    import capstone

    target = coff.pe_bytes_at(image, audit.BASE, audit.SIZE)
    audit.require_original(target)
    literal = audit.DESTINATIONS['??_C@_09NMANGJFI@etama?4anm?$AA@']
    if coff.pe_bytes_at(image, literal, 10) != b'etama.anm\x00':
        raise ValueError('original Effect ANM filename changed')
    ti, tf, tc = audit.decode_owner(target)
    if len(ti) != 19 or tuple(tf) != audit.TARGET_FIELDS or tc != audit.CALL_ORDER:
        raise ValueError('original Effect callback census changed')
    raw, fields = coff.object_function(obj, audit.SYMBOL)
    linked, bound = audit.bind_candidate(raw, fields)
    ci, cf, cc = audit.decode_owner(linked)
    if (cf != bound or cc != tc or
            [(kind, dest) for _, kind, dest in bound] !=
            [(kind, dest) for _, kind, dest in tf]):
        raise ValueError('Effect source field/call identity or order changed')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    if len(tg) != audit.TARGET_BLOCKS:
        raise ValueError('original Effect callback graph changed')
    differences = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return {
        'target_bytes': len(target), 'candidate_bytes': len(linked),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'bound_fields': len(bound), 'ordered_calls': cc,
        'bindings': fields, 'original_fields': tf,
        'original_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': tg == cg,
        'overlapping_differences': len(differences), 'difference_offsets': differences,
        'missing_target_bytes': max(0, len(target) - len(linked)),
        'extra_candidate_bytes': max(0, len(linked) - len(target)),
        'complete_bytes_equal': linked == target,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'resolved_sha256': hashlib.sha256(linked).hexdigest(),
        'target_sha256': hashlib.sha256(target).hexdigest(),
    }


def run(output):
    coff = module('effect_context_coff', 'compare-coff-function.py')
    audit = module('effect_context_audit', 'inspect-effect-added.py')
    image = coff.verified_target()
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        callee, profile = validate_units(tomllib.load(stream)['units'])
    source_paths = ('src/EffectManager.cpp', 'src/AnmManagerBackground.cpp')
    inputs = {path: (ROOT / path).read_bytes() for path in source_paths}
    sources = [inputs[path].decode().replace('\r\n', '\n') for path in source_paths]
    report = {
        'result': 'incomplete', 'diagnostic_only': True, 'exactness_credit': 'none',
        'source_commit': subprocess.check_output(
            ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
        'source_sha256': {path: hashlib.sha256(data).hexdigest()
                          for path, data in inputs.items()},
        'recipe_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'profile': profile, 'canonical_get_anm_profile': callee['profile'], 'rows': [],
    }
    for name, text in variants(*sources):
        with tempfile.TemporaryDirectory(prefix=name + '-', dir=output) as directory:
            path = Path(directory) / 'EffectManager.cpp'
            obj = Path(directory) / 'EffectManager.obj'
            path.write_text(text)
            source_hash = hashlib.sha256(path.read_bytes()).hexdigest()
            subprocess.run([str(ROOT / 'scripts/compile-probe.sh'), str(path), str(obj),
                            *profile], cwd=ROOT, capture_output=True, text=True, check=True)
            row = {'name': name, 'source_sha256': source_hash,
                   'callback': compare_callback(coff, audit, obj, image)}
            if name == 'baseline':
                # Keep the maintained strict original-extent inspector as an
                # independent baseline check. It is never relaxed for 45 bytes.
                row['strict_baseline'] = audit.diagnose(obj)
            else:
                raw, fields = coff.object_function(obj, callee['symbol'])
                target = coff.pe_bytes_at(image, callee['target_address'], callee['size'])
                if fields or len(raw) != callee['size'] or raw != target:
                    raise ValueError('co-visible actual GetAnm fails complete canonical body')
                row['actual_get_anm'] = {
                    'address': hex(callee['target_address']), 'bytes': len(raw),
                    'fields': 0, 'complete_bytes_equal': True,
                    'sha256': hashlib.sha256(raw).hexdigest(),
                }
            report['rows'].append(row)
        (output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
        result = row['callback']
        print(f"{name}: {result['candidate_bytes']}/{result['target_bytes']} bytes, "
              f"{result['candidate_instructions']} instructions, "
              f"{result['bound_fields']} fields, "
              f"{result['missing_target_bytes']} missing bytes", flush=True)
    report['result'] = 'completed'
    (output / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / '.analysis/effect-resource-context')
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to((ROOT / '.analysis').resolve()):
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    if (output / 'report.json').is_symlink():
        raise ValueError('refusing symlinked experiment report')
    (output / 'report.json').unlink(missing_ok=True)
    run(output)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
