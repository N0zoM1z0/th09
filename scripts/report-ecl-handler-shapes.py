#!/usr/bin/env python3
"""Report RunEcl opcode-handler instruction-shape drift against verified TH09.

This diagnostic first requires the complete RunEcl CFG/relocation identity audit
to pass.  It then compares each opcode-rooted handler while normalizing only
fields whose identities are independently paired by that audit, plus relative
branch displacements.  Scalar immediates remain byte-significant: e.g.
0x00400000 is a flag mask here, not automatically treated as an image address.

The report is diagnostic only and grants no exactness or partial-byte credit.
"""

import argparse
import difflib
import importlib.util
import json
from pathlib import Path

import capstone

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004086C0
TARGET_CODE_BYTES = 14792
RUN_ECL_SYMBOL = "?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z"


def load_module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def normalize_instruction(ins, owner_base, zero_fields):
    raw = bytearray(ins.bytes)
    start = ins.address - owner_base

    for field in zero_fields:
        if start <= field < start + ins.size:
            rel = field - start
            if rel < 0 or rel + 4 > len(raw):
                raise RuntimeError("paired relocation field crosses instruction")
            raw[rel:rel + 4] = b"\0\0\0\0"

    if (
        ins.mnemonic.startswith("j")
        and len(ins.operands) == 1
        and ins.operands[0].type == capstone.x86.X86_OP_IMM
    ):
        if ins.imm_size <= 0:
            raise RuntimeError("direct branch lacks decoded immediate field")
        raw[ins.imm_offset:ins.imm_offset + ins.imm_size] = bytes(ins.imm_size)

    return bytes(raw)


def normalized_stream(code, owner_base, zero_fields):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(code, owner_base))
    if sum(ins.size for ins in instructions) != len(code):
        raise RuntimeError("incomplete instruction decode")
    return [
        (ins.address - owner_base, normalize_instruction(ins, owner_base, zero_fields))
        for ins in instructions
    ]


def slice_handler(stream, start, end):
    return [raw for offset, raw in stream if start <= offset < end]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "object",
        nargs="?",
        type=Path,
        default=ROOT / "build" / "matching" / "EclManager.obj",
    )
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    compare = load_module(
        "coff_compare", ROOT / "scripts" / "compare-coff-function.py"
    )
    audit_mod = load_module(
        "ecl_audit", ROOT / "scripts" / "audit-ecl-callsite-identities.py"
    )
    boundary = load_module(
        "ecl_boundaries", ROOT / "scripts" / "report-ecl-handler-boundaries.py"
    )

    audit = audit_mod.audit(args.object)
    if not audit["audit_passed"]:
        raise RuntimeError("RunEcl identity audit failed: " + ",".join(audit["failures"]))
    if not audit["complete_graph_shapes_agree"] or not audit["table_roots_agree"]:
        raise RuntimeError("RunEcl graph/table pairing is unavailable")

    candidate_raw, _ = compare.object_function(
        args.object, RUN_ECL_SYMBOL, include_symbol_locations=True
    )
    candidate_code_bytes = audit["candidate_code_bytes"]
    candidate_code = candidate_raw[:candidate_code_bytes]
    target_code = compare.pe_bytes_at(
        compare.verified_target(), BASE, TARGET_CODE_BYTES
    )

    candidate_zero = set()
    target_zero = set()

    for row in audit["direct_calls"]:
        candidate_zero.add(int(row["candidate_offset"]) + 1)
        target_zero.add(int(row["target_address"]) - BASE + 1)

    for row in audit["body_data_fields"]:
        candidate_zero.add(int(row["candidate_offset"]))
        target_zero.add(int(row["target_address"]) - BASE)

    candidate_stream = normalized_stream(candidate_code, BASE, candidate_zero)
    target_stream = normalized_stream(target_code, BASE, target_zero)

    candidate_entries = boundary.candidate_opcode_entries(compare, args.object)
    target_entries = boundary.target_opcode_entries(compare)
    candidate_starts = sorted(set(candidate_entries) | {candidate_code_bytes})
    target_starts = sorted(set(target_entries) | {TARGET_CODE_BYTES})

    regions = []
    seen = set()
    for opcode, (candidate_start, target_start) in enumerate(
        zip(candidate_entries, target_entries), start=1
    ):
        pair = (candidate_start, target_start)
        if pair in seen:
            continue
        seen.add(pair)

        candidate_end = candidate_starts[candidate_starts.index(candidate_start) + 1]
        target_end = target_starts[target_starts.index(target_start) + 1]
        candidate_handler = slice_handler(
            candidate_stream, candidate_start, candidate_end
        )
        target_handler = slice_handler(target_stream, target_start, target_end)

        matcher = difflib.SequenceMatcher(
            a=candidate_handler, b=target_handler, autojunk=False
        )
        matching = sum(block.size for block in matcher.get_matching_blocks())
        exact_shape = candidate_handler == target_handler
        regions.append({
            "opcode": opcode,
            "candidate_offset": candidate_start,
            "target_offset": target_start,
            "candidate_bytes": candidate_end - candidate_start,
            "target_bytes": target_end - target_start,
            "candidate_instructions": len(candidate_handler),
            "target_instructions": len(target_handler),
            "matching_instructions": matching,
            "exact_shape": exact_shape,
        })

    mismatches = [row for row in regions if not row["exact_shape"]]
    payload = {
        "diagnostic_only": True,
        "exactness_credit": "none",
        "object": str(args.object),
        "identity_audit_passed": True,
        "candidate_code_bytes": candidate_code_bytes,
        "target_code_bytes": TARGET_CODE_BYTES,
        "unique_handlers": len(regions),
        "matching_handler_shapes": len(regions) - len(mismatches),
        "mismatching_handler_shapes": len(mismatches),
        "mismatches": mismatches,
    }

    if args.json:
        print(json.dumps(payload, indent=2))
        return

    print(
        "RunEcl normalized handler shapes: "
        f"{payload['matching_handler_shapes']}/{payload['unique_handlers']} match target"
    )
    if not mismatches:
        print("all handler shapes agree (whole-owner exactness still unproven)")
        return

    print("opcode  candidate  target     bytes(C/T)  instructions(match/C/T)")
    for row in mismatches:
        print(
            f"{row['opcode']:>6}  "
            f"0x{row['candidate_offset']:04X}    "
            f"0x{row['target_offset']:04X}    "
            f"{row['candidate_bytes']:>4}/{row['target_bytes']:<4}      "
            f"{row['matching_instructions']:>3}/"
            f"{row['candidate_instructions']}/"
            f"{row['target_instructions']}"
        )


if __name__ == "__main__":
    main()
