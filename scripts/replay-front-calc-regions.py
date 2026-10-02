#!/usr/bin/env python3
"""Replay two diagnostic FrontCalc regions; never award function exactness.

Packet 716's 1510-byte candidate has an unresolved completion CFG between
these regions. Each region is independently placed at its reviewed target
address. This is NOT a link/replay of the complete callback or a match unit.
Destinations below are independently reviewed TH09 IDA/canonical-unit facts,
not values fitted from the compared relocation fields.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "coff_compare", ROOT / "scripts" / "compare-coff-function.py"
)
assert SPEC is not None and SPEC.loader is not None
COFF = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(COFF)

BASE = 0x00417630
SYMBOL = "?FrontCalcCallback@@YIHPAX@Z"
DESTINATIONS = {
    "?g_AnmManager@@3PAVAnmManager@@A": 0x004DC550,
    "?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z": 0x00436F30,
    "?g_FrontDynamicSprite@@3HA": 0x004DC690,
    "?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z": 0x00401560,
    "?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z": 0x00436AC0,
    "?Update@FrontCalcMessageRuntimeView@@QAEHXZ": 0x00416590,
    "?g_GameManager@@3UFrontCalcGameManagerView@@A": 0x004A7D90,
    "??4ZunTimer@@QAEXH@Z": 0x00401500,
    "?CreateCircleType1@FrontCalcCollisionView@@QAEPAXPBUFrontCalcPlayerCollisionPoint@@MMHH@Z": 0x0041CFE0,
    "?CreateCircleType4@FrontCalcCollisionView@@QAEPAXPBUFrontCalcPlayerCollisionPoint@@MMHHH@Z": 0x0041D0D0,
    "?Reset@FrontCalcAddedOwnerStateView@@QAEXXZ": 0x0041D7E0,
    "?g_SoundPlayer@@3UFrontCalcSoundPlayerView@@A": 0x004DC698,
    "?PlaySoundByIdx@FrontCalcSoundPlayerView@@QAEXHH@Z": 0x0043E2F0,
    "?RegisterChain@ScreenEffect@@SIPAU1@W4ScreenEffectType@@HHHHHH@Z": 0x00422D20,
    "?IsGameMode2@FrontCalcGameManagerView@@QAEHXZ": 0x00415D30,
    "__real@00000000": 0x0048E314,
    "__real@3f800000": 0x0048E2A4,
    "?g_NeutralMessageSource@@3PAXA": 0x004A7E80,
    "?Setup@FrontCalcMessageRuntimeView@@QAEXHH@Z": 0x00416160,
    "?g_FrontRuntimeValues90@@3PAHA": 0x004A7E90,
    "?SetupRandomForSide@FrontCalcMessageRuntimeView@@QAEXH@Z": 0x004162D0,
    "?BeginAuxTransition@FrontCalcFrontSideView@@QAEXXZ": 0x00415D50,
    "?CleanupGameplayState@GameManagerSetupLayout@@SIXXZ": 0x0041B5C6,
    "?IsGameMode0@FrontCalcGameManagerView@@QAEHXZ": 0x0040E470,
    "?IsGameMode1@FrontCalcGameManagerView@@QAEHXZ": 0x00404970,
    "?g_TitleNameTableIndex@@3HA": 0x004A7EAC,
    "?g_GameStageValue@@3HA": 0x004A7E8C,
    "?g_TransitionResultTableAlt@@3PAHA": 0x004A1510,
    "?g_TransitionResultTable@@3PAHA": 0x004A1504,
    "?g_SelectedOpponentParameter@@3HA": 0x004A7DF4,
}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    args = parser.parse_args()
    target = COFF.verified_target()
    code, relocations = COFF.object_function(args.object, SYMBOL)
    if len(code) != 1510:
        raise ValueError(f"Packet 716 candidate extent changed: {len(code)} != 1510")
    rows = []
    for name, start, target_offset, size in (
        ("panel-message-prefix", 0, 0, 395),
        ("frame-dispatch-through-return", 0x2B9, 0x2A6, 813),
    ):
        replay = bytearray(code[start : start + size])
        count = 0
        for relocation in relocations:
            offset = int(relocation["offset"])
            if offset < start + size and offset + 4 > start:
                if not start <= offset <= start + size - 4:
                    raise ValueError("relocation crosses diagnostic region boundary")
                destination = DESTINATIONS[str(relocation["symbol"])]
                value = destination + int(relocation["addend"])
                kind = int(relocation["type_id"])
                if kind == 0x14:
                    value -= BASE + target_offset + offset - start + 4
                elif kind != 0x06:
                    raise ValueError(f"unsupported relocation type {kind:#x}")
                struct.pack_into("<I", replay, offset - start, value & 0xFFFFFFFF)
                count += 1
        expected = COFF.pe_bytes_at(target, BASE + target_offset, size)
        differences = [i for i, pair in enumerate(zip(replay, expected)) if pair[0] != pair[1]]
        rows.append({
            "region": name,
            "object_offset": hex(start),
            "target_offset": hex(target_offset),
            "size": size,
            "reviewed_relocations": count,
            "difference_count": len(differences),
            "first_difference_offsets": [hex(i) for i in differences[:8]],
        })
    print(json.dumps({
        "diagnostic_only": True,
        "complete_function_exact": False,
        "raw_function_sha256": hashlib.sha256(code).hexdigest(),
        "regions": rows,
    }, indent=2))
    return int(any(row["difference_count"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
