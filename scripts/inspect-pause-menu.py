#!/usr/bin/env python3
"""Replay complete PauseMenu code/alignment/table bytes; diagnostic, no credit.

External bindings are reviewed TH09 boundaries, independently of candidate
positions. Local bindings use real COFF section/value coordinates. Supplying an
object does not attest its source, profile, includes or producer environment.
"""

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE, CODE_SIZE, PHYSICAL_SIZE = 0x434740, 1734, 1776
SYMBOL = '?OnUpdate@PauseMenu@@QAEHXZ'
DESTINATIONS = {
    '??0Float3@@QAE@MMM@Z': 0x4010B0,
    '?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z': 0x436F30,
    '?HasFlagBit0@AsciiGameManagerView@@QAEHXZ': 0x4343B0,
    '?IsReplayNeutral@AsciiGameManagerView@@QAEHXZ': 0x415D20,
    '?IsVisible@AnmVm@@QAEHXZ': 0x434370,
    '?PlaySoundByIdx@AsciiSoundPlayerView@@QAEXHH@Z': 0x43E2F0,
    '?ResumeAfterPause@AsciiSoundPlayerView@@QAEXXZ': 0x423498,
    '?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x403E00,
    '?SetInvisible@AnmVm@@QAEXXZ': 0x434380,
    '?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z': 0x436AC0,
    '?StopAudio@Supervisor@@QAEHXZ': 0x42FD30,
    # Historical unresolved production alias: only for old-object diagnostics.
    '?PrepareResultScreen@AsciiSupervisorView@@QAEHXZ': 0x42FD30,
    '?WasPressed@AsciiInputView@@QAEGG@Z': 0x40E2D0,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x4DC550,
    '?g_AsciiInput@@3UAsciiInputView@@A': 0x4ACF34,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x4CE458,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x4A7D90,
    '?g_SoundPlayer@@3UAsciiSoundPlayerView@@A': 0x4DC698,
    '?g_Supervisor@@3VSupervisor@@A': 0x4B3100,
    '__imp__timeGetTime@0': 0x48E238,
}


