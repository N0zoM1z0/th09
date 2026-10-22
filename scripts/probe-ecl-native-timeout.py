#!/usr/bin/env python3
"""Reproduce a native-bitfield RunEcl control without granting exactness credit.

Compile the maintained source and a four-byte native bitfield value update,
serially, through the genuine first-include chain and pinned VC7.1 profile.
Compare each complete original owner before describing opcode 155. A locally
similar handler never overrides a rejected whole-owner field/layout result.
"""
from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))


def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def native_state(source):
    start = source.index('    case TH09_ECL_OPCODE_SET_TIMEOUT_SPELL:')
    end = source.index('    case TH09_ECL_OPCODE_SET_SPECIAL_INTERACTION:', start)
    block = source[start:end]
    old_read = ('        unsigned char value;\n'
                '        memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));')
    old_call = ('        state.primaryFlags337C = Th09EclRunState::AssignTimeoutSpellBit('
                'state.primaryFlags337C, value);')
    if block.count(old_read) != 1 or block.count(old_call) != 1:
        raise ValueError('maintained opcode 155 source changed; review the control')
    block = block.replace(old_read,
                          '        unsigned int value = '
                          'Th09EclRunState::RawByte(instruction, 0);')
    block = block.replace(old_call,
                          '        state.primaryFlags337C = '
                          'Th09EclRunState::UpdatedTimeoutWord(\n'
                          '            state.primaryFlags337C, value);')
    source = source[:start] + block + source[end:]
    anchor = 'inline unsigned int AssignTimeoutSpellBit('
    if source.count(anchor) != 1:
        raise ValueError('maintained timeout helper declaration changed')
    helper = '''union TimeoutWordValue
{
    unsigned int raw;
    EnemyStateFlagBits bits;
};
typedef char TimeoutWordValueSizeIs04[(sizeof(TimeoutWordValue)==4)?1:-1];
__forceinline unsigned int UpdatedTimeoutWord(unsigned int previous,
                                             unsigned int incoming)
{
    TimeoutWordValue result;
    result.raw = previous;
    result.bits.timeoutSpell24 = incoming;
    return result.raw;
}
'''
    return source.replace(anchor, helper + '\n' + anchor, 1)


def handler(data, entries, base):
    import capstone

    start = entries[154]
    end = min(offset for offset in entries if offset > start)
    if not 0 <= start < end <= len(data):
        raise ValueError('opcode 155 leaves the complete owner')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    instructions = list(decoder.disasm(data[start:end], base + start))
    if sum(ins.size for ins in instructions) != end - start:
        raise ValueError('opcode 155 did not decode completely')
    return {'offset': start, 'bytes': end - start, 'instructions': [
        {'offset': ins.address - base, 'bytes': ins.bytes.hex(),
         'mnemonic': ins.mnemonic, 'operands': ins.op_str}
        for ins in instructions]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path,
                        default=ROOT / '.analysis/ecl-native-timeout')
    args = parser.parse_args()
    output = args.output.resolve()
    if not output.is_relative_to((ROOT / '.analysis').resolve()):
        parser.error('output must be below this repository .analysis directory')
    output.mkdir(parents=True, exist_ok=True)
    report_path = output / 'report.json'
    variant = output / 'native-state.inl'
    if report_path.is_symlink() or variant.is_symlink():
        raise ValueError('refusing symlinked probe outputs')
    report_path.unlink(missing_ok=True)
    probe = module('timeout_probe', 'probe-ecl-variant.py')
    coff = module('timeout_coff', 'compare-coff-function.py')
    boundaries = module('timeout_boundaries', 'report-ecl-handler-boundaries.py')
    target = coff.pe_bytes_at(coff.verified_target(), 0x004086C0, 15564)
    target_handler = handler(target, boundaries.target_opcode_entries(coff),
                             0x004086C0)
    state = ROOT / 'src/EclRunState.inl'
    variant.write_text(native_state(state.read_text()))
    report = {'diagnostic_only': True, 'exactness_credit': 'none',
              'profile': probe.pinned_profile(), 'target_opcode155': target_handler,
              'rows': []}
    for name, replacement in (('baseline', None), ('native', variant)):
        carrier = output / name
        binding = probe.stage_variants(carrier, state=replacement)
        row = {'name': name, 'staged_source_hashes': binding}
        try:
            row['complete_owner'] = probe.replay(carrier)
        except ValueError as exc:
            diagnostic = carrier / 'replay-diagnostic.json'
            if not diagnostic.is_file():
                raise
            row['complete_owner'] = json.loads(diagnostic.read_text())
            row['whole_owner_rejection'] = str(exc)
        obj = carrier / 'EclManager.obj'
        raw, _ = coff.object_function(obj, boundaries.RUN_ECL_SYMBOL)
        row['opcode155'] = handler(raw, boundaries.candidate_opcode_entries(coff, obj),
                                    0x004086C0)
        report['rows'].append(row)
        report_path.write_text(json.dumps(report, indent=2) + '\n')
        complete = row['complete_owner']
        print(f"{name}: {complete['physical_bytes']} bytes, "
              f"{complete['original_field_count']} fields, "
              f"{complete['ordinary_differences']} ordinary differences; "
              f"opcode155={row['opcode155']['bytes']} bytes; "
              f"fields_agree={complete['field_positions_agree']}", flush=True)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
