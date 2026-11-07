"""Original-backed Etama record array counts, sentinels, and linear scan.

The real pinned VC7.1 machine-code comparison is run separately for all
affected complete owners. This test protects the recovered typed C++ source
and rejects conflating allocation capacity with active record capacity.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EtamaNativePoolLimits(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/EtamaNativePoolLimits.hpp').read_text()
        cls.native = (ROOT/'src/BulletManager.hpp').read_text()
        cls.boss = (ROOT/'src/BulletManager.cpp').read_text()
        cls.lifecycle = (ROOT/'src/EtamaControllerInit.cpp').read_text()

    def test_real_arrays_and_contiguous_sentinel_invariant(self):
        for name,count in (('primaryBullets',176),('secondaryBullets',361),
                           ('lasers',48)):
            with self.subTest(name=name):
                self.assertIn(f'{name}[{count}]',self.native)
        for key,role in (
                ('PrimaryRecordCount','primaryBullets'),
                ('SecondaryRecordCount','secondaryBullets'),
                ('LaserRecordCount','lasers')):
            self.assertIn(key+' = sizeof(((EtamaController *)0)->'+role+')',
                          self.header)
        for phrase in ('PrimaryUsableCount = PrimaryRecordCount - 1',
                       'SecondaryUsableCount = SecondaryRecordCount - 1',
                       'LinearScanCount = PrimaryRecordCount + SecondaryUsableCount',
                       'EtamaNativePoolContiguousSentinelLayout',
                       'offsetof(EtamaController, secondaryBullets) ==',
                       'offsetof(EtamaController, lasers) =='):
            self.assertIn(phrase,self.header)

    def test_native_init_and_big_boss_use_one_source_of_capacity(self):
        self.assertIn('#include "EtamaNativePoolLimits.hpp"',self.boss)
        self.assertIn('#include "EtamaNativePoolLimits.hpp"',self.lifecycle)
        for source in (self.lifecycle,self.boss):
            self.assertIn('EtamaNativePoolLimits::PrimaryUsableCount',source)
            self.assertIn('EtamaNativePoolLimits::SecondaryUsableCount',source)
        for value in ('LinearScanCount','LaserRecordCount'):
            self.assertIn('EtamaNativePoolLimits::'+value,self.boss)
        for index in (175,360):
            self.assertNotIn(f'Bullets[{index}]',self.lifecycle)
            self.assertNotIn(f'Bullets[{index}]',self.boss)
        self.assertNotIn('for (int i = 0; i < 536;',self.boss)
        self.assertNotIn('for (int i = 0; i < 48;',self.boss)

    def test_original_scan_includes_primary_sentinel_not_secondary_sentinel(self):
        primary_total,secondary_total=176,361
        first_secondary=primary_total
        usable_secondary=secondary_total-1
        positions=tuple(range(primary_total+usable_secondary))
        self.assertEqual(len(positions),536)
        self.assertIn(primary_total-1,positions)
        self.assertIn(first_secondary+usable_secondary-1,positions)
        self.assertNotIn(first_secondary+secondary_total-1,positions)
        self.assertEqual(sum(p<primary_total-1 for p in positions),175)
        self.assertEqual(sum(p>=first_secondary for p in positions),360)

    def test_all_affected_canonical_source_owners_stay_distinct(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        for name,size in (('etama-controller-initialize',173),
                          ('etama-controller-reset-projectiles',95),
                          ('etama-controller-update',2216),
                          ('etama-spawn-single-bullet',1932),
                          ('bullet-advance-transform-program',1320)):
            with self.subTest(unit=name):
                self.assertEqual(units[name].get('compare_size',units[name]['size']),size)
        self.assertEqual(len([n for n,u in units.items()
                              if u['source']=='src/BulletManager.cpp']),18)
        self.assertEqual(len([n for n,u in units.items()
                              if u['source']=='src/EtamaControllerInit.cpp']),2)


if __name__=='__main__':
    unittest.main()
