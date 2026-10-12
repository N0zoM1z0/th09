"""Pure-input guards for the source-present Type14/22 full-owner diagnostic.

This suite intentionally runs under the project's python -S CI environment:
Capstone and the private original executable are needed only by the optional
actual PE/COFF comparison, never by these deterministic binder tests.
"""

import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_1422_draw_inspector',
    ROOT / 'scripts' / 'inspect-exattack-type14-22-draw.py',
)
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class Type1422DrawBindingTests(unittest.TestCase):
    def rows(self):
        return [
            dict(offset=4 * i, type='DIR32',
                 symbol='?g_AnmManager@@3PAVAnmManager@@A', addend=0)
            for i in range(53)
        ]

    def test_binds_only_known_fields_without_changing_raw_input(self):
        raw = bytes(974)
        linked, fields = INSPECT.bind_candidate(raw, self.rows())
        self.assertEqual(len(fields), 53)
        self.assertEqual(len(linked), 974)
        self.assertEqual(raw, bytes(974))
        self.assertEqual(struct.unpack_from('<I', linked, 0)[0], 0x004DC550)
        self.assertEqual(struct.unpack_from('<I', linked, 4 * 52)[0], 0x004DC550)

    def test_unknown_symbol_type_and_unreviewed_addend_are_rejected(self):
        changes = (
            dict(symbol='?NotATargetOwnedSymbol@@3H'),
            dict(type='REL32'),
            dict(type='NOT_A_RELOCATION'),
            dict(addend=8),
        )
        for change in changes:
            rows = self.rows()
            rows[0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                INSPECT.bind_candidate(bytes(974), rows)

    def test_wrong_field_count_overlap_and_extent_are_rejected(self):
        rows = self.rows()
        with self.assertRaisesRegex(ValueError, 'all 53'):
            INSPECT.bind_candidate(bytes(974), rows[:-1])
        for bad in (0, -1, 972):
            rows = self.rows()
            rows[1]['offset'] = bad
            with self.subTest(offset=bad), self.assertRaises(ValueError):
                INSPECT.bind_candidate(bytes(974), rows)

    def test_supervisor_field_addend_is_precise(self):
        rows = self.rows()
        rows[1].update(
            symbol='?g_Supervisor@@3UExAttackType1422DrawSupervisorView@@A',
            addend=8,
        )
        linked, fields = INSPECT.bind_candidate(bytes(974), rows)
        self.assertEqual(fields[1][2], 0x004B3108)
        self.assertEqual(struct.unpack_from('<I', linked, 4)[0], 0x004B3108)

    @unittest.skipUnless(importlib.util.find_spec('capstone') is not None,
                         'CI python -S intentionally has no Capstone decoder')
    def test_truncated_or_invalid_return_fails_decoding(self):
        with self.assertRaises(ValueError):
            INSPECT.decode_owner(b'\x90')
        with self.assertRaises(ValueError):
            INSPECT.decode_owner(b'\xC2\x08\x00')


if __name__ == '__main__':
    unittest.main()
