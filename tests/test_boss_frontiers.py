"""No-target input-gate regressions for cold large-owner replay orchestration."""

import importlib.util
from pathlib import Path
from unittest import TestCase
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location('check_boss_frontiers',
                                            ROOT / 'scripts/check-boss-frontiers.py')
frontiers = importlib.util.module_from_spec(sp)
sp.loader.exec_module(frontiers)


class BossFrontierChecks(TestCase):
    def test_noncrediting_never_uses_claimed_acceptance(self):
        document = {'diagnostic_only': True, 'exactness_credit': 'none',
                    'target_bytes': 600, 'bound_fields': 27}
        self.assertIs(frontiers.noncrediting(document, name='test',
                                            target_size=600, fields=27), document)
        for change in ({'diagnostic_only': False}, {'exactness_credit': 'accepted'},
                       {'exactness_credit': ''}, {'target_bytes': 594},
                       {'bound_fields': 26}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontiers.noncrediting({**document, **change}, name='test',
                                       target_size=600, fields=27)

    def test_exact_physical_extent_may_exceed_logical_authored_extent(self):
        for logical, physical in ((215, 232), (2195, 2216), (147, 147)):
            complete = {'result': 'exact', 'size': logical, 'matched_bytes': logical,
                        'compared_size': physical, 'matched_compared_bytes': physical,
                        'relocations': [{}, {}]}
            with self.subTest(logical=logical, physical=physical), patch.object(
                    frontiers, 'parsed', return_value=complete):
                result = frontiers.canonical('sample')
                self.assertEqual((result['logical_bytes'], result['physical_bytes']),
                                 (logical, physical))
                self.assertEqual(result['result'], 'exact')

    def test_reject_partial_or_missing_physical_match(self):
        full = {'result': 'exact', 'size': 215, 'matched_bytes': 215,
                'compared_size': 232, 'matched_compared_bytes': 232}
        for field, value in (('result', 'near'), ('matched_bytes', 214),
                             ('compared_size', 214), ('matched_compared_bytes', 231)):
            with self.subTest(field=field), patch.object(
                    frontiers, 'parsed', return_value={**full, field: value}), self.assertRaises(RuntimeError):
                frontiers.canonical('sample')

    def test_malformed_and_rejected_diagnostic_documents_fail_closed(self):
        for output in ('not-json', '[]', '{"error": "bad owner"}',
                       '{"result": "error", "message": "unknown target"}'):
            with self.subTest(output=output), patch.object(
                    frontiers, 'execute', return_value=output), self.assertRaises(RuntimeError):
                frontiers.parsed(['scripts/diagnostic.py'], diagnostic=True)

    def test_title_protects_three_independent_exact_units(self):
        checked = []
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical', side_effect=lambda unit: checked.append(unit) or {'unit': unit}):
            owners, exact = frontiers.run_title(protect_exact=True)
        self.assertEqual(owners, {})
        self.assertEqual([row['unit'] for row in exact],
                         ['title-screen-setup-thread', 'title-screen-music-room',
                          'title-screen-draw-music-room'])
        self.assertEqual(build.call_count, 2)
        self.assertEqual(checked, [row['unit'] for row in exact])

    def test_target_independent_title_probe_does_not_invent_receipts(self):
        with patch.object(frontiers, 'build') as build, patch.object(
                frontiers, 'canonical') as canonical:
            self.assertEqual(frontiers.run_title(protect_exact=False), ({}, []))
        build.assert_not_called()
        canonical.assert_not_called()
