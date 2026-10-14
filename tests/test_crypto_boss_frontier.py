"""Original decryption-table leaf cold owner and exact crypt pair guards."""

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
            'target_bytes': 220, 'candidate_bytes': 220,
            'target_instructions': 77, 'candidate_instructions': 77,
            'bound_fields': 12, 'ordered_calls': 2,
            'target_blocks': 11, 'candidate_blocks': 11,
            'complete_direct_graphs_agree': True,
            'complete_overlap_differences': 38,
            'complete_bytes_equal': False,
        }

    def test_full_original_leaf_and_both_independent_exact_functions(self):
        with patch.object(frontier, 'build_nonexact') as compile_nonexact, patch.object(
                frontier, 'build') as build, patch.object(
                frontier, 'parsed', return_value=self.evidence()), patch.object(
                frontier, 'canonical', side_effect=lambda x: {'unit': x}):
            owners, siblings = frontier.run_decrypt_table(protect_exact=True)
        compile_nonexact.assert_called_once_with(
            'src/FileDecryptTable.cpp','build/matching/FileDecryptTable.obj',
            reference_unit='file-system-decrypt')
        self.assertEqual(build.call_count, 2)
        self.assertEqual([x['unit'] for x in siblings],
                         ['file-system-decrypt','file-system-encrypt'])
        self.assertEqual(owners['file_decrypt_table']['ordinary_differences'],38)
        self.assertEqual(owners['file_decrypt_table']['bound_fields'],12)

    def test_negative_or_open_state_never_creates_exact_credit(self):
        with patch.object(frontier,'build_nonexact'), patch.object(
                frontier,'build') as build, patch.object(
                frontier,'parsed',return_value=self.evidence()):
            owners, exact=frontier.run_decrypt_table(protect_exact=False)
        self.assertEqual(exact, [])
        self.assertEqual(owners['file_decrypt_table']['ordered_calls'],2)
        build.assert_not_called()

    def test_incorrect_original_owner_or_missing_field_fails_closed(self):
        for modified in (
                {'diagnostic_only': False}, {'exactness_credit': 'accepted'},
                {'target_bytes':219}, {'candidate_bytes':219},
                {'target_instructions':76}, {'candidate_instructions':76},
                {'bound_fields':11}, {'ordered_calls':3},
                {'target_blocks':10}, {'candidate_blocks':10},
                {'complete_direct_graphs_agree':False},
                {'complete_overlap_differences':37},
                {'complete_bytes_equal':True}):
            with self.subTest(modified=modified), patch.object(
                    frontier,'build_nonexact'), patch.object(
                    frontier,'parsed',return_value={**self.evidence(),**modified}), self.assertRaises(RuntimeError):
                frontier.run_decrypt_table(protect_exact=False)


if __name__ == '__main__':
    unittest.main()
