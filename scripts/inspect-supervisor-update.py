#!/usr/bin/env python3
"""Compare the complete Supervisor callback, including its owned switch data.

This diagnostic binds operands independently of their candidate offsets. It
does not add a match row or replace source/backend and cold-replay evidence.
"""

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct


ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00431110
CODE_SIZE = 844
OWNED_SIZE = 996
SYMBOL = "?OnUpdate@Supervisor@@SIHPAV1@@Z"
TARGET_SHA256 = "10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822"

# TH09-local identities reviewed through callback registration, target operands
# and callee/global evidence. Source aliases and view names remain provisional.
# In particular, RegisterStateObject is the genuine int/ECX title registrar;
# RegisterState9Object is Ending registration. Neither is a fabricated helper.
DESTINATIONS = {
    "?CheckVersion@SupervisorMethodView@?ANON@@QAEHPADHH@Z": 0x00430150,
    "?GameManager_CutChain@@YIXXZ": 0x0041A827,
    "?GameManager_RegisterChain@@YIHXZ": 0x0041B9D0,
    "?RegisterState9Object@@YIHXZ": 0x0040F080,
    "?RegisterStateObject@@YIHH@Z": 0x0042AB13,
    "?ResetForSupervisorFrame@AnmManager@@QAEXXZ": 0x0042F770,
    "?ResetSession@SupervisorNetworkState@@QAEHXZ": 0x00432F70,
    "?ResetSupervisorServiceChannels@@YIXXZ": 0x0042E9C0,
    "?ServiceSupervisorResources@AnmManagerSupervisorView@?ANON@@QAEHXZ": 0x0043CDD0,
    "?SupervisorNetworkMarkInactive@@YIXPAX@Z": 0x0042E9B0,
    "?SupervisorServiceUpdate@@YIHPAVSupervisor@@@Z": 0x00430AA0,
    "?SupervisorSubthreadIsRunning@@YIHPAVSupervisor@@@Z": 0x0042ED70,
    "?UpdateLoadingVms@AnmManagerSupervisorView@?ANON@@QAEXPAXH@Z": 0x00439560,
    "?UpdateSupervisorFrame@SoundPlayerSupervisorView@?ANON@@QAEXXZ": 0x0042FBE0,
    "?g_AnmManager@@3PAVAnmManager@@A": 0x004DC550,
    "?g_ScreenEffectCounter@@3HA": 0x004AC884,
    "?g_SoundPlayerSupervisorView@@3USoundPlayerSupervisorView@?ANON@@A": 0x004DC698,
    "?g_Supervisor@@3VSupervisor@@A": 0x004B3100,
    "?g_SupervisorFrameOffsets@@3PAUSupervisorFrameOffset@?ANON@@A": 0x004B3260,
    "?g_SupervisorLoadingVms@@3PAEA": 0x004B38B0,
    "?g_SupervisorNetworkState@@3PAUSupervisorNetworkState@@A": 0x004B42D0,
    "?g_SupervisorState794@@3HA": 0x004B3894,
    "?g_VersionString@@3PADA": 0x0048F1C4,
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


def normalize(symbol):
    return re.sub(r"\?A0x[0-9a-f]+", "?ANON", symbol)


def bind(raw, relocations):
    linked = bytearray(raw)
    rows = []
    occupied = set()
    table_starts = []
    for row in relocations:
        offset = row["offset"]
        require(0 <= offset <= len(raw) - 4, "field outside complete owner")
        require(not occupied.intersection(range(offset, offset + 4)), "overlapping fields")
        occupied.update(range(offset, offset + 4))
        require(row["type"] in ("REL32", "DIR32"), "unsupported relocation type")
        local = row["symbol_section"] == row["owner_section"]
        if local:
            value = row["symbol_value"] - row["owner_value"]
            require(0 <= value < len(raw), "local label outside owner")
            destination = BASE + value + row["addend"]
            if row["type"] == "DIR32" and value > offset:
                table_starts.append(value)
        else:
            symbol = normalize(row["symbol"])
            require(symbol in DESTINATIONS, "unreviewed external symbol: " + symbol)
            destination = DESTINATIONS[symbol] + row["addend"]
        encoded = destination - (BASE + offset + 4 if row["type"] == "REL32" else 0)
        struct.pack_into("<I", linked, offset, encoded & 0xFFFFFFFF)
        rows.append(dict(row, destination=destination, local=local))
    require(bool(table_starts), "no independently located switch artifacts")
    return bytes(linked), rows, min(table_starts)


def decode(raw, table_start, md, x86):
    instructions = list(md.disasm(raw[:table_start], BASE))
    require(sum(i.size for i in instructions) == table_start, "incomplete code decoding")
    fields = []
    data_references = set()
    calls = []
    branches = []
    starts = {i.address for i in instructions}
    for ins in instructions:
        if ins.mnemonic == "call":
            operand = ins.operands[0]
            if operand.type == x86.X86_OP_IMM:
                fields.append([ins.address - BASE + ins.imm_offset, "REL32", operand.imm])
                calls.append(dict(address=ins.address, destination=operand.imm))
            else:
                require(operand.type == x86.X86_OP_MEM, "unreviewed indirect call form")
                calls.append(dict(address=ins.address, indirect=ins.op_str))
        for operand in ins.operands:
            if operand.type == x86.X86_OP_MEM and 0x00400000 <= operand.mem.disp < 0x004E7000:
                fields.append([ins.address - BASE + ins.disp_offset, "DIR32", operand.mem.disp])
                if BASE + table_start <= operand.mem.disp < BASE + len(raw):
                    data_references.add(operand.mem.disp - BASE)
            if operand.type == x86.X86_OP_IMM and 0x00400000 <= operand.imm < 0x004E7000:
                if ins.mnemonic != "call" and not ins.mnemonic.startswith("j"):
                    fields.append([ins.address - BASE + ins.imm_offset, "DIR32", operand.imm])
        if ins.mnemonic.startswith("j") and ins.operands[0].type == x86.X86_OP_IMM:
            destination = ins.operands[0].imm
            require(destination in starts, "branch outside instruction boundaries")
            branches.append([ins.address, destination])
    tables = []
    table_positions = set()
    for ins in instructions:
        if ins.mnemonic != "jmp" or ins.operands[0].type != x86.X86_OP_MEM:
            continue
        start = ins.operands[0].mem.disp - BASE
        require(start in data_references, "unreviewed indirect jump form")
        end = min([p for p in data_references if p > start] + [len(raw)])
        require((end - start) % 4 == 0, "incomplete switch table")
        entries = []
        for position in range(start, end, 4):
            require(position not in table_positions, "overlapping switch tables")
            table_positions.add(position)
            destination = struct.unpack_from("<I", raw, position)[0]
            require(destination in starts, "table entry outside instruction boundaries")
            fields.append([position, "DIR32", destination])
            entries.append(destination)
        tables.append(dict(instruction=ins.address, offset=start, entries=entries))
    index_starts = data_references - {t["offset"] for t in tables}
    require(len(index_starts) == 1, "unreviewed switch-index ownership")
    index_start = next(iter(index_starts))
    index_end = min([p for p in data_references if p > index_start] + [len(raw)])
    require(index_end - index_start == 12, "unreviewed switch-index extent")
    positions = set()
    for position, kind, destination in fields:
        require(position not in positions, "duplicated decoded field")
        positions.add(position)
    return dict(
        instructions=[dict(address=i.address, bytes=i.bytes.hex(), instruction=i.mnemonic + " " + i.op_str)
                      for i in instructions],
        fields=sorted(fields), branches=branches, calls=calls, tables=tables,
        case_index_alignment=raw[index_start:index_end].hex(),
    )


def inspect(path, symbol=SYMBOL):
    # Optional decoder is imported only for this private-target entrypoint.
    import capstone
    from capstone import x86

    spec = importlib.util.spec_from_file_location("supervisor_coff", ROOT / "scripts/compare-coff-function.py")
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    require(hashlib.sha256(image).hexdigest() == TARGET_SHA256, "wrong TH09 target")
    target = coff.pe_bytes_at(image, BASE, OWNED_SIZE)
    raw, relocations = coff.object_function(Path(path), symbol, include_symbol_locations=True)
    linked, bindings, table_start = bind(raw, relocations)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    candidate = decode(linked, table_start, md, x86)
    wanted = decode(target, CODE_SIZE, md, x86)
    require(len(wanted["fields"]) == 92, "unexpected target operand inventory")
    require(candidate["fields"] == sorted([r["offset"], r["type"], r["destination"]] for r in bindings),
            "candidate relocation/operand inventory differs")
    external = lambda rows, size: Counter((kind, destination) for p, kind, destination in rows
                                          if not BASE <= destination < BASE + size)
    target_external = external(wanted["fields"], OWNED_SIZE)
    candidate_external = external(candidate["fields"], len(linked))
    differences = [n for n, (a, b) in enumerate(zip(linked, target)) if a != b]
    report = dict(
        object=str(path), object_sha256=hashlib.sha256(Path(path).read_bytes()).hexdigest(), symbol=symbol,
        target_sha256=TARGET_SHA256, raw_sha256=hashlib.sha256(raw).hexdigest(),
        linked_sha256=hashlib.sha256(linked).hexdigest(), target_address=BASE,
        target_authored_bytes=CODE_SIZE, target_owned_bytes=OWNED_SIZE,
        candidate_owned_bytes=len(linked), candidate_pre_table_bytes=table_start,
        complete_equal=linked == target, overlapping_differences=len(differences),
        absent_bytes=max(0, OWNED_SIZE - len(linked)), excess_bytes=max(0, len(linked) - OWNED_SIZE),
        first_difference_offset=differences[0] if differences else None,
        target_fields=len(wanted["fields"]), candidate_fields=len(bindings),
        target_calls=len(wanted["calls"]), candidate_calls=len(candidate["calls"]),
        external_field_inventory_equal=candidate_external == target_external,
        missing_external_fields=[[kind, destination, count] for (kind, destination), count
                                 in (target_external - candidate_external).items()],
        extra_external_fields=[[kind, destination, count] for (kind, destination), count
                               in (candidate_external - target_external).items()],
        candidate_instructions=len(candidate["instructions"]), target_instructions=len(wanted["instructions"]),
        candidate_table_counts=[len(t["entries"]) for t in candidate["tables"]],
        target_table_counts=[len(t["entries"]) for t in wanted["tables"]],
        case_index_alignment_equal=candidate["case_index_alignment"] == wanted["case_index_alignment"],
        bindings=bindings, candidate=candidate, target=wanted,
    )
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    parser.add_argument("--symbol", default=SYMBOL, help="actual observed COFF owner symbol")
    parser.add_argument("--full", action="store_true", help="include every operand, instruction and table entry")
    args = parser.parse_args()
    try:
        report = inspect(args.object, args.symbol)
    except (OSError, ValueError, ImportError, KeyError, struct.error) as exc:
        print(json.dumps(dict(result="error", error=str(exc)), indent=2))
        return 2
    exact = report["complete_equal"]
    if not args.full:
        report = {key: value for key, value in report.items() if key not in ("bindings", "candidate", "target")}
    print(json.dumps(report, indent=2))
    return 0 if exact else 1


if __name__ == "__main__":
    raise SystemExit(main())
