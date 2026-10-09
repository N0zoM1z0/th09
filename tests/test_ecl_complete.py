import importlib.util
from pathlib import Path
import struct
import unittest

PATH = Path(__file__).resolve().parents[1] / 'scripts' / 'inspect-ecl-complete.py'
SPEC = importlib.util.spec_from_file_location('ecl_complete', PATH)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


def field(offset=0, kind='DIR32', local=False):
    return dict(offset=offset, type=kind, symbol_section=1 if local else 0,
                owner_section=1, symbol_value=8, owner_value=0, addend=0)


class CompleteEclLinkTests(unittest.TestCase):
    def test_local_label_keeps_actual_candidate_position(self):
        # A paired target label at +9 must not hide the candidate label at +8.
        linked, occupied = MODULE.link_owner(bytes(16), [field(local=True)], {0: 0x400009}, base=0x400000)
        self.assertEqual(struct.unpack_from('<I', linked)[0], 0x400008)
        self.assertEqual(occupied, {0, 1, 2, 3})

    def test_relative_call_uses_independent_destination(self):
        linked, _ = MODULE.link_owner(bytes(16), [field(kind='REL32')], {0: 0x400020}, base=0x400000)
        self.assertEqual(struct.unpack_from('<I', linked)[0], 0x1C)

    def test_missing_external_binding_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'unbound external'):
            MODULE.link_owner(bytes(16), [field()], {})

    def test_overlapping_and_truncated_fields_fail_closed(self):
        for rows in ([field(), field(offset=2)], [field(offset=14)]):
            with self.subTest(rows=rows), self.assertRaisesRegex(ValueError, 'out-of-range or overlapping'):
                MODULE.link_owner(bytes(16), rows, {0: 123})

    def test_outside_owner_label_cannot_use_external_override(self):
        row = field(local=True)
        row['symbol_value'] = 16
        with self.assertRaisesRegex(ValueError, 'outside owner'):
            MODULE.link_owner(bytes(16), [row], {0: 123})


if __name__ == '__main__':
    unittest.main()
