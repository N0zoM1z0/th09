#!/usr/bin/env python3
"""Cold-build one natural RunEcl operand context; diagnostic only, no exact credit."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
CASES = ('baseline', 'jump-native-time', 'jump-typed-wire',
         'raw-int-macro', 'raw-byte-macro', 'raw-family-macros')
RAW_INT = '''inline int &RawInt(
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(instruction) +
        0x0C + operandIndex * 4);
}
'''
RAW_BYTE = '''inline unsigned char &RawByte(
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset)
{
    return *(reinterpret_cast<unsigned char *>(instruction) +
             0x0C + byteOffset);
}
'''
INT_MACRO = ('#define TH09_ECL_RAW_INT(instruction, operandIndex) '
             '(*reinterpret_cast<int *>(reinterpret_cast<unsigned char *>(instruction) '
             '+ 0x0C + (operandIndex) * 4))\n')
BYTE_MACRO = ('#define TH09_ECL_RAW_BYTE(instruction, byteOffset) '
              '(*(reinterpret_cast<unsigned char *>(instruction) + 0x0C + (byteOffset)))\n')
TIMER = 'ZunTimer &timer = *reinterpret_cast<ZunTimer *>(&context->time008);'
DIRECT_TIMER = 'ZunTimer &timer = context->time008;'
JUMP_WIRE = '''struct RelativeJumpWireView
{
    Th09EclRawInstructionHeaderView header00;
    int time0C;
    int displacement10;
};
typedef char Th09JumpTimeAt0C[offsetof(RelativeJumpWireView, time0C) == 0x0C ? 1 : -1];
typedef char Th09JumpDisplacementAt10[offsetof(RelativeJumpWireView, displacement10) == 0x10 ? 1 : -1];

'''


def digest(data):
    return hashlib.sha256(data).hexdigest()


def replace_once(text, before, after):
    if text.count(before) != 1:
        raise ValueError('reviewed source anchor changed: '+before.splitlines()[0])
    return text.replace(before, after, 1)


def render(case, inputs):
    """Return only changed files; preserve argument evaluation and lvalue types."""
    if case not in CASES:
        raise ValueError('unknown RunEcl context')
    if case == 'baseline':
        return {}
    if case.startswith('jump-'):
        name = 'EclRunControl.inl'
        source = replace_once(inputs[name].decode(), TIMER, DIRECT_TIMER)
        if case == 'jump-typed-wire':
            begin = source.index('    case TH09_ECL_OPCODE_JUMP:\n')
            end = source.index('    case TH09_ECL_OPCODE_SET_INT:\n', begin)
            jump = source[begin:end]
            jump = replace_once(jump, '        int value;',
                                '        const Th09EclRunControl::RelativeJumpWireView &wire =\n'
                                '            *reinterpret_cast<const Th09EclRunControl::RelativeJumpWireView *>(instruction);\n'
                                '        int value;')
            jump = replace_once(jump, '&Th09EclRunControl::RawInt(instruction, 0),',
                                '&wire.time0C,')
            jump = replace_once(jump, 'Th09EclRunControl::RawInt(instruction, 1));',
                                'reinterpret_cast<const Th09EclRunControl::RelativeJumpWireView *>(instruction)->displacement10);')
            source = source[:begin]+jump+source[end:]
            source = replace_once(source, 'namespace Th09EclRunControl\n{\n',
                                  'namespace Th09EclRunControl\n{\n\n'+JUMP_WIRE)
        return {name: source.encode()}
    use_int = case in ('raw-int-macro', 'raw-family-macros')
    use_byte = case in ('raw-byte-macro', 'raw-family-macros')
    if use_int and inputs['EclRunControl.inl'].decode().count(RAW_INT) != 1:
        raise ValueError('reviewed raw integer accessor semantics changed')
    if use_byte and inputs['EclRunState.inl'].decode().count(RAW_BYTE) != 1:
        raise ValueError('reviewed raw byte accessor semantics changed')
    changed = {}
    for name, data in inputs.items():
        source = data.decode()
        if use_int:
            if name == 'EclRunControl.inl':
                source = replace_once(source, RAW_INT, INT_MACRO)
            source = re.sub(r'(?<![\w:.>])(?:Th09EclRunControl::)?RawInt\s*\(',
                            'TH09_ECL_RAW_INT(', source)
        if use_byte:
            if name == 'EclRunState.inl':
                source = replace_once(source, RAW_BYTE, BYTE_MACRO)
            source = re.sub(r'(?<![\w:.>])(?:Th09EclRunState::)?RawByte\s*\(',
                            'TH09_ECL_RAW_BYTE(', source)
        if source.encode() != data:
            changed[name] = source.encode()
    return changed


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def collect_inputs():
    source_root = (ROOT/'src').resolve()
    inputs = {}

    def collect(name):
        path = (source_root/name).resolve()
        if not path.is_relative_to(source_root):
            raise ValueError('quoted project include escapes source root')
        if name in inputs:
            return
        data = path.read_bytes()
        inputs[name] = data
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', data, re.M):
            collect((Path(name).parent/include.decode()).as_posix())

    collect('EclManager.cpp')
    return inputs


def run(case, output):
    sys.path.insert(0, str(ROOT/'scripts'))
    coff = module('raw_context_coff', 'compare-coff-function.py')
    variant = module('raw_context_variant', 'probe-ecl-variant.py')
    image = coff.verified_target()
    inputs = collect_inputs()
    observed = {ROOT/'src'/name: data for name, data in inputs.items()}
    for name in ('config/match-units.toml', 'config/tools.lock.toml',
                 'config/functions.csv', 'config/target.toml',
                 'scripts/compile-probe.sh', 'scripts/compare-coff-function.py',
                 'scripts/inspect-ecl-complete.py', 'scripts/audit-ecl-callsite-identities.py',
                 'scripts/probe-ecl-variant.py', 'scripts/report-ecl-handler-shapes.py',
                 'scripts/probe-runecl-raw-context.py'):
        observed[ROOT/name] = (ROOT/name).read_bytes()
    units = tomllib.loads(observed[ROOT/'config/match-units.toml'].decode())['units']
    carrier = units['enemy-attached-effect-update']
    if carrier['source'] != 'src/EclManager.cpp':
        raise ValueError('reviewed ECL carrier changed')
    changed = render(case, inputs)
    if set(changed) - set(variant.FRAGMENTS) or carrier['profile'] != variant.pinned_profile():
        raise ValueError('context leaves the reviewed first-include/profile contract')
    staged = {**inputs, **changed}
    report = dict(state='incomplete', diagnostic_only=True, exactness_credit='none',
                  case=case, target_sha256=digest(image), profile=carrier['profile'],
                  source_inputs_sha256={p.relative_to(ROOT).as_posix(): digest(b)
                                        for p, b in observed.items()},
                  compiled_inputs_sha256={n: digest(b) for n, b in staged.items()})
    report_path = output/(case+'.json')
    if report_path.is_symlink():
        raise ValueError('refusing symlinked report')
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    build_root = ROOT/'build/diagnostics'
    build_root.mkdir(parents=True, exist_ok=True)
    try:
        with tempfile.TemporaryDirectory(prefix='runecl-raw-', dir=build_root) as td:
            stage = Path(td)
            report['staged_first_include_sha256'] = variant.stage_variants(stage, overrides=changed)
            try:
                report['whole_owner'] = variant.replay(stage)
            finally:
                full = module('raw_context_complete', 'inspect-ecl-complete.py')
                if (stage/'EclManager.obj').is_file():
                    try:
                        raw, rows = coff.object_function(stage/'EclManager.obj', full.SYMBOL,
                                                         include_symbol_locations=True)
                        report.update(raw_hex=raw.hex(), coff=rows)
                    except (OSError, ValueError) as error:
                        report['capture_error'] = str(error)
                if (stage/'complete.json').is_file():
                    report['runecl'] = json.loads((stage/'complete.json').read_text())
                if (stage/'replay-diagnostic.json').is_file():
                    report['replay_diagnostic'] = json.loads(
                        (stage/'replay-diagnostic.json').read_text())
            if 'raw_hex' not in report or 'runecl' not in report:
                raise ValueError('completed replay lacks its actual object/whole-owner capture')
            if any((stage/n).read_bytes() != staged[n] for n in variant.FRAGMENTS):
                raise ValueError('staged source changed during compilation')
        if any(p.read_bytes() != data for p, data in observed.items()):
            raise ValueError('live source/provider changed during compilation')
        report['state'] = 'complete-diagnostic'
        report['temporary_compiler_outputs_removed'] = True
    except (OSError, KeyError, ValueError, RuntimeError) as error:
        report.update(state='rejected', error=str(error))
        report_path.write_text(json.dumps(report, indent=2)+'\n')
        raise
    report_path.write_text(json.dumps(report, indent=2)+'\n')
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=CASES, default='baseline')
    parser.add_argument('--output', type=Path, default=ROOT/'.analysis/runecl-raw-context')
    args = parser.parse_args()
    output = args.output.resolve()
    analysis = (ROOT/'.analysis').resolve()
    if not output.is_relative_to(analysis) or output == analysis:
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    report_path = output/(args.case+'.json')
    if report_path.is_symlink():
        parser.error('refusing symlinked report')
    report_path.write_text(json.dumps(dict(state='incomplete', diagnostic_only=True,
                                          exactness_credit='none', case=args.case))+'\n')
    try:
        report = run(args.case, output)
    except (OSError, KeyError, ValueError, RuntimeError) as error:
        failure = json.loads(report_path.read_text())
        failure.update(state='rejected', diagnostic_only=True, exactness_credit='none',
                       case=args.case, error=str(error))
        report_path.write_text(json.dumps(failure, indent=2)+'\n')
        print('RunEcl raw context rejected:', error, file=sys.stderr)
        return 2
    result = report['runecl']
    print(args.case, result['physical_bytes'], 'bytes;', result['relocations'], 'fields;',
          result['ordinary_different_bytes'], 'ordinary differences;',
          result['field_different_bytes'], 'field differences; diagnostic only')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
