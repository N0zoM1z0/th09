"""Constructor byte/relocation evidence remains separate from caller similarity."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('title_score_probe', ROOT/'scripts/probe-title-score-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class ScoreConstructorEvidence(unittest.TestCase):
    def test_exact_body_alias_requires_complete_bytes_and_no_fields(self):
        original = bytes(range(18))
        exact = probe.constructor_observation(original, [], original)
        self.assertTrue(exact['native_body_equal'])
        self.assertEqual(exact['exactness_credit'], 'none')
        for raw, rows in ((original+b'\0\0', []), (original[:-1], []),
                          (b'\xff'+original[1:], []),
                          (original, [dict(offset=9, type='REL32', symbol='_memset', addend=0)])):
            with self.subTest(raw=raw, rows=rows):
                result = probe.constructor_observation(raw, rows, original)
                self.assertFalse(result['native_body_equal'])
                self.assertEqual(result['exactness_credit'], 'none')

    def test_changed_native_constructor_source_requires_review(self):
        with self.assertRaises(ValueError):
            probe.source_context('int TitleScreenView::DrawResult() {}',
                '    int InsertIntoTable();',
                'ScoreRecordCtorView::ScoreRecordCtorView()\n{ memset(this,1,sizeof(*this)); }',
                'ctor-table')

    def test_unknown_context_and_missing_owner_are_rejected(self):
        for source, variant in [('', 'table-storage'), ('', 'invented')]:
            with self.subTest(variant=variant), self.assertRaises(ValueError):
                probe.source_context(source, '', '', variant)
