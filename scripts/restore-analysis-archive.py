#!/usr/bin/env python3
"""Selectively restore SHA-256-verified generated TH09 analysis JSON evidence.

The ignored 2026-11-04 tar.zst stores old large generated snapshots. This
helper never extracts arbitrary tar paths, never overwrites existing content,
and restores only explicit manifest-listed .analysis paths after verifying both
local archive identity and the selected member's original size and SHA-256.

Default mode previews; --apply writes selected absent paths. The archive is
local-only, not a tracked Factory acceptance receipt.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
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


def safe_path(root: Path, relative: str) -> Path:
    path = Path(relative)
    if (path.is_absolute() or '..' in path.parts or path.parts[:1] != ('.analysis',)
            or path.suffix != '.json' or '\x00' in relative):
        raise ValueError('restore target must be a manifest-listed .analysis/*.json path')
    destination = root / path
    if any(item.is_symlink() for item in (destination, *destination.parents) if item != root):
        raise ValueError('refusing symlink in restore path: '+relative)
    return destination


def check_receipt(root: Path) -> tuple[Path, dict[str, dict]]:
    archive, manifest, receipt = (root / path for path in (ARCHIVE, MANIFEST, RECEIPT))
    if any(p.is_symlink() or not p.is_file() for p in (archive, manifest, receipt)):
        raise ValueError('archived evidence package is missing or is a symlink')
    record = json.loads(receipt.read_text(encoding='utf-8'))
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
        safe_path(root, relative)
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


def restore(root: Path, paths: list[str], *, apply: bool = False) -> dict:
    root = root.resolve(strict=True)
    archive, rows = check_receipt(root)
    if apply and not paths:
        raise ValueError('specify one or more --path for selective restore')
    if len(paths) != len(set(paths)):
        raise ValueError('restore paths are duplicated')
    pending: list[tuple[Path, dict]] = []
    already_present = 0
    for relative in paths:
        target = safe_path(root, relative)
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
            safe_path(root, row['path'])
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
                requested=len(paths), absent_files=len(pending),
                bytes_to_restore=sum(row['bytes'] for _, row in pending),
                already_present=already_present, archive=str(ARCHIVE))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--path', action='append', default=[],
                        help='one exact .analysis/file.json path from the manifest; repeatable')
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    try:
        print(json.dumps(restore(ROOT, args.path, apply=args.apply), indent=2))
        return 0
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError, json.JSONDecodeError) as exc:
        parser.exit(2, str(exc)+'\n')


if __name__ == '__main__':
    raise SystemExit(main())
