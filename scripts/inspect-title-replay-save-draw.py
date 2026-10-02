#!/usr/bin/env python3
"""Read-only complete DrawReplaySave replay; diagnostic only, never credit."""
import importlib.util
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'result_draw_diagnostic', ROOT / 'scripts/inspect-title-result-draw.py')
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)
replay.BASE, replay.SIZE = 0x4239F6, 800
replay.SYMBOL = '?DrawReplaySave@TitleScreenView@@QAEHXZ'
# Shared target operands re-reviewed in this complete TH09-local owner.
shared = [
    '?AddFormatText@AsciiManager@@QAAXPAUFloat3@@PBDZZ',
    '?AddString@AsciiManager@@QAEXPAUFloat3@@PBD@Z',
    '?g_AsciiManager@@3VAsciiManager@@A',
    '?g_TitleAlphabet@@3PADA',
    '??_C@_01IDAFKMJL@_?$AA@',
    '??_C@_04FOHLOOIG@?$CF?48s?$AA@',
    '__real@3ccccccd', '__real@3f800000', '__real@3f99999a',
    '__real@40000000', '__real@41400000', '__real@41800000',
    '__real@c1000000',
]
replay.DESTINATIONS = {name: replay.DESTINATIONS[name] for name in shared}
# Addresses/literal contents come from target instructions/PE, not a
# candidate relocation-offset solve. Replay time is a runtime-filled view.
replay.DESTINATIONS.update({
    '?g_ReplayPlayTimeText@@3PADA': 0x4AC879,
    '??_C@_08FLEDHNDO@?9?9?1?9?9?1?9?9?$AA@': 0x48F588,
    '??_C@_08GGNFKEJM@?9?9?9?9?9?9?9?9?$AA@': 0x48F57C,
    '??_C@_0BB@OFJFINLA@No?4?$CF?42d?5?$CF?48s?5?$CF8s?$AA@': 0x48F5B0,
    '__real@3dcccccd': 0x48E4B0,
    '__real@41500000': 0x48F554,
    '__real@42000000': 0x48E5E0,
    '__real@42b40000': 0x48F550,
    '__real@43400000': 0x48E3CC,
    '__real@43600000': 0x48F5AC,
})

if __name__ == '__main__':
    replay.main()
