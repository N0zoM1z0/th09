"""Source-context diagnostics must keep missing bytes and real callees visible."""
from copy import deepcopy
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import sys
import tomllib
import unittest
from unittest.mock import Mock, patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect_resource_context', ROOT / 'scripts/probe-effect-resource-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class EffectResourceContext(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_real_callee_and_caller_profiles_remain_separate(self):
        callee, profile = probe.validate_units(self.units)
        self.assertEqual(callee['size'], 20)
        self.assertEqual(profile, self.units['effect-manager-update']['profile'])
        self.assertIn('/EHsc', callee['profile'])
        self.assertNotIn('/EHsc', profile)

    def test_wrong_callee_metadata_is_rejected_before_compilation(self):
        for key, value in (
            ('source', 'src/EffectManager.cpp'), ('size', 19),
            ('symbol', '?GetAnm@FakeManager@@QAEPAVAnmLoaded@@H@Z'),
            ('target_address', 0x00401661), ('relocations', [{'offset': 1}]),
            ('profile', []),
        ):
            units = deepcopy(self.units)
            units['anm-manager-get-anm'][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                probe.validate_units(units)
        for name in ('anm-manager-get-anm', 'effect-manager-update'):
            units = deepcopy(self.units)
            del units[name]
            with self.subTest(missing=name), self.assertRaises(ValueError):
                probe.validate_units(units)

    @staticmethod
    def comparison_fixture():
        # Synthetic data, unrelated to any TH09 instruction or target bytes.
        target = bytes(range(51))
        fields = [(4, 'DIR32', 100), (15, 'REL32', 200),
                  (27, 'DIR32', 300), (36, 'REL32', 400)]
        audit = SimpleNamespace(
            BASE=1, SIZE=51, SYMBOL='fixture', TARGET_FIELDS=tuple(fields),
            TARGET_BLOCKS=3, CALL_ORDER=[200, 400], require_original=Mock(),
            DESTINATIONS={'??_C@_09NMANGJFI@etama?4anm?$AA@': 300},
            bind_candidate=Mock(return_value=(target[:45], fields)),
            decode_owner=Mock(side_effect=[([None] * 19, fields, [200, 400]),
                                           ([None] * 15, fields, [200, 400])]))
        coff = SimpleNamespace(
            pe_bytes_at=Mock(side_effect=[target, b'etama.anm\x00']),
            object_function=Mock(return_value=(target[:45], [])))
        modules = {
            'capstone': SimpleNamespace(x86=SimpleNamespace(X86_OP_IMM=1)),
            'diagnostic_branch_alignment': SimpleNamespace(
                direct_control_flow=Mock(return_value=[1, 2, 3])),
        }
        return audit, coff, modules

    def test_zero_overlap_differences_never_hides_six_missing_bytes(self):
        audit, coff, modules = self.comparison_fixture()
        with patch.dict(sys.modules, modules):
            report = probe.compare_callback(coff, audit, Path('fixture.obj'), b'image')
        self.assertEqual(report['overlapping_differences'], 0)
        self.assertEqual(report['missing_target_bytes'], 6)
        self.assertEqual(report['extra_candidate_bytes'], 0)
        self.assertFalse(report['complete_bytes_equal'])
        audit.require_original.assert_called_once()

    def test_changed_decoded_call_is_rejected_even_with_matching_fields(self):
        audit, coff, modules = self.comparison_fixture()
        fields = list(audit.TARGET_FIELDS)
        audit.decode_owner.side_effect = [([None] * 19, fields, [200, 400]),
                                          ([None] * 15, fields, [200, 401])]
        with patch.dict(sys.modules, modules), self.assertRaises(ValueError):
            probe.compare_callback(coff, audit, Path('fixture.obj'), b'image')

    def test_output_outside_analysis_stops_before_any_target_or_compiler_work(self):
        with (patch.object(sys, 'argv', ['probe', '--output', str(ROOT / 'build')]),
              patch.object(probe, 'run') as run, self.assertRaises(SystemExit) as result):
            probe.main()
        self.assertEqual(result.exception.code, 2)
        run.assert_not_called()


if __name__ == '__main__':
    unittest.main()
