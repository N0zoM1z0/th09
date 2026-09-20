#!/usr/bin/env python3
"""Apply reproducible origin reviews within the TH09 game-code linker band."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

from tracking_csv import read_rows, rewrite_selected_rows


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
COMPILER_EVIDENCE_ID = "vc71-generated-helper-review-2026-09-19"
MATH_INTRINSIC_EVIDENCE_ID = "vc71-math-intrinsic-comdat-review-2026-09-20"
FROZEN_GAME_COHORT = ROOT / "config" / "game-origin-review-cohort.txt"
VECTOR_CONSTRUCTOR = "0x00401470"
SCALAR_DELETING_DESTRUCTORS = {
    "0x0042CC30",
    "0x0042CC60",
    "0x00431680",
    "0x00440C20",
    "0x00440C50",
}
MATH_INTRINSICS = {
    "0x00401060": ("@cosf@4", "558becd94508d9ff5dc20400"),
    "0x00401070": ("@sinf@4", "558becd94508d9fe5dc20400"),
    "0x00401080": ("@sqrtf@4", "558becd94508d9fa5dc20400"),
}
MATH_RUNTIME_RECLASSIFICATIONS = {"0x00405710", "0x00436AA0", "0x00436AB0"}
D3DX_HEADER_RECLASSIFICATIONS = {"0x00401290", "0x004012C0", "0x004012E0"}
COMPILER_GENERATED = {VECTOR_CONSTRUCTOR, *SCALAR_DELETING_DESTRUCTORS, *MATH_INTRINSICS}
AUTHORED_EVIDENCE_ID = "game-code-origin-sweep-2026-09-19"
AMBIGUOUS_EVIDENCE_ID = "game-special-member-origin-review-2026-09-19"
GAME_BAND_END = 0x0044E000
EXPECTED_GAME_COHORT = 387
EXPECTED_GAME_COHORT_DIGEST = "8fc4d184e62c0e19d28aa5ca120f158a7f25d5fac28b2735f2372f8db81a7077"
EXPECTED_AUTHORED_DIGEST = "34e71ba5aea3a0f72223997bc6c8e6d24445450d5bd2053d44edd92836cc7aaa"
EXPECTED_AMBIGUOUS_DIGEST = "126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695"
RETAIN_UNKNOWN = {
    "0x0040FCA0",
    "0x00412090", "0x004120A0", "0x004120C0", "0x004120D0", "0x00412100",
    "0x00414FA0", "0x00414FE0", "0x00415020",
    "0x004157A0", "0x004157B0",
    "0x00418750", "0x004187B0",
    "0x0041A470", "0x0041A490", "0x0041CEB0",
    "0x0041EE00", "0x0041EE20", "0x0041EE30",
    "0x0041F010", "0x0041F030",
    "0x00420830", "0x00420C40",
    "0x0042AC30", "0x0042B190", "0x0042F3B0", "0x0042F3F0",
    "0x0043D2B0",
    "0x00435AF0", "0x00435D40", "0x00435D70", "0x00435DA0",
    "0x00435DC0", "0x00435EC0", "0x004343D0",
}
SHARED_OR_FOLDED = {"0x0042F3F0", "0x004343D0", "0x00435EC0", "0x0043D2B0"}
DETAILED_SPECIAL_MEMBER_EVIDENCE = {
    "0x004157A0", "0x004157B0", "0x0042AC30", "0x0042B190", "0x00435DC0"
}
AUTHORED_TAIL_JUMPS = {
    "0x0040F8B0", "0x00416590", "0x00417130", "0x0041F300",
    "0x004324C0", "0x00432EA0", "0x004335D0",
}


def load_script(filename: str, module_name: str):
    path = ROOT / "scripts" / filename
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def rel32_target(address: int, body: bytes, displacement_offset: int) -> int:
    displacement = struct.unpack_from("<i", body, displacement_offset)[0]
    return address + displacement_offset + 4 + displacement


def verify_compiler_bodies() -> None:
    tail = load_script("review-static-init-tail.py", "th09_game_origin_target")
    tail.verify_target_tail()
    data = tail.TARGET.read_bytes()
    _, sections = tail.parse_image(data)

    vector_body = tail.read_va(data, sections, int(VECTOR_CONSTRUCTOR, 0), 38)
    expected_vector = bytes.fromhex(
        "558bec8b4510487819538b5d14568b7508578d78018bceffd303750c4f75f65f5e5b5dc21000"
    )
    if vector_body != expected_vector:
        raise ValueError("VC7.1 vector-constructor helper body changed")

    for address_text, (symbol, expected_hex) in MATH_INTRINSICS.items():
        body = tail.read_va(data, sections, int(address_text, 0), 12)
        if body != bytes.fromhex(expected_hex):
            raise ValueError(f"VC7.1 math intrinsic body changed at {address_text} ({symbol})")

    fixed_template = bytearray.fromhex(
        "558bec568bf1e800000000f6450801740956e80000000083c4048bc65e5dc20400"
    )
    for address_text in SCALAR_DELETING_DESTRUCTORS:
        address = int(address_text, 0)
        body = bytearray(tail.read_va(data, sections, address, 33))
        first_target = rel32_target(address, body, 7)
        second_target = rel32_target(address, body, 19)
        body[7:11] = b"\0" * 4
        body[19:23] = b"\0" * 4
        if body != fixed_template:
            raise ValueError(f"deleting-destructor shape changed at {address_text}")
        if not (0x00401000 <= first_target < 0x0044E000):
            raise ValueError(f"ordinary destructor leaves game band at {address_text}")
        if second_target != 0x0047B249:
            raise ValueError(f"deleting destructor no longer calls j__free at {address_text}")


def review_compiler(write: bool) -> dict[str, object]:
    verify_compiler_bodies()
    function_rows = {row["address"]: row for row in read_rows(FUNCTIONS)}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    pending: set[str] = set()
    already_applied: set[str] = set()
    for address in COMPILER_GENERATED:
        function = function_rows[address]
        origin = origin_rows[address]
        if function["status"] == "unclassified" and origin["origin"] == "unknown":
            pending.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == "Compiler"
            and function["owner"] == "compiler_generated"
            and origin["origin"] == "compiler_generated"
            and origin["subsystem"] == "Compiler"
            and origin["disposition"] == "exclude"
            and origin["evidence_id"]
            == (
                MATH_INTRINSIC_EVIDENCE_ID
                if address in MATH_INTRINSICS
                else COMPILER_EVIDENCE_ID
            )
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected compiler-helper ledger state at {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "compiler_generated"
        row["subsystem"] = "Compiler"
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = (
            MATH_INTRINSIC_EVIDENCE_ID
            if row["address"] in MATH_INTRINSICS
            else COMPILER_EVIDENCE_ID
        )

    def mutate_function(row: dict[str, str]) -> None:
        row["module"] = "Compiler"
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["owner"] = "compiler_generated"
        if row["address"] == VECTOR_CONSTRUCTOR:
            row["evidence"] = (
                "Target body and many repository VC7.1 relocations identify the "
                "four-argument ??_H vector-constructor iterator helper."
            )
            row["notes"] = (
                "Compiler-emitted array construction helper; excluded from the "
                "authored denominator without exactness credit."
            )
        elif row["address"] in MATH_INTRINSICS:
            symbol, _ = MATH_INTRINSICS[row["address"]]
            row["proposed_name"] = symbol
            row["evidence"] = (
                "Pinned VC7.1 /Oi /Gr compilation of an ordinary math caller "
                f"automatically emits the link-once COMDAT {symbol}; two cold probes "
                "reproduce all 12 target bytes with no relocations."
            )
            row["notes"] = (
                "Compiler-emitted math intrinsic support, not authored game source; "
                "excluded from the authored denominator without authored exactness credit."
            )
        else:
            row["evidence"] = (
                "Complete 33-byte extent matches the reviewed VC7.1 scalar deleting-"
                "destructor template after masking two rel32 fields; calls a game "
                "destructor then exact j__free."
            )
            row["notes"] = (
                "Compiler-generated scalar deleting destructor; excluded from the "
                "authored denominator without exactness credit."
            )

    origin_count = rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete compiler-helper ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "vector_constructor_helpers": 1,
        "scalar_deleting_destructors": len(SCALAR_DELETING_DESTRUCTORS),
        "math_intrinsic_helpers": len(MATH_INTRINSICS),
    }


def review_authored(write: bool) -> dict[str, object]:
    tail = load_script("review-static-init-tail.py", "th09_game_authored_target")
    tail.verify_target_tail()
    function_list = read_rows(FUNCTIONS)
    function_rows = {row["address"]: row for row in function_list}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    cohort = {
        line.strip()
        for line in FROZEN_GAME_COHORT.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    }
    if len(cohort) != EXPECTED_GAME_COHORT:
        raise ValueError(
            f"expected {EXPECTED_GAME_COHORT} frozen game candidates, got {len(cohort)}"
        )
    cohort_digest = hashlib.sha256(
        ("\n".join(sorted(cohort, key=lambda value: int(value, 0))) + "\n").encode()
    ).hexdigest()
    if cohort_digest != EXPECTED_GAME_COHORT_DIGEST:
        raise ValueError(f"frozen game cohort digest changed: {cohort_digest}")
    if not RETAIN_UNKNOWN.issubset(cohort):
        raise ValueError("reviewed-unknown special-member set left the frozen cohort")
    if not set(MATH_INTRINSICS).issubset(cohort):
        raise ValueError("math intrinsic reclassification left the frozen cohort")
    if not MATH_RUNTIME_RECLASSIFICATIONS.issubset(cohort):
        raise ValueError("math-runtime reclassification left the frozen cohort")
    if not D3DX_HEADER_RECLASSIFICATIONS.issubset(cohort):
        raise ValueError("D3DX header-inline reclassification left the frozen cohort")
    selected = (
        cohort
        - RETAIN_UNKNOWN
        - set(MATH_INTRINSICS)
        - MATH_RUNTIME_RECLASSIFICATIONS
        - D3DX_HEADER_RECLASSIFICATIONS
    )
    digest = hashlib.sha256(
        ("\n".join(sorted(selected, key=lambda value: int(value, 0))) + "\n").encode()
    ).hexdigest()
    if digest != EXPECTED_AUTHORED_DIGEST:
        raise ValueError(f"authored selection digest changed: {digest}")

    data = tail.TARGET.read_bytes()
    _, sections = tail.parse_image(data)
    previous_end = 0
    for function in function_list:
        address = int(function["address"], 0)
        end = int(function["span_end"], 0) + 1
        if address < previous_end:
            raise ValueError(f"overlapping function extents at {function['address']}")
        previous_end = end
        if function["address"] not in selected:
            continue
        body = tail.read_va(data, sections, address, int(function["size"]))
        terminal = body[-1] == 0xC3 or (len(body) >= 3 and body[-3] == 0xC2)
        if function["address"] in AUTHORED_TAIL_JUMPS:
            terminal = len(body) >= 5 and body[-5] == 0xE9
        if not terminal:
            raise ValueError(f"unreviewed terminal boundary at {function['address']}")

    pending: set[str] = set()
    already_applied: set[str] = set()
    for address in selected:
        function = function_rows[address]
        origin = origin_rows[address]
        if function["status"] == "unclassified" and origin["origin"] == "unknown":
            pending.add(address)
        elif (
            function["owner"] == "authored"
            and origin["origin"] == "authored_game"
            and origin["disposition"] == "authored"
            and origin["confidence"] == "high"
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected authored-review state at {address}")
    for address in RETAIN_UNKNOWN:
        origin = origin_rows[address]
        if origin["origin"] != "unknown" or origin["disposition"] != "review":
            raise ValueError(f"ambiguous special member was prematurely classified: {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "authored_game"
        row["subsystem"] = row["subsystem"] or "Game"
        row["disposition"] = "authored"
        row["confidence"] = "high"
        row["evidence_id"] = AUTHORED_EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        row["module"] = row["module"] or "Game"
        row["owner"] = "authored"
        row["evidence"] = (
            "Full target extent decodes to a terminal ret/tail-jump; placement, "
            "callgraph, and nontrivial program behavior establish game-authored "
            "origin after separate runtime/compiler/special-member review."
        )
        row["notes"] = (
            "Boundary/origin review only; no source-presence or exact-codegen credit. "
            "Ambiguous member-only constructors/destructors were excluded from this set."
        )

    origin_count = rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete authored-game ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "retained_origin_unknown": len(RETAIN_UNKNOWN),
        "reclassified_compiler_intrinsics": len(MATH_INTRINSICS),
        "reclassified_math_runtime": len(MATH_RUNTIME_RECLASSIFICATIONS),
        "reclassified_d3dx_header_inline": len(D3DX_HEADER_RECLASSIFICATIONS),
        "reviewed_bytes": sum(int(function_rows[address]["size"]) for address in selected),
        "selection_digest": digest,
    }


def review_ambiguous(write: bool) -> dict[str, object]:
    tail = load_script("review-static-init-tail.py", "th09_game_ambiguous_target")
    tail.verify_target_tail()
    function_rows = {row["address"]: row for row in read_rows(FUNCTIONS)}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    digest = hashlib.sha256(
        ("\n".join(sorted(RETAIN_UNKNOWN, key=lambda value: int(value, 0))) + "\n").encode()
    ).hexdigest()
    if digest != EXPECTED_AMBIGUOUS_DIGEST:
        raise ValueError(f"reviewed-unknown selection digest changed: {digest}")
    if not SHARED_OR_FOLDED.issubset(RETAIN_UNKNOWN):
        raise ValueError("shared/folded subset left reviewed-unknown cohort")

    data = tail.TARGET.read_bytes()
    _, sections = tail.parse_image(data)
    for address in RETAIN_UNKNOWN:
        function = function_rows[address]
        body = tail.read_va(data, sections, int(address, 0), int(function["size"]))
        if address == "0x0042F3F0":
            terminal = len(body) == 5 and body[0] == 0xE9
        else:
            terminal = body[-1] == 0xC3 or (len(body) >= 3 and body[-3] == 0xC2)
        if not terminal:
            raise ValueError(f"ambiguous candidate boundary changed at {address}")

    pending: set[str] = set()
    already_applied: set[str] = set()
    for address in RETAIN_UNKNOWN:
        function = function_rows[address]
        origin = origin_rows[address]
        if (
            function["status"] == "unclassified"
            and origin["origin"] == "unknown"
            and origin["disposition"] == "review"
            and origin["evidence_id"] != AMBIGUOUS_EVIDENCE_ID
        ):
            pending.add(address)
        elif (
            function["status"] == "unclassified"
            and function["owner"] in {"", "unknown"}
            and origin["origin"] == "unknown"
            and origin["disposition"] == "review"
            and origin["confidence"] == "unknown"
            and origin["evidence_id"] == AMBIGUOUS_EVIDENCE_ID
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected reviewed-unknown state at {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "unknown"
        row["subsystem"] = ""
        row["disposition"] = "review"
        row["confidence"] = "unknown"
        row["evidence_id"] = AMBIGUOUS_EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        address = row["address"]
        if address == "0x0042F3F0":
            row["evidence"] = (
                "Reviewed complete five-byte tail-jump extent. The physical body can "
                "represent an optimized explicit wrapper, destructor variant, or "
                "compiler adjustment/alias thunk, so unique origin is not observable."
            )
            row["notes"] = (
                "Boundary is closed; physical owner and authored-versus-generated "
                "origin remain deliberately unknown."
            )
        elif address == "0x0043D2B0":
            row["evidence"] = (
                "Reviewed complete one-byte ret extent with three distinct game-code "
                "callers and a data/vtable reference. An empty folded body cannot "
                "identify its logical source owner or explicit/implicit origin."
            )
            row["notes"] = (
                "Boundary is closed; shared empty-body ownership remains deliberately unknown."
            )
        elif address == "0x004343D0":
            row["evidence"] = (
                "Reviewed complete three-byte mov eax,ecx; ret extent. Fresh attested "
                "TH09 xrefs show ordinary code calls plus constructor-callback/data uses; "
                "exact caller manifests bind the same VA to Float3::operator float* and "
                "several constructor-shaped symbols, proving shared/folded ownership."
            )
            row["notes"] = (
                "Known aliases include Float3::operator float* and multiple identity/empty "
                "constructor views; boundary is closed but physical origin stays unknown."
            )
        elif address == "0x00435EC0":
            row["evidence"] = (
                "Reviewed complete three-byte mov eax,[ecx]; ret extent. Nineteen IDA "
                "call sites and independently reproduced source aliases establish a "
                "shared/folded physical body without a unique source owner."
            )
            row["notes"] = (
                "Known aliases include TextHelper::GetFormat and SoundManager "
                "GetDirectSound; boundary is closed but physical origin stays unknown."
            )
        else:
            if address not in DETAILED_SPECIAL_MEMBER_EVIDENCE:
                row["evidence"] = (
                    "Reviewed complete terminal-ret extent; the body performs only "
                    "member/array construction or destruction and returns this. Target "
                    "code cannot distinguish an explicit out-of-line special member "
                    "from the same VC7.1 implicitly generated body."
                )
                row["notes"] = (
                    "Boundary is closed; explicit authored versus implicit compiler-"
                    "generated origin remains deliberately unknown."
                )
            else:
                suffix = (
                    " Boundary/origin disposition remains deliberately unknown because "
                    "explicit and implicit VC7.1 source forms are observationally equivalent."
                )
                if suffix.strip() not in row["notes"]:
                    row["notes"] = row["notes"].rstrip() + suffix

    origin_count = rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete reviewed-unknown ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "special_members": len(RETAIN_UNKNOWN - SHARED_OR_FOLDED),
        "shared_or_folded": len(SHARED_OR_FOLDED),
        "selection_digest": digest,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="update reviewed ledgers")
    parser.add_argument(
        "--group", choices=("compiler", "authored", "ambiguous"), default="compiler"
    )
    args = parser.parse_args()
    if args.group == "compiler":
        result = review_compiler(args.apply)
    elif args.group == "authored":
        result = review_authored(args.apply)
    else:
        result = review_ambiguous(args.apply)
    print(json.dumps(result, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: game-origin review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
