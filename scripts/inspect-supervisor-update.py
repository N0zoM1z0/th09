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
OWNED_SHA256 = "c3ccfdefb056e202f64d3f31a313e200b1494fa49184d0fcd5ea3c6de248327c"

# TH09-local identities reviewed through callback registration, target operands
# and callee/global evidence. Source aliases and view names remain provisional.
# Title, Ending and GameManager references use their maintained native symbols.
# Their shared declarations do not prove complete product/link closure.
DESTINATIONS = {
    "?CheckVersion@SupervisorVersionView@@QAEHPBDHH@Z": 0x00430150,
    "?GameManager_CutChain@@YIXXZ": 0x0041A827,
    "?RegisterChain@GameManagerSetupLayout@@SIHXZ": 0x0041B9D0,
    "?RegisterChain@Ending@@SIHXZ": 0x0040F080,
    "?RegisterTitleScreen@@YIHH@Z": 0x0042AB13,
    "?ResetForSupervisorFrame@AnmManager@@QAEXXZ": 0x0042F770,
    "?ResetSession@SupervisorNetworkState@@QAEHXZ": 0x00432F70,
    "?ResetSupervisorServiceChannels@@YIXXZ": 0x0042E9C0,
    "?ServicePreloadedAnims@AnmManager@@QAEHXZ": 0x0043CDD0,
    "?SupervisorNetworkMarkInactive@@YIXPAX@Z": 0x0042E9B0,
    "?SupervisorServiceUpdate@@YIHPAVSupervisor@@@Z": 0x00430AA0,
    "?SupervisorSubthreadIsRunning@@YIHPAVSupervisor@@@Z": 0x0042ED70,
    "?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z": 0x00439560,
    "?UpdateFades@SoundPlayer@@QAEXXZ": 0x0042FBE0,
    "?g_AnmManager@@3PAVAnmManager@@A": 0x004DC550,
    "?g_ScreenEffectCounter@@3HA": 0x004AC884,
    "?g_SoundPlayer@@3VSoundPlayer@@A": 0x004DC698,
    "?g_Supervisor@@3VSupervisor@@A": 0x004B3100,
    "?g_SupervisorLoadingVms@@3PAUAnmVm@@A": 0x004B38B0,
    "?g_SupervisorNetworkState@@3PAUSupervisorNetworkState@@A": 0x004B42D0,
    "??_C@_05EJHOHPCC@0150a?$AA@": 0x0048F1C4,
}


def require(condition, message):
    if not condition:
        raise ValueError(message)


VERSION_LITERAL_SYMBOL = "??_C@_05EJHOHPCC@0150a?$AA@"
VERSION_LITERAL_BYTES = b"0150a\0"
VERSION_LITERAL_TARGET = 0x0048F1C4


def check_version_literal(data, relocations, original):
    """Verify the actual referenced COFF definition, not merely its name/address."""
    rows = [row for row in relocations if row["symbol"] == VERSION_LITERAL_SYMBOL]
    require(len(rows) == 1, "expected one native version literal reference")
    row = rows[0]
    require(row["type"] == "DIR32" and row["addend"] == 0,
            "version literal reference type/addend differs")
    require(len(data) >= 20, "truncated version literal COFF header")
    machine, count = struct.unpack_from("<HH", data)
    optional = struct.unpack_from("<H", data, 16)[0]
    section, value = row.get("symbol_section"), row.get("symbol_value")
    require(machine == 0x14C and optional == 0 and type(section) is int and
            1 <= section <= count and type(value) is int and value >= 0,
            "version literal lacks an actual i386 COFF definition")
    header = 20 + (section - 1) * 40
    require(header + 40 <= len(data), "truncated version literal section header")
    length, start = struct.unpack_from("<II", data, header + 16)
    relocations_count = struct.unpack_from("<H", data, header + 32)[0]
    flags = struct.unpack_from("<I", data, header + 36)[0]
    require((flags & 0x40000040) == 0x40000040 and
            not flags & (0x80000000 | 0x20000000 | 0x20 | 0x80) and relocations_count == 0,
            "version literal is not relocation-free read-only initialized data")
    require(start >= 20 + count * 40 and value + len(VERSION_LITERAL_BYTES) <= length and
            start + length <= len(data), "version literal exceeds actual section")
    payload = data[start + value:start + value + len(VERSION_LITERAL_BYTES)]
    require(payload == VERSION_LITERAL_BYTES and original == VERSION_LITERAL_BYTES,
            "actual source/original version literal payload differs")
    return dict(symbol=row["symbol"], target=VERSION_LITERAL_TARGET, size=len(payload),
                symbol_section=section, symbol_value=value, addend=row["addend"],
                payload_hex=payload.hex(), payload_sha256=hashlib.sha256(payload).hexdigest())


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


