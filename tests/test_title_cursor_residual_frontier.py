"""Fail-closed, original-SHA-bound Title reverse/inactive cursor codegen guard.

The 38 unlike byte positions were found by complete cold PE/VC7.1 COFF
comparison. No copied target executable, old object or site module is needed
for these malicious-evidence tests; the focused replay tests real source.
"""

import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_cursor_residual_guards', ROOT/'scripts/check-boss-frontiers.py')
frontiers = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontiers)


class TitleCursorReverseInactiveResidualGuards(unittest.TestCase):
    @staticmethod
    def evidence(variant='reverse'):
        offsets = sorted(frontiers.TITLE_CURSOR_REVERSE_INACTIVE_ORIGINAL_RESIDUAL_OFFSETS)
        return {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'variant': variant,
            'original_sha256': frontiers.TITLE_CURSOR_REVERSE_INACTIVE_TARGET_SHA256[variant],
            'target_bytes': 175, 'candidate_bytes': 175,
            'target_instructions': 55, 'candidate_instructions': 55,
            'original_fields': 2, 'bound_fields': 2,
            'ordered_calls': 2, 'ordered_calls_agree': True,
            'original_blocks': 5, 'candidate_blocks': 4,
            'direct_graphs_agree': False,
            'difference_offsets': [hex(n) for n in offsets],
            'overlap_differences': len(offsets),
            'missing_target_bytes': 0, 'extra_candidate_bytes': 0,
            'complete_bytes_equal': False,
        }

    def test_original_38_offsets_protect_137_already_identical_bytes(self):
        positions = frontiers.TITLE_CURSOR_REVERSE_INACTIVE_ORIGINAL_RESIDUAL_OFFSETS
        self.assertEqual(len(positions), 38)
        self.assertEqual(len(positions | (set(range(175)) - positions)), 175)
        for variant in ('reverse', 'inactive'):
            with self.subTest(variant=variant):
                frontiers.guard_title_cursor_reverse_inactive(variant, self.evidence(variant))

    def test_reject_new_differences_in_previously_equal_original_bytes(self):
        for variant in ('reverse', 'inactive'):
            original = self.evidence(variant)
            for offset in (0x00, 0x0C, 0x35, 0x56, 0x67, 0x6C, 0xAE):
                offsets = sorted(int(s, 16) for s in original['difference_offsets'])
                offsets.append(offset)
                offsets.sort()
                corrupted = {**original, 'difference_offsets': [hex(i) for i in offsets],
                             'overlap_differences': len(offsets)}
                with self.subTest(variant=variant, offset=hex(offset)), self.assertRaises(RuntimeError):
                    frontiers.guard_title_cursor_reverse_inactive(variant, corrupted)

    def test_reject_missing_source_binding_and_contradictory_equality(self):
        for variant in ('reverse', 'inactive'):
            original = self.evidence(variant)
            changes = [
                {'original_sha256': '0'*64},
                {'variant': 'active'},
                {'target_bytes': 174}, {'candidate_bytes': 174},
                {'target_instructions': 54}, {'candidate_instructions': 54},
                {'original_fields': 1}, {'bound_fields': 1},
                {'ordered_calls': 1}, {'ordered_calls_agree': False},
                {'original_blocks': 4}, {'candidate_blocks': 6},
                {'candidate_blocks': 4, 'direct_graphs_agree': True},
                {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
                {'overlap_differences': 37},
                {'complete_bytes_equal': True},
                {'diagnostic_only': False},
                {'exactness_credit': 'accepted'},
            ]
            for change in changes:
                with self.subTest(variant=variant, change=change), self.assertRaises(RuntimeError):
                    frontiers.guard_title_cursor_reverse_inactive(variant, {**original, **change})

    def test_reject_malformed_duplicate_and_unordered_original_offsets(self):
        original = self.evidence()
        offsets = original['difference_offsets']
        for altered in (list(reversed(offsets)),
                        offsets[:-1]+[offsets[0]],
                        offsets[:-1]+['0xgg'],
                        offsets[:-1]+['0x08'],
                        offsets[:-1]+['8'],
                        offsets[:-1]+[8],
                        offsets[:-1]+['0X8'],
                        offsets[:-1]+['0x-1'],
                        offsets[:-1]+['0x0'],
                        [], None):
            with self.subTest(altered=altered), self.assertRaises(RuntimeError):
                frontiers.guard_title_cursor_reverse_inactive(
                    'reverse', {**original, 'difference_offsets': altered})

    def test_allow_narrower_physical_residual_without_claiming_exact(self):
        for variant in ('reverse', 'inactive'):
            original = self.evidence(variant)
            half = original['difference_offsets'][:11]
            frontiers.guard_title_cursor_reverse_inactive(
                variant, {**original, 'difference_offsets': half,
                          'overlap_differences': len(half)})
            exact = {**original, 'candidate_blocks': 5,
                     'direct_graphs_agree': True,
                     'difference_offsets': [], 'overlap_differences': 0,
                     'complete_bytes_equal': True}
            frontiers.guard_title_cursor_reverse_inactive(variant, exact)
            # Even a byte-identical *diagnostic* cannot itself grant accepted
            # original-code exactness. The independent canonical unit and
            # Factory replay must still be issued and accepted separately.
            with self.assertRaises(RuntimeError):
                frontiers.guard_title_cursor_reverse_inactive(
                    variant, {**exact, 'exactness_credit': 'accepted'})

    def test_unreviewed_third_variant_rejected(self):
        with self.assertRaises(RuntimeError):
            frontiers.guard_title_cursor_reverse_inactive('active', self.evidence())


if __name__ == '__main__':
    unittest.main()
