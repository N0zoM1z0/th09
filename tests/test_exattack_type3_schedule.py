"""Target-independent Type3 field-order and real write-chronology gates."""

import importlib.util
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest

try:
    import capstone
except ImportError:  # Public CI intentionally runs without site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location(
    'type3_z_schedule', ROOT / 'scripts/inspect-exattack-type3-schedule.py')
inspection = importlib.util.module_from_spec(sp)
sp.loader.exec_module(inspection)


def instruction(offset, name='nop', op_str='', *, destination=None):
    operands = ([] if destination is None else [SimpleNamespace(
        type=capstone.x86.X86_OP_IMM, imm=destination)])
    return SimpleNamespace(address=inspection.BASE + offset, mnemonic=name,
                           op_str=op_str, operands=operands)


def chronology(y_offset, z_offset, tail_offset):
    result = [instruction(0x10, 'call', destination=inspection.MIDDLE_Y_CALLEE),
              instruction(y_offset, 'call', destination=inspection.MIDDLE_Y_CALLEE),
              instruction(0x260, 'call', destination=inspection.MIDDLE_Y_CALLEE),
              instruction(z_offset, 'mov', inspection.MIDDLE_Z_FIELD),
              instruction(tail_offset, 'mov', inspection.TAIL_X_FIELD)]
    return sorted(result, key=lambda item: item.address)


class ExAttackType3ScheduleTests(unittest.TestCase):
    @unittest.skipIf(capstone is None, "Capstone unavailable under python -S")
    def test_the_verified_original_and_two_wrong_instruction_schedules(self):
        cases = [(0x18E, 0x196, 0x19D, 'matches-original-chronology'),
                 (0x195, 0x18E, 0x19D, 'zero-before-middle-y-call'),
                 (0x18E, 0x1A5, 0x19D, 'zero-after-tail-x-load')]
        for y, z, tail, expected in cases:
            with self.subTest(expected=expected):
                result = inspection.middle_z_order(chronology(y, z, tail))
                self.assertEqual(result['classification'], expected)
                self.assertEqual(result['middle_z_store_offset'], hex(z))

    @unittest.skipIf(capstone is None, "Capstone unavailable under python -S")
    def test_unreviewed_missing_or_duplicate_semantic_instruction_is_rejected(self):
        original = chronology(0x18E, 0x196, 0x19D)
        for changed in (original[:-1], original[:-2], original + [
                instruction(0x1B0, 'mov', inspection.MIDDLE_Z_FIELD)]):
            with self.assertRaises(ValueError):
                inspection.middle_z_order(changed)

    @staticmethod
    def synthetic_binding():
        destination = 0x004A8380
        name = '?testOriginalAddress@@3HA'
        target = bytearray(inspection.SIZE)
        fields, rows = [], []
        for i in range(38):
            original_off = 4 + 18*i
            candidate_off = 9 + 18*i
            kind = 'DIR32' if i % 2 == 0 else 'REL32'
            original_value = destination - (inspection.BASE + original_off + 4 if kind == 'REL32' else 0)
            struct.pack_into('<I', target, original_off, original_value & 0xFFFFFFFF)
            fields.append((original_off, kind, destination))
            rows.append({'offset': candidate_off, 'type': kind,
                         'symbol': name, 'addend': 0})
        return bytes(target), fields, rows, {name: destination}

    def test_every_original_and_candidate_field_is_bound_independently(self):
        target, fields, rows, referents = self.synthetic_binding()
        candidate = inspection.bind_candidate(bytes(inspection.SIZE), rows,
                                              target, fields, referents)
        self.assertEqual(len(candidate), inspection.SIZE)
        self.assertEqual(len(rows), 38)
        for row, (_, kind, dest) in zip(rows, fields):
            expect = dest - (inspection.BASE + row['offset'] + 4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', candidate, row['offset'])[0],
                             expect & 0xFFFFFFFF)

    def test_reject_unsupported_relocation_order_addend_or_target_corruption(self):
        target, fields, rows, referents = self.synthetic_binding()
        samples = [rows[:-1], list(reversed(rows)),
                   [dict(rows[0], symbol='?invented@@3HA')] + rows[1:],
                   [dict(rows[0], addend=4)] + rows[1:],
                   [dict(rows[0], type='REL32')] + rows[1:],
                   rows[:1] + [dict(rows[1], offset=rows[0]['offset'] + 1)] + rows[2:]]
        for modified in samples:
            with self.subTest(sample=modified[:2]), self.assertRaises(ValueError):
                inspection.bind_candidate(bytes(inspection.SIZE), modified,
                                          target, fields, referents)
        corrupt = bytearray(target)
        corrupt[fields[0][0]] ^= 1
        with self.assertRaises(ValueError):
            inspection.bind_candidate(bytes(inspection.SIZE), rows, bytes(corrupt),
                                      fields, referents)
        with self.assertRaises(ValueError):
            inspection.bind_candidate(bytes(inspection.SIZE - 1), rows,
                                      target, fields, referents)


if __name__ == '__main__':
    unittest.main()
