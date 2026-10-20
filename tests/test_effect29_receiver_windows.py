"""Reject incomplete or misattributed native polar/accumulation windows."""
import copy
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('effect29_windows', ROOT/'scripts/inspect-effect-type29.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def instruction(offset, mnemonic, operand='', destination=0):
    return SimpleNamespace(address=audit.BASE+offset, mnemonic=mnemonic, op_str=operand,
                           operands=[SimpleNamespace(imm=destination)])


class Effect29ReceiverWindowTests(unittest.TestCase):
    def fixture(self):
        result, center = [], 0
        for n in range(13):
            offset = 0xBF if n == 0 else (audit.CENTER_RELOAD_OFFSETS[center]-13
                                        if n % 2 else result[-1].address-audit.BASE+80)
            result.append(instruction(offset, 'call', destination=audit.POLAR))
            result.extend([instruction(offset+5, 'lea', 'eax, [edi + 0xc]'),
                           instruction(offset+8, 'push', 'eax')])
            if n % 2:
                result.append(instruction(audit.CENTER_RELOAD_OFFSETS[center], 'lea',
                                          'ecx, [ebp - 0x24]'))
                center += 1
            result.append(instruction(offset+16, 'call', destination=audit.ACCUMULATE))
        return result

    def test_actual_six_center_reloads_are_distinct_from_seven_vertex_calls(self):
        code = self.fixture()
        row = audit.receiver_windows(code, original=True)
        self.assertEqual((len(row['windows']), row['center_ecx_reloads'], row['vertex_ecx_reloads']),
                         (13, 6, 0))
        candidate = [i for i in code if not i.op_str.startswith('ecx,')]
        current = audit.receiver_windows(candidate)
        self.assertEqual((current['center_ecx_reloads'], current['vertex_ecx_reloads']), (0, 0))
        with self.assertRaisesRegex(ValueError, 'six local-center'):
            audit.receiver_windows(candidate, original=True)

    def test_read_only_ecx_operands_and_pre_call_setup_are_not_reloads(self):
        code = self.fixture()
        code.insert(0, instruction(0xB0, 'mov', 'ecx, esi'))
        code.insert(2, instruction(0xC4, 'cmp', 'ecx, eax'))
        row = audit.receiver_windows(code, original=True)
        self.assertEqual((row['center_ecx_reloads'], row['vertex_ecx_reloads']), (6, 0))

    def test_missing_pair_interposed_call_wrong_owner_and_reload_fail_closed(self):
        original = self.fixture()
        bad = [original[:-1]]
        inserted = copy.deepcopy(original)
        inserted.insert(1, instruction(0xC4, 'call', destination=0x4010B0))
        bad.append(inserted)
        wrong = copy.deepcopy(original)
        wrong[3].operands[0].imm = 0x405731
        bad.append(wrong)
        wrong_reload = copy.deepcopy(original)
        next(i for i in wrong_reload if i.op_str.startswith('ecx,')).op_str = 'ecx, [ebp - 0x20]'
        bad.append(wrong_reload)
        for code in bad:
            with self.subTest(code=code), self.assertRaises(ValueError):
                audit.receiver_windows(code, original=True)


if __name__ == '__main__':
    unittest.main()
