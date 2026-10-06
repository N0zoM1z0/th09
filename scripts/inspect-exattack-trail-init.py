#!/usr/bin/env python3
"""Complete Type19/21 initializer diagnostic; no partial or accepted exact credit."""
import argparse
import importlib.util
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
DESTINATIONS = {
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '?AllocateDynamicData@ExAttackType19AllocateView@@QAEXHH@Z': 0x440d90,
    '?AllocateDynamicData@ExAttackType21AllocateView@@QAEXHH@Z': 0x440d90,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x401560,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x441890,
    '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x404910,
    '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z': 0x406200,
    '?GetSideState@ExAttackType19SideLookupView@@QAEPAUExAttackType19SideView@@XZ': 0x440cb0,
    '?GetSideState@ExAttackType21SideLookupView@@QAEPAUExAttackType21SideView@@XZ': 0x440cb0,
    '?SelectSide@Supervisor@@QAEXH@Z': 0x401440,
    '?TransformPopupX@ExAttackType8GameManagerView@@QAEMM@Z': 0x401680,
    '?TransformPopupY@ExAttackType8GameManagerView@@QAEMM@Z': 0x4016b0,
    '?g_ExAttackPlayfieldWidth@@3MA': 0x4a80e8,
    '?g_GameManager@@3UExAttackType8GameManagerView@@A': 0x4a7d90,
    '?g_ReplayRng@@3URngRuntimeView@@A': 0x4ace0c,
    '?g_Supervisor@@3VSupervisor@@A': 0x4b3100,
    '__real@3d888889': 0x491648,
    '__real@3e4f8c3d': 0x491608,
    '__real@3f000000': 0x490f54,
    '__real@40490fdb': 0x48e2c4,
    '__real@40c90fdb': 0x48e4c4,
    '__real@41c00000': 0x48ef10,
    '__real@42a00000': 0x48e318,
    '__real@43000000': 0x48e3bc,
    '__real@43e00000': 0x48e3b8,
    '__real@bd888889': 0x49164c,
}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--type', required=True, choices=['19', '21'])
    args, remaining = parser.parse_known_args()
    spec = importlib.util.spec_from_file_location('trail_base', ROOT / 'scripts/inspect-exattack-type01.py')
    diagnostic = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(diagnostic)
    diagnostic.BASE, diagnostic.SIZE = (0x44A030, 763) if args.type == '19' else (0x44B220, 727)
    diagnostic.SYMBOL = '?ExAttackInitializeCallbackType' + args.type + '@@YIHPAUExAttackRecord@@@Z'
    diagnostic.DESTINATIONS = DESTINATIONS
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    for symbol, address in DESTINATIONS.items():
        if symbol.startswith('__real@') and coff.pe_bytes_at(pe, address, 4) != struct.pack('<I', int(symbol[7:], 16)):
            raise ValueError('target float identity differs: ' + symbol)
    sys.argv = [sys.argv[0]] + remaining
    diagnostic.main()

if __name__ == '__main__':
    main()
