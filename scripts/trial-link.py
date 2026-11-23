#!/usr/bin/env python3
"""Trial-link every canonical exact carrier to list whole-image closure work.

This is a diagnostic for the Windows i386 product phase, not a product build
and not an exactness Oracle. It compiles each distinct (source, profile)
carrier from config/match-units.toml under build/link-probe/, links them with
the pinned VC7.1 linker and libraries, and classifies duplicate and unresolved
symbols against the tracked target references.

  scripts/trial-link.py build     compile carriers serially at low priority
  scripts/trial-link.py link      link and classify diagnostics
  scripts/trial-link.py verify    check each unit against its trial carrier and
                                  every other profile of the same source

Use verify-trial-layouts.py after both links to check the linked bytes and
relocations against the selected carriers.
"""
from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/link-probe'
VC = ROOT / '.tools/msvc710/Vc7'
# Import libraries cover the target's nine import descriptors; d3dx8.lib is
# static. d3dx8.lib requests libci, libc and amdmath, which /MT replaces.
LIBS = ['libcmt.lib', 'kernel32.lib', 'user32.lib', 'gdi32.lib', 'advapi32.lib', 'ole32.lib',
        'winmm.lib', 'd3d8.lib', 'd3dx8.lib', 'dinput8.lib', 'dsound.lib', 'dxguid.lib', 'dxerr8.lib']


def pinned_d3dx8() -> Path:
    path = VC / 'PlatformSDK/Lib/d3dx8.lib'
    expected = tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['msvc71']['d3dx8_sha256']
    if not path.is_file() or hashlib.sha256(path.read_bytes()).hexdigest() != expected:
        raise SystemExit('missing or mismatched pinned Platform SDK d3dx8.lib')
    return path


def library_symbols() -> set[str]:
    """Public names of the linked archives, from each first linker member."""
    import struct
    names = set()
    folders = [VC / 'lib', VC / 'PlatformSDK/Lib']
    d3dx8 = pinned_d3dx8()
    for library in LIBS:
        path = d3dx8 if library == d3dx8.name else next(
            (p for f in folders for p in f.iterdir() if p.name.lower() == library), None)
        if path is None:
            raise SystemExit('missing link library ' + library)
        data = path.read_bytes()
        if data[:8] != b'!<arch>\n':
            continue
        size = int(data[8 + 48:8 + 58].decode().strip())
        member = data[8 + 60:8 + 60 + size]
        count = struct.unpack_from('>I', member, 0)[0]
        table = member[4 + 4 * count:]
        names |= {n.decode('latin1') for n in table.split(b'\0')[:count]}
    return names
LINK_FLAGS = ['/NOLOGO', '/SUBSYSTEM:WINDOWS', '/MACHINE:X86', '/INCREMENTAL:NO',
              '/NODEFAULTLIB:libci.lib', '/NODEFAULTLIB:libc.lib', '/NODEFAULTLIB:amdmath.lib']
