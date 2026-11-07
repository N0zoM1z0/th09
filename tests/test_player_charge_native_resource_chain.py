"""The nonexact ChargeAttack boss borrows its exact native Player resource leaf."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class PlayerChargeNativeResourceChain(unittest.TestCase):
    def test_native_zero_storage_inheritance_and_direct_calls(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        resource=(ROOT/'src/PlayerResourceSubtractView.hpp').read_text()
        self.assertIn('int UpdateChargeAttack();',header)
        self.assertIn('PlayerLifecycleSizeIs30F70',header)
        self.assertIn('struct PlayerResourceSubtractView : PlayerLifecycleView',resource)
        self.assertIn('struct PlayerStateEffectView : PlayerResourceSubtractView', (ROOT/'src/PlayerStateEffectView.hpp').read_text())
        self.assertNotIn('reinterpret_cast<PlayerResourceSubtractView *>(this)',source)
        self.assertEqual(source.count('SubtractResourceClamped('),3)
        self.assertIn('SubtractResourceClamped(300.0f);',source)
        self.assertIn('SubtractResourceClamped(200.0f);',source)
        self.assertIn('SubtractResourceClamped(100.0f);',source)

    def test_leaf_and_boss_identity_remain_distinct(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        leaf=units['player-subtract-resource-clamped']
        self.assertEqual((leaf['target_address'],leaf['size']),(0x0041F310,51))
        self.assertEqual(leaf['symbol'],'?SubtractResourceClamped@PlayerLifecycleView@@QAEXM@Z')
        # The separate, nonexact ChargeAttack remains a tracked source frontier.
        self.assertIn('PlayerChargeAttackUpdate.cpp',(ROOT/'src/PlayerChargeAttackUpdate.cpp').as_posix())

if __name__=='__main__':unittest.main()
