#!/usr/bin/env python3
"""Cold-compare ReplayMenu with real TitleScreen declarations and native TU.

The bounded original/COFF inspector checks only DrawReplayMenu. Other functions
emitted by the native TU receive no exactness claim. This probe preserves source,
ABI and manifest profile, records actual input/carrier hashes, and removes its
temporary compiler outputs after each completed run.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
SETUP_DECLARATION = '    static void __cdecl TitleSetupThread(void *unused);'
SETUP_DEFINITION = '\nvoid __cdecl TitleScreenView::TitleSetupThread('
MENU_GLOBALS = 'extern AsciiManager g_AsciiManager;'
MENU_DEFINITION = 'int TitleScreenView::DrawReplayMenu()'


def make_contexts(title: str, menu: str, mode: str = 'all') -> dict[str, str]:
    """Use the real owner declarations, retaining the complete menu body."""
    if mode not in ('declarations', 'native', 'all'):
        raise ValueError('unknown TitleScreen context')
    if (title.count(SETUP_DECLARATION) != 1 or
            title.count(SETUP_DEFINITION) != 1 or
            'DrawReplayMenu' in title or
            menu.count(MENU_GLOBALS) != 1 or
            menu.count(MENU_DEFINITION) != 1 or
            '#include "ReplayDataOriginalView.hpp"' not in title or
            '#include "ReplayDataOriginalView.hpp"' not in menu):
        raise ValueError('TitleScreen declaration/body markers changed; review the probe')
    title = title.replace(SETUP_DECLARATION,
                          '    int DrawReplayMenu();\n' + SETUP_DECLARATION, 1)
    boundary = title.index(SETUP_DEFINITION)
    tail = menu[menu.index(MENU_GLOBALS):]
    prefix = '#include <stdio.h>\n#include <string.h>\n'
    result = {'standalone': menu}
    if mode in ('declarations', 'all'):
        result['declarations'] = prefix + title[:boundary] + '\n' + tail
    if mode in ('native', 'all'):
        result['native'] = prefix + title + '\n' + tail
    return result


def checked_output(path: Path, *, root: Path | None = None) -> Path:
    root = ROOT if root is None else root
    result = path.resolve()
    if not result.is_relative_to((root / '.analysis').resolve()):
        raise ValueError('output must be below this repository .analysis directory')
    return result


def source_snapshot() -> dict[str, str]:
    paths = subprocess.check_output([
        'git', 'ls-files', '--cached', '--others', '--exclude-standard', '-z', '--',
        'src', 'config/target.toml', 'config/match-units.toml', 'config/tools.lock.toml',
        'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
        'scripts/diagnostic_branch_alignment.py',
        'scripts/inspect-title-replay-menu-draw.py',
        'scripts/probe-title-replay-menu-context.py'], cwd=ROOT)
    return {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest()
            for name in paths.decode().split('\0')
            if name and (ROOT / name).is_file()}


def load(name: str, filename: str):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def run(output: Path, mode: str = 'all') -> dict:
    output = checked_output(output)
    if os.environ.get('CL') or os.environ.get('_CL_'):
        raise ValueError('ambient CL/_CL_ flags would change the pinned profile')
    coff = load('title_context_coff', 'compare-coff-function.py')
    coff.verified_target()
    audit = load('title_context_audit', 'inspect-title-replay-menu-draw.py')
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    reference = units['title-screen-setup-thread']
    profile = reference['profile']
    if (reference['source'] != 'src/TitleScreen.cpp' or not profile or
            any(not isinstance(flag, str) for flag in profile)):
        raise ValueError('TitleScreen source/profile changed; review the probe')
    before = source_snapshot()
    contexts = make_contexts((ROOT / 'src/TitleScreen.cpp').read_text(),
                             (ROOT / 'src/TitleScreenReplayMenuDraw.cpp').read_text(), mode)
    result = dict(diagnostic_only=True, exactness_credit='none',
                  scope='complete DrawReplayMenu only', profile=profile,
                  source_head=subprocess.check_output(
                      ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
                  target_owner_sha256=audit.ORIGINAL_SHA256,
                  source_sha256=before, contexts={})
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='title-replay-context-', dir=output) as temporary:
        scratch = Path(temporary)
        baseline = None
        for name, text in contexts.items():
            source, obj = scratch / f'{name}.cpp', scratch / f'{name}.obj'
            source.write_text(text)
            invocation = subprocess.run([
                'nice', '-n', '10', str(ROOT / 'scripts/compile-probe.sh'),
                str(source), str(obj), *profile], cwd=ROOT, text=True, capture_output=True)
            if invocation.returncode or not obj.is_file():
                raise ValueError('cold compiler failed: ' +
                                 (invocation.stdout + invocation.stderr)[-1500:])
            raw, fields = coff.object_function(obj, audit.SYMBOL)
            comparison = audit.diagnose(obj)
            if baseline is None:
                baseline = (raw, fields)
            result['contexts'][name] = dict(
                carrier_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                raw_sha256=hashlib.sha256(raw).hexdigest(), fields=fields,
                same_raw_and_fields_as_standalone=(raw, fields) == baseline,
                comparison=comparison)
    if source_snapshot() != before:
        raise ValueError('source/Oracle inputs changed during the native probe')
    (output / 'report.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path,
                        default=ROOT / '.analysis/title-replay-menu-context')
    parser.add_argument('--context', choices=('declarations', 'native', 'all'), default='all')
    args = parser.parse_args()
    try:
        result = run(args.output, args.context)
    except (OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        parser.exit(2, f'TitleScreen context probe rejected: {error}\n')
    for name, row in result['contexts'].items():
        comparison = row['comparison']
        print(f"{name}: {comparison['candidate_bytes']} bytes / "
              f"{comparison['bound_fields']} fields / "
              f"{comparison['overlap_differences']} differences; "
              f"same raw/fields={row['same_raw_and_fields_as_standalone']}; no exact credit")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
