#!/usr/bin/env python3
"""Selectively restore SHA-256-verified generated TH09 evidence.

The ignored 2026-11-04 tar.zst stores old large generated snapshots. This
helper never extracts arbitrary tar paths, never overwrites existing content,
and restores only explicit manifest-listed paths after verifying both
local archive identity and the selected member's original size and SHA-256.

Default mode previews; --apply writes selected absent paths. The archive is
local-only, not a tracked Factory acceptance receipt. --bundle selects a dated
generated-evidence package containing build objects and analysis JSON/TOML.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BASE = Path('.analysis/handoff-archive-20261104')
ARCHIVE = BASE / 'historical-generated-json.tar.zst'
MANIFEST = BASE / 'historical-generated-json.manifest.json'
RECEIPT = BASE / 'historical-generated-json.receipt.json'


def file_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()


def safe_path(root: Path, relative: str, *, bundle: bool = False,
              prefix: bool = False) -> Path:
    path = Path(relative)
    if (path.is_absolute() or '..' in path.parts or '\x00' in relative
            or path.as_posix() != (relative.rstrip('/') if prefix else relative)):
        raise ValueError('restore target must be a canonical repository-relative path')
    if bundle:
        allowed = (path.parts[:1] == ('build',) and path.suffix == '.obj'
                   or path.parts[:1] == ('.analysis',) and path.suffix in ('.json', '.toml'))
        if prefix:
            allowed = path.parts[:1] in (('build',), ('.analysis',))
        if not allowed:
            raise ValueError('bundle targets must be build/*.obj or .analysis JSON/TOML')
    elif path.parts[:1] != ('.analysis',) or path.suffix != '.json' or prefix:
        raise ValueError('restore target must be a manifest-listed .analysis/*.json path')
    destination = root / path
    if any(item.is_symlink() for item in (destination, *destination.parents) if item != root):
        raise ValueError('refusing symlink in restore path: '+relative)
    return destination


def check_receipt(root: Path, bundle: Path | None = None) -> tuple[Path, dict[str, dict]]:
    if bundle is None:
        paths = (ARCHIVE, MANIFEST, RECEIPT)
    else:
        if (bundle.parent != Path('.analysis')
                or re.fullmatch(r'handoff-archive-\d{8}', bundle.name) is None):
            raise ValueError('bundle must be .analysis/handoff-archive-YYYYMMDD')
        paths = tuple(bundle / name for name in ('generated-evidence.tar.zst',
                                                'manifest.json', 'receipt.json'))
    archive, manifest, receipt = (root / path for path in paths)
    if any(item.is_symlink() for p in (archive, manifest, receipt)
           for item in (p, *p.parents) if item != root):
        raise ValueError('archived evidence package contains a symlink path')
    if any(p.is_symlink() or not p.is_file() for p in (archive, manifest, receipt)):
        raise ValueError('archived evidence package is missing or is a symlink')
    record = json.loads(receipt.read_text(encoding='utf-8'))
    if bundle is not None and record.get('bundle_kind') != 'generated-evidence-v1':
        raise ValueError('unreviewed generated-evidence bundle kind')
    if (file_sha256(archive) != record['archive_sha256']
            or file_sha256(manifest) != record['manifest_sha256']):
        raise ValueError('archive or manifest bytes do not match the cleanup receipt')
    document = json.loads(manifest.read_text(encoding='utf-8'))
    if (record['count_archived_and_removed'] != document['count']
            or record['original_bytes'] != document['original_bytes']):
        raise ValueError('archive count/size disagree with original manifest')
    rows: dict[str, dict] = {}
    total = 0
    for row in document['files']:
        relative = row['path']
        safe_path(root, relative, bundle=bundle is not None)
        if relative in rows:
            raise ValueError('duplicate file in archive manifest: '+relative)
        if (type(row['bytes']) is not int or row['bytes'] < 0
                or not isinstance(row['sha256'], str) or len(row['sha256']) != 64
                or any(c not in '0123456789abcdef' for c in row['sha256'])):
            raise ValueError('invalid original file length/digest in archive manifest')
        rows[relative] = row
        total += row['bytes']
    if total != document['original_bytes']:
        raise ValueError('manifest original logical byte sum differs')
    return archive, rows


def restore(root: Path, paths: list[str], *, apply: bool = False,
            bundle: Path | None = None, prefixes: list[str] | None = None) -> dict:
    root = root.resolve(strict=True)
    archive, rows = check_receipt(root, bundle)
    prefixes = prefixes or []
    if prefixes and bundle is None:
        raise ValueError('--prefix requires an explicit --bundle')
    if apply and not paths and not prefixes:
        raise ValueError('specify one or more --path or --prefix for selective restore')
    if len(paths) != len(set(paths)):
        raise ValueError('restore paths are duplicated')
    selected = set(paths)
    for prefix in prefixes:
        safe_path(root, prefix, bundle=True, prefix=True)
        matches = {p for p in rows if p == prefix.rstrip('/')
                   or p.startswith(prefix.rstrip('/') + '/')}
        if not matches:
            raise ValueError('prefix has no manifest-listed members: '+prefix)
        selected.update(matches)
    pending: list[tuple[Path, dict]] = []
    already_present = 0
    for relative in sorted(selected):
        target = safe_path(root, relative, bundle=bundle is not None)
        if relative not in rows:
            raise ValueError('path is not present in this verified archive: '+relative)
        row = rows[relative]
        if target.exists():
            if not target.is_file() or target.stat().st_size != row['bytes'] or file_sha256(target) != row['sha256']:
                raise ValueError('existing file differs: refusing overwrite: '+relative)
            already_present += 1
        else:
            pending.append((target, row))
    content: list[tuple[Path, dict, bytes]] = []
    if apply:
        # Preflight *every* chosen archived file before creating any output.
        for target, row in pending:
            process = subprocess.run(
                ['tar', '--use-compress-program=zstd', '-xOf', str(archive), row['path']],
                cwd=root, capture_output=True, check=True)
            payload = process.stdout
            if len(payload) != row['bytes'] or hashlib.sha256(payload).hexdigest() != row['sha256']:
                raise ValueError('archived member is not byte-identical: '+row['path'])
            content.append((target, row, payload))
        for target, row, payload in content:
            target.parent.mkdir(parents=True, exist_ok=True)
            safe_path(root, row['path'], bundle=bundle is not None)
            created = False
            try:
                with target.open('xb') as stream:
                    created = True
                    stream.write(payload)
                if 'mtime_ns' in row:
                    os.utime(target, ns=(row['mtime_ns'], row['mtime_ns']))
            except Exception:
                # Never delete a third party's pre-existing file if the
                # exclusive create failed due to a concurrent write.
                if created:
                    target.unlink(missing_ok=True)
                raise
    return dict(mode='apply' if apply else 'preview', archived_files=len(rows),
                requested=len(selected), absent_files=len(pending),
                bytes_to_restore=sum(row['bytes'] for _, row in pending),
                already_present=already_present, archive=archive.relative_to(root).as_posix())


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--path', action='append', default=[],
                        help='one exact manifest-listed path; repeatable')
    parser.add_argument('--apply', action='store_true')
    parser.add_argument('--bundle', type=Path,
                        help='dated .analysis/handoff-archive-YYYYMMDD generated-evidence package')
    parser.add_argument('--prefix', action='append', default=[],
                        help='select a manifest-listed packet/build prefix within --bundle; repeatable')
    args = parser.parse_args()
    try:
        print(json.dumps(restore(ROOT, args.path, apply=args.apply,
                                 bundle=args.bundle, prefixes=args.prefix), indent=2))
        return 0
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError, json.JSONDecodeError) as exc:
        parser.exit(2, str(exc)+'\n')


if __name__ == '__main__':
    raise SystemExit(main())
