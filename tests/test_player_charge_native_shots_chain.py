"""ChargeAttack's entire original-bound shot dispatch uses real native views."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerChargeNativeShotsChain(unittest.TestCase):
    def test_native_shot_and_charge_method_chain(self):
        shots=(ROOT/'src/PlayerSpawnShotsView.hpp').read_text()
        charge=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertIn('struct PlayerSpawnShotsView : PlayerStateEffectView',shots)
        self.assertIn('int UpdateChargeAttack();',charge)
        self.assertIn('PlayerLifecycleSizeIs30F70',charge)
        self.assertNotIn('reinterpret_cast<PlayerSpawnShotsView *>(this)',source)
        self.assertEqual(source.count('SpawnShots(static_cast<int>('),2)
        self.assertIn('SpawnShots(static_cast<int>(timer30398), 1);',source)
        self.assertIn('SpawnShots(static_cast<int>(timer303A4), 0);',source)

    def test_exact_callee_abi_remains_unmodified(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        spec=units['player-spawn-shots']
        self.assertEqual(spec['size'],190)
        self.assertEqual(spec['symbol'],'?SpawnShots@PlayerLifecycleView@@QAIXHH@Z')

if __name__=='__main__':unittest.main()
