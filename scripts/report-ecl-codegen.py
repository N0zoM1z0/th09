#!/usr/bin/env python3
"""Report the bounded VC7.1 RunEcl code/table/aggregate-call oracle."""

from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[1]
COMPARE_SCRIPT = ROOT / "scripts" / "compare-coff-function.py"
RUN_ECL_SYMBOL = "?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z"
TARGET_LOGICAL_SIZE = 0x39C8
TARGET_PHYSICAL_SIZE = 0x3CCC
EASING_TABLE_COUNT = 6
OPCODE_TABLE_COUNT = 187
TARGET_DIRECT_CALL_COUNT = 375
CANDIDATE_DIRECT_CALL_COUNT = 375
TARGET_INDIRECT_CALL_COUNT = 4
TARGET_STACK_FRAME_SIZE = 0x168
TARGET_SPAWN_ENEMY_CALL_COUNT = 1
SPAWN_ENEMY_SYMBOL = (
    "?SpawnEnemy@EnemyManagerView@@QAEPAUEnemyView@@"
    "FPAUEnemyFloat3@@HCHPAHH@Z"
)
FLOAT3_ADD_SYMBOL = "??HFloat3@@QBE?AU0@ABU0@@Z"
TARGET_FIRST_WORLD_RESULT_HOME = "-0x168"
TARGET_FIRST_WORLD_COPY_SOURCE = "returned_eax"
EXPECTED_RESOLVER_CALLS = {
    "?ResolveInt@Th09EclRunControl@@YIHPAUEnemyView@@H@Z": 131,
    "?ResolveFloat@EnemyView@@QAEMM@Z": 100,
    "?ResolveIntLValue@Th09EclRunControl@@YIPAHPAUEnemyView@@PAHGH@Z": 17,
    "?ResolveFloatLValue@Th09EclRunControl@@YIPAMPAUEnemyView@@PAMGH@Z": 24,
}


def load_compare_module():
    spec = importlib.util.spec_from_file_location("th09_compare_coff", COMPARE_SCRIPT)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {COMPARE_SCRIPT}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def contiguous_dir32_runs(relocations: list[dict[str, object]]):
    runs: list[list[dict[str, object]]] = []
    current: list[dict[str, object]] = []
    for relocation in relocations:
        offset = int(relocation["offset"])
        if (
            relocation["type"] == "DIR32"
            and (not current or offset == int(current[-1]["offset"]) + 4)
        ):
            current.append(relocation)
        else:
            if current:
                runs.append(current)
            current = [relocation] if relocation["type"] == "DIR32" else []
    if current:
        runs.append(current)
    return runs


def vc71_stack_frame_size(code: bytearray) -> int:
    prefix = bytes(code[:5])
    if prefix != b"\x55\x8b\xec\x81\xec":
        raise ValueError(
            f"RunEcl candidate has unexpected VC7.1 prologue: {prefix.hex()}"
        )
    return int.from_bytes(code[5:9], "little")


def code_and_alignment_sizes(code: bytes) -> tuple[int, int]:
    """Decode the full pre-table extent and review post-return alignment."""
    try:
        import capstone
    except ImportError as error:
        raise ValueError("RunEcl extent decoding requires optional Capstone") from error

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, 0))
    if sum(instruction.size for instruction in instructions) != len(code):
        raise ValueError("incomplete RunEcl pre-table instruction decoding")
    returns = [index for index, instruction in enumerate(instructions)
               if instruction.mnemonic == "ret"]
    if not returns:
        raise ValueError("RunEcl code has no terminal return")
    final_return = returns[-1]
    terminal = instructions[final_return]
    code_size = terminal.address + terminal.size
    alignment_size = len(code) - code_size
    if alignment_size > 15:
        raise ValueError("RunEcl post-return alignment exceeds the reviewed bound")
    for instruction in instructions[final_return + 1:]:
        operands = instruction.operands
        plain_nop = instruction.mnemonic == "nop"
        self_lea = (
            instruction.mnemonic == "lea" and len(operands) == 2
            and operands[0].type == capstone.x86.X86_OP_REG
            and operands[1].type == capstone.x86.X86_OP_MEM
            and operands[1].mem.base == operands[0].reg
            and operands[1].mem.index == 0 and operands[1].mem.disp == 0
        )
        if not (plain_nop or self_lea):
            raise ValueError("unreviewed instruction after RunEcl terminal return")
    for instruction in instructions[:final_return + 1]:
        if (instruction.mnemonic.startswith("j") and instruction.operands
                and instruction.operands[0].type == capstone.x86.X86_OP_IMM
                and not 0 <= instruction.operands[0].imm < code_size):
            raise ValueError("RunEcl direct branch leaves the decoded code extent")
    return code_size, alignment_size