# Reviewed TH09 target calls RegisterTitleScreen (source alias
# RegisterStateObject) only ONCE: mode 0 and mode 1 both reach that physical
# call via independently observed ECX=0/1 input paths. A codegen candidate
# with three physical calls is nonexact even if all are behaviorally valid.
TITLE_REGISTER_TARGET = 0x0042AB13
SUBTHREAD_PREDICATE_TARGET = 0x0042ED70


def count_direct_calls_to(calls, destination):
    """Count actual decoded direct calls, never branch/indirect placeholders."""
    require(isinstance(destination, int) and destination > 0, "unreviewed destination")
    count = 0
    for entry in calls:
        require(isinstance(entry, dict) and isinstance(entry.get("address"), int),
                "unreviewed decoded call entry")
        if "destination" in entry:
            require(isinstance(entry["destination"], int), "bad direct callee")
            count += int(entry["destination"] == destination)
        else:
            require("indirect" in entry, "unreviewed call identity")
    return count


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
    require(hashlib.sha256(target).hexdigest() == OWNED_SHA256, "wrong complete Supervisor owner")
    raw, relocations = coff.object_function(Path(path), symbol, include_symbol_locations=True)
    version_literal = check_version_literal(Path(path).read_bytes(), relocations,
        coff.pe_bytes_at(image, VERSION_LITERAL_TARGET, len(VERSION_LITERAL_BYTES)))
    linked, bindings, table_start = bind(raw, relocations)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    candidate = decode(linked, table_start, md, x86)
    wanted = decode(target, CODE_SIZE, md, x86)
    require(len(wanted["fields"]) == 92, "unexpected target operand inventory")
    original_register_calls = count_direct_calls_to(wanted["calls"], TITLE_REGISTER_TARGET)
    require(original_register_calls == 1, "original title registration call count drifted")
    candidate_register_calls = count_direct_calls_to(candidate["calls"], TITLE_REGISTER_TARGET)
    original_subthread_calls = count_direct_calls_to(wanted["calls"], SUBTHREAD_PREDICATE_TARGET)
    require(original_subthread_calls == 2, "original subthread predicate call count drifted")
    candidate_subthread_calls = count_direct_calls_to(candidate["calls"], SUBTHREAD_PREDICATE_TARGET)
    require(candidate["fields"] == sorted([r["offset"], r["type"], r["destination"]] for r in bindings),
            "candidate relocation/operand inventory differs")
    external = lambda rows, size: Counter((kind, destination) for p, kind, destination in rows
                                          if not BASE <= destination < BASE + size)
    target_external = external(wanted["fields"], OWNED_SIZE)
    candidate_external = external(candidate["fields"], len(linked))
    differences = [n for n, (a, b) in enumerate(zip(linked, target)) if a != b]
    missing = list(range(len(linked), len(target)))
    report = dict(
        diagnostic_only=True, exactness_credit="none",
        object=str(path), object_sha256=hashlib.sha256(Path(path).read_bytes()).hexdigest(), symbol=symbol,
        target_sha256=TARGET_SHA256, original_owned_sha256=OWNED_SHA256,
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        linked_sha256=hashlib.sha256(linked).hexdigest(), target_address=BASE,
        target_authored_bytes=CODE_SIZE, target_owned_bytes=OWNED_SIZE,
        candidate_owned_bytes=len(linked), candidate_pre_table_bytes=table_start,
        complete_equal=linked == target, overlapping_differences=len(differences),
        original_difference_offsets=differences + missing,
        absent_bytes=max(0, OWNED_SIZE - len(linked)), excess_bytes=max(0, len(linked) - OWNED_SIZE),
        first_difference_offset=differences[0] if differences else None,
        target_fields=len(wanted["fields"]), candidate_fields=len(bindings),
        target_calls=len(wanted["calls"]), candidate_calls=len(candidate["calls"]),
        target_title_register_calls=original_register_calls,
        candidate_title_register_calls=candidate_register_calls,
        shared_title_register_call_agree=(original_register_calls == candidate_register_calls),
        target_subthread_predicate_calls=original_subthread_calls,
        candidate_subthread_predicate_calls=candidate_subthread_calls,
        subthread_predicate_calls_agree=(original_subthread_calls == candidate_subthread_calls),
        external_field_inventory_equal=candidate_external == target_external,
        missing_external_fields=[[kind, destination, count] for (kind, destination), count
                                 in (target_external - candidate_external).items()],
        extra_external_fields=[[kind, destination, count] for (kind, destination), count
                               in (candidate_external - target_external).items()],
        candidate_instructions=len(candidate["instructions"]), target_instructions=len(wanted["instructions"]),
        candidate_table_counts=[len(t["entries"]) for t in candidate["tables"]],
        target_table_counts=[len(t["entries"]) for t in wanted["tables"]],
        case_index_alignment_equal=candidate["case_index_alignment"] == wanted["case_index_alignment"],
        version_literal_definition=version_literal,
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
