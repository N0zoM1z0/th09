#!/usr/bin/env python3
"""Complete Type19/21 initializer diagnostic; no partial or accepted exact credit."""
import argparse
import hashlib
import importlib.util
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
DESTINATIONS = {
    '?AllocateDynamicData@ExAttackAllocateRecordView@@QAEXHH@Z': 0x440d90,
    '?GetSideState@ExAttackRecordSideView@@QAEPAUExAttackGameManagerSideView@@XZ': 0x440cb0,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x401560,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x441890,
    '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x404910,
    '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z': 0x406200,
    '?SelectSide@Supervisor@@QAEXH@Z': 0x401440,
    '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': 0x401680,
    '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': 0x4016b0,
    '?g_ExAttackPlayfieldWidth@@3MA': 0x4a80e8,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x4a7d90,
    '?g_ReplayRng@@3URngRuntimeView@@A': 0x4ace0c,
    '?g_Supervisor@@3VSupervisor@@A': 0x4b3100,
    '__real@3d888889': 0x491648,
    '__real@3e4f8c3d': 0x491608,
    '__real@3f000000': 0x490f54,
    '__real@40490fdb': 0x48e2c4,
    '__real@40c90fdb': 0x48e4c4,
    '__real@41c00000': 0x48ef10,
    '__real@42a00000': 0x48e318,
    '__real@43000000': 0x48e3bc,
    '__real@43e00000': 0x48e3b8,
    '__real@bd888889': 0x49164c,
}


# Original-byte hashes are independent of the source/COFF-produced comparison.
# This full-function address-operand census is checked *before* running the
# existing 763/727-byte candidate diagnostic.  Counting COFF fields alone
# cannot prove that they still refer to the original instruction operands.
ORIGINAL_OWNER = {
    '19': (0x0044A030, 763, 57,
           'fc7b630cff17c32048de02f169f5c39710727027941aa502db823e357563cf67'),
    '21': (0x0044B220, 727, 53,
           'b5271179d169d482952d887b03ad08e10a19c74654590e32598551d0a523ca47'),
}


def address_operands(body, base, capstone):
    """Independently decode direct calls and absolute PE data operands.

    Relative jumps are local control flow, never external relocation fields.
    Neither a symbol name nor a COFF relocation is consulted here.
    """
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(body, base))
    if (not instructions or sum(ins.size for ins in instructions) != len(body)
            or instructions[-1].mnemonic != 'ret'):
        raise ValueError('ExAttack trail owner is not fully decoded through return')
    fields = []
    for ins in instructions:
        offset = ins.address - base
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or
                    ins.operands[0].type != capstone.x86.X86_OP_IMM or
                    ins.imm_size != 4):
                raise ValueError('unreviewed ExAttack trail indirect/short call')
            fields.append((offset + ins.imm_offset, 'REL32', ins.operands[0].imm))
            continue
        for operand in ins.operands:
            if (operand.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4 and
                    0x00400000 <= operand.mem.disp < 0x00500000):
                fields.append((offset + ins.disp_offset, 'DIR32', operand.mem.disp))
            elif (operand.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4 and
                    0x00400000 <= operand.imm < 0x00500000):
                fields.append((offset + ins.imm_offset, 'DIR32', operand.imm))
    return fields


def bind_original_fields(target, candidate, rows, original_fields, *,
                         base, destinations, original_digest, expected_count):
    """Bind complete original-disassembled field identities in code order.

    The candidate is a raw relocatable COFF function. Its machine field
    encodings are independently decoded *after* binding, so an out-of-order
    operand, unowned displacement or spurious address is never accepted just
    because the count or an aligned instruction score happens to agree.
    """
    if (hashlib.sha256(target).hexdigest() != original_digest or
            len(target) != len(candidate) or
            len(rows) != expected_count or len(original_fields) != expected_count):
        raise ValueError('ExAttack trail original owner/relocation census changed')
    linked = bytearray(candidate)
    occupied = set()
    for row, (target_offset, target_kind, target_destination) in zip(rows, original_fields):
        offset = row.get('offset')
        kind = row.get('type')
        symbol = row.get('symbol')
        addend = row.get('addend')
        if (type(offset) is not int or type(addend) is not int or
                offset != target_offset or kind != target_kind or
                kind not in ('REL32', 'DIR32') or
                offset < 0 or offset + 4 > len(candidate) or
                not isinstance(symbol, str) or symbol not in destinations or
                destinations[symbol] + addend != target_destination):
            raise ValueError('ExAttack trail candidate/retail field identity differs')
        field_range = set(range(offset, offset + 4))
        if occupied & field_range:
            raise ValueError('ExAttack trail overlapping COFF address fields')
        occupied.update(field_range)
        value = target_destination
        if kind == 'REL32':
            value -= base + offset + 4
        encoded = struct.pack('<I', value & 0xFFFFFFFF)
        if target[offset:offset + 4] != encoded:
            raise ValueError('ExAttack trail original byte/operand encoding differs')
        linked[offset:offset + 4] = encoded
    return bytes(linked)


def verify_original_address_inventory(callback_type, object_path, coff):
    """Pin all 57/53 original PE field *locations and identities*, not just counts."""
    import capstone

    base, size, count, original_digest = ORIGINAL_OWNER[callback_type]
    target = coff.pe_bytes_at(coff.verified_target(), base, size)
    candidate, rows = coff.object_function(
        object_path, '?ExAttackInitializeCallbackType' + callback_type +
        '@@YIHPAUExAttackRecord@@@Z')
    original_fields = address_operands(target, base, capstone)
    linked = bind_original_fields(
        target, candidate, rows, original_fields,
        base=base, destinations=DESTINATIONS,
        original_digest=original_digest, expected_count=count)
    if address_operands(linked, base, capstone) != original_fields:
        raise ValueError('ExAttack trail COFF binding does not decode to retail operands')
    return len(original_fields)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--type', required=True, choices=['19', '21'])
    args, remaining = parser.parse_known_args()
    spec = importlib.util.spec_from_file_location('trail_base', ROOT / 'scripts/inspect-exattack-type01.py')
    diagnostic = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(diagnostic)
    diagnostic.BASE, diagnostic.SIZE = (0x44A030, 763) if args.type == '19' else (0x44B220, 727)
    diagnostic.SYMBOL = '?ExAttackInitializeCallbackType' + args.type + '@@YIHPAUExAttackRecord@@@Z'
    diagnostic.DESTINATIONS = DESTINATIONS
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    for symbol, address in DESTINATIONS.items():
        if symbol.startswith('__real@') and coff.pe_bytes_at(pe, address, 4) != struct.pack('<I', int(symbol[7:], 16)):
            raise ValueError('target float identity differs: ' + symbol)
    # The first argument left after --type is the cold source object; --diff
    # and --instructions are delegated to the existing full-body comparator.
    source_args = argparse.ArgumentParser(add_help=False)
    source_args.add_argument('object', type=Path)
    selected, _ = source_args.parse_known_args(remaining)
    verify_original_address_inventory(args.type, selected.object, coff)
    sys.argv = [sys.argv[0]] + remaining
    diagnostic.main()

if __name__ == '__main__':
    main()
