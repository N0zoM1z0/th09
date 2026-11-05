"""Type6 may bind only the independently reconstructed native interfaces."""
import importlib.util
from pathlib import Path
import sys
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type6_native_interfaces', ROOT/'scripts/inspect-exattack-type6.py')
module = importlib.util.module_from_spec(spec)
with patch.object(sys, 'path', [str(ROOT/'scripts'), *sys.path]):
    spec.loader.exec_module(module)


class NativeType6Interfaces(unittest.TestCase):
    def test_bindings_have_independent_native_owners_and_reject_retired_aliases(self):
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        expected = {
            '??0BulletSpawnDescriptorCtorView@@QAE@XZ': 0x40D500,
            '?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z': 0x41DFF0,
            '?SetInterrupt@AnmVm@@QAEXF@Z': 0x406790,
            '?AddNormalizeAngle@@YIMMM@Z': 0x42AED0,
            '?g_GameManager@@3UAsciiGameManagerView@@A': 0x4A7D90,
        }
        independent = {}
        for name, unit in units.items():
            if name == 'exattack-type6-update':
                continue
            for row in [{'symbol': unit['symbol'], 'target': unit['target_address']}] + unit.get('relocations', []):
                independent.setdefault(row['symbol'], set()).add(row['target'])
        for symbol, address in expected.items():
            with self.subTest(symbol=symbol):
                self.assertEqual(independent[symbol], {address})
                self.assertEqual(module.diagnostic.DESTINATIONS[symbol], address)
        for retired in (
            '??0ExAttackType6BulletDescriptorStorage@@QAE@XZ',
            '?CheckBulletCollision@ExAttackType6PlayerView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z',
            '?SetInterrupt@ExAttackType6InterruptView@@QAEXF@Z',
            '?AddNormalizeAngle@@YGMMM@Z',
            '?g_GameManager@@3UExAttackType6GameManagerView@@A',
        ):
            with self.subTest(retired=retired), self.assertRaises(KeyError):
                module.diagnostic.DESTINATIONS[retired]


if __name__ == '__main__':
    unittest.main()