def report(object_path):
    import capstone

    spec = importlib.util.spec_from_file_location(
        'coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, PHYSICAL_SIZE)
    raw, rows = coff.object_function(
        object_path, SYMBOL, include_symbol_locations=True)
    linked = bytearray(raw)
    occupied = set()
    for row in rows:
        offset = row['offset']
        if offset < 0 or offset + 4 > len(raw):
            raise ValueError('truncated COFF field')
        if occupied.intersection(range(offset, offset + 4)):
            raise ValueError('overlapping COFF fields')
        occupied.update(range(offset, offset + 4))
        if row['symbol_section'] == row['owner_section']:
            relative = row['symbol_value'] - row['owner_value'] + row['addend']
            if not 0 <= relative < len(raw):
                raise ValueError('owner-local destination outside complete extent')
            destination = BASE + relative
        else:
            destination = DESTINATIONS[row['symbol']] + row['addend']
        value = destination
        if row['type'] == 'REL32':
            value -= BASE + offset + 4
        elif row['type'] != 'DIR32':
            raise ValueError('unreviewed COFF relocation type')
        struct.pack_into('<I', linked, offset, value & 0xFFFFFFFF)

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    imm, mem = capstone.x86.X86_OP_IMM, capstone.x86.X86_OP_MEM

    def decode(data):
        result = list(decoder.disasm(data, BASE))
        if sum(i.size for i in result) != len(data):
            raise ValueError('incomplete instruction decode')
        return result

    def describe(data, table_offset, target_code_size=None):
        decoded = decode(bytes(data[:table_offset]))
        returns = [i.address - BASE + i.size for i in decoded if i.mnemonic == 'ret']
        if not returns:
            raise ValueError('missing complete return boundary')
        code_size = max(returns)
        if target_code_size is not None and code_size != target_code_size:
            raise ValueError('target return boundary changed')
        alignment = bytes(data[code_size:table_offset])
        if alignment not in (b'', b'\x8b\xff', b'\x90', b'\x90\x90'):
            raise ValueError('unreviewed trailing alignment')
        instructions = [i for i in decoded if i.address < BASE + code_size]
        boundaries = {i.address for i in instructions}
        table = struct.unpack('<10I', bytes(data[table_offset:]))
        if not all(x in boundaries for x in table):
            raise ValueError('switch destination is not a code instruction boundary')
        fields, calls, branches, dispatches = [], [], [], []
        for i in instructions:
            off = i.address - BASE
            if i.mnemonic.startswith('j') and i.operands[0].type == imm:
                if i.operands[0].imm not in boundaries:
                    raise ValueError('branch destination outside code boundaries')
                branches.append([off, i.mnemonic, i.operands[0].imm - BASE])
            if i.mnemonic == 'call' and i.operands[0].type == imm:
                if i.imm_size != 4:
                    raise ValueError('unexpected relative call width')
                fields.append([off + i.imm_offset, 'REL32'])
                calls.append(i.operands[0].imm)
            for operand in i.operands:
                if operand.type == mem and 0x400000 <= operand.mem.disp < 0x4E7000:
                    if i.disp_size != 4:
                        raise ValueError('unexpected absolute-memory width')
                    fields.append([off + i.disp_offset, 'DIR32'])
                elif operand.type == imm and 0x400000 <= operand.imm < 0x4E7000:
                    if i.mnemonic != 'call' and not i.mnemonic.startswith('j'):
                        if i.imm_size != 4:
                            raise ValueError('unexpected absolute-immediate width')
                        fields.append([off + i.imm_offset, 'DIR32'])
            if i.mnemonic == 'jmp' and i.operands[0].type != imm:
                operand = i.operands[0]
                if (operand.type != mem or operand.mem.base != 0
                        or operand.mem.index != capstone.x86.X86_REG_EDI
                        or operand.mem.scale != 4
                        or operand.mem.disp != BASE + table_offset):
                    raise ValueError('unreviewed switch dispatch operand')
                dispatches.append(off)
        if len(dispatches) != 1:
            raise ValueError('expected one ten-entry switch dispatch')
        fields.extend([table_offset + 4 * i, 'DIR32'] for i in range(10))
        return {
            'code_bytes': code_size, 'alignment_hex': alignment.hex(),
            'table_offset': table_offset,
            'table_destinations': [hex(x) for x in table],
            'instructions': len(instructions), 'fields': fields,
            'ordered_direct_calls': [hex(x) for x in calls],
            'internal_branches': branches,
        }

    local_tables = [r for r in rows if r['offset'] == len(raw) - 40]
    if len(local_tables) != 1:
        raise ValueError('complete ten-entry table absent from COFF extent')
    candidate = describe(linked, len(raw) - 40)
    original = describe(target, PHYSICAL_SIZE - 40, CODE_SIZE)
    actual_fields = Counter((r['offset'], r['type']) for r in rows)
    if Counter(map(tuple, candidate['fields'])) != actual_fields:
        raise ValueError('decoded operand/table fields do not cover all COFF records')
    if Counter(x[1] for x in original['fields']) != Counter({'DIR32': 60, 'REL32': 46}):
        raise ValueError('independent target field census changed: ' + str(Counter(x[1] for x in original['fields'])))
    differences = [
        {'offset': i, 'candidate': a, 'target': b}
        for i, (a, b) in enumerate(zip(linked, target)) if a != b
    ]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'provenance': 'Supplied-object diagnostic; source/profile not attested.',
        'object': str(object_path), 'candidate': candidate, 'target': original,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'linked_sha256': hashlib.sha256(linked).hexdigest(),
        'fields': rows, 'physical_bytes': len(raw),
        'overlap_differences': len(differences),
        'missing_bytes': max(0, len(target) - len(raw)),
        'excess_bytes': max(0, len(raw) - len(target)),
        'differences': differences, 'complete_equal': linked == target,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', nargs='?', type=Path,
                        default=ROOT / 'build/matching/AsciiManagerMenu.obj')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = report(args.object)
    if args.output:
        args.output.write_text(json.dumps(result, indent=2) + '\n')
    print('PauseMenu diagnostic:', result['physical_bytes'], 'physical bytes,',
          len(result['fields']), 'fields,', result['overlap_differences'],
          'overlap differences,', result['missing_bytes'], 'missing,',
          result['excess_bytes'], 'excess; no exactness credit')


if __name__ == '__main__':
    main()
