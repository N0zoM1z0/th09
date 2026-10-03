import importlib.util
from pathlib import Path
from types import SimpleNamespace as NS
import unittest
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/audit-ecl-callsite-identities.py'
spec = importlib.util.spec_from_file_location('ecl_audit', SOURCE)
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)
TYPES = NS(x86=NS(X86_OP_IMM=2, X86_OP_MEM=3))


class EclCallsiteAuditTests(unittest.TestCase):
    def test_other_legitimate_callee_does_not_pass_membership(self):
        owners = {'cos': [(0x401060, 'cos-unit')], 'sin': [(0x401070, 'sin-unit')]}
        expected, proof, agrees = audit.direct_identity('cos', owners, {}, 0x401070)
        self.assertEqual(expected, 0x401060)
        self.assertFalse(agrees)
        self.assertEqual(proof['canonical_units'], ['cos-unit'])
        self.assertIsNone(audit.direct_identity('unknown', owners, {}, 0x401060)[2])

    def test_conflicting_owners_cannot_fall_back_to_alias(self):
        owners = {'call': [(1, 'first'), (2, 'second')]}
        with self.assertRaisesRegex(ValueError, 'conflicting canonical'):
            audit.direct_identity('call', owners, {'call': 1}, 1)

    def test_relocation_overlap_bounds_and_type_fail_closed(self):
        good = {'offset': 0, 'type': 'DIR32'}
        for bad in ({'offset': 3, 'type': 'REL32'},
                    {'offset': 5, 'type': 'DIR32'},
                    {'offset': 4, 'type': 'REL16'}):
            with self.assertRaises(ValueError):
                audit.validate_relocation_fields(bytes(8), [good, bad])
        self.assertEqual(audit.validate_relocation_fields(bytes(4), [good]), [good])

    def test_image_base_flag_mask_is_not_an_address_operand(self):
        def instruction(address, mnemonic, value):
            return NS(address=address, mnemonic=mnemonic, operands=[NS(type=2, imm=value)],
                      imm_size=4, imm_offset=1, disp_size=0, disp_offset=0)
        with patch.object(audit, 'capstone', TYPES):
            fields = audit.target_data([instruction(0, 'and', 0x400000),
                                        instruction(5, 'mov', 0x4A7D90)])
        self.assertEqual(len(fields), 1)
        self.assertEqual(fields[0]['address'], 6)
        self.assertEqual(fields[0]['value'], 0x4A7D90)

    def test_unknown_or_mid_instruction_switch_target_is_rejected(self):
        base = audit.BASE
        jump = NS(address=base, size=7, mnemonic='jmp',
                  operands=[NS(type=3, mem=NS(scale=4, base=0, disp=0x4A0000))])
        ret = NS(address=base + 7, size=1, mnemonic='ret', operands=[])
        with patch.object(audit, 'capstone', TYPES):
            with self.assertRaises(ValueError):
                audit.cfg([jump, ret], {}, False)
            with self.assertRaises(ValueError):
                audit.cfg([jump, ret], {'opcode': {'address': 0x4A0000, 'entries': [base + 6]}}, False)
            result = audit.cfg([jump, ret], {'opcode': {'address': 0x4A0000, 'entries': [base + 7]}}, False)
        self.assertEqual(result['roots']['opcode'], [1])
        self.assertEqual(result['table_use_counts'], {'opcode': 1})


if __name__ == '__main__':
    unittest.main()
