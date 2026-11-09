"""The death-state owner uses real side, sound and effect receivers."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeDeathSideReceivers(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/PlayerDeath.cpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            cls.unit = tomllib.load(f)['units']['player-enter-death-state']

    def test_actual_native_subsystems(self):
        s = self.source
        self.assertNotIn('struct PlayerDeathSideStateView', s)
        self.assertNotIn('struct PlayerDeathSoundView', s)
        self.assertNotIn('struct PlayerDeathFrontSideView', s)
        self.assertNotIn('struct PlayerDeathEffectManagerView', s)
        self.assertIn('sideState->shotType20 == 8', s)
        self.assertIn('sideState->effectManager0C->SpawnEffectInFixedSlot(', s)
        self.assertIn('sideState->frontSide18->ClearState(0);', s)
        self.assertIn('g_SoundPlayer.PlaySoundPositionedByIdx(', s)
        self.assertIn('g_SoundPlayer.PlaySoundByIdx(', s)
        self.assertIn('updateState00 = 4;', s)
        self.assertIn('timer303C8 = 0;', s)
        self.assertIn('vmHandle->vms->SetInterrupt(1);', s)
        self.assertIn('active->active = 0;', s)
        self.assertIn('ownerState30410.Reset();', s)
        self.assertIn('header24.Reset();', s)
        self.assertIn('GameManagerValueCCView *>(&g_GameManager.valueB0)', s)
        self.assertIn('SubtractSaturating(1500);', s)
        self.assertNotIn('PlayerDeathCounterView', s)
        self.assertNotIn('g_PlayerDeathCounterState', s)
        self.assertNotIn('PlayerDeathAttackTargetView', s)
        self.assertNotIn('PlayerDeathAttackOwnerView', s)
        self.assertIn('opponentState->enemyManager10->FindActiveEnemyBySideCategory(3)', s)
        self.assertEqual(s.count('CardAttackStartView *>(opponentState->cardAttack14)->StartAttack('), 4)
        for old in ('PlayerDeathTimerView', 'PlayerDeathVmHandleView',
                    'PlayerDeathActiveView', 'PlayerDeathOwnerResetView',
                    'PlayerDeathHeaderResetView', 'PlayerDeathInterruptView'):
            self.assertNotIn(old, s)

    def test_original_complete_function_and_calls(self):
        u = self.unit
        self.assertEqual((u['target_address'], u['size'], len(u['relocations'])),
                         (0x0041E420, 753, 38))
        rows = {r['offset']: r for r in u['relocations']}
        checks = (
            (0x2C, '?g_SoundPlayer@@3VSoundPlayer@@A', 0x004DC698, 'DIR32'),
            (0x31, '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z', 0x0043E380, 'REL32'),
            (0x1D1, '?SpawnEffectInFixedSlot@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@HI@Z', 0x0040CD70, 'REL32'),
            (0x1FD, '?g_SoundPlayer@@3VSoundPlayer@@A', 0x004DC698, 'DIR32'),
            (0x202, '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 0x0043E2F0, 'REL32'),
            (0x20E, '?ClearState@FrontSide@@QAEXH@Z', 0x0041A380, 'REL32'),
        )
        for offset, symbol, target, kind in checks:
            with self.subTest(offset=offset):
                row = rows[offset]
                self.assertEqual((row['symbol'], row['target'], row['type']),
                                 (symbol, target, kind))
        native = (
            (0x22A, '??4ZunTimer@@QAEXH@Z', 0x00401500),
            (0x292, '?SetInterrupt@AnmVm@@QAEXF@Z', 0x00406790),
            (0x2DE, '?Reset@PlayerOwnerStateView@@QAEXXZ', 0x0041D7E0),
            (0x2E6, '?Reset@PlayerHeaderStateView@@QAEXXZ', 0x0041BDC0),
        )
        for offset, symbol, target in native:
            with self.subTest(native_offset=offset):
                row = rows[offset]
                self.assertEqual((row['symbol'], row['target'], row['type']),
                                 (symbol, target, 'REL32'))
        self.assertEqual((rows[0x2B4]['symbol'],rows[0x2B4]['target'],rows[0x2B4].get('addend',0)),
                         ('?g_GameManager@@3UAsciiGameManagerView@@A',0x004A7D90,0xB0))
        self.assertEqual((rows[0x2B9]['symbol'],rows[0x2B9]['target'],rows[0x2B9]['type']),
                         ('?SubtractSaturating@GameManagerValueCCView@@QAEXH@Z',0x00403E30,'REL32'))
        attacks = (
            (0x9D, '?FindActiveEnemyBySideCategory@EnemyManagerView@@QAEPAUEnemyView@@H@Z', 0x0040F7D0),
            (0xBD, '?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z', 0x00403E50),
            (0xF1, '?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z', 0x00403E50),
            (0x13D, '?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z', 0x00403E50),
            (0x186, '?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z', 0x00403E50),
        )
        for offset, symbol, target in attacks:
            with self.subTest(attack_offset=offset):
                row = rows[offset]
                self.assertEqual((row['symbol'], row['target'], row['type']),
                                 (symbol, target, 'REL32'))


if __name__ == '__main__':
    unittest.main()
