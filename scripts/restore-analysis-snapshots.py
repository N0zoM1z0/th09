#!/usr/bin/env python3
"""Restore Git-backed analysis copies removed during reviewed housekeeping.

Default is a preview. --apply restores absent files only,verifying each original
SHA-256 and checkout line endings;existing different files are never overwritten.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def destination(root: Path, relative: str) -> Path:
    path = Path(relative)
    if path.is_absolute() or '..' in path.parts or path.parts[:1] != ('.analysis',):
        raise ValueError('snapshot destination must stay below .analysis')
    result = root / path
    for parent in (result, *result.parents):
        if parent == root:
            break
        if parent.is_symlink():
            raise ValueError('snapshot path contains a symlink')
    return result


def content(root: Path, row: dict) -> bytes:
    oid = row.get('git_blob', '')
    if len(oid) != 40 or any(c not in '0123456789abcdef' for c in oid):
        raise ValueError('invalid Git blob identity')
    raw = subprocess.check_output(['git', 'cat-file', 'blob', oid], cwd=root)
    lines = row.get('crlf_lines', [])
    if lines:
        parts = raw.split(b'\n')
        if (not isinstance(lines, list) or any(type(i) is not int for i in lines)
                or lines != sorted(set(lines)) or any(i < 0 or i >= len(parts)-1 for i in lines)):
            raise ValueError('invalid snapshot line-ending map')
        selected = set(lines)
        raw = b'\n'.join(line + (b'\r' if i in selected else b'') for i, line in enumerate(parts))
    if len(raw) != row['bytes'] or hashlib.sha256(raw).hexdigest() != row['sha256']:
        raise ValueError('Git snapshot bytes disagree with original SHA-256')
    return raw


def restore(root: Path, document: dict, *, prefix: str = '', apply: bool = False) -> dict:
    root = root.resolve(strict=True)
    rows = document.get('copies')
    if not isinstance(rows, list):
        raise ValueError('snapshot manifest requires a copies list')
    if prefix:
        destination(root, prefix)
    pending = []
    existing = 0
    seen = set()
    for row in rows:
        relative = row['path']
        if prefix and relative != prefix.rstrip('/') and not relative.startswith(prefix.rstrip('/')+'/'):
            continue
        path = destination(root, relative)
        if path in seen:
            raise ValueError('duplicate snapshot path')
        seen.add(path)
        raw = content(root, row)
        if path.exists():
            if not path.is_file() or path.read_bytes() != raw:
                raise ValueError('existing snapshot differs;refusing overwrite: '+relative)
            existing += 1
        else:
            pending.append((path, raw))
    # Preflight all rows before creating any output.
    if apply:
        for path, raw in pending:
            path.parent.mkdir(parents=True, exist_ok=True)
            destination(root, path.relative_to(root).as_posix())
            with path.open('xb') as stream:
                stream.write(raw)
    return dict(mode='apply' if apply else 'preview', absent_files=len(pending),
                logical_bytes=sum(len(raw) for _, raw in pending), already_present=existing)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('manifest', type=Path)
    parser.add_argument('--prefix', default='', help='restore only this .analysis packet or file')
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    try:
        print(json.dumps(restore(ROOT, json.loads(args.manifest.read_text()),
                                 prefix=args.prefix, apply=args.apply), indent=2))
        return 0
    except (ValueError, KeyError, OSError, subprocess.CalledProcessError) as error:
        parser.exit(2, str(error)+'\n')


if __name__ == '__main__':
    raise SystemExit(main())
