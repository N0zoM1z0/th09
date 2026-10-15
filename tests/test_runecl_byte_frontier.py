"""Original-anchored, target-independent full 15,564-byte RunEcl regression gates.

Do not confuse one near-matching opcode with an accepted exact function. Only
Factory canonical replay may grant exact credit; this tests rejection paths.
"""
import copy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
loader = importlib.util.spec_from_file_location(
    'strict_runecl', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(loader)
loader.loader.exec_module(frontier)


class OriginalBoundRunEclFrontier(unittest.TestCase):
    @staticmethod
    def canonical_evidence():
        # Original physical windows and quantities independently decoded from
        # the retail TH09 binary; input fixtures are deliberately synthetic.
        four = [0x278, 0x27A, 0x27B, 0x27C, 0x27D, 0x27E, 0x280]
        timeout = [0x30B0, 0x30B1, 0x30B2, 0x30B3, 0x30B4, 0x30B5,
                   0x30B6, 0x30B7, 0x30B9, 0x30BA, 0x30BB, 0x30BC,
                   0x30BD, 0x30BE, 0x30BF, 0x30C0, 0x30C1, 0x30C2,
                   0x30C4, 0x30C5, 0x30C6, 0x30C8, 0x30CA]
        complete = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'target_owner_sha256': frontier.RUNECL_TARGET_OWNER_SHA,
            'identity_audit_passed': True,
            'physical_bytes': 15564, 'relocations': 598,
            'complete_bytes_equal': False,
            'field_different_bytes': 0, 'table_different_bytes': 0,
            'different_bytes': 30, 'ordinary_different_bytes': 30,
            'differences': [
                {'offset': off, 'target': 2, 'candidate': 3,
                 'relocation_field': False} for off in four + timeout],
        }
        shapes = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'identity_audit_passed': True,
            'candidate_code_bytes': 14792, 'target_code_bytes': 14792,
            'unique_handlers': 148, 'matching_handler_shapes': 146,
            'mismatching_handler_shapes': 2,
            'mismatches': [
                {'opcode': opcode, 'candidate_offset': off,
                 'target_offset': off, 'candidate_bytes': size,
                 'target_bytes': size, 'candidate_instructions': n,
                 'target_instructions': n, 'matching_instructions': 6,
                 'exact_shape': False}
                for opcode, (off, size, _, n)
                in frontier.RUNECL_WINDOWS.items()],
        }
        return complete, shapes

    def test_exact_full_physical_30_byte_frontier_is_noncrediting(self):
        complete, shapes = self.canonical_evidence()
        report = frontier.validate_runecl_byte_frontier(complete, shapes)
        self.assertEqual(report['ordinary_differences'], 30)
        self.assertEqual(report['differences_by_opcode'], {4: 7, 155: 23})
        self.assertEqual(report['remaining_opcode_mismatches'], [4, 155])
        self.assertEqual(report['matching_handler_shapes'], 146)

    def test_one_opcode_can_be_fixed_without_introducing_a_false_exact_claim(self):
        complete, shapes = self.canonical_evidence()
        complete['differences'] = [x for x in complete['differences']
                                   if x['offset'] >= 0x30B0]
        complete['different_bytes'] = 23
        complete['ordinary_different_bytes'] = 23
        shapes['mismatches'] = shapes['mismatches'][1:]
        shapes['matching_handler_shapes'] = 147
        shapes['mismatching_handler_shapes'] = 1
        result = frontier.validate_runecl_byte_frontier(complete, shapes)
        self.assertEqual(result['differences_by_opcode'], {4: 0, 155: 23})
        self.assertEqual(result['remaining_opcode_mismatches'], [155])

    def test_full_physical_convergence_is_still_noncrediting(self):
        complete, shapes = self.canonical_evidence()
        complete.update(differences=[], ordinary_different_bytes=0,
                        different_bytes=0, complete_bytes_equal=True)
        shapes.update(mismatches=[], matching_handler_shapes=148,
                      mismatching_handler_shapes=0)
        report = frontier.validate_runecl_byte_frontier(complete, shapes)
        self.assertEqual(report['ordinary_differences'], 0)
        self.assertEqual(report['remaining_opcode_mismatches'], [])
        self.assertEqual(report['matching_handler_shapes'], 148)
        self.assertEqual(complete['exactness_credit'], 'none')

    def test_consistent_bytes_and_shapes_are_required(self):
        complete, shapes = self.canonical_evidence()
        complete.update(differences=[], ordinary_different_bytes=0,
                        different_bytes=0)
        shapes.update(mismatches=[], matching_handler_shapes=148,
                      mismatching_handler_shapes=0)
        with self.assertRaisesRegex(RuntimeError, 'equality disagrees'):
            frontier.validate_runecl_byte_frontier(complete, shapes)
        complete['complete_bytes_equal'] = True
        shapes['mismatches'] = [self.canonical_evidence()[1]['mismatches'][0]]
        shapes.update(matching_handler_shapes=147, mismatching_handler_shapes=1)
        with self.assertRaisesRegex(RuntimeError, 'residual and opcode shape'):
            frontier.validate_runecl_byte_frontier(complete, shapes)

    def test_changed_target_identity_field_table_or_code_extent_fails(self):
        for key, value in (
                ('target_owner_sha256', '0' * 64),
                ('physical_bytes', 15563), ('relocations', 597),
                ('identity_audit_passed', False),
                ('field_different_bytes', 1), ('table_different_bytes', 1),
                ('complete_bytes_equal', True),
                ('diagnostic_only', False), ('exactness_credit', 'accepted')):
            a, b = self.canonical_evidence()
            a[key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                frontier.validate_runecl_byte_frontier(a, b)
        for key, value in (
                ('target_code_bytes', 14791), ('candidate_code_bytes', 14791),
                ('unique_handlers', 147), ('matching_handler_shapes', 145),
                ('identity_audit_passed', False),
                ('diagnostic_only', False), ('exactness_credit', 'accepted')):
            a, b = self.canonical_evidence()
            b[key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                frontier.validate_runecl_byte_frontier(a, b)

    def test_regressed_byte_count_malformed_or_foreign_opcode_is_rejected(self):
        bad = (
            # Even 30 differences are invalid if one leaves both original
            # opcode windows or exceeds the individual seven-byte ceiling.
            lambda a: a['differences'][0].update(offset=0x294),
            lambda a: a['differences'][0].update(offset=0x30DD),
            lambda a: a['differences'][0].update(offset=0x30CF),
            lambda a: a['differences'][0].update(relocation_field=True),
            lambda a: a['differences'][0].update(target=3),
            lambda a: a['differences'][0].update(candidate=256),
            lambda a: a['differences'][0].update(offset=-1),
            lambda a: a['differences'].pop(),
            lambda a: a.update(different_bytes=31),
            lambda a: a.update(ordinary_different_bytes=31),
            lambda a: a['differences'].append({'offset': 0x287, 'target': 2,
                                                'candidate': 3,
                                                'relocation_field': False}),
            lambda a: a['differences'][1].update(offset=0x30B0),
        )
        for alter in bad:
            a, b = self.canonical_evidence()
            alter(a)
            with self.subTest(alter=alter), self.assertRaises(RuntimeError):
                frontier.validate_runecl_byte_frontier(a, b)

    def test_handler_record_wrong_physical_offset_or_instruction_census_fails(self):
        mutations = (
            ('opcode', 5), ('target_offset', 0x277 + 1),
            ('candidate_offset', 0x277 + 1), ('target_bytes', 28),
            ('candidate_bytes', 28), ('target_instructions', 7),
            ('candidate_instructions', 7), ('matching_instructions', 5),
            ('exact_shape', True),
        )
        for key, value in mutations:
            a, b = self.canonical_evidence()
            b['mismatches'][0][key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                frontier.validate_runecl_byte_frontier(a, b)
        for name, mutation in (
                ('duplicate', lambda b: b['mismatches'].append(
                    copy.deepcopy(b['mismatches'][0]))),
                ('census', lambda b: b.update(mismatching_handler_shapes=3)),
                ('matching', lambda b: b.update(matching_handler_shapes=145))):
            a, b = self.canonical_evidence()
            mutation(b)
            with self.subTest(name=name), self.assertRaises(RuntimeError):
                frontier.validate_runecl_byte_frontier(a, b)


if __name__ == '__main__':
    unittest.main()