def first_world_result_shape(
    code: bytearray, relocations: list[dict[str, object]]
) -> dict[str, object]:
    calls = [
        int(relocation["offset"])
        for relocation in relocations
        if relocation["type"] == "REL32"
        and relocation["symbol"] == FLOAT3_ADD_SYMBOL
    ]
    if not calls:
        return {"status": "unrecognized", "reason": "no Float3 addition call"}
    field = min(calls)
    lea = field - 8
    if (
        lea < 0
        or bytes(code[lea : lea + 2]) != b"\x8d\x85"
        or bytes(code[field - 2 : field + 1]) != b"\x50\xe8\x00"
    ):
        return {"status": "unrecognized", "reason": "first result setup changed"}
    displacement = int.from_bytes(code[lea + 2 : lea + 6], "little", signed=True)
    after_call = field + 4
    if bytes(code[after_call : after_call + 2]) == b"\x8b\x10":
        copy_source = "returned_eax"
    elif bytes(code[after_call : after_call + 2]) == b"\x8b\x95":
        copy_source = "stack_local"
    else:
        return {"status": "unrecognized", "reason": "first result copy changed"}
    home = f"-0x{-displacement:X}" if displacement < 0 else f"+0x{displacement:X}"
    return {
        "status": "observed",
        "operator_plus_relocation": field,
        "home": home,
        "copy_source": copy_source,
    }


