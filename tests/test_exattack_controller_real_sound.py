"""Real TH09 SoundPlayer linker identity across exact ExAttack Spawn and leaf.

Original PE-bound cold replay is required for exactness; these input tests
reject invented C++ receiver/global aliases before any match-unit comparison.
"""

import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_sound_owners', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class ExAttackRealSoundPlayer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_genuine_global_member_class_and_shared_exact_callee(self):
        source = (ROOT / 'src/ExAttackController.cpp').read_text()
        header = (ROOT / 'src/SoundPlayer.hpp').read_text()
        self.assertIn('#include "SoundPlayer.hpp"', source)
        self.assertNotIn('ExAttackSoundPlayerView', source)
        self.assertIn('g_SoundPlayer.PlaySoundByIdx(45,', source)
        self.assertIn('extern SoundPlayer g_SoundPlayer;', header)
        self.assertIn('void PlaySoundByIdx(int index, int pan);', header)
        frontier.validate_exattack_controller_real_sound(self.units)
        u = self.units['exattack-controller-spawn']
        fields = u['relocations']
        self.assertEqual(len(fields), 7)
        self.assertEqual([(v['offset'], v['type'], v['symbol'], v['target'])
                          for v in fields[:2]], [
            (0x8e, 'DIR32', frontier.EXATTACK_REAL_SOUND_GLOBAL, 0x4dc698),
            (0x93, 'REL32', frontier.EXATTACK_REAL_SOUND_CALLEE, 0x43e2f0),
        ])
        self.assertEqual((u['size'], u['target_address']), (321, 0x4151f0))
        sound = self.units['soundplayer-play-sound-by-idx']
        self.assertEqual((sound['symbol'], sound['target_address'], sound['size']),
                         (frontier.EXATTACK_REAL_SOUND_CALLEE, 0x43e2f0, 140))

    def test_cold_focused_exattack_protects_callee_but_all_deduplicates(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical', side_effect=lambda u: {'unit': u}) as exact:
            result = frontier.replay_exattack_real_sound_exact(prechecked=False)
        self.assertEqual(result, [{'unit': 'soundplayer-play-sound-by-idx'}])
        build.assert_called_once_with('soundplayer-play-sound-by-idx')
        exact.assert_called_once_with('soundplayer-play-sound-by-idx')
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'canonical') as exact:
            result = frontier.replay_exattack_real_sound_exact(prechecked=True)
        self.assertEqual(result, [])
        build.assert_not_called()
        exact.assert_not_called()

    def test_wrong_global_member_relocation_or_exact_method_is_rejected(self):
        mutations = (
            ('exattack-controller-spawn', 'source', 'src/Other.cpp'),
            ('exattack-controller-spawn', 'object', 'build/matching/Other.obj'),
            ('exattack-controller-spawn', 'target_address', 0x4151f1),
            ('exattack-controller-spawn', 'size', 322),
            ('exattack-controller-spawn', 'profile', ['/Ob0']),
            ('soundplayer-play-sound-by-idx', 'source', 'src/Other.cpp'),
            ('soundplayer-play-sound-by-idx', 'object', 'build/matching/Fake.obj'),
            ('soundplayer-play-sound-by-idx', 'symbol',
             '?PlaySoundByIdx@ExAttackSoundPlayerView@@QAEXHH@Z'),
            ('soundplayer-play-sound-by-idx', 'target_address', 0x43e2f1),
            ('soundplayer-play-sound-by-idx', 'size', 139),
            ('soundplayer-play-sound-by-idx', 'profile', ['/Ob0']),
        )
        for name, field, incorrect in mutations:
            invalid = copy.deepcopy(self.units)
            invalid[name][field] = incorrect
            with self.subTest(unit=name, field=field), self.assertRaises(RuntimeError):
                frontier.validate_exattack_controller_real_sound(invalid)
        for index in range(2):
            for field, incorrect in (
                    ('target', 0x4dc699),
                    ('symbol', '?g_SoundPlayer@@3UExAttackSoundPlayerView@@A'),
                    ('type', 'REL32' if index == 0 else 'DIR32'),
                    ('offset', 0x8d if index == 0 else 0x94),
                    ('addend', 4)):
                invalid = copy.deepcopy(self.units)
                invalid['exattack-controller-spawn']['relocations'][index][field] = incorrect
                with self.subTest(index=index, field=field), self.assertRaises(RuntimeError):
                    frontier.validate_exattack_controller_real_sound(invalid)
        invalid = copy.deepcopy(self.units)
        invalid['exattack-controller-spawn']['relocations'].pop()
        with self.assertRaises(RuntimeError):
            frontier.validate_exattack_controller_real_sound(invalid)

    def test_forged_sound_callee_fails_before_canonical_credit(self):
        forged = copy.deepcopy(self.units)
        forged['exattack-controller-spawn']['relocations'][1]['symbol'] = (
            '?PlaySoundByIdx@ExAttackSoundPlayerView@@QAEXHH@Z')
        with (patch.object(frontier.tomllib, 'load', return_value={'units': forged}),
              patch.object(frontier, 'build') as build,
              patch.object(frontier, 'canonical') as exact,
              self.assertRaises(RuntimeError)):
            frontier.replay_exattack_real_sound_exact(prechecked=False)
        build.assert_not_called()
        exact.assert_not_called()


if __name__ == '__main__':
    unittest.main()
