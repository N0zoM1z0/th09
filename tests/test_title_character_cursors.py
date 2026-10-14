"""No-target acceptance guards for TH09 Active/Reverse/Inactive cursor owners."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:  # The target-independent CI runs under python -S.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'cursor_gates', ROOT / 'scripts/inspect-title-character-cursors.py')
checker = importlib.util.module_from_spec(spec)
spec.loader.exec_module(checker)


class CursorOriginalFieldGates(unittest.TestCase):
    @staticmethod
    def synthetic(variant):
        model = dict(checker.OWNERS[variant])
        target = bytearray(model['size'])
        for offset, kind, _, dest in model['fields']:
            struct.pack_into('<I', target, offset,
                             checker.encoded(kind, dest, model['address'], offset))
        model['sha256'] = hashlib.sha256(target).hexdigest()
        return model, bytes(target)

    @staticmethod
    def rows(model, shifted=0):
        return [dict(offset=off + (shifted if i else 0), type=kind,
                     symbol=name, addend=0)
                for i, (off, kind, name, _) in enumerate(model['fields'])]

    def test_three_independent_original_canonical_field_inventories(self):
        for variant, original in checker.OWNERS.items():
            with self.subTest(variant=variant):
                model, data = self.synthetic(variant)
                checker.target_manifest(data, model)
                self.assertIn(original['size'], (175, 211))
                self.assertIn(len(model['fields']), (2, 6))

    def test_candidate_binding_uses_actual_shifted_fields_not_target_offsets(self):
        for variant in checker.OWNERS:
            with self.subTest(variant=variant):
                model, _ = self.synthetic(variant)
                rows = self.rows(model, shifted=1)
                linked = checker.bind_candidate(bytes(model['size'] + 6), rows, model)
                for row, (_, kind, _, dest) in zip(rows, model['fields']):
                    value = struct.unpack_from('<I', linked, row['offset'])[0]
                    self.assertEqual(value, checker.encoded(kind, dest,
                                                              model['address'], row['offset']))

    def test_original_target_bytes_and_operand_corruption_rejected(self):
        for variant in checker.OWNERS:
            model, target = self.synthetic(variant)
            for offset, _, _, _ in model['fields']:
                bad = bytearray(target)
                bad[offset] ^= 1
                with self.subTest(variant=variant, offset=offset), self.assertRaises(ValueError):
                    checker.target_manifest(bytes(bad), model)
            with self.assertRaises(ValueError):
                checker.target_manifest(target[:-1], model)

    def test_reject_missing_overlapping_reordered_and_fake_fields(self):
        for variant in checker.OWNERS:
            model, _ = self.synthetic(variant)
            full = self.rows(model, shifted=1)
            bad = [full[:-1], list(reversed(full)),
                   [dict(full[0], type='DIR32' if full[0]['type'] == 'REL32' else 'REL32')] + full[1:],
                   [dict(full[0], symbol='?invented@@3HA')] + full[1:],
                   [dict(full[0], addend=4)] + full[1:],
                   full[:1] + [dict(full[1], offset=full[0]['offset'] + 1)] + full[2:]]
            for fields in bad:
                with self.subTest(variant=variant, fields=fields[:2]), self.assertRaises(ValueError):
                    checker.bind_candidate(bytes(model['size'] + 6), fields, model)
            with self.assertRaises(ValueError):
                checker.bind_candidate(bytes(3), full, model)

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_actual_decoding_distinguishes_branch_from_absolute_data(self):
        base = 0x00424FB0
        # MOV ECX,[actual global]; CALL destination; JMP forward; RET 0x10
        data = bytes.fromhex('8b0d50c54d00e800000000eb00c21000')
        instructions, fields, calls = checker.actual_fields(data, base)
        self.assertEqual(sum(i.size for i in instructions), len(data))
        self.assertEqual(fields, [(2, 'DIR32', 0x4DC550),
                                  (7, 'REL32', base + 11)])
        self.assertEqual(calls, [base + 11])

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_wrong_return_abi_or_indirect_call_does_not_pass(self):
        for data in (bytes.fromhex('ff d0 c2 10 00'), bytes.fromhex('90 c3')):
            with self.subTest(code=data.hex()), self.assertRaises(ValueError):
                checker.actual_fields(data, 0x00424FB0)


if __name__ == '__main__':
    unittest.main()