def report(object_path: Path) -> dict[str, object]:
    compare = load_compare_module()
    image = compare.verified_target()
    code, relocations = compare.object_function(object_path, RUN_ECL_SYMBOL)
    runs = contiguous_dir32_runs(relocations)
    if not runs:
        raise ValueError("RunEcl candidate has no DIR32 relocation run")
    compiler_tables = max(runs, key=len)
    expected_table_count = EASING_TABLE_COUNT + OPCODE_TABLE_COUNT
    if len(compiler_tables) != expected_table_count:
        raise ValueError(
            "RunEcl compiler-table relocation count differs: "
            f"{len(compiler_tables)}/{expected_table_count}"
        )

    table_offset = int(compiler_tables[0]["offset"])
    if len(code) - table_offset != expected_table_count * 4:
        raise ValueError("RunEcl candidate has bytes after the two compiler tables")
    logical_size, alignment_size = code_and_alignment_sizes(bytes(code[:table_offset]))
    target_size, target_alignment = code_and_alignment_sizes(
        compare.pe_bytes_at(image, 0x004086C0, TARGET_LOGICAL_SIZE)
    )
    if target_size != TARGET_LOGICAL_SIZE or target_alignment != 0:
        raise ValueError("reviewed RunEcl target code extent differs")

    direct_calls = Counter(
        str(relocation["symbol"])
        for relocation in relocations
        if relocation["type"] == "REL32"
    )
    direct_call_count = sum(direct_calls.values())
    if direct_call_count != CANDIDATE_DIRECT_CALL_COUNT:
        raise ValueError(
            "RunEcl direct-call count differs: "
            f"{direct_call_count}/{CANDIDATE_DIRECT_CALL_COUNT} expected candidate"
        )
    resolver_calls = {
        symbol: direct_calls[symbol]
        for symbol in EXPECTED_RESOLVER_CALLS
    }
    if resolver_calls != EXPECTED_RESOLVER_CALLS:
        raise ValueError(
            f"RunEcl resolver multiplicities differ: {resolver_calls!r}"
        )
    if any("AssignFlagField" in symbol for symbol in direct_calls):
        raise ValueError("RunEcl candidate leaked synthetic AssignFlagField calls")

    stack_frame_size = vc71_stack_frame_size(code)
    spawn_enemy_calls = direct_calls[SPAWN_ENEMY_SYMBOL]
    first_world_result = first_world_result_shape(code, relocations)

    return {
        "target": {
            "logical_code_size": TARGET_LOGICAL_SIZE,
            "physical_code_and_tables_size": TARGET_PHYSICAL_SIZE,
            "direct_calls": TARGET_DIRECT_CALL_COUNT,
            "indirect_calls": TARGET_INDIRECT_CALL_COUNT,
            "stack_frame_size": TARGET_STACK_FRAME_SIZE,
            "spawn_enemy_call_sites": TARGET_SPAWN_ENEMY_CALL_COUNT,
            "first_world_result_home": TARGET_FIRST_WORLD_RESULT_HOME,
            "first_world_copy_source": TARGET_FIRST_WORLD_COPY_SOURCE,
            "easing_table_entries": EASING_TABLE_COUNT,
            "opcode_table_entries": OPCODE_TABLE_COUNT,
        },
        "candidate": {
            "object": str(object_path),
            "raw_function_sha256": hashlib.sha256(code).hexdigest(),
            "logical_code_size": logical_size,
            "physical_code_and_tables_size": len(code),
            "pre_table_size": table_offset,
            "post_return_alignment_size": alignment_size,
            "logical_size_gap": TARGET_LOGICAL_SIZE - logical_size,
            "physical_size_gap": TARGET_PHYSICAL_SIZE - len(code),
            "direct_calls": direct_call_count,
            "stack_frame_size": stack_frame_size,
            "stack_frame_gap": TARGET_STACK_FRAME_SIZE - stack_frame_size,
            "spawn_enemy_call_sites": spawn_enemy_calls,
            "first_world_result": first_world_result,
            "relocations": len(relocations),
            "dir32_relocations": sum(
                relocation["type"] == "DIR32" for relocation in relocations
            ),
            "resolver_calls": resolver_calls,
            "compiler_tables": {
                "offset": table_offset,
                "entries": len(compiler_tables),
                "easing_entries": EASING_TABLE_COUNT,
                "opcode_entries": OPCODE_TABLE_COUNT,
            },
        },
        "status": "NON-EXACT",
        # Aggregate relocation/name counts are not a complete callee-identity
        # replay. In particular, unknown aliases and indirect calls are not
        # decoded or independently bound by this diagnostic.
        "known_callsite_mismatches": None,
        "call_identity_validation": "not_performed",
        "claim": (
            "candidate REL32 relocation count and four named operand-resolver "
            "multiplicities checked; complete direct-call destinations and "
            "indirect-call counts are not validated by this diagnostic. "
            "Stack/local layout, code-block order, relocations and bytes remain open"
        ),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    object_path = args.object.resolve()
    if not object_path.is_file():
        print(f"missing object: {object_path}", file=sys.stderr)
        return 1
    try:
        result = report(object_path)
    except (OSError, ValueError) as error:
        print(f"ECL codegen report failed: {error}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        candidate = result["candidate"]
        print(
            "RunEcl NON-EXACT: "
            f"{candidate['logical_code_size']}/{TARGET_LOGICAL_SIZE} code "
            f"+ {candidate['post_return_alignment_size']} alignment, "
            f"{candidate['physical_code_and_tables_size']}/"
            f"{TARGET_PHYSICAL_SIZE} physical, "
            f"{candidate['direct_calls']}/{TARGET_DIRECT_CALL_COUNT} immediate "
            "direct calls"
        )
        print(
            "operand resolvers: "
            + ", ".join(
                f"{symbol.split('@', 1)[0][1:]}={count}"
                for symbol, count in candidate["resolver_calls"].items()
            )
        )
        print(
            "stack frame: "
            f"0x{candidate['stack_frame_size']:X}/0x{TARGET_STACK_FRAME_SIZE:X}; "
            "SpawnEnemy call sites: "
            f"{candidate['spawn_enemy_call_sites']}/"
            f"{TARGET_SPAWN_ENEMY_CALL_COUNT}"
        )
        print("compiler tables: 6 easing + 187 opcode entries")
        print(
            "first world result: "
            f"{candidate['first_world_result']} "
            f"(target home {TARGET_FIRST_WORLD_RESULT_HOME}, "
            f"copy {TARGET_FIRST_WORLD_COPY_SOURCE})"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
