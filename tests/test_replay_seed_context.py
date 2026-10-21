import copy
import importlib.util
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'replay_seed_context', ROOT/'scripts/probe-replay-seed-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class ReplaySeedEvidenceTests(unittest.TestCase):
    def setUp(self):
        self.row = dict(symbol='real-setter', type='REL32', addend=0,
                        symbol_section=3, symbol_value=0, owner_section=8,
                        owner_value=0, offset=0x206)
        self.unit = dict(target_address=0x420160, size=14, relocations=[])

    def test_setter_must_have_complete_original_body(self):
        body = bytes(range(14))
        accepted = probe.native_definition(body, [], body, self.unit, 0x420160, 14)
        self.assertTrue(accepted['original_equal'])
        for actual, rows, target in (
                (body[:-1], [], body), (body, [], body[:-1]),
                (body[:-1]+b'\xff', [], body), (body, [self.row], body)):
            with self.subTest(actual=actual, rows=rows, target=target):
                with self.assertRaises(ValueError):
                    probe.native_definition(actual, rows, target, self.unit, 0x420160, 14)

    def test_equal_bytes_do_not_authorize_another_target_or_extent(self):
        for key, value in (('target_address', 0x420170), ('size', 13),
                           ('relocations', [self.row])):
            unit = dict(self.unit, **{key: value})
            with self.subTest(key=key), self.assertRaises(ValueError):
                probe.native_definition(bytes(14), [], bytes(14), unit, 0x420160, 14)

    def test_undefined_symbol_is_not_a_visible_definition(self):
        accepted = probe.seed_call([self.row], 'real-setter', defined=True)
        self.assertTrue(accepted['same_object_definition'])
        for section in (0, -1, 8, True):
            with self.subTest(section=section), self.assertRaises(ValueError):
                probe.seed_call([dict(self.row, symbol_section=section)],
                                'real-setter', defined=True)

    def test_actual_call_identity_addend_and_coordinates_are_required(self):
        for change in ({'symbol': 'surrogate'}, {'type': 'DIR32'}, {'addend': 4},
                       {'symbol_value': 1}, {'owner_section': 0},
                       {'owner_section': False}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.seed_call([dict(self.row, **change)], 'real-setter', defined=True)
        for rows in ([], [self.row, self.row]):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                probe.seed_call(rows, 'real-setter', defined=True)

    def test_baseline_cannot_silently_gain_a_same_object_setter(self):
        with self.assertRaises(ValueError):
            probe.seed_call([self.row], 'real-setter', defined=False)
        row = dict(self.row, symbol_section=0)
        self.assertFalse(probe.seed_call([row], 'real-setter', defined=False)
                         ['same_object_definition'])

    def test_source_extraction_rejects_ambiguous_or_changed_boundaries(self):
        inputs = probe.source_inputs()
        for mutation in ('duplicate', 'missing', 'reordered'):
            altered = copy.copy(inputs)
            text = altered['ReplayRuntimeLeaves.cpp'].decode()
            if mutation == 'duplicate':
                text += '\nvoid ReplayRngView::SetSeed(unsigned short x) {}\n'
            elif mutation == 'missing':
                text = text.replace('void ReplayRngView::SetSeed(', 'void Other::SetSeed(')
            else:
                text = 'void ReplayRngView::ResetGenerationCount() {}\n' + text
            altered['ReplayRuntimeLeaves.cpp'] = text.encode()
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                probe.case_source('seed-before', altered)
        altered = dict(inputs, **{'ReplayInput.cpp': b'no configuration copy'})
        with self.assertRaises(ValueError):
            probe.case_source('config-assignment', altered)
        with self.assertRaises(ValueError):
            probe.case_source('invented-case', inputs)

    def test_missing_or_external_project_includes_fail_closed(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root/'src').mkdir()
            (root/'src/ReplayInput.cpp').write_text('#include "absent.hpp"\n')
            with self.assertRaises(ValueError):
                probe.source_inputs(root)
            (root/'outside.hpp').write_text('outside source scope')
            (root/'src/absent.hpp').symlink_to(root/'outside.hpp')
            with self.assertRaises(ValueError):
                probe.source_inputs(root)

    def test_output_outside_analysis_is_rejected_before_creation(self):
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary)/'must-not-exist'
            with self.assertRaises(ValueError):
                probe.run('seed-before', destination)
            self.assertFalse(destination.exists())


if __name__ == '__main__':
    unittest.main()
