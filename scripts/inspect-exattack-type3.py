#!/usr/bin/env python3
"""TH09 v1.50a Type3 update: complete target-bound diagnostic, not exact credit.

The callback at 0x00442750 has a close VC7.1 candidate.  Every relocation
below is grounded in an independently canonical TH09 helper/global or in the
TH09-local target callsite/view contract.  No relocation is guessed from a
matching target field, and reordered/missing fields fail closed.  Use this on
fresh maintained-source objects; inspecting an object does not attest its build.
"""

import importlib.util
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]

spec = importlib.util.spec_from_file_location(
    "exattack_type3_base_diagnostic", ROOT / "scripts/inspect-exattack-type01.py"
)
diagnostic = importlib.util.module_from_spec(spec)
spec.loader.exec_module(diagnostic)

# The same relocation identities are also exercised by the exact units for
# Float3, ZunTimer, Supervisor, and Player collision handling.  The two
# inverse-popup transforms and VM interrupt have separate canonical owners.
diagnostic.BASE = 0x00442750
diagnostic.SIZE = 780
diagnostic.SYMBOL = "?ExAttackUpdateCallbackType3@@YIHPAUExAttackRecord@@@Z"
diagnostic.DESTINATIONS = {
    "??PZunTimer@@QAEIH@Z": 0x00401520,
    "??GFloat3@@QBE?AU0@ABU0@@Z": 0x00401140,
    "??BPlayerPositionView@@QAEPAMXZ": 0x004343D0,
    "?g_Supervisor@@3VSupervisor@@A": 0x004B3100,
    "?SelectSide@Supervisor@@QAEXH@Z": 0x00401440,
    "?g_GameManager@@3UAsciiGameManagerView@@A": 0x004A7D90,
    "?InverseTransformPopupX@AsciiGameManagerView@@QAEMM@Z": 0x00442420,
    "?InverseTransformPopupY@AsciiGameManagerView@@QAEMM@Z": 0x00442450,
    "?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z": 0x0041DFF0,
    "??YFloat3@@QAEAAU0@ABU0@@Z": 0x00405730,
    "__real@c2000000": 0x004915F0,
    "__real@44280000": 0x004915EC,
    "__real@44000000": 0x004915E8,
    "??4ZunTimer@@QAEXH@Z": 0x00401500,
    "?SetInterrupt@AnmVm@@QAEXF@Z": 0x00406790,
}


def validate_inputs(object_path):
    spec = importlib.util.spec_from_file_location(
        "exattack_type3_coff", ROOT / "scripts/compare-coff-function.py"
    )
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    verified = coff.verified_target()
    raw, fields = coff.object_function(object_path, diagnostic.SYMBOL)
    if len(raw) != diagnostic.SIZE or len(fields) != 38:
        raise ValueError("Type3 update candidate extent/field count changed; review required")
    if sorted(set(row["symbol"] for row in fields)) != sorted(diagnostic.DESTINATIONS):
        raise ValueError("Type3 update relocation referents changed; review required")
    for symbol, destination in diagnostic.DESTINATIONS.items():
        if symbol.startswith("__real@"):
            bits = int(symbol[7:], 16)
            if coff.pe_bytes_at(verified, destination, 4) != struct.pack("<I", bits):
                raise ValueError("reviewed Type3 float literal does not match target: " + symbol)


if __name__ == "__main__":
    import argparse
    import sys

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    parser.add_argument("--diff", action="store_true")
    parser.add_argument("--instructions", action="store_true")
    arguments = parser.parse_args()
    validate_inputs(arguments.object)
    sys.argv = [sys.argv[0], str(arguments.object)]
    if arguments.diff:
        sys.argv.append("--diff")
    if arguments.instructions:
        sys.argv.append("--instructions")
    diagnostic.main()
