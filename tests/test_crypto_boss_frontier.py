"""New exact crypt-table selector and independently exact Decrypt/Encrypt pair.

Pure-input orchestration falsifiers; only source-cold original-bound strict
compare-coff-function replay can award canonical exactness.
"""

import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'crypto_boss_frontier', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class FileSystemCryptColdAudit(unittest.TestCase):
    @staticmethod
    def evidence():
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_sha256':
                'c9195eb6365a7e3d6905b3e2cead12ed480c4fd9ad968992db2b397e9b6fd8a7',
            'target_bytes': 220, 'candidate_bytes': 220,
            'target_instructions': 77, 'candidate_instructions': 77,
            'bound_fields': 12, 'ordered_calls': 2,
            'target_blocks': 11, 'candidate_blocks': 11,
            'complete_direct_graphs_agree': True,
            'complete_overlap_differences': 0,
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': True,
        }

    @staticmethod
    def strict(unit):
        return {'unit': unit, 'logical_bytes': 220 if unit.endswith('table') else 259,
                'physical_bytes': 220 if unit.endswith('table') else 259,
                'relocations': 12 if unit.endswith('table') else 3,
                'result': 'exact'}

    def test_cold_exact_selector_and_both_independent_exact_functions(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'parsed', return_value=self.evidence()) as diag, patch.object(
                frontier, 'canonical', side_effect=self.strict) as canon:
            owners, siblings = frontier.run_decrypt_table(protect_exact=True)
        self.assertEqual(build.call_count, 3)
        self.assertEqual([x.args[0] for x in build.call_args_list], [
            'file-system-try-decrypt-table', 'file-system-decrypt',
            'file-system-encrypt'])
        self.assertEqual(diag.call_count, 1)
        self.assertEqual(canon.call_count, 3)
        self.assertEqual([x['unit'] for x in siblings], [
            'file-system-try-decrypt-table', 'file-system-decrypt',
            'file-system-encrypt'])
        self.assertEqual(owners['file_decrypt_table']['ordinary_differences'], 0)
        self.assertTrue(owners['file_decrypt_table']['exact'])
        self.assertEqual(owners['file_decrypt_table']['bound_fields'], 12)

    def test_focused_selector_still_cold_checks_own_exactness_when_skipping_siblings(self):
        with patch.object(frontier, 'build') as build, patch.object(
                frontier, 'parsed', return_value=self.evidence()), patch.object(
                frontier, 'canonical', side_effect=self.strict):
            owners, exact = frontier.run_decrypt_table(protect_exact=False)
        self.assertEqual(len(exact), 1)
        self.assertEqual(exact[0]['unit'], 'file-system-try-decrypt-table')
        build.assert_called_once_with('file-system-try-decrypt-table')
        self.assertTrue(owners['file_decrypt_table']['exact'])

    def test_incorrect_original_owner_bytes_fields_or_graph_fail_closed(self):
        changes = (
            {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
            {'target_sha256': '0'*64},
            {'target_bytes': 219}, {'candidate_bytes': 219},
            {'target_instructions': 76}, {'candidate_instructions': 76},
            {'bound_fields': 11}, {'ordered_calls': 3},
            {'target_blocks': 10}, {'candidate_blocks': 10},
            {'complete_direct_graphs_agree': False},
            {'complete_overlap_differences': 1},
            {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
            {'complete_bytes_equal': False},
        )
        for modified in changes:
            with self.subTest(modified=modified), patch.object(
                    frontier, 'build'), patch.object(
                    frontier, 'parsed', return_value={**self.evidence(), **modified}), patch.object(
                    frontier, 'canonical', side_effect=self.strict), self.assertRaises(RuntimeError):
                frontier.run_decrypt_table(protect_exact=False)

    def test_fabricated_strict_exact_census_fails_even_with_perfect_diagnostic(self):
        strict = self.strict('file-system-try-decrypt-table')
        for corrupted in ({'logical_bytes': 219}, {'physical_bytes': 219},
                          {'relocations': 11}):
            with self.subTest(corrupted=corrupted), patch.object(
                    frontier, 'build'), patch.object(
                    frontier, 'parsed', return_value=self.evidence()), patch.object(
                    frontier, 'canonical', return_value={**strict, **corrupted}), self.assertRaises(RuntimeError):
                frontier.run_decrypt_table(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
