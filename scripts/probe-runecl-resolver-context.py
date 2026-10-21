#!/usr/bin/env python3
"""Compile one genuine resolver visibility context; never award exact credit.

The old whole-EclOperands include failed on incompatible GameManager globals.
The maintained headers now share that declaration. Check actual native bodies,
their complete post-return tables and same-object callers before diagnosing
RunEcl. Temporary source/OBJ/PDB is removed after one serial nice10 compile.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
TARGET_SHA256 = '10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822'
MODELS = {
    'int': ('ecl-resolve-int', 0x405760, 1572, 1980, 165, 131),
    'int-lvalue': ('ecl-resolve-int-lvalue', 0x405F20, 491, 726, 39, 17),
    'float-lvalue': ('ecl-resolve-float-lvalue', 0x406210, 702, 968, 49, 24),
    'float': ('ecl-resolve-float', 0x4068A0, 1633, 2044, 121, 100),
}
CASES = ('all-before', 'all-after', *MODELS)
SYMBOLS = {
    'int': '?ResolveInt@Th09EclRunControl@@YIHPAUEnemyView@@H@Z',
    'int-lvalue': '?ResolveIntLValue@Th09EclRunControl@@YIPAHPAUEnemyView@@PAHGH@Z',
    'float-lvalue': '?ResolveFloatLValue@Th09EclRunControl@@YIPAMPAUEnemyView@@PAMGH@Z',
    'float': '?ResolveFloat@EnemyView@@QAEMM@Z',
}
ANONYMOUS_NAMESPACE = re.compile(r'@\?A0x[0-9a-fA-F]{8}@@')


def digest(data):
    return hashlib.sha256(data).hexdigest()


def module(filename):
    spec = importlib.util.spec_from_file_location(
        'resolver_context_' + filename.replace('-', '_'), ROOT/'scripts'/filename)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def selected_source(source, case):
    """Use actual maintained function bodies, with no replacement declarations."""
    if case in ('all-before', 'all-after'):
        return source
    markers = ('int __fastcall ResolveInt(', 'int *__fastcall ResolveIntLValue(',
               'float *__fastcall ResolveFloatLValue(',
               '} // namespace Th09EclRunControl', 'float EnemyView::ResolveFloat(')
    if any(source.count(marker) != 1 for marker in markers):
        raise ValueError('reviewed resolver definition boundaries changed')
    positions = [source.index(marker) for marker in markers]
    if positions != sorted(positions):
        raise ValueError('reviewed resolver definition order changed')
    prefix = source[:positions[0]]
    close = '} // namespace Th09EclRunControl\n'
    if case == 'float':
        return prefix + close + source[positions[4]:]
    index = ('int', 'int-lvalue', 'float-lvalue').index(case)
    return prefix + source[positions[index]:positions[index+1]] + close


def compare_definition(raw, rows, unit, target):
    """Bind all fields, retaining actual local coordinates and complete tables."""
    extent = unit.get('compare_size', unit['size'])
    expected = unit.get('relocations', [])
    if len(raw) != extent or len(target) != extent or len(rows) != len(expected):
        raise ValueError('native resolver complete physical extent/fields differ')
    linked = bytearray(raw)
    previous = 0
    labels, namespaces = [], []
    for actual, reviewed in zip(rows, expected):
        offset, kind = actual['offset'], actual['type']
        if (type(offset) is not int or offset < previous or offset+4 > extent
                or offset != reviewed['offset'] or kind != reviewed['type']
                or kind not in ('DIR32', 'REL32')):
            raise ValueError('native resolver field offset/type/coverage differs')
        if (actual['symbol_section'] > 0
                and actual['symbol_section'] == actual['owner_section']):
            relative = actual['symbol_value']-actual['owner_value']+actual['addend']
            destination = unit['target_address'] + relative
            if not 0 <= relative < extent or destination != reviewed['target']:
                raise ValueError('native resolver actual local destination differs')
            if actual['symbol'] != reviewed['symbol']:
                labels.append(dict(offset=offset, actual=actual['symbol'],
                                   reviewed=reviewed['symbol'], relative=relative))
        else:
            if actual['symbol'] != reviewed['symbol']:
                normalize = lambda s: ANONYMOUS_NAMESPACE.sub('@?ANONYMOUS@@', s)
                if (normalize(actual['symbol']) != normalize(reviewed['symbol'])
                        or normalize(actual['symbol']) == actual['symbol']):
                    raise ValueError('native resolver external decorated identity differs')
                namespaces.append(dict(offset=offset, actual=actual['symbol'],
                                       reviewed=reviewed['symbol']))
            destination = reviewed['target'] + actual['addend']
        value = (destination - (unit['target_address']+offset+4
                                if kind == 'REL32' else 0)) & 0xffffffff
        if struct.unpack_from('<I', target, offset)[0] != value:
            raise ValueError('native resolver original operand/addend differs')
        struct.pack_into('<I', linked, offset, value)
        previous = offset+4
    if linked != target:
        raise ValueError('native resolver complete original bytes differ')
    return dict(logical_bytes=unit['size'], physical_bytes=extent, fields=len(rows),
                complete_original_equal=True, linked_sha256=digest(linked),
                original_sha256=digest(target), private_label_changes=labels,
                anonymous_namespace_changes=namespaces)


def require_definition_calls(rows, symbol, count, body_fields):
    calls = [row for row in rows if row['symbol'] == symbol]
    if len(calls) != count:
        raise ValueError('native resolver caller reference count differs')
    sections = set()
    for row in calls:
        section = row.get('symbol_section')
        if (row['type'] != 'REL32' or row['addend'] != 0
                or type(section) is not int or section <= 0
                or section == row.get('owner_section') or row.get('symbol_value') != 0):
            raise ValueError('native resolver caller lacks its same-object definition')
        sections.add(section)
    if (len(sections) != 1 or not body_fields
            or {row['owner_section'] for row in body_fields} != sections
            or {row['owner_value'] for row in body_fields} != {0}):
        raise ValueError('native resolver caller/body coordinates disagree')
    return dict(calls=count, same_object_definition=True, section=sections.pop())


def run(case, output):
    sys.path.insert(0, str(ROOT/'scripts'))
    coff = module('compare-coff-function.py')
    image = coff.verified_target()
    if digest(image) != TARGET_SHA256:
        raise ValueError('original Japanese TH09 v1.50a target required')
    report_path = output/(case+'.json')
    if report_path.is_symlink():
        raise ValueError('refusing symlinked resolver report')
    report = dict(state='incomplete', case=case, diagnostic_only=True,
                  exactness_credit='none', target_sha256=digest(image))
    report_path.write_text(json.dumps(report)+'\n')
    observed = {}

    def collect(name):
        path = (ROOT/'src'/name).resolve()
        if not path.is_relative_to((ROOT/'src').resolve()):
            raise ValueError('project include escapes source root')
        if path in observed:
            return
        data = path.read_bytes()
        observed[path] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent/include.decode()).as_posix())

    try:
        collect('EclManager.cpp')
        collect('EclOperands.cpp')
        for name in ('config/target.toml', 'config/tools.lock.toml',
                     'config/match-units.toml', 'config/functions.csv',
                     'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                     'scripts/inspect-ecl-complete.py', 'scripts/audit-ecl-callsite-identities.py',
                     'scripts/inspect-enemy-manager-update.py',
                     'scripts/report-ecl-handler-shapes.py',
                     'scripts/check-boss-frontiers.py',
                     'scripts/probe-runecl-resolver-context.py'):
            observed[ROOT/name] = (ROOT/name).read_bytes()
        # Bind transitive diagnostic helpers as well as the direct entrypoints.
        for directory in ('scripts', 'config'):
            for path in (ROOT/directory).iterdir():
                if path.is_file() and path.suffix in ('.py', '.sh', '.toml', '.csv', '.json', '.txt'):
                    observed[path] = path.read_bytes()
        units = tomllib.loads(observed[ROOT/'config/match-units.toml'].decode())['units']
        carrier = units['enemy-attached-effect-update']
        if carrier['source'] != 'src/EclManager.cpp':
            raise ValueError('native ECL carrier changed')
        profile = carrier['profile']
        selected = tuple(MODELS) if case.startswith('all-') else (case,)
        for key in selected:
            name, address, size, extent, fields, _ = MODELS[key]
            unit = units[name]
            if (unit['source'] != 'src/EclOperands.cpp' or unit['profile'] != profile
                    or unit['symbol'] != SYMBOLS[key]
                    or unit['target_address'] != address or unit['size'] != size
                    or unit.get('compare_size', size) != extent
                    or len(unit.get('relocations', [])) != fields):
                raise ValueError('canonical resolver source/profile/extent/fields changed')
        source = observed[ROOT/'src/EclManager.cpp'].decode().replace('\r\n', '\n')
        include = '#include "EclOperands.cpp"\n'
        if case == 'all-after':
            source += '\n'+include
        else:
            anchor = '#include "ExAttackType8GameManagerView.hpp"'
            if source.count(anchor) != 1:
                raise ValueError('native GameManager include changed')
            source = source.replace(anchor, anchor+'\n'+include, 1)
        operand_source = selected_source(
            observed[ROOT/'src/EclOperands.cpp'].decode().replace('\r\n', '\n'), case)
        report.update(profile=profile, inputs_sha256={
            p.relative_to(ROOT).as_posix(): digest(b) for p, b in observed.items()})
        with tempfile.TemporaryDirectory(prefix='native-resolvers-', dir=output) as temporary:
            staged = Path(temporary)
            for path, data in observed.items():
                if path.is_relative_to(ROOT/'src'):
                    relative = path.relative_to(ROOT/'src')
                    (staged/relative).parent.mkdir(parents=True, exist_ok=True)
                    (staged/relative).write_bytes(data)
            (staged/'EclManager.cpp').write_text(source)
            (staged/'EclOperands.cpp').write_text(operand_source)
            inputs = {p.relative_to(staged).as_posix(): digest(p.read_bytes())
                      for p in staged.rglob('*') if p.is_file()}
            obj = staged/'unit.obj'
            build = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                    str(staged/'EclManager.cpp'), str(obj), *profile],
                                   cwd=ROOT, capture_output=True, text=True)
            report.update(compile_exit=build.returncode,
                          compile_log=build.stdout+build.stderr,
                          compiled_inputs_sha256=inputs)
            if build.returncode:
                raise ValueError('native resolver compiler failed')
            enemy = module('inspect-enemy-manager-update.py')
            for name, symbol in (('runecl', enemy.RUNECL_SYMBOL), ('enemy', enemy.SYMBOL)):
                raw, rows = coff.object_function(obj, symbol, include_symbol_locations=True)
                report[name+'-actual'] = dict(raw_hex=raw.hex(), coff=rows)
            report['native_definitions'] = {}
            for key in selected:
                name, _, _, extent, _, count = MODELS[key]
                unit = units[name]
                body, fields = coff.object_function(obj, unit['symbol'], include_symbol_locations=True)
                proof = compare_definition(body, fields, unit,
                                           coff.pe_bytes_at(image, unit['target_address'], extent))
                proof['caller_definition'] = require_definition_calls(
                    report['runecl-actual']['coff'], unit['symbol'], count, fields)
                report['native_definitions'][name] = dict(raw_hex=body.hex(), coff=fields, **proof)
            report['runecl'] = module('inspect-ecl-complete.py').report(obj)
            report['enemy'] = enemy.diagnose(obj)
            shape = subprocess.run([sys.executable, '-B', str(ROOT/'scripts/report-ecl-handler-shapes.py'),
                                    '--json', str(obj)], cwd=ROOT, capture_output=True, text=True)
            if shape.returncode:
                raise ValueError('complete resolver handler shape inspection failed')
            report['handler_shapes'] = json.loads(shape.stdout)
            frontier = module('check-boss-frontiers.py')
            frontier.guard_enemy_update(report['enemy'])
            try:
                frontier.validate_runecl_byte_frontier(report['runecl'], report['handler_shapes'])
                report['runecl_frontier_passed'] = True
            except RuntimeError as error:
                report.update(runecl_frontier_passed=False, frontier_error=str(error))
            if any(digest((staged/n).read_bytes()) != h for n, h in inputs.items()):
                raise ValueError('staged resolver source changed during compilation')
        if any(path.read_bytes() != data for path, data in observed.items()):
            raise ValueError('resolver live source/provider changed during compilation')
        report['state'] = 'complete-diagnostic'
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        report.update(state='rejected', error=str(error))
        report_path.write_text(json.dumps(report, indent=2)+'\n')
        raise
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='all-before')
    parser.add_argument('--output', type=Path, default=ROOT/'.analysis/runecl-resolver-context')
    args = parser.parse_args()
    output = args.output.resolve()
    if (not output.is_relative_to((ROOT/'.analysis').resolve())
            or output == (ROOT/'.analysis').resolve()):
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    try:
        report = run(args.case, output)
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        print('Native resolver context rejected:', error, file=sys.stderr)
        return 2
    print(args.case, report['runecl']['physical_bytes'], 'physical bytes;',
          report['runecl']['different_bytes'], 'RunEcl differences;',
          report['enemy']['complete_differences'], 'Enemy differences;',
          'native definitions original-equal; diagnostic only')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
