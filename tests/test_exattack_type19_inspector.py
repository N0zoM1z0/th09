"""Target-independent safeguards for the ExAttack Type19 complete audit."""

import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'exattack_type19_oracle', ROOT / 'scripts/inspect-exattack-type19.py')
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class Type19BindingTests(unittest.TestCase):
    def row(self, **changes):
        row = dict(offset=0, type='DIR32',
                   symbol='?g_GameManager@@3UExAttackType19UpdateGameManagerView@@A',
                   addend=4)
        row.update(changes)
        return row

    def test_incomplete_field_inventory_rejected(self):
        with self.assertRaisesRegex(ValueError, 'missing or adding'):
            INSPECT.bind_candidate(bytes(INSPECT.SIZE), [self.row()])

    def test_unknown_symbol_rejected_before_binding(self):
        rows = [self.row(offset=i * 4) for i in range(INSPECT.EXPECTED_COUNT)]
        rows[0]['symbol'] = '?InventedGlobal@@3IA'
        with self.assertRaisesRegex(ValueError, 'unreviewed Type19 COFF symbol'):
            INSPECT.bind_candidate(bytes(INSPECT.SIZE), rows)

    def test_spurious_member_offset_rejected(self):
        rows = [self.row(offset=i * 4) for i in range(INSPECT.EXPECTED_COUNT)]
        rows[4]['addend'] = 8
        with self.assertRaisesRegex(ValueError, 'GameManager field offset'):
            INSPECT.bind_candidate(bytes(INSPECT.SIZE), rows)

    def test_overlapping_field_rejected(self):
        rows = [self.row(offset=i * 4) for i in range(INSPECT.EXPECTED_COUNT)]
        rows[4]['offset'] = rows[3]['offset']
        with self.assertRaisesRegex(ValueError, 'outside physical extent or overlapping'):
            INSPECT.bind_candidate(bytes(INSPECT.SIZE), rows)

    def test_literal_addend_rejected(self):
        rows = [self.row(offset=i * 4) for i in range(INSPECT.EXPECTED_COUNT)]
        rows[5] = self.row(offset=20, symbol='__real@3f800000', addend=4)
        with self.assertRaisesRegex(ValueError, 'float literal addend'):
            INSPECT.bind_candidate(bytes(INSPECT.SIZE), rows)


if __name__ == '__main__':
    unittest.main()
