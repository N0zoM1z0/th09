"""Source ownership and status of the original 23-byte Player option ctor."""
from pathlib import Path
import csv
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerOptionConstructorSource(unittest.TestCase):
    def test_real_native_member_constructs_without_manual_offsets(self):
        source = (ROOT/'src/PlayerOptionConstructor.cpp').read_text()
        header = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerOptionStateCtorView::PlayerOptionStateCtorView()', source)
        self.assertNotIn('PlayerShotCtorView::PlayerShotCtorView()', source)
        self.assertIn('AnmVm vm00;', header)
        self.assertIn('ZunTimer timer2E0;', header)
        self.assertIn('PlayerOptionNativeAnmVmAt00', header)
        self.assertIn('PlayerOptionStateCtorSizeIs2F4', header)

    def test_source_does_not_change_unresolved_authored_origin(self):
        with (ROOT/'config/functions.csv').open(newline='') as f:
            record = next(row for row in csv.DictReader(f)
                          if row['address'].lower() == '0x0041f010')
        self.assertEqual((record['size'], record['status']), ('23', 'unclassified'))
        self.assertEqual(record['source_file'],'src/PlayerOptionConstructor.cpp')
        self.assertEqual(record['owner'],'')
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        self.assertFalse(any(u['target_address'] == 0x0041F010 for u in units.values()))
        self.assertEqual((units['anm-vm-constructor']['target_address'],
                          units['zun-timer-constructor']['target_address']),
                         (0x00403A10, 0x00403390))
        oracle = (ROOT/'scripts/inspect-player-option-constructor.py').read_text()
        self.assertIn("ORIGINAL_SHA256 = 'eb751682", oracle)
        self.assertIn("'unclassified special member; non-crediting source equivalence'", oracle)


if __name__ == '__main__':
    unittest.main()
