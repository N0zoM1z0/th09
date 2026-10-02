#!/usr/bin/env python3
"""Inspect the complete TH09 Player movement owner; diagnostic, never credit.

The COFF auxiliary size includes two adjacent switch tables. Distinguish that
physical extent from the reviewed 1835-byte target code. All destinations below
come from TH09 operand consumers/canonical units, never fitted target fields.
Requires Capstone for bounded instruction diagnostics. Does not build or write.
"""
from __future__ import annotations

import argparse
from collections import Counter
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0041C170
CODE_SIZE = 1835
PHYSICAL_SIZE = 1900
SYMBOL = "?UpdateMovementAndOptions@PlayerLifecycleView@@QAEHXZ"
DESTINATIONS = {
    "?g_ReplayInputStates@@3PAUPlayerMovementReplayInputState@@A": 0x004ACE18,
    "?g_GameConfiguration@@3PAUGameConfiguration@@A": 0x004A7E78,
    "?g_PlayerFocusEffectIds@@3PAHA": 0x004A1A10,
    "__real@00000000": 0x0048E314,
    "?g_PlayerMovementSupervisor@@3UPlayerMovementSupervisorView@@A": 0x004B3100,
    "?g_PlayerPlayfieldMinX@@3MA": 0x004A80F0,
    "?g_PlayerPlayfieldMinY@@3MA": 0x004A80F4,
    "?g_PlayerPlayfieldWidth@@3MA": 0x004A80F8,
    "?g_PlayerPlayfieldHeight@@3MA": 0x004A80FC,
    "?g_AnmManager@@3PAVAnmManager@@A": 0x004DC550,
    "?IsHeld@PlayerMovementReplayInputState@@QAEGG@Z": 0x00415CB0,
    "?SpawnEffectInFixedSlot@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@HI@Z": 0x0040CD70,
    "?SetInterrupt@PlayerMovementEffectVmView@@QAEXF@Z": 0x00406790,
    "?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z": 0x00403E00,
    "??BPlayerPositionView@@QAEPAMXZ": 0x004343D0,
    "??GPlayerPositionView@@QBE?AU0@ABU0@@Z": 0x00401140,
    "??HPlayerPositionView@@QBE?AU0@ABU0@@Z": 0x00401100,
    "?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z": 0x00436F30,
    "??EZunTimer@@QAEXH@Z": 0x00401510,
}


def coff_symbols(path: Path, coff, names: set[str]) -> dict[str, tuple[int, int]]:
    data = path.read_bytes()
    machine, _, _, offset, count, optional_size, _ = struct.unpack_from("<HHIIIHH", data)
    if machine != 0x14C or optional_size:
        raise ValueError("expected i386 COFF")
    strings_start = offset + 18 * count
    strings_size = struct.unpack_from("<I", data, strings_start)[0]
    strings = data[strings_start:strings_start + strings_size]
    symbols = {}
    index = 0
    while index < count:
        raw, value, section, _, _, auxiliaries = struct.unpack_from(
            "<8sIhHBB", data, offset + 18 * index
        )
        name = coff.coff_name(raw, strings)
        if name in names:
            if name in symbols and symbols[name] != (section, value):
                raise ValueError("ambiguous COFF symbol: " + name)
            symbols[name] = (section, value)
        index += 1 + auxiliaries
    return symbols


