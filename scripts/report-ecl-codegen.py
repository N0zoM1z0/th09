#!/usr/bin/env python3
"""Report the bounded VC7.1 RunEcl code/table/aggregate-call oracle."""

from __future__ import annotations

import argparse
from collections import Counter
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
TARGET_DIRECT_CALL_COUNT = 376
TARGET_INDIRECT_CALL_COUNT = 3
TARGET_STACK_FRAME_SIZE = 0x168
TARGET_SPAWN_ENEMY_CALL_COUNT = 1
SPAWN_ENEMY_SYMBOL = (
    "?SpawnEnemy@EnemyManagerView@@QAEPAUEnemyView@@"
    "FPAUEnemyFloat3@@HCHPAHH@Z"
)
EXPECTED_RESOLVER_CALLS = {
    "?ResolveInt@Th09EclRunControl@@YIHPAUEnemyView@@H@Z": 131,
    "?ResolveFloat@Th09EclRunControl@@YIMPAUEnemyView@@M@Z": 100,
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


def report(object_path: Path) -> dict[str, object]:
    compare = load_compare_module()
    compare.verified_target()
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

    logical_size = int(compiler_tables[0]["offset"])
    if len(code) - logical_size != expected_table_count * 4:
        raise ValueError("RunEcl candidate has bytes after the two compiler tables")

    direct_calls = Counter(
        str(relocation["symbol"])
        for relocation in relocations
        if relocation["type"] == "REL32"
    )
    direct_call_count = sum(direct_calls.values())
    if direct_call_count != TARGET_DIRECT_CALL_COUNT:
        raise ValueError(
            "RunEcl direct-call count differs: "
            f"{direct_call_count}/{TARGET_DIRECT_CALL_COUNT}"
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

    return {
        "target": {
            "logical_code_size": TARGET_LOGICAL_SIZE,
            "physical_code_and_tables_size": TARGET_PHYSICAL_SIZE,
            "direct_calls": TARGET_DIRECT_CALL_COUNT,
            "indirect_calls": TARGET_INDIRECT_CALL_COUNT,
            "stack_frame_size": TARGET_STACK_FRAME_SIZE,
            "spawn_enemy_call_sites": TARGET_SPAWN_ENEMY_CALL_COUNT,
            "easing_table_entries": EASING_TABLE_COUNT,
            "opcode_table_entries": OPCODE_TABLE_COUNT,
        },
        "candidate": {
            "object": str(object_path),
            "logical_code_size": logical_size,
            "physical_code_and_tables_size": len(code),
            "logical_size_gap": TARGET_LOGICAL_SIZE - logical_size,
            "physical_size_gap": TARGET_PHYSICAL_SIZE - len(code),
            "direct_calls": direct_call_count,
            "stack_frame_size": stack_frame_size,
            "stack_frame_gap": TARGET_STACK_FRAME_SIZE - stack_frame_size,
            "spawn_enemy_call_sites": spawn_enemy_calls,
            "relocations": len(relocations),
            "dir32_relocations": sum(
                relocation["type"] == "DIR32" for relocation in relocations
            ),
            "resolver_calls": resolver_calls,
            "compiler_tables": {
                "offset": logical_size,
                "entries": len(compiler_tables),
                "easing_entries": EASING_TABLE_COUNT,
                "opcode_entries": OPCODE_TABLE_COUNT,
            },
        },
        "status": "NON-EXACT",
        "known_callsite_mismatches": {
            "SpawnEnemy": {
                "target": TARGET_SPAWN_ENEMY_CALL_COUNT,
                "candidate": spawn_enemy_calls,
                "reason": (
                    "target opcodes 93/94 fold into one shared member-call tail; "
                    "candidate emits one member call per lexical handler"
                ),
            }
        },
        "claim": (
            "aggregate direct-call count and four operand-resolver "
            "multiplicities match; callee identity/site folding, stack/local "
            "layout, code-block order, relocations and bytes remain open"
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
            f"{candidate['logical_code_size']}/{TARGET_LOGICAL_SIZE} logical, "
            f"{candidate['physical_code_and_tables_size']}/"
            f"{TARGET_PHYSICAL_SIZE} physical, "
            f"{candidate['direct_calls']}/{TARGET_DIRECT_CALL_COUNT} aggregate "
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
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
