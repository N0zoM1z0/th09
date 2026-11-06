#!/usr/bin/env python3
"""Verify the original service's two explicit ECX input preparations.

The callee's unused register does not establish a parameterless source API.
This audit covers observed caller values, not unique original C++ arity.
"""
from __future__ import annotations

import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x00430AA0, 1633
OWNER_SHA256 = "26d2783077746e99d8ec7240e444ff18c33861508cc63d6af8004bf5c9c1b31f"
INPUT_BASE, INPUT_SIZE = 0x0042BE40, 819
INPUT_SHA256 = "c2b3c085016cc92f18da26b4ac6ad40f6e4c505dd87560d2f4e2144902e86504"
STATE_ROOT = 0x004B42D0
CALLS = (0x00430EC8, 0x00431046)


def require_bytes(owner: bytes, address: int, expected: bytes) -> None:
    offset = address - BASE
    if offset < 0 or offset + len(expected) > len(owner):
        raise ValueError("input preparation extends outside the complete owner")
    if owner[offset:offset + len(expected)] != expected:
        raise ValueError(f"original input preparation differs at {address:#x}")


def direct_call_destination(owner: bytes, address: int) -> int:
    offset = address - BASE
    if offset < 0 or offset + 5 > len(owner) or owner[offset] != 0xE8:
        raise ValueError("missing complete direct input call")
    return address + 5 + struct.unpack_from("<i", owner, offset + 1)[0]


def audit_call_inputs(owner: bytes) -> dict:
    """Decode the reviewed caller contract; target identity is checked by main."""
    if len(owner) != SIZE:
        raise ValueError("input caller requires its complete 1633-byte owner")
    # On the packet-ready edge EDI holds the current network state; that edge
    # reaches the ECX load without taking the wait/critical-section exit.
    require_bytes(owner, 0x00430E98, b"\x8b\x3d" + struct.pack("<I", STATE_ROOT))
    require_bytes(owner, 0x00430E9E, bytes.fromhex("8b87b000000085c0"))
    require_bytes(owner, 0x00430EA6, bytes.fromhex("751a"))
    require_bytes(owner, 0x00430EC2, bytes.fromhex("8b8fac000000"))
    require_bytes(owner, 0x00431041, b"\xb9" + struct.pack("<I", 2))
    for address in CALLS:
        if direct_call_destination(owner, address) != INPUT_BASE:
            raise ValueError("input call redirects to a different callee")
    return {
        "caller_base": hex(BASE), "caller_bytes": SIZE,
        "callee_base": hex(INPUT_BASE),
        "calls": [
            {"address": hex(CALLS[0]), "ecx_width": 32,
             "source": "current network state + 0xAC", "state_root": hex(STATE_ROOT)},
            {"address": hex(CALLS[1]), "ecx_width": 32, "source": "immediate", "value": 2},
        ],
        "original_cpp_arity": "not uniquely established by an unused callee register",
        "exactness_credit": "none; observed original caller contract only",
    }


def audit_original() -> dict:
    sys.path.insert(0, str(ROOT / "scripts"))
    spec = importlib.util.spec_from_file_location("input_calls_coff", ROOT / "scripts/compare-coff-function.py")
    if spec is None or spec.loader is None:
        raise ValueError("cannot load the target verifier")
    compare = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(compare)
    image = compare.verified_target()
    owner = compare.pe_bytes_at(image, BASE, SIZE)
    callee = compare.pe_bytes_at(image, INPUT_BASE, INPUT_SIZE)
    if hashlib.sha256(owner).hexdigest() != OWNER_SHA256:
        raise ValueError("original input caller identity differs")
    if hashlib.sha256(callee).hexdigest() != INPUT_SHA256:
        raise ValueError("original input callee identity differs")
    report = audit_call_inputs(owner)
    text_base, text_size = 0x00401000, 576512
    code = compare.pe_bytes_at(image, text_base, text_size)
    candidates = []
    for offset in range(len(code) - 4):
        if code[offset] == 0xE8:
            destination = text_base + offset + 5 + struct.unpack_from("<i", code, offset + 1)[0]
            if destination == INPUT_BASE:
                candidates.append(text_base + offset)
    if tuple(candidates) != CALLS:
        raise ValueError("physical direct input-call census differs")
    report.update(target_sha256=hashlib.sha256(image).hexdigest(),
                  caller_sha256=OWNER_SHA256, callee_sha256=INPUT_SHA256,
                  physical_e8_candidates=[hex(x) for x in candidates],
                  computed_indirect_calls="not excluded by this census")
    return report


def main() -> int:
    try:
        print(json.dumps(audit_original(), indent=2))
    except (OSError, ValueError, struct.error) as error:
        print(json.dumps({"result": "error", "message": str(error)}))
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
