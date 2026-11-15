#!/usr/bin/env python3
"""Ratchet check for source-present functions that are not exact yet.

Each entry in config/frontiers.toml names a function, its source and
pinned VC7.1 profile, the original extent (address, size, SHA-256) and the
set of byte offsets that currently differ from the original after
relocation fields are masked.  The check rebuilds the function and fails
if any byte that matched before now differs, or if the size changes.
A strictly smaller residual is reported as an improvement; record it with
--update.  This is diagnostic only: exact credit still requires a
canonical unit checked by build-match-unit.py and compare-coff-function.py.

Usage:
  scripts/check-frontier.py                 check every frontier
  scripts/check-frontier.py replay-save     check selected frontiers
  scripts/check-frontier.py --update NAME   record an improved residual
  scripts/check-frontier.py --show NAME     also print differing instructions
"""
from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import importlib.util
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / 'config/frontiers.toml'


def load_coff_tools():
    spec = importlib.util.spec_from_file_location(
        'compare_coff_function', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def parse_offsets(items: list[str]) -> set[int]:
    offsets: set[int] = set()
    for item in items:
        low, _, high = item.partition('-')
        offsets.update(range(int(low, 16), int(high or low, 16) + 1))
    return offsets


def format_offsets(offsets: set[int]) -> list[str]:
    runs: list[str] = []
    ordered = sorted(offsets)
    start = previous = None
    for value in ordered + [None]:
        if start is not None and value == previous + 1:
            previous = value
            continue
        if start is not None:
            runs.append(f'{start:X}' if start == previous else f'{start:X}-{previous:X}')
        start = previous = value
    return runs


def build(name: str, entry: dict) -> Path:
    obj = ROOT / 'build/frontier' / f'{name}.obj'
    obj.parent.mkdir(parents=True, exist_ok=True)
    result = subprocess.run(
        ['nice', '-n', '12', 'scripts/compile-probe.sh', entry['source'], str(obj), *entry['profile']],
        cwd=ROOT, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f'{name}: compile failed\n{result.stdout[-2000:]}{result.stderr[-1000:]}')
    return obj


def measure(coff, image: bytes, entry: dict, obj: Path) -> tuple[int, set[int], bytes, bytes, list]:
    address, size = entry['address'], entry['size']
    original = coff.pe_bytes_at(image, address, size)
    digest = hashlib.sha256(original).hexdigest()
    if digest != entry['target_sha256']:
        raise RuntimeError(f'original extent hash changed: {digest}')
    candidate, relocations = coff.object_function(obj, entry['symbol'], include_symbol_locations=True)
    masked = {row['offset'] + i for row in relocations for i in range(4)}
    differing = {i for i in range(min(size, len(candidate)))
                 if i not in masked and original[i] != candidate[i]}
    differing.update(range(min(size, len(candidate)), max(size, len(candidate))))
    return len(candidate), differing, original, candidate, relocations


def show(original: bytes, candidate: bytes, differing: set[int], address: int) -> None:
    try:
        import capstone
    except ImportError:
        print('    (install capstone for instruction listings)')
        return
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    target = {i.address: i for i in md.disasm(original, 0)}
    ours = {i.address: i for i in md.disasm(candidate, 0)}
    shown = 0
    for offset in sorted(target):
        ins = target[offset]
        if any(o in differing for o in range(offset, offset + ins.size)):
            mine = ours.get(offset)
            print(f'    +{offset:04X} original: {ins.mnemonic} {ins.op_str:<32}'
                  f' ours: {mine.mnemonic + " " + mine.op_str if mine else "(misaligned)"}')
            shown += 1
            if shown == 40:
                print('    ...')
                return


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('names', nargs='*', help='frontier names (default: all)')
    parser.add_argument('--update', action='store_true', help='record improved residuals')
    parser.add_argument('--show', action='store_true', help='print differing instructions')
    parser.add_argument('--jobs', type=int, default=4)
    args = parser.parse_args()

    config = tomllib.loads(CONFIG.read_text())['frontier']
    names = args.names or sorted(config)
    unknown = [n for n in names if n not in config]
    if unknown:
        print(f'unknown frontier(s): {", ".join(unknown)}; known: {", ".join(sorted(config))}', file=sys.stderr)
        return 2
    coff = load_coff_tools()
    image = coff.verified_target()

    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        objects = dict(zip(names, pool.map(lambda n: build(n, config[n]), names)))

    status = 0
    updates: dict[str, set[int]] = {}
    for name in names:
        entry = config[name]
        recorded = parse_offsets(entry['residual'])
        size, differing, original, candidate, _ = measure(coff, image, entry, objects[name])
        new = differing - recorded
        label = f'{name}: {size}/{entry["size"]} bytes, {len(differing)} differ (recorded {len(recorded)})'
        if size != entry['candidate_size'] or new:
            status = 1
            print(f'REGRESSED {label}')
            if size != entry['candidate_size']:
                print(f'    size changed from {entry["candidate_size"]}')
            if new:
                print(f'    newly differing offsets: {", ".join(format_offsets(new))}')
        elif not differing and size == entry['size']:
            print(f'EXACT     {label}: add a canonical unit and run compare-coff-function.py')
            updates[name] = differing
        elif differing < recorded:
            print(f'IMPROVED  {label}' + ('' if args.update else ' (run with --update to record)'))
            updates[name] = differing
        else:
            print(f'OK        {label}')
        if args.show and differing:
            show(original, candidate, differing, entry['address'])

    if args.update and updates and status == 0:
        text = CONFIG.read_text()
        for name, differing in updates.items():
            old = 'residual = ' + repr(config[name]['residual']).replace("'", '"')
            if old not in text:
                print(f'cannot locate residual for {name} in {CONFIG}', file=sys.stderr)
                return 1
            text = text.replace(old, 'residual = ' + repr(format_offsets(differing)).replace("'", '"'), 1)
        CONFIG.write_text(text)
        print(f'recorded {len(updates)} improved residual(s) in {CONFIG.relative_to(ROOT)}')
    return status


if __name__ == '__main__':
    raise SystemExit(main())
