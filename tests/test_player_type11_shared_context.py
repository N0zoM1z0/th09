"""Target-independent rejection tests for the shared Type11 focused replay."""
import copy
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch, call

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type11_shared_frontier', ROOT/'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class Type11SharedReplay(unittest.TestCase):
    @staticmethod
    def caller():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256': '3d6ac30acacf80d7a3b9919c4afeb541a9a42b8793e7f7035eab86d384760e92',
            'target_bytes': 732, 'candidate_bytes': 732, 'relocations': 36,
            'target_instructions': 233, 'candidate_instructions': 233,
            'target_calls': 35, 'candidate_calls': 35, 'ordered_calls_agree': True,
            'direct_blocks': [11, 11], 'complete_direct_graphs_agree': True,
            'complete_differences_if_same_extent': 0, 'difference_offsets': [],
            'complete_bytes_agree': True,
        }

    def replay(self, evidence, protect_exact=True):
        # The three existing boss guards have independent corruption tests;
        # isolate source/profile reuse and the newly exact caller contract here.
        with patch.object(boss, 'run_ecl', return_value=({'runecl': {}}, [])) as ecl, \
                patch.object(boss, 'parsed', side_effect=[
                    {'overlapping_differences': 44}, {'complete_differences': 29},
                    evidence]), \
                patch.object(boss, 'guard_exattack_type19_update') as trail, \
                patch.object(boss, 'guard_enemy_update') as enemy, \
                patch.object(boss, 'cold_replay_exact_source', side_effect=[
                    [{'unit': 'shared'}], [{'unit': 'remaining-math'}]]) as exact, \
                patch.object(boss, 'build') as build:
            result = boss.run_player_type11(protect_exact=protect_exact)
            return result, ecl, trail, enemy, exact, build

    def test_reuses_boss_object_and_replays_only_changed_sources(self):
        (owners, exact), ecl, trail, enemy, replay, build = self.replay(self.caller())
        ecl.assert_called_once_with(include_trail=False, include_interpreter=True,
                                    protect_exact=False)
        trail.assert_called_once()
        enemy.assert_called_once()
        self.assertEqual(replay.call_args_list, [
            call('src/EclManager.cpp', boss.ECL_EXACT_CARRIERS,
                 already_built=('build/matching/EclManager.obj',)),
            call('src/Float3.cpp', {'build/matching/Float3.obj': 4})])
        build.assert_not_called()
        self.assertEqual(len(exact), 2)
        self.assertEqual(owners['player_position_type11']['different_bytes'], 0)

    def test_diagnostics_only_still_cold_builds_the_caller_profile(self):
        (_, exact), _, _, _, replay, build = self.replay(self.caller(), False)
        self.assertEqual(exact, [])
        replay.assert_not_called()
        build.assert_called_once_with('player-position-callback-type11')

    def test_rejects_incomplete_or_contradictory_original_comparisons(self):
        for field, bad in (
                ('diagnostic_only', False), ('exactness_credit', 'partial'),
                ('target_sha256', '0'*64), ('target_bytes', 731),
                ('candidate_bytes', 731), ('relocations', 35),
                ('target_instructions', 232), ('candidate_instructions', 232),
                ('target_calls', 34), ('candidate_calls', 34),
                ('ordered_calls_agree', False), ('direct_blocks', [11, 10]),
                ('complete_direct_graphs_agree', False),
                ('complete_differences_if_same_extent', 1),
                ('complete_differences_if_same_extent', False),
                ('difference_offsets', [0x2DC]), ('complete_bytes_agree', False)):
            report = copy.deepcopy(self.caller())
            report[field] = bad
            with self.subTest(field=field), self.assertRaises(RuntimeError):
                self.replay(report)


if __name__ == '__main__':
    unittest.main()
