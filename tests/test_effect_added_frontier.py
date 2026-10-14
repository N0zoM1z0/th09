"""Live-TU independence guards for EffectManager's 51-byte resource leaf."""

import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'boss_effect_frontier', ROOT / 'scripts/check-boss-frontiers.py')
boss = importlib.util.module_from_spec(spec)
spec.loader.exec_module(boss)


class EffectLoaderWithExactSiblings(unittest.TestCase):
    @staticmethod
    def evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_bytes': 51, 'candidate_bytes': 51,
            'target_instructions': 19, 'candidate_instructions': 19,
            'target_relocations': 4, 'bound_relocations': 4,
            'ordered_calls_match': True,
            'differing_bytes': 8,
            'original_blocks': 3, 'candidate_blocks': 3,
            'complete_direct_graphs_agree': True,
        }

    def test_source_cold_loader_still_keeps_both_exact_tu_functions(self):
        with patch.object(boss, 'build') as build, patch.object(
                boss, 'parsed', return_value=self.evidence()), patch.object(
                boss, 'canonical', side_effect=lambda u: {'unit': u}):
            open_owners, exact = boss.run_effect(protect_exact=True)
        self.assertEqual(open_owners['effect_added_callback']['ordinary_differences'], 8)
        self.assertEqual(open_owners['effect_added_callback']['bound_fields'], 4)
        self.assertEqual([x['unit'] for x in exact],
                         ['effect-manager-update', 'effect-manager-draw',
                          'effect-manager-draw-shared'])
        build.assert_called_once_with('effect-manager-update')

    def test_diagnostic_only_never_fabricates_a_match_receipt(self):
        with patch.object(boss, 'build'), patch.object(
                boss, 'parsed', return_value=self.evidence()):
            report, exact = boss.run_effect(protect_exact=False)
        self.assertEqual(exact, [])
        self.assertEqual(report['effect_added_callback']['target_bytes'], 51)

    def test_original_fields_full_extent_and_call_census_fail_closed(self):
        for corrupt in ({'target_bytes': 50}, {'bound_relocations': 3},
                        {'target_relocations': 3}, {'target_instructions': 18},
                        {'candidate_instructions': 20}, {'ordered_calls_match': False},
                        {'exactness_credit': 'accepted'}, {'diagnostic_only': False},
                        {'original_blocks': 2}, {'candidate_blocks': 4},
                        {'complete_direct_graphs_agree': False}):
            with self.subTest(corrupt=corrupt), patch.object(boss, 'build'), patch.object(
                    boss, 'parsed', return_value={**self.evidence(), **corrupt}), self.assertRaises(RuntimeError):
                boss.run_effect(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
