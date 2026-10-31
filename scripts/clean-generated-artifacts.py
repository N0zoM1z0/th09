#!/usr/bin/env python3
"""Safely clear disposable TH09 build caches for a clean handoff.

Run without flags to preview. Pass --apply to remove the wholly ignored
build/ tree plus disposable debug/bytecode caches inside ignored experiments.
Analysis objects are preserved unless --reviewed-objects supplies their hashes.
Preserve .analysis/ candidate source, scripts, disassemblies, JSON evidence,
all tracked files, tools, and the private original target resources/th09.exe.
"""

from __future__ import annotations

import argparse
from dataclasses import dataclass
import hashlib
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
    fingerprints: tuple[tuple[Path, str], ...]

    def summary(self) -> dict[str, int]:
        return {
            'build_files': len(self.build_files),
            'build_bytes': self.build_bytes,
            'analysis_compiler_files': len(self.analysis_files),
            'analysis_compiler_bytes': self.analysis_bytes,
            'total_files': len(self.build_files) + len(self.analysis_files),
            'total_bytes': self.build_bytes + self.analysis_bytes,
        }


def fingerprint(path: Path) -> str:
    if path.is_symlink():
        return 'symlink:' + os.readlink(path)
    if not path.is_file():
        raise ValueError('cache file disappeared or changed type: ' + str(path))
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def safe_parents(root: Path, path: Path) -> None:
    if not path.is_relative_to(root):
        raise ValueError('cache candidate outside repository')
    for parent in path.parents:
        if parent == root:
            break
        if parent.is_symlink():
            raise ValueError('symlink cache parent: ' + str(parent))


def reviewed_objects(root: Path, manifest: Path | None) -> dict[Path, str]:
    if manifest is None:
        return {}
    manifest = manifest if manifest.is_absolute() else root / manifest
    if '..' in manifest.parts:
        raise ValueError('object-review manifest path contains parent traversal')
    safe_parents(root, manifest)
    if manifest.is_symlink():
        raise ValueError('refusing a symlink object-review manifest')
    document = json.loads(manifest.read_text(encoding='utf-8'))
    rows = document.get('objects') if isinstance(document, dict) else None
    if not isinstance(rows, list):
        raise ValueError('object-review manifest requires an objects list')
    result = {}
    for row in rows:
        if not isinstance(row, dict) or not isinstance(row.get('path'), str):
            raise ValueError('invalid reviewed object row')
        relative = Path(row['path'])
        digest = row.get('sha256')
        if (relative.is_absolute() or '..' in relative.parts
                or relative.parts[:1] != ('.analysis',)
                or relative.suffix.lower() != '.obj'
                or not isinstance(digest, str) or len(digest) != 64
                or any(c not in '0123456789abcdef' for c in digest)):
            raise ValueError('invalid reviewed object path or SHA-256')
        item = root / relative
        safe_parents(root, item)
        if item.is_symlink() or item in result or fingerprint(item) != digest:
            raise ValueError('reviewed object is duplicated, changed or a symlink')
        result[item] = digest
    return result


def inspect(root: Path, *, object_review: Path | None = None) -> Plan:
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
    reviewed = reviewed_objects(root, object_review)
    if build.is_dir():
        for directory, dirs, names in os.walk(build, followlinks=False):
            # Never walk out into symlinked directories.
            for d in dirs:
                if (Path(directory) / d).is_symlink():
                    build_files.append(Path(directory) / d)
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
                if item.suffix.lower() == '.obj' and item not in reviewed:
                    continue
                if item.suffix.lower() in COMPILER_SUFFIXES and item.is_file() and not item.is_symlink():
                    analysis_files.append(item)
                    analysis_bytes += item.stat().st_size
    if not set(reviewed).issubset(analysis_files):
        raise ValueError('reviewed object was not selected from the ignored analysis tree')
    fingerprints = tuple((item, fingerprint(item)) for item in sorted(build_files + analysis_files))
    if any(dict(fingerprints)[item] != digest for item, digest in reviewed.items()):
        raise ValueError('reviewed object changed during inspection')
    return Plan(root, tuple(sorted(build_files)), build_bytes,
                tuple(sorted(analysis_files)), analysis_bytes, fingerprints)


def apply(plan: Plan, manifest: Path = MANIFEST) -> dict[str, int]:
    """Delete only the selected artifacts and write a local ignored audit."""
    if (manifest.is_absolute() or '..' in manifest.parts
            or manifest.parts[:1] != ('.analysis',) or manifest.suffix != '.json'):
        raise ValueError('manifest must stay below the ignored .analysis directory')
    root = plan.root
    build = root / 'build'
    analysis = root / '.analysis'
    if build.is_symlink() or analysis.is_symlink():
        raise ValueError('a cache root changed to a symlink after inspection')
    for item, digest in plan.fingerprints:
        safe_parents(root, item)
        if fingerprint(item) != digest:
            raise ValueError('cache file changed after inspection: ' + str(item))
    if inspect(root).build_files != plan.build_files:
        raise ValueError('build tree changed after inspection')
    destination = root / manifest
    safe_parents(root, destination)
    if destination.is_symlink():
        raise ValueError('refusing a symlink cleanup manifest')
    if destination.exists():
        previous = json.loads(destination.read_text(encoding='utf-8'))
        if not isinstance(previous, dict):
            raise ValueError('invalid earlier cleanup manifest')
        runs = previous.get('runs')
        if runs is None:
            runs = [previous]
        if (not isinstance(runs, list)
                or any(not isinstance(run, dict) or not isinstance(run.get('summary'), dict)
                       or any(type(run['summary'].get(key)) is not int or run['summary'][key] < 0
                              for key in plan.summary()) for run in runs)):
            raise ValueError('invalid earlier cleanup manifest')
    else:
        runs = []
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
        'scope': 'ignored build/, analysis debug/bytecode caches and explicitly hash-reviewed objects',
        'summary': plan.summary(),
        'deleted_build_files': [p.relative_to(root).as_posix() for p in plan.build_files],
        'deleted_analysis_compiler_files': [p.relative_to(root).as_posix() for p in plan.analysis_files],
        'reviewed_fingerprints': {p.relative_to(root).as_posix(): digest for p, digest in plan.fingerprints},
        'preserved': [
            'resources/th09.exe', 'config and tracked sources',
            '.analysis probe scripts, candidate sources, JSON/text evidence and IDA databases',
            '.analysis objects absent from the explicit SHA-256 review list',
            '.tools toolchain and installed compilers',
        ],
    }
    destination.parent.mkdir(parents=True, exist_ok=True)
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
    parser.add_argument('--reviewed-objects', type=Path,
                        help='JSON objects list with repository-relative .analysis paths and SHA-256 values')
    args = parser.parse_args()
    # No tracked files are expected in these two generated cache roots.
    tracked = subprocess.check_output(['git', 'ls-files', '--', 'build', '.analysis'], cwd=ROOT, text=True)
    if tracked.strip():
        raise RuntimeError('refusing cleanup because build/ or .analysis/ has tracked files')
    plan = inspect(ROOT, object_review=args.reviewed_objects)
    summary = plan.summary()
    print(json.dumps({'mode': 'apply' if args.apply else 'dry-run', **summary}, indent=2))
    if args.apply:
        apply(plan)
        print('Local audit: ' + str(MANIFEST))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
