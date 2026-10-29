"""Preserve the native storage identity and the unknown meaning of opcode148."""
import copy
import unittest

from test_ecl_instruction_mask_storage import audit


def field():
    return dict(target_address=0x40B2F3, symbol='?g_GameManager@@3UAsciiGameManagerView@@A',
                addend=0x380, symbol_section=0, symbol_value=0,
                expected_value=0x4A8110, target_value=0x4A8110,
                identity_agrees=True, operand_role_agrees=True,
                instruction_mnemonics=['add', 'add'],
                candidate_memory_width=4, target_memory_width=4,
                candidate_memory_access=3, target_memory_access=3,
                candidate_immediate_values=[1800], target_immediate_values=[1800],
                provenance={'canonical_reference_units': ['game-manager-on-update']})


class RunEclGameManagerValue380Tests(unittest.TestCase):
    def test_one_native_root_does_not_assign_a_gameplay_meaning(self):
        result = audit.validate_game_manager_value380_field([field()])
        self.assertEqual((result['root'], result['addend'], result['width']),
                         (0x4A7D90, 0x380, 4))
        self.assertEqual(result['operation'], 'add1800')
        self.assertEqual(result['gameplay_meaning'], 'unknown')
        self.assertEqual(result['original_nominal_member_ownership'], 'unknown')
        self.assertEqual(result['exactness_credit'], 'none')

    def test_equal_numeric_alias_and_fake_storage_cannot_replace_the_native_root(self):
        retired = '?g_BossLifeMarkerProtocolValue@Th09EclRunState@@3HA'
        self.assertNotIn(retired, audit.DATA_ALIASES)
        for change in (dict(symbol=retired, addend=0),
                       dict(symbol='?g_GameManager@@3UGameManagerSetupLayout@@A'),
                       dict(addend=0), dict(addend=0x37C), dict(addend=False),
                       dict(symbol_section=1), dict(symbol_section=False),
                       dict(symbol_value=4), dict(symbol_value=False),
                       dict(provenance={'independent_global_review': 'same address'})):
            with self.subTest(change=change), self.assertRaisesRegex(ValueError, 'native root'):
                audit.validate_game_manager_value380_field([{**field(), **change}])

    def test_decoded_add_constant_width_and_access_are_independent(self):
        for change in (dict(candidate_memory_width=1), dict(target_memory_width=2),
                       dict(candidate_memory_width=4.0), dict(candidate_memory_access=1),
                       dict(target_memory_access=2), dict(candidate_memory_access=3.0),
                       dict(instruction_mnemonics=['mov', 'add']),
                       dict(candidate_immediate_values=[1801]),
                       dict(target_immediate_values=[]),
                       dict(candidate_immediate_values=[1800.0]),
                       dict(target_immediate_values=(1800,)),
                       dict(operand_role_agrees=False), dict(identity_agrees=None),
                       dict(expected_value=0x4A810C), dict(target_value=0x4A8114)):
            with self.subTest(change=change), self.assertRaisesRegex(ValueError, 'native root'):
                audit.validate_game_manager_value380_field([{**field(), **change}])

    def test_only_the_original_field_can_supply_the_operation(self):
        for rows in ([], [field(), field()], [{**field(), 'target_address': 0x40B2F2}]):
            with self.subTest(rows=rows), self.assertRaisesRegex(ValueError, 'inventory'):
                audit.validate_game_manager_value380_field(copy.deepcopy(rows))


if __name__ == '__main__':
    unittest.main()
