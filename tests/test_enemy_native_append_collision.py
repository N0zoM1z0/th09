"""Enemy collision-box wrapper and boss share the true EnemyView receiver."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SYMBOL='?AppendPlayerCollisionBox@EnemyView@@QAEXPBUFloat3@@0@Z'

class NativeEnemyAppendCollision(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.core=(ROOT/'src/EnemyManagerCore.inl').read_text()
        cls.header=(ROOT/'src/EnemyManager.hpp').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_receiver_and_full_82_byte_callee(self):
        self.assertIn('void EnemyView::AppendPlayerCollisionBox(',self.core)
        self.assertIn('void AppendPlayerCollisionBox(const Float3 *position, const Float3 *size);',self.header)
        self.assertIn('EnemyManagerView *manager00;',self.header)
        self.assertNotIn('EnemyAppendCollisionView',self.core)
        self.assertFalse((ROOT/'src/EnemyAppendCollisionView.hpp').exists())
        self.assertEqual(self.core.count('enemy->AppendPlayerCollisionBox('),2)
        u=self.units['enemy-collision-append-box']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x410250,82,2))
        self.assertEqual(u['symbol'],SYMBOL)
        self.assertEqual([r['target'] for r in u['relocations']],
                         [0x40F5A0,0x40F8E0])

    def test_boss_strict_call_identity_and_nonexact_credit(self):
        inspector=(ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        self.assertIn(repr(SYMBOL),inspector)
        self.assertIn('PLAYER_DAMAGE_FIELDS = (0x8BF, 0x8FF)',inspector)
        self.assertEqual(self.units['enemy-collision-append-box']['size'],82)

if __name__=='__main__': unittest.main()
