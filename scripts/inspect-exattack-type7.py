#!/usr/bin/env python3
"""Complete Type7 routing diagnostic with reviewed TH09 destinations."""
import importlib.util
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type01_diagnostic', ROOT / 'scripts/inspect-exattack-type01.py')
diagnostic = importlib.util.module_from_spec(spec)
spec.loader.exec_module(diagnostic)
diagnostic.__doc__ = __doc__
diagnostic.BASE, diagnostic.SIZE = 0x4464A0, 799
diagnostic.SYMBOL = '?ExAttackUpdateCallbackType7@@YIHPAUExAttackRecord@@@Z'
diagnostic.DESTINATIONS = {
    '??0Float3@@QAE@MMM@Z': 0x4010B0,
    '??OZunTimer@@QAEIH@Z': 0x403DE0,
    '?HasTickedEvery@ZunTimer@@QAEHH@Z': 0x404920,
    '?AddNormalizeAngle@@YGMMM@Z': 0x42AED0,
    '??0ExAttackType7BulletDescriptorStorage@@QAE@XZ': 0x40D500,
    '?InstallWaitTransform@BulletSpawnDescriptor@@QAEXHHH@Z': 0x412760,
    '?InstallVectorAccelerationTransform@BulletSpawnDescriptor@@QAEXHHHMM@Z': 0x412730,
    '?SpawnBulletPatternPrimary@EtamaController@@QAEPAUBullet@@PAUBulletSpawnDescriptor@@@Z': 0x4130F0,
    '?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z': 0x41DFF0,
    '??4ZunTimer@@QAEXH@Z': 0x401500,
    '?SetInterrupt@ExAttackType7InterruptView@@QAEXF@Z': 0x406790,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x441890,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '??BZunTimer@@QAEMXZ': 0x4014F0,
    '?ExAttackInterpolate2D@@YIXPAM0000MM@Z': 0x42AF80,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x401560,
    '?g_GameManager@@3UExAttackType7GameManagerView@@A': 0x4A7D90,
    '__real@3dcccccd': 0x48E4B0,
    '__real@40000000': 0x48EF5C,
    '__real@3c088889': 0x49162C,
    '__real@43f00000': 0x4915A8,
    '__real@c2000000': 0x4915F0,
    '__real@43300000': 0x48EF64,
    '__real@c3300000': 0x48EF68,
}

if __name__ == '__main__':
    diagnostic.main()
