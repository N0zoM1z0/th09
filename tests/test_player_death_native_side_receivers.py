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


if __name__ == '__main__':
    unittest.main()
