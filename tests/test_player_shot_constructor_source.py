"""Source-reconstructed 44-byte shot special member remains origin-unclassified."""
from pathlib import Path
import csv
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class PlayerShotConstructorSource(unittest.TestCase):
    def test_parent_carrier_contains_implicit_member_construction_body(self):
        carrier = (ROOT/'src/PlayerRuntime.cpp').read_text()
        header = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerShotCtorView::PlayerShotCtorView()\n{\n}',carrier)
        self.assertIn('PlayerLifecycleView::PlayerLifecycleView()\n{\n}',carrier)
        self.assertIn('PlayerConstructedVector3View trailVectors2B0[32];',header)
        self.assertIn('PlayerPositionView hitbox430;',header)
        self.assertIn('PlayerTimerCtorView timer454;',header)
        self.assertIn('PlayerShotNativeConstructedVectors',header)
        self.assertNotIn('extern void __ehvec_ctor(',carrier)
        self.assertNotIn('PlayerShotCtorView::PlayerShotCtorView()',
                         (ROOT/'src/PlayerOptionConstructor.cpp').read_text())

    def test_unclassified_44_byte_owner_not_promoted_to_authored_exact(self):
        with (ROOT/'config/functions.csv').open(newline='') as f:
            row=next(x for x in csv.DictReader(f) if x['address']=='0x0041F030')
        self.assertEqual((row['size'],row['status'],row['source_file'],row['owner']),
                         ('44','unclassified','src/PlayerRuntime.cpp',''))
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertFalse(any(v['target_address']==0x0041F030 for v in units.values()))
        self.assertEqual((units['anm-vm-constructor']['target_address'],
                          units['zun-timer-constructor']['target_address']),
                         (0x00403A10,0x00403390))
        reader=(ROOT/'scripts/inspect-player-shot-constructor.py').read_text()
        self.assertIn("ORIGINAL_SHA256 = 'df4c963a",reader)
        self.assertIn('SIZE = 44',reader)
        self.assertIn('??_H@YGXPAXIHP6EPAX0@Z@Z',reader)
        self.assertIn('0x004343D0',reader)


if __name__=='__main__':unittest.main()
