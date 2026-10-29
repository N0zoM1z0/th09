"""Native root and decoded access widths cannot be replaced by equal addresses."""
import copy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('ecl_mask_storage', ROOT/'scripts/audit-ecl-callsite-identities.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def field():
    return dict(target_address=0x408887, symbol='?g_GameManager@@3UAsciiGameManagerView@@A',
                addend=0x120, symbol_section=0, symbol_value=0,
                expected_value=0x4A7EB0, target_value=0x4A7EB0,
                identity_agrees=True, operand_role_agrees=True,
                instruction_mnemonics=['mov', 'mov'],
                candidate_memory_width=4, target_memory_width=4,
                candidate_memory_access=1, target_memory_access=1,
                provenance={'canonical_reference_units': ['game-manager-on-update']})


class RunEclInstructionMaskStorage(unittest.TestCase):
    def test_original_word_reader_has_one_native_root_and_preserves_ownership_unknown(self):
        proof = audit.validate_instruction_mask_field([field()])
        self.assertEqual((proof['root'], proof['addend'], proof['width']), (0x4A7D90, 0x120, 4))
        self.assertEqual(proof['original_nominal_member_ownership'], 'unknown')

    def test_equal_address_aliases_and_defined_surrogates_are_rejected(self):
        for symbol in ('?g_DifficultyMask@Th09EclRunOwner@@3IA',
                       '?g_EnemyTimelineInstructionMask@@3EA',
                       '?g_GameManager@@3UGameManagerSetupLayout@@A'):
            with self.subTest(symbol=symbol), self.assertRaisesRegex(ValueError, 'native root/width'):
                audit.validate_instruction_mask_field([{**field(), 'symbol': symbol}])
        self.assertNotIn('?g_DifficultyMask@Th09EclRunOwner@@3IA', audit.DATA_ALIASES)
        for change in (dict(symbol_section=1), dict(symbol_section=False),
                       dict(symbol_value=4), dict(addend=0), dict(addend=True),
                       dict(provenance={'independent_alias_review': 'equal address'})):
            with self.subTest(change=change), self.assertRaisesRegex(ValueError, 'native root/width'):
                audit.validate_instruction_mask_field([{**field(), **change}])

    def test_byte_word_or_mismatched_decoded_access_is_rejected(self):
        for change in (dict(candidate_memory_width=1), dict(candidate_memory_width=2),
                       dict(target_memory_width=1), dict(candidate_memory_width=None),
                       dict(candidate_memory_width=4.0), dict(candidate_memory_access=2),
                       dict(target_memory_access=2),
                       dict(instruction_mnemonics=['movzx', 'mov']),
                       dict(operand_role_agrees=False), dict(identity_agrees=None),
                       dict(target_value=0x4A7EB1), dict(expected_value=0x4A7EB1)):
            with self.subTest(change=change), self.assertRaisesRegex(ValueError, 'native root/width'):
                audit.validate_instruction_mask_field([{**field(), **change}])

    def test_missing_duplicate_and_other_original_fields_cannot_substitute(self):
        for rows in ([], [field(), field()], [{**field(), 'target_address': 0x408886}]):
            with self.subTest(rows=rows), self.assertRaisesRegex(ValueError, 'inventory'):
                audit.validate_instruction_mask_field(copy.deepcopy(rows))
