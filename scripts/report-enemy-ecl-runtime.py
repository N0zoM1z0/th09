#!/usr/bin/env python3
"""Report the bounded VC7.1 post-ECL Enemy owner Oracle."""

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
MOVEMENT_SYMBOL = "?UpdateMovement@EnemyView@@QAEXXZ"
SHOT_ANM_SYMBOL = "?UpdateShotAndAnm@EnemyView@@QAEXXZ"
ANM_HELPER_SYMBOL = (
    "?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z"
)

TARGET_MOVEMENT_LOGICAL_SIZE = 0x3C7
TARGET_SHOT_ANM_SIZE = 0x15D
TARGET_MOVEMENT_DIRECT_CALLS = 21
TARGET_MOVEMENT_INDIRECT_CALLS = 1
TARGET_SHOT_ANM_DIRECT_CALLS = 9
MOVEMENT_EASING_TABLE_ENTRIES = 6

# The retained natural member-source candidate differs only at the still-open
# compiler-private entry ABI and its resulting register save/restore shape.
EXPECTED_MOVEMENT_LOGICAL_SIZE = 0x3C8
EXPECTED_MOVEMENT_PHYSICAL_SIZE = 0x3E0
EXPECTED_SHOT_ANM_SIZE = 0x165


def load_compare_module():
    spec = importlib.util.spec_from_file_location(
        "th09_compare_coff", COMPARE_SCRIPT
    )
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {COMPARE_SCRIPT}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def report(object_path: Path) -> dict[str, object]:
    compare = load_compare_module()
    compare.verified_target()
    movement, movement_relocations = compare.object_function(
        object_path, MOVEMENT_SYMBOL
    )
    shot_anm, shot_anm_relocations = compare.object_function(
        object_path, SHOT_ANM_SYMBOL
    )

    movement_table = [
        relocation
        for relocation in movement_relocations
        if relocation["type"] == "DIR32"
        and EXPECTED_MOVEMENT_LOGICAL_SIZE
        <= int(relocation["offset"])
        < EXPECTED_MOVEMENT_PHYSICAL_SIZE
    ]
    if len(movement_table) != MOVEMENT_EASING_TABLE_ENTRIES:
        raise ValueError(
            "movement easing table differs: "
            f"{len(movement_table)}/{MOVEMENT_EASING_TABLE_ENTRIES}"
        )
    if [int(row["offset"]) for row in movement_table] != list(
        range(
            EXPECTED_MOVEMENT_LOGICAL_SIZE,
            EXPECTED_MOVEMENT_PHYSICAL_SIZE,
            4,
        )
    ):
        raise ValueError("movement easing table is not the expected DIR32 run")

    movement_direct = Counter(
        str(relocation["symbol"])
        for relocation in movement_relocations
        if relocation["type"] == "REL32"
    )
    shot_anm_direct = Counter(
        str(relocation["symbol"])
        for relocation in shot_anm_relocations
        if relocation["type"] == "REL32"
    )
    if sum(movement_direct.values()) != TARGET_MOVEMENT_DIRECT_CALLS:
        raise ValueError("movement direct-call count differs from target")
    if sum(shot_anm_direct.values()) != TARGET_SHOT_ANM_DIRECT_CALLS:
        raise ValueError("shot/ANM direct-call count differs from target")
    if movement_direct[ANM_HELPER_SYMBOL] != 1:
        raise ValueError("movement owner must call the ANM helper once")
    if shot_anm_direct[ANM_HELPER_SYMBOL] != 5:
        raise ValueError("shot/ANM owner must call the ANM helper five times")

    if len(movement) != EXPECTED_MOVEMENT_PHYSICAL_SIZE:
        raise ValueError(
            f"movement physical size differs: {len(movement)}/"
            f"{EXPECTED_MOVEMENT_PHYSICAL_SIZE}"
        )
    if len(shot_anm) != EXPECTED_SHOT_ANM_SIZE:
        raise ValueError(
            f"shot/ANM size differs: {len(shot_anm)}/"
            f"{EXPECTED_SHOT_ANM_SIZE}"
        )

    return {
        "target": {
            "movement_logical_size": TARGET_MOVEMENT_LOGICAL_SIZE,
            "movement_direct_calls": TARGET_MOVEMENT_DIRECT_CALLS,
            "movement_indirect_calls": TARGET_MOVEMENT_INDIRECT_CALLS,
            "shot_anm_size": TARGET_SHOT_ANM_SIZE,
            "shot_anm_direct_calls": TARGET_SHOT_ANM_DIRECT_CALLS,
        },
        "candidate": {
            "object": str(object_path),
            "movement_logical_size": EXPECTED_MOVEMENT_LOGICAL_SIZE,
            "movement_physical_size": len(movement),
            "movement_easing_table_entries": len(movement_table),
            "movement_direct_calls": sum(movement_direct.values()),
            "movement_relocations": len(movement_relocations),
            "movement_sha256": hashlib.sha256(
                movement[:EXPECTED_MOVEMENT_LOGICAL_SIZE]
            ).hexdigest(),
            "shot_anm_size": len(shot_anm),
            "shot_anm_direct_calls": sum(shot_anm_direct.values()),
            "shot_anm_relocations": len(shot_anm_relocations),
            "shot_anm_sha256": hashlib.sha256(shot_anm).hexdigest(),
            "anm_helper_calls": {
                "movement": movement_direct[ANM_HELPER_SYMBOL],
                "shot_anm": shot_anm_direct[ANM_HELPER_SYMBOL],
            },
        },
        "status": "NON-EXACT",
        "known_abi_gap": {
            "target": "compiler-private Enemy in ESI/EDI",
            "candidate": "natural __thiscall EnemyView member",
            "movement_logical_size_gap": (
                EXPECTED_MOVEMENT_LOGICAL_SIZE - TARGET_MOVEMENT_LOGICAL_SIZE
            ),
            "shot_anm_size_gap": (
                EXPECTED_SHOT_ANM_SIZE - TARGET_SHOT_ANM_SIZE
            ),
        },
        "claim": (
            "complete maintained behavior and target-equal direct-call totals; "
            "entry ABI, registers, relocations and bytes remain non-exact"
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
        print(f"post-ECL owner report failed: {error}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        candidate = result["candidate"]
        print(
            "post-ECL owners NON-EXACT: movement "
            f"{candidate['movement_logical_size']}/"
            f"{TARGET_MOVEMENT_LOGICAL_SIZE} logical + "
            f"{candidate['movement_easing_table_entries']} table entries; "
            f"shot/ANM {candidate['shot_anm_size']}/"
            f"{TARGET_SHOT_ANM_SIZE} bytes"
        )
        print(
            "direct calls: movement "
            f"{candidate['movement_direct_calls']}/"
            f"{TARGET_MOVEMENT_DIRECT_CALLS}, shot/ANM "
            f"{candidate['shot_anm_direct_calls']}/"
            f"{TARGET_SHOT_ANM_DIRECT_CALLS}; ANM helper sites 1+5"
        )
        print("remaining gap: compiler-private ESI/EDI entry ABI")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
