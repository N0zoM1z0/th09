#!/usr/bin/env python3
"""Classify target-proven runtime-library residual and math-runtime candidates."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

from tracking_csv import rewrite_selected_rows, rows_by_address


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
EVIDENCE_ID = "runtime-residual-origin-review-2026-09-19"
MATH_EVIDENCE_ID = "vc71-math-runtime-comdat-review-2026-09-20"
D3DX_INLINE_EVIDENCE_ID = "d3dx8-header-inline-comdat-review-2026-09-20"
D3DX_CROSS_EVIDENCE_ID = "d3dx8-cross-header-inline-comdat-review-2026-09-21"
MATH_RUNTIME = {"0x00405710", "0x00436AA0", "0x00436AB0"}
D3DX_HEADER_INLINE = {"0x00401290", "0x004012C0", "0x004012E0"}
D3DX_CROSS_INLINE = {"0x0042E920"}
D3DX_HEADER_INLINE_ALL = D3DX_HEADER_INLINE | D3DX_CROSS_INLINE
AUTHORED_RECLASSIFICATIONS = MATH_RUNTIME | D3DX_HEADER_INLINE_ALL
AUTHORED_SWEEP_EVIDENCE_ID = "game-code-origin-sweep-2026-09-19"
REVIEW = {
    "0x00401290": (40, "D3DX8"),
    "0x004012C0": (31, "D3DX8"),
    "0x004012E0": (21, "D3DX8"),
    "0x00405710": (18, "MathRuntime"),
    "0x0042E920": (83, "D3DX8"),
    "0x00436AA0": (15, "MathRuntime"),
    "0x00436AB0": (14, "MathRuntime"),
    "0x00454650": (11, "D3DX8"),
    "0x0045E550": (266, "D3DX8"),
    "0x004866AD": (7, "CRT"),
    "0x00487119": (23, "CRT"),
    "0x0048A853": (8943, "DXErr8"),
}
BODY_DIGESTS = {
    "0x00401290": "3e38f567cb3b84f4e6da58f372fee8f6c34c48358fc0054a83db9ade26bfbfc7",
    "0x004012C0": "7ab81ef75aaeb8fdc804bd92be5aff0fe4b42e0a6f4ca6c8a8b3028fa8072a38",
    "0x004012E0": "d8624d2d2864e02690baf75753f49b404c5c430130abb66c2ffeb9a5f6022766",
    "0x00405710": "cda195dc6138ad85d8749faa392e96be3379221cfc73591c2271b9218032fb65",
    "0x0042E920": "551a67f468b441cda9b9a497588f0a54bf40b148e3af2329b9ef981d6eceeddf",
    "0x00436AA0": "4bc39e2b580fb068fb03b7eb5b285cebaa42116117f671e310daa34d726313c9",
    "0x00436AB0": "e2e393daeb8f2e57ac1acd1ab67c1df7b5c0bec2a470986103a64b0afd72d816",
    "0x00454650": "7e677f9a9597993b548e844081ccd47705e9b61e48f68694fb421c8a63fb3cd8",
    "0x0045E550": "b63f9a1de8acb9e2bb0fff2a2d9407484374a01e5f68215d57a13f32f397960e",
    "0x004866AD": "033799cc10934d542e3dd680172558386460376b4274444eb305840b463cea64",
    "0x00487119": "291f2e628a8a5575284959cf6b50caa66f4ab7b79ac6b04b57883cc176b198c6",
    "0x0048A853": "08b8e2b8b873824d991edf97121ec9f59143eef3961ef061b355dcd31433bdb0",
}
DXERR_CONTRIBUTION_DIGEST = (
    "8d05bac00455d91f9e94e0a9a3597fc593fb9c77d89a22de36006455f0dd6c52"
)
DXERR_STRINGS = {
    0x0049DA84: b"E_POINTER\0",
    0x0049CB1C: b"DIERR_ALREADYINITIALIZED\0",
    0x0049CCA4: b"VFW_E_BAD_KEY\0",
    0x0049A604: b"DMUS_E_INVALID_CONTAINER_OBJECT\0",
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


def verify_dispatch_initializer(body: bytes) -> None:
    if not body.startswith(b"\x8b\x44\x24\x04") or body[-1:] != b"\xc3":
        raise ValueError("D3DX dispatch initializer prologue/terminal changed")
    position = 4
    offsets: set[int] = set()
    targets: list[int] = []
    while position < len(body) - 1:
        if body[position : position + 2] == b"\xc7\x00":
            offset = 0
            immediate = position + 2
            width = 6
        elif body[position : position + 2] == b"\xc7\x40":
            offset = body[position + 2]
            immediate = position + 3
            width = 7
        elif body[position : position + 2] == b"\xc7\x80":
            offset = struct.unpack_from("<I", body, position + 2)[0]
            immediate = position + 6
            width = 10
        else:
            raise ValueError(f"unexpected D3DX initializer opcode at +0x{position:X}")
        target = struct.unpack_from("<I", body, immediate)[0]
        if offset in offsets or not (0x00464D50 <= target <= 0x00466F50):
            raise ValueError("D3DX dispatch initializer target/offset changed")
        offsets.add(offset)
        targets.append(target)
        position += width
    if position != len(body) - 1 or len(targets) != 34:
        raise ValueError("D3DX dispatch initializer extent changed")


def verify_target_bodies() -> None:
    target = load_script("review-static-init-tail.py", "th09_runtime_residual_target")
    target.verify_target_tail()
    data = target.TARGET.read_bytes()
    _, sections = target.parse_image(data)
    bodies: dict[str, bytes] = {}
    for address_text, (size, _) in REVIEW.items():
        body = target.read_va(data, sections, int(address_text, 0), size)
        digest = hashlib.sha256(body).hexdigest()
        if digest != BODY_DIGESTS[address_text]:
            raise ValueError(f"reviewed body changed at {address_text}: {digest}")
        bodies[address_text] = body

    if rel32_target(0x00401290, bodies["0x00401290"], 0x23) != 0x00401080:
        raise ValueError("D3DXVec3Length no longer calls @sqrtf@4")

    if rel32_target(0x00405710, bodies["0x00405710"], 10) != 0x0047B73A:
        raise ValueError("@fmodf@8 no longer calls __CIfmod")
    if rel32_target(0x00436AA0, bodies["0x00436AA0"], 7) != 0x0047D280:
        raise ValueError("@acosf@4 no longer calls __CIacos")
    if bodies["0x00436AB0"] != bytes.fromhex(
        "558becd94508d9e8d9f35dc20400"
    ):
        raise ValueError("@atanf@4 inline FPATAN body changed")

    if rel32_target(0x00454650, bodies["0x00454650"], 7) != 0x0047D6E8:
        raise ValueError("D3DX non-return helper no longer calls CRT longjmp")
    verify_dispatch_initializer(bodies["0x0045E550"])
    if rel32_target(0x004866AD, bodies["0x004866AD"], 3) != 0x0047D416:
        raise ValueError("__fptrap no longer calls __amsg_exit")
    abort_body = bodies["0x00487119"]
    abort_targets = (
        rel32_target(0x00487119, abort_body, 3),
        rel32_target(0x00487119, abort_body, 10),
        rel32_target(0x00487119, abort_body, 19),
    )
    if abort_targets != (0x00483A2C, 0x00489274, 0x0047DE37):
        raise ValueError("CRT abort call sequence changed")

    contribution = target.read_va(data, sections, 0x0048A853, 10672)
    if hashlib.sha256(contribution).hexdigest() != DXERR_CONTRIBUTION_DIGEST:
        raise ValueError("DXErr8 contribution changed")
    dxerr_body = bodies["0x0048A853"]
    if not dxerr_body.endswith(b"\x5d\xc2\x04\x00"):
        raise ValueError("DXGetErrorString8A terminal boundary changed")
    for address, expected in DXERR_STRINGS.items():
        if target.read_va(data, sections, address, len(expected)) != expected:
            raise ValueError(f"DXErr8 diagnostic string changed at 0x{address:08X}")
        if dxerr_body.count(struct.pack("<I", address)) != 1:
            raise ValueError(f"DXErr8 string reference changed at 0x{address:08X}")


def evidence_id_for(address: str) -> str:
    if address in D3DX_CROSS_INLINE:
        return D3DX_CROSS_EVIDENCE_ID
    if address in D3DX_HEADER_INLINE:
        return D3DX_INLINE_EVIDENCE_ID
    if address in MATH_RUNTIME:
        return MATH_EVIDENCE_ID
    return EVIDENCE_ID


def review(write: bool) -> dict[str, object]:
    verify_target_bodies()
    function_rows = rows_by_address(FUNCTIONS)
    origin_rows = rows_by_address(ORIGINS)
    pending: set[str] = set()
    already_applied: set[str] = set()
    for address, (size, subsystem) in REVIEW.items():
        function = function_rows[address]
        origin = origin_rows[address]
        if int(function["size"]) != size:
            raise ValueError(f"candidate extent changed at {address}")
        if (
            address in AUTHORED_RECLASSIFICATIONS
            and function["status"] == "unclassified"
            and function["owner"] == "authored"
            and origin["origin"] == "authored_game"
            and origin["evidence_id"] == AUTHORED_SWEEP_EVIDENCE_ID
        ):
            pending.add(address)
        elif (
            address not in AUTHORED_RECLASSIFICATIONS
            and function["status"] == "unclassified"
            and origin["origin"] == "unknown"
        ):
            pending.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == subsystem
            and function["owner"] == "library"
            and origin["origin"] == "library"
            and origin["subsystem"] == subsystem
            and origin["disposition"] == "exclude"
            and origin["evidence_id"] == evidence_id_for(address)
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected runtime-residual state at {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        subsystem = REVIEW[row["address"]][1]
        row["origin"] = "library"
        row["subsystem"] = subsystem
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = evidence_id_for(row["address"])

    def mutate_function(row: dict[str, str]) -> None:
        address = row["address"]
        subsystem = REVIEW[address][1]
        row["module"] = subsystem
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["owner"] = "library"
        if address in D3DX_HEADER_INLINE_ALL:
            symbol, signature, detail = {
                "0x00401290": (
                    "D3DXVec3Length",
                    "float __fastcall D3DXVec3Length(const D3DXVECTOR3 *value)",
                    "Its sole relocation resolves to @sqrtf@4 @ 0x00401080.",
                ),
                "0x004012C0": (
                    "D3DXVec3LengthSq",
                    "float __fastcall D3DXVec3LengthSq(const D3DXVECTOR3 *value)",
                    "The target and COMDAT have no relocations.",
                ),
                "0x004012E0": (
                    "D3DXVec3Dot",
                    "float __fastcall D3DXVec3Dot(const D3DXVECTOR3 *lhs, const D3DXVECTOR3 *rhs)",
                    "The target and COMDAT have no relocations.",
                ),
                "0x0042E920": (
                    "D3DXVec3Cross",
                    "D3DXVECTOR3 *__fastcall D3DXVec3Cross(D3DXVECTOR3 *out, const D3DXVECTOR3 *lhs, const D3DXVECTOR3 *rhs)",
                    "The target and /Ob1 COMDAT are 83/83 exact with no relocations.",
                ),
            }[address]
            row["proposed_name"] = symbol
            row["calling_convention"] = "__fastcall"
            row["signature"] = signature
            row["evidence"] = (
                "Pinned VC7.1 PlatformSDK d3dx8math.inl defines this helper directly. "
                "Compiling that unmodified SDK header under the target-backed /Gr /O2 "
                "inline profile automatically emits the matching link-once COMDAT; two "
                "cold target-bound comparisons reproduce every ordinary byte. "
                + detail
            )
            row["notes"] = (
                "D3DX8 SDK/header-inline support, not ZUN-authored source. Excluded from "
                "the authored denominator; no maintained-source or canonical authored "
                "exactness credit is assigned to the SDK body."
            )
        elif address in MATH_RUNTIME:
            symbol, signature, detail = {
                "0x00405710": (
                    "@fmodf@8",
                    "float __fastcall fmodf(float x, float y)",
                    "18-byte wrapper calling target __CIfmod",
                ),
                "0x00436AA0": (
                    "@acosf@4",
                    "float __fastcall acosf(float value)",
                    "15-byte wrapper calling target __CIacos",
                ),
                "0x00436AB0": (
                    "@atanf@4",
                    "float __fastcall atanf(float value)",
                    "14-byte inline FLD1/FPATAN wrapper",
                ),
            }[address]
            row["proposed_name"] = symbol
            row["calling_convention"] = "__fastcall"
            row["signature"] = signature
            row["evidence"] = (
                "Canonical authored manifests resolve this target as "
                f"{symbol}. A pinned VC7.1 <math.h> probe automatically emits the "
                f"same link-once COMDAT; two cold target-bound comparisons reproduce "
                f"the complete {detail} with all relocation destinations reviewed."
            )
            row["notes"] = (
                "VC7.1 math-runtime/library helper, not authored game source. "
                "Excluded from the authored denominator with no canonical authored "
                "exactness or source-presence credit."
            )
        elif address == "0x00454650":
            row["evidence"] = (
                "Complete non-return extent pushes longjmp value 1 and calls the "
                "reviewed CRT longjmp; its only IDA xref is data from the enclosing "
                "reviewed D3DX8 routine."
            )
            row["notes"] = "D3DX8 internal exception helper; boundary ends at non-return call."
        elif address == "0x0045E550":
            row["evidence"] = (
                "Complete terminal-ret extent initializes 34 dispatch slots solely "
                "with addresses in the reviewed D3DX8 linker band; sole IDA caller "
                "is a reviewed D3DX8 routine."
            )
            row["notes"] = "D3DX8 internal dispatch-table initializer."
        elif address == "0x004866AD":
            row["evidence"] = (
                "Exact seven-byte __fptrap extent pushes CRT message 2 and ends in "
                "a non-return call to reviewed CRT __amsg_exit; all IDA uses are CRT."
            )
            row["notes"] = "Microsoft CRT floating-point trap helper."
        elif address == "0x00487119":
            row["evidence"] = (
                "Exact 23-byte _abort extent performs the canonical CRT sequence "
                "__NMSG_WRITE(10), raise(22), then non-return _exit(3)."
            )
            row["notes"] = "Microsoft CRT abort implementation; sole IDA caller is CRT terminate."
        else:
            row["proposed_name"] = "DXGetErrorString8A"
            row["evidence"] = (
                "Complete 8,943-byte terminal-ret4 extent maps HRESULT families to "
                "their symbolic names, including E_POINTER, DIERR, VFW, and DMUS; "
                "the adjoining target contribution is hash-frozen."
            )
            row["notes"] = (
                "Microsoft DXErr8 library DXGetErrorString8A; sole IDA caller is the "
                "game DirectPlay error formatter."
            )

    origin_count = rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete runtime-residual ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "reviewed_bytes": sum(size for size, _ in REVIEW.values()),
        "subsystems": {
            name: sum(subsystem == name for _, subsystem in REVIEW.values())
            for name in ("MathRuntime", "D3DX8", "CRT", "DXErr8")
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="update reviewed ledgers")
    args = parser.parse_args()
    print(json.dumps(review(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: runtime-residual review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
