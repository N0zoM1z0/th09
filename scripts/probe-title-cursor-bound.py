#!/usr/bin/env python3
"""Cold-compare an unadopted Title cursor loop-bound hypothesis.

The candidate keeps the end value in the actual count parameter storage and
reads that storage at the loop latch. Qualifiers model this tested access form;
they do not establish the original C++ spelling. Complete target-bound audits
and the maintained residual guard run independently. Fewer differing bytes do
not justify adoption when previously equal positions regress.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def candidate_source(source):
    start = source.index('void TitleScreenView::SetCharacterCursorReverse(')
    end = source.index('int TitleScreenView::SetCharacterSettingSprites(', start)
    block = source[start:end]
    intro = ('    i32 vmOffset = index * (i32)sizeof(AnmVmView);\n'
             '    i32 vmStride = stride * (i32)sizeof(AnmVmView);\n'
             '    while (index < end)\n    {')
    replacement = ('    i32 vmOffset;\n    i32 vmStride;\n'
                   '    if (index < count)\n    {\n'
                   '        vmOffset = index * (i32)sizeof(AnmVmView);\n'
                   '        vmStride = stride * (i32)sizeof(AnmVmView);\n'
                   '        const volatile i32 &loopEnd = count;\n'
                   '        do\n        {')
    tail = ('        index += stride;\n        vmOffset += vmStride;\n'
            '    }\n\n    index = start + stride * selectedCharacter;')
    new_tail = tail.replace('    }\n\n',
                            '        } while (index < loopEnd);\n    }\n\n', 1)
    bound = '    i32 end = index + count * stride;'
    if any(block.count(text) != 2 for text in (intro, tail, bound)):
        raise ValueError('maintained cursor source changed; review this experiment')
    block = block.replace(intro, replacement).replace(tail, new_tail)
    block = block.replace(bound, '    count = index + count * stride;')
    return source[:start] + block + source[end:]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path,
                        default=ROOT / '.analysis/title-cursor-bound')
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to((ROOT / '.analysis').resolve()):
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    report_path = output / 'report.json'
    if report_path.is_symlink():
        raise ValueError('refusing symlinked probe report')
    report_path.unlink(missing_ok=True)
    source = (ROOT / 'src/TitleScreen.cpp').read_bytes()
    candidate = candidate_source(source.decode().replace('\r\n', '\n')).encode()
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        unit = tomllib.load(stream)['units']['title-screen-setup-thread']
    if unit['source'] != 'src/TitleScreen.cpp':
        raise ValueError('Title compiler carrier changed')
    audit = module('cursor_bound_audit', 'inspect-title-character-cursors.py')
    frontier = module('cursor_bound_frontier', 'check-boss-frontiers.py')
    report = {'diagnostic_only': True, 'exactness_credit': 'none',
              'source_commit': subprocess.check_output(
                  ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
              'profile': unit['profile'], 'rows': []}
    for name, data in (('baseline', source), ('candidate', candidate)):
        carrier = output / name
        carrier.mkdir(exist_ok=True)
        path, obj = carrier / 'Title.cpp', carrier / 'Title.obj'
        if path.is_symlink() or obj.is_symlink():
            raise ValueError('refusing symlinked probe compiler outputs')
        path.write_bytes(data)
        obj.unlink(missing_ok=True)
        subprocess.run([str(ROOT / 'scripts/compile-probe.sh'), str(path), str(obj),
                        *unit['profile']], cwd=ROOT, capture_output=True,
                       text=True, check=True)
        row = {'name': name, 'source_sha256': hashlib.sha256(data).hexdigest(),
               'owners': {}}
        for variant in ('reverse', 'inactive'):
            result = audit.diagnose(obj, variant)
            owner = {'complete_owner': result}
            try:
                frontier.guard_title_cursor_reverse_inactive(variant, result)
                owner['maintained_frontier'] = 'passed'
            except RuntimeError as exc:
                if name == 'baseline':
                    raise
                owner['maintained_frontier'] = 'rejected'
                owner['rejection'] = str(exc)
            if name == 'candidate':
                before = report['rows'][0]['owners'][variant]['complete_owner']
                old = {int(x, 0) for x in before['difference_offsets']}
                new = {int(x, 0) for x in result['difference_offsets']}
                owner['restored_positions'] = [hex(x) for x in sorted(old - new)]
                owner['regressed_positions'] = [hex(x) for x in sorted(new - old)]
            row['owners'][variant] = owner
            print(f"{name}/{variant}: {result['candidate_bytes']} bytes, "
                  f"{result['overlap_differences']} differences, "
                  f"{result['candidate_blocks']} blocks; "
                  f"frontier={owner['maintained_frontier']}", flush=True)
        report['rows'].append(row)
        report_path.write_text(json.dumps(report, indent=2) + '\n')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
