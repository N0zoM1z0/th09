#!/usr/bin/env python3
"""Complete Type21 routing diagnostic; full byte differences, never exact credit."""
import importlib.util
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type01_diagnostic', ROOT / 'scripts/inspect-exattack-type01.py')
diagnostic = importlib.util.module_from_spec(spec)
spec.loader.exec_module(diagnostic)
diagnostic.__doc__ = __doc__
diagnostic.BASE, diagnostic.SIZE = 0x44B500, 1074
diagnostic.SYMBOL = '?ExAttackUpdateCallbackType21@@YIHPAUExAttackRecord@@@Z'
# Existing canonical helper/global identities, and independently checked PE float bits.
# The descriptor-storage constructor aliases the reviewed 0x214-byte constructor.
diagnostic.DESTINATIONS = {
    '??OZunTimer@@QAEIH@Z': 0x403de0,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x441890,
    '__real@3f000000': 0x490f54,
    '?AddNormalizeAngle@@YGMMM@Z': 0x42aed0,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '__real@3e4f8c3d': 0x491608,
    '?g_GameManager@@3UExAttackType8GameManagerView@@A': 0x4a7d90,
    '??GFloat3@@QBE?AU0@ABU0@@Z': 0x401140,
    '__real@44800000': 0x491660,
    '??0ExAttackType21BulletDescriptorStorage@@QAE@XZ': 0x40d500,
    '__real@3f800000': 0x48e2a4,
    '?SpawnBulletPatternPrimary@EtamaController@@QAEPAUBullet@@PAUBulletSpawnDescriptor@@@Z': 0x4130f0,
    '__real@3f333333': 0x4915f4,
    '?SpawnBulletPatternSecondary@EtamaController@@QAEPAUBullet@@PAUBulletSpawnDescriptor@@@Z': 0x4131c0,
    '??BZunTimer@@QAEMXZ': 0x4014f0,
    '?ExAttackInterpolate2D@@YIXPAM0000MM@Z': 0x42af80,
    '??4ZunTimer@@QAEXH@Z': 0x401500,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x401560,
}

if __name__ == '__main__':
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    for symbol, address in diagnostic.DESTINATIONS.items():
        if symbol.startswith('__real@'):
            if coff.pe_bytes_at(pe, address, 4) != struct.pack('<I', int(symbol[7:], 16)):
                raise ValueError('target float identity differs: ' + symbol)
    diagnostic.main()