COMMON_FLAGS = {'/MT', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF', '/Gr', '/Oy-', '/I', 'src'}


def short(profile) -> str:
    return ' '.join(flag for flag in profile if flag not in COMMON_FLAGS)


def wine_env() -> dict[str, str]:
    env = dict(os.environ, DISPLAY='', WAYLAND_DISPLAY='', WINEDEBUG='-all', CL='', _CL_='')
    for key in ('WINEPREFIX', 'WINEARCH'):
        env.pop(key, None)
    env['PATH'] = str(ROOT / '.tools/factory-bin') + ':' + env['PATH']
    return env


def winpath(env, *paths) -> list[str]:
    result = subprocess.run(['winepath', '-w', *map(str, paths)], env=env, check=True,
                            capture_output=True, text=True)
    return result.stdout.splitlines()


def check_pins() -> None:
    keys = tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['msvc71']
    for name, key in (('cl.exe', 'compiler_sha256'), ('c1.dll', 'c_frontend_sha256'),
                      ('c1xx.dll', 'cpp_frontend_sha256'), ('c2.dll', 'backend_sha256'),
                      ('link.exe', 'linker_sha256')):
        if hashlib.sha256((VC / 'bin' / name).read_bytes()).hexdigest() != keys[key]:
            raise SystemExit('pinned VC7.1 component mismatch: ' + name)


def load_units() -> dict:
    return tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']


def build() -> None:
    check_pins()
    env = wine_env()
    groups = collections.defaultdict(lambda: collections.defaultdict(list))
    for name, unit in load_units().items():
        groups[tuple(unit['profile'])][unit['source']].append(name)
    index = []
    covered = {source for profile in groups.values() for source in profile}
    dominant = max(groups, key=lambda p: len(groups[p]))
    # Sources outside every canonical unit (constructor-only carriers and
    # the near-exact movement owner) use the dominant profile without inline
    # expansion; their constructor bodies match the original under /Ob0.
    product_only = tuple('/Ob0' if flag == '/Ob1' else flag for flag in dominant)
    # product/ holds link glue that is not reconstructed original source.
    for path in sorted((ROOT / 'src').glob('*.cpp')) + sorted((ROOT / 'product').glob('*.cpp')):
        name = path.relative_to(ROOT).as_posix()
        if name not in covered and not path.name.startswith('.'):
            groups[product_only][name] = []
            print(f'product-only source {name}')
    for number, (profile, sources) in enumerate(sorted(groups.items())):
        directory = OUT / f'p{number:02d}'
        directory.mkdir(parents=True, exist_ok=True)
        for stale in directory.glob('*.obj'):
            stale.unlink()
        names = sorted(sources)
        inc, sdk, lib, slib, src, out, *sources_win = winpath(
            env, VC / 'include', VC / 'PlatformSDK/Include', VC / 'lib', VC / 'PlatformSDK/Lib',
            ROOT / 'src', directory, *[ROOT / name for name in names])
        flags = [src if flag == 'src' else flag for flag in profile]
        result = subprocess.run(
            ['nice', '-n', '10', 'wine', str(VC / 'bin/cl.exe'), '/nologo', '/c', *flags,
             '/Fo' + out + '\\', '/Fd' + out + '\\carrier.pdb', *sources_win],
            cwd=directory, env=dict(env, INCLUDE=f'{inc};{sdk}', LIB=f'{lib};{slib}'),
            capture_output=True, text=True, errors='replace')
        errors = re.findall(r'^.*\berror [CD]\d+.*$', result.stdout + result.stderr, re.M)
        print(f'{directory.name}: {len(names)} sources, {short(profile)}, rc={result.returncode}', flush=True)
        for line in errors[:10]:
            print('   ', line)
        if result.returncode:
            raise SystemExit('carrier compilation failed')
        for name in names:
            index.append(dict(profile=list(profile), source=name,
                              object=f'{directory.name}/{Path(name).stem}.obj', units=sorted(sources[name])))
    (OUT / 'index.json').write_text(json.dumps(index, indent=1))
    print('carriers', len(index))


def coff_externals(path: Path) -> tuple[set[str], set[str]]:
    """Return (defined, undefined) external symbol names of a COFF object."""
    import struct
    data = path.read_bytes()
    _, sections, _, table, count, optional, _ = struct.unpack_from('<HHIIIHH', data, 0)
    strings = table + 18 * count
    defined, undefined = set(), set()
    index = 0
    while index < count:
        offset = table + 18 * index
        raw = data[offset:offset + 8]
        if raw[:4] == b'\0\0\0\0':
            start = strings + struct.unpack_from('<I', raw, 4)[0]
            name = data[start:data.index(b'\0', start)].decode('latin1')
        else:
            name = raw.rstrip(b'\0').decode('latin1')
        value, section, _, storage, aux = struct.unpack_from('<IhHBB', data, offset + 8)
        if storage == 2:
            (defined if section > 0 or value else undefined).add(name)
        index += 1 + aux
    return defined, undefined


def alias_table(index) -> dict[str, str]:
    """Map undefined symbols to a defined symbol bound to the same original address.

    Every binding comes from the canonical manifest: a unit's own symbol at
    its target address, or a relocation symbol with its recorded target. An
    undefined name whose original destination is the address of exactly one
    defined name resolves to that name at link time.
    """
    defined, undefined = set(), set()
    for entry in index:
        d, u = coff_externals(OUT / entry['object'])
        defined |= d; undefined |= u
    units = load_units()
    by_address = collections.defaultdict(set)
    for unit in units.values():
        by_address[unit['target_address']].add(unit['symbol'])
        for field in unit.get('relocations', []):
            if 'target' in field:
                by_address[field['target']].add(field['symbol'])
    aliases = {}
    for address, names in by_address.items():
        real = sorted(n for n in names if n in defined)
        if not real:
            continue
        # Several defined names at one address are identical bodies the
        # original linker folded; prefer the canonical unit symbol.
        canonical = [u['symbol'] for u in units.values() if u['target_address'] == address and u['symbol'] in defined]
        real = canonical or real
        for name in names:
            if name in undefined and name not in defined:
                aliases[name] = real[0]
    return aliases


def selected_carriers(index) -> list[dict]:
    # One carrier per source: a source compiled under several profiles keeps
    # the profile of most of its units in the product link.
    by_source = collections.defaultdict(list)
    for entry in index:
        by_source[entry['source']].append(entry)
    selected = {source: max(entries, key=lambda entry: len(entry['units']))
                for source, entries in by_source.items()}
    return [entry for entry in index if entry is selected[entry['source']]]


def link(reverse: bool = False) -> None:
    check_pins()
    d3dx8 = pinned_d3dx8()
    stem = 'trial-reversed' if reverse else 'trial'
    outputs = [OUT / f'{stem}.{suffix}' for suffix in ('exe', 'map')]
    data = ROOT / 'build/product' / ('ProductDataReversed.obj' if reverse else 'ProductData.obj')
    resources = ROOT / 'build/product/ProductResources.res'
    for required in (data, resources):
        if not required.is_file():
            hint = 'product-data.py --reverse' if reverse else 'product-data.py'
            raise SystemExit(f'missing link input {required.relative_to(ROOT)}. Run {hint} first.')
    env = wine_env()
    all_carriers = json.loads((OUT / 'index.json').read_text())
    index = selected_carriers(all_carriers)
    for entry in all_carriers:
        if entry not in index:
            print(f"product deviation: {entry['source']} {short(entry['profile'])} carrier not linked; "
                  f"{len(entry['units'])} units take the majority-profile body")
    product = tomllib.loads((ROOT / 'config/product-link.toml').read_text())
    aliases = alias_table(index)
    aliases.update(product.get('library_data', {}))
    extra = winpath(env, data, resources)
    (OUT / 'aliases.json').write_text(json.dumps(aliases, indent=1, sort_keys=True))
    print('address-bound aliases', len(aliases))
    lib, slib, d3dx8_win = winpath(env, VC / 'lib', VC / 'PlatformSDK/Lib', d3dx8)
    libraries = [f'"{d3dx8_win}"' if name == d3dx8.name else name for name in LIBS]
    object_by_source = {entry['source']: entry['object'].replace('/', '\\') for entry in index}
    sources = list(object_by_source)
    if reverse:
        sources.reverse()
    # Move each competing carrier after its reviewed COMDAT provider.
    # Preserve this selection when varying physical layout.
    precedence = product.get('comdat_precedence', {})
    for alternate, provider in precedence.items():
        if alternate not in sources or provider not in sources or alternate == provider:
            raise SystemExit(f'invalid COMDAT precedence: {provider} before {alternate}')
        if sources.index(alternate) < sources.index(provider):
            sources.remove(alternate)
            sources.insert(sources.index(provider) + 1, alternate)
    if any(sources.index(provider) > sources.index(alternate) for alternate, provider in precedence.items()):
        raise SystemExit('conflicting COMDAT precedence')
    objects = [object_by_source[source] for source in sources]
    objects = extra[::-1] + objects if reverse else objects + extra
    (OUT / f'{stem}.rsp').write_text('\r\n'.join(
        [*LINK_FLAGS, f'/OUT:{stem}.exe', f'/MAP:{stem}.map',
         *[f'/ALTERNATENAME:{alias}={real}' for alias, real in sorted(aliases.items())],
         *objects, *libraries]) + '\r\n')
    for output in outputs:
        output.unlink(missing_ok=True)
    result = subprocess.run(['nice', '-n', '10', 'wine', str(VC / 'bin/link.exe'), f'@{stem}.rsp'],
                            cwd=OUT, env=dict(env, LIB=f'{lib};{slib}'),
                            capture_output=True, text=True, errors='replace')
    log = result.stdout + result.stderr
    (OUT / f'{stem}.log').write_text(log)
    print('link rc', result.returncode, dict(collections.Counter(re.findall(r'\b(LNK\d{4})\b', log))))
    for line in re.findall(r'^.*\b(?:fatal error|warning) LNK\d+.*$', log, re.M)[:10]:
        print('   ', line)
    classify(log)
    if result.returncode or any(not output.is_file() for output in outputs):
        for output in outputs:
            output.unlink(missing_ok=True)
        raise SystemExit(result.returncode or 'link output missing')


def classify(log: str) -> None:
    unresolved = collections.defaultdict(set)
    for match in re.finditer(r'^(\S+) : error LNK20(?:01|19): unresolved external symbol .*? \((\S+)\)(?: referenced|$)',
                             log, re.M):
        unresolved[match.group(2)].add(match.group(1))
    duplicates = collections.defaultdict(set)
    for match in re.finditer(r'^(\S+) : error LNK2005: .*? \((\S+)\) already defined in (\S+)$', log, re.M):
        duplicates[match.group(2)].update({match.group(1), match.group(3)})
    units = load_units()
    candidates = {int(row['address'], 16) for row in csv.DictReader(open(ROOT / 'config/functions.csv'))}
    origins = {int(row['address'], 16): row['origin']
               for row in csv.DictReader(open(ROOT / 'config/function-origins.csv'))}
    canonical = {unit['symbol']: unit['target_address'] for unit in units.values()}
    canonical_addresses = set(canonical.values())
    targets = collections.defaultdict(set)
    for unit in units.values():
        for field in unit.get('relocations', []):
            if 'target' in field:
                targets[field['symbol']].add(field['target'])
    categories = collections.defaultdict(list)
    for symbol in unresolved:
        target = min(targets[symbol]) if targets.get(symbol) else None
        if symbol.startswith('__imp_'):
            kind = 'import'
        elif target is not None and target in candidates:
            kind = ('code alias of an exact function' if target in canonical_addresses
                    else f'code without exact source ({origins.get(target, "unknown")})')
        elif target is not None:
            kind = 'data without definition'
        else:
            kind = 'no tracked reference'
        categories[kind].append(symbol)
    sources = {entry['object'].split('/')[1]: entry['source']
               for entry in json.loads((OUT / 'index.json').read_text())}
    duplicate_kinds = collections.Counter()
    for symbol, objects in duplicates.items():
        same = len({sources.get(name, name) for name in objects}) == 1
        duplicate_kinds['same source, several profiles' if same else 'several sources'] += 1
    print('unresolved symbols', len(unresolved))
    for kind, symbols in sorted(categories.items(), key=lambda item: -len(item[1])):
        print(f'  {len(symbols):5} {kind}')
    print('duplicate symbols', len(duplicates), dict(duplicate_kinds))
    (OUT / 'classified.json').write_text(json.dumps(dict(
        unresolved={kind: sorted(symbols) for kind, symbols in categories.items()},
        duplicates={symbol: sorted(objects) for symbol, objects in duplicates.items()}), indent=1))


def verify() -> None:
    spec = importlib.util.spec_from_file_location('compare_coff', ROOT / 'scripts/compare-coff-function.py')
    compare = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(compare)
    units = load_units()
    by_source = collections.defaultdict(dict)
    for entry in json.loads((OUT / 'index.json').read_text()):
        by_source[entry['source']][tuple(entry['profile'])] = OUT / entry['object']

    def exact(name, path) -> bool:
        try:
            report = compare.compare_unit(name, path)
        except (OSError, KeyError, ValueError) as exc:
            print('   ', name, 'error:', exc)
            return False
        return (report.get('result') == 'exact' and report.get('matched_bytes') == report.get('size')
                and report.get('matched_compared_bytes') == report.get('compared_size'))

    failures = []
    alternates = collections.defaultdict(lambda: collections.defaultdict(list))
    for name, unit in sorted(units.items()):
        variants = by_source[unit['source']]
        if not exact(name, variants[tuple(unit['profile'])]):
            failures.append(name)
        if len(variants) > 1:
            for profile, path in variants.items():
                alternates[unit['source']][profile]
                if profile != tuple(unit['profile']) and not exact(name, path):
                    alternates[unit['source']][profile].append(name)
    print('units', len(units), 'failing in their own trial carrier', len(failures), failures[:10])
    for source, profiles in sorted(alternates.items()):
        unifying = [short(profile) for profile, failed in profiles.items() if not failed]
        print(f'{source}: unifying {unifying or "none"};',
              {short(profile): len(failed) for profile, failed in profiles.items()})
    (OUT / 'verify.json').write_text(json.dumps(dict(
        own_failures=failures,
        alternates={source: {short(p): f for p, f in profiles.items()} for source, profiles in alternates.items()}),
        indent=1))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('action', choices=('build', 'link', 'verify'))
    parser.add_argument('--reverse', action='store_true', help='link the game objects in reverse order')
    args = parser.parse_args()
    subprocess.run([sys.executable, '-B', str(ROOT / 'scripts/verify-target.py')], check=True,
                   stdout=subprocess.DEVNULL)
    if args.action == 'link':
        link(args.reverse)
    else:
        {'build': build, 'verify': verify}[args.action]()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
