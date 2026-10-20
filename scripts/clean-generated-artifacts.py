#!/usr/bin/env python3
"""Safely clear disposable TH09 build caches for a clean handoff.

Run without flags to preview. Pass --apply to remove the wholly ignored
build/ tree plus compiler binaries inside ignored .analysis/ experiments.
Preserve .analysis/ candidate source, scripts, disassemblies, JSON evidence,
all tracked files, tools, and the private original target resources/th09.exe.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
# .idb is also an IDA database. Never classify it as a disposable compiler
# cache by extension, even though MSVC can use the same suffix.
COMPILER_SUFFIXES = frozenset({'.obj', '.pdb', '.pyc', '.pch', '.ilk'})
MANIFEST = Path('.analysis/handoff-cleanup-20261013/generated-cache-manifest.json')


@dataclass(frozen=True)
class Plan:
    root: Path
    build_files: tuple[Path, ...]
    build_bytes: int
    analysis_files: tuple[Path, ...]
    analysis_bytes: int

    def summary(self) -> dict[str, int]:
        return {
            'build_files': len(self.build_files),
            'build_bytes': self.build_bytes,
            'analysis_compiler_files': len(self.analysis_files),
            'analysis_compiler_bytes': self.analysis_bytes,
            'total_files': len(self.build_files) + len(self.analysis_files),
            'total_bytes': self.build_bytes + self.analysis_bytes,
        }


def inspect(root: Path) -> Plan:
    """Only inspect explicitly ignored cache roots; do not follow symlinks."""
    root = root.resolve(strict=True)
    build = root / 'build'
    analysis = root / '.analysis'
    for candidate in (build, analysis):
        if candidate.is_symlink():
            raise ValueError('refusing to traverse symlink cache root: ' + str(candidate))
        if candidate.exists() and not candidate.is_dir():
            raise ValueError('cache root is not a directory: ' + str(candidate))
    build_files: list[Path] = []
    analysis_files: list[Path] = []
    build_bytes = analysis_bytes = 0
    if build.is_dir():
        for directory, dirs, names in os.walk(build, followlinks=False):
            # Never walk out into symlinked directories.
            dirs[:] = [d for d in dirs if not (Path(directory) / d).is_symlink()]
            for name in names:
                item = Path(directory) / name
                if item.is_symlink():
                    # A symlink is discarded by rmtree, not its destination.
                    build_files.append(item)
                    continue
                if item.is_file():
                    build_files.append(item)
                    build_bytes += item.stat().st_size
    if analysis.is_dir():
        for directory, dirs, names in os.walk(analysis, followlinks=False):
            dirs[:] = [d for d in dirs if not (Path(directory) / d).is_symlink()]
            for name in names:
                item = Path(directory) / name
                if item.suffix.lower() in COMPILER_SUFFIXES and item.is_file() and not item.is_symlink():
                    analysis_files.append(item)
                    analysis_bytes += item.stat().st_size
    return Plan(root, tuple(sorted(build_files)), build_bytes,
                tuple(sorted(analysis_files)), analysis_bytes)


def apply(plan: Plan, manifest: Path = MANIFEST) -> dict[str, int]:
    """Delete only the selected artifacts and write a local ignored audit."""
    if manifest.is_absolute() or '..' in manifest.parts or manifest.parts[:1] != ('.analysis',):
        raise ValueError('manifest must stay below the ignored .analysis directory')
    root = plan.root
    build = root / 'build'
    analysis = root / '.analysis'
    if build.is_symlink() or analysis.is_symlink():
        raise ValueError('a cache root changed to a symlink after inspection')
    for item in plan.analysis_files:
        if item.is_symlink() or not item.is_relative_to(analysis):
            raise ValueError('analysis candidate outside safe cache root')
        if item.exists():
            item.unlink()
    if build.is_dir():
        shutil.rmtree(build)
    # Bytecode-only directories are also disposable, but an experiment's
    # source and results directory itself is retained for reproducibility.
    for parent in sorted(analysis.rglob('__pycache__'), key=lambda p: len(p.parts), reverse=True) if analysis.is_dir() else []:
        if parent.is_dir() and not parent.is_symlink() and not list(parent.iterdir()):
            parent.rmdir()
    record = {
        'scope': 'ignored build/ plus ignored .analysis compiler binaries only',
        'summary': plan.summary(),
        'deleted_build_files': [p.relative_to(root).as_posix() for p in plan.build_files],
        'deleted_analysis_compiler_files': [p.relative_to(root).as_posix() for p in plan.analysis_files],
        'preserved': [
            'resources/th09.exe', 'config and tracked sources',
            '.analysis probe scripts, candidate sources, JSON/text evidence and IDA databases',
            '.tools toolchain and installed compilers',
        ],
    }
    destination = root / manifest
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.is_symlink():
        raise ValueError('refusing a symlink cleanup manifest')
    if destination.exists():
        previous = json.loads(destination.read_text(encoding='utf-8'))
        runs = previous.get('runs')
        if runs is None:
            # Upgrade the first-run legacy manifest without losing the
            # previously recorded full path inventory.
            runs = [previous]
        if not isinstance(runs, list):
            raise ValueError('invalid earlier cleanup manifest')
    else:
        runs = []
    if plan.summary()['total_files'] == 0 and runs:
        return plan.summary()  # Preserve prior audit on a no-op rerun.
    runs.append(record)
    keys = tuple(plan.summary())
    aggregate = {key: sum(run['summary'][key] for run in runs) for key in keys}
    result = {
        'schema_version': 1,
        'scope': record['scope'],
        'summary': aggregate,
        'runs': runs,
        'preserved': record['preserved'],
    }
    destination.write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    return plan.summary()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true',
                        help='actually delete the reviewed generated caches (default: dry run)')
    args = parser.parse_args()
    # No tracked files are expected in these two generated cache roots.
    tracked = subprocess.check_output(['git', 'ls-files', '--', 'build', '.analysis'], cwd=ROOT, text=True)
    if tracked.strip():
        raise RuntimeError('refusing cleanup because build/ or .analysis/ has tracked files')
    plan = inspect(ROOT)
    summary = plan.summary()
    print(json.dumps({'mode': 'apply' if args.apply else 'dry-run', **summary}, indent=2))
    if args.apply:
        apply(plan)
        print('Local audit: ' + str(MANIFEST))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
