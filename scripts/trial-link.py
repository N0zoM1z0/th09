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
        'winmm.lib', 'd3d8.lib', 'd3dx8.lib', 'dinput8.lib', 'dsound.lib', 'dxguid.lib']
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
                      ('c1xx.dll', 'cpp_frontend_sha256'), ('c2.dll', 'backend_sha256')):
        if hashlib.sha256((VC / 'bin' / name).read_bytes()).hexdigest() != keys[key]:
            raise SystemExit('pinned compiler component mismatch: ' + name)


def load_units() -> dict:
    return tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']


def build() -> None:
    check_pins()
    env = wine_env()
    groups = collections.defaultdict(lambda: collections.defaultdict(list))
    for name, unit in load_units().items():
        groups[tuple(unit['profile'])][unit['source']].append(name)
    index = []
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
            cwd=ROOT, env=dict(env, INCLUDE=f'{inc};{sdk}', LIB=f'{lib};{slib}'),
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


def link() -> None:
    env = wine_env()
    index = json.loads((OUT / 'index.json').read_text())
    lib, slib = winpath(env, VC / 'lib', VC / 'PlatformSDK/Lib')
    (OUT / 'trial.rsp').write_text('\r\n'.join(
        [*LINK_FLAGS, '/OUT:trial.exe', '/MAP:trial.map',
         *[entry['object'].replace('/', '\\') for entry in index], *LIBS]) + '\r\n')
    result = subprocess.run(['nice', '-n', '10', 'wine', str(VC / 'bin/link.exe'), '@trial.rsp'],
                            cwd=OUT, env=dict(env, LIB=f'{lib};{slib}'),
                            capture_output=True, text=True, errors='replace')
    log = result.stdout + result.stderr
    (OUT / 'trial.log').write_text(log)
    print('link rc', result.returncode, dict(collections.Counter(re.findall(r'\b(LNK\d{4})\b', log))))
    for line in re.findall(r'^.*\b(?:fatal error|warning) LNK\d+.*$', log, re.M)[:10]:
        print('   ', line)
    classify(log)


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
    args = parser.parse_args()
    subprocess.run([sys.executable, '-B', str(ROOT / 'scripts/verify-target.py')], check=True,
                   stdout=subprocess.DEVNULL)
    {'build': build, 'link': link, 'verify': verify}[args.action]()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