def inspect(path: Path) -> dict[str, object]:
    import capstone

    spec = importlib.util.spec_from_file_location(
        "th09_coff", ROOT / "scripts/compare-coff-function.py"
    )
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, PHYSICAL_SIZE)
    code, relocations = coff.object_function(path, SYMBOL)
    symbols = coff_symbols(path, coff, {SYMBOL} | {r["symbol"] for r in relocations})
    owner_section, owner_offset = symbols[SYMBOL]
    replay = bytearray(code)
    internal_offsets = []
    table_positions = []
    for row in relocations:
        name, offset, addend = row["symbol"], row["offset"], row["addend"]
        if name in DESTINATIONS:
            destination = DESTINATIONS[name]
        elif name in symbols and symbols[name][0] == owner_section:
            destination = BASE + symbols[name][1] - owner_offset
            internal_offsets.append(offset)
            if name in {r["symbol"] for r in relocations if r["offset"] < len(code) - 64}:
                table_positions.append(symbols[name][1] - owner_offset)
        else:
            raise ValueError("unreviewed external relocation: " + name)
        if row["type"] == "DIR32":
            value = destination + addend
        elif row["type"] == "REL32":
            signed_addend = struct.unpack("<i", struct.pack("<I", addend))[0]
            value = destination + signed_addend - (BASE + offset + 4)
        else:
            raise ValueError("unsupported relocation type")
        struct.pack_into("<I", replay, offset, value & 0xFFFFFFFF)
    tables = sorted(set(table_positions))
    if tables != [len(code) - 64, len(code) - 32] or len(internal_offsets) != 18:
        raise ValueError("expected two eight-entry tables and eighteen internal fields")
    if sorted(o for o in internal_offsets if o >= tables[0]) != list(range(tables[0], len(code), 4)):
        raise ValueError("switch-table field coverage is incomplete")
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(blob):
        instructions = list(decoder.disasm(blob, BASE))
        if sum(i.size for i in instructions) != len(blob):
            raise ValueError("incomplete instruction decoding")
        return instructions

    prefix = decode(replay[:tables[0]])
    returns = [i for i in prefix if i.mnemonic == "ret"]
    if not returns:
        raise ValueError("no terminal return before tables")
    body_size = returns[-1].address - BASE + returns[-1].size
    if tables[0] - body_size > 15:
        raise ValueError("post-return alignment exceeds compiler alignment bound")
    if any(i.mnemonic not in {"nop", "lea", "mov", "int3"} for i in prefix if i.address >= BASE + body_size):
        raise ValueError("unreviewed post-return code")
    candidate_ins = decode(replay[:body_size])
    target_ins = decode(target[:CODE_SIZE])

    def normalized(instruction):
        if instruction.mnemonic.startswith("j") or instruction.mnemonic == "call":
            if instruction.operands and instruction.operands[0].type == capstone.x86.X86_OP_IMM:
                return instruction.mnemonic, "direct"
        return instruction.mnemonic, re.sub(
            r"0x[0-9a-f]+",
            lambda match: "external" if int(match[0], 16) >= 0x400000 else match[0],
            instruction.op_str,
        )

    alignment = difflib.SequenceMatcher(
        None, [normalized(i) for i in target_ins],
        [normalized(i) for i in candidate_ins], autojunk=False,
    )
    def calls(instructions):
        return [
            i.operands[0].imm for i in instructions
            if i.mnemonic == "call" and i.operands[0].type == capstone.x86.X86_OP_IMM
        ]
    target_calls, candidate_calls = calls(target_ins), calls(candidate_ins)
    full_equal = len(replay) == len(target) and replay == target
    return {
        "diagnostic_only": True,
        "exactness_credit": "none",
        "result": "complete-byte-agreement-diagnostic" if full_equal else "non-exact",
        "target_address": hex(BASE),
        "target_code_bytes": CODE_SIZE,
        "target_physical_bytes": PHYSICAL_SIZE,
        "candidate_code_bytes": body_size,
        "candidate_physical_bytes": len(code),
        "candidate_post_return_alignment_bytes": tables[0] - body_size,
        "candidate_table_bytes": len(code) - tables[0],
        "target_instructions": len(target_ins),
        "candidate_instructions": len(candidate_ins),
        "relocations": len(relocations),
        "internal_relocations": len(internal_offsets),
        "direct_calls": len(candidate_calls),
        "direct_call_order_agrees": candidate_calls == target_calls,
        "direct_call_destinations": {hex(k): v for k, v in Counter(candidate_calls).items()},
        "normalized_aligned_instructions": sum(m.size for m in alignment.get_matching_blocks()),
        "normalized_alignment_is_not_byte_proof": True,
        "raw_function_sha256": hashlib.sha256(code).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(inspect(args.object), indent=2))
    except (OSError, ValueError, ImportError, struct.error) as error:
        print("error: " + str(error), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
