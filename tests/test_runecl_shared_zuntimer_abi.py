"""RunEcl uses the exact real ZunTimer class, not an independently mangled timer.

All complete private-target owners are verified by the cold Factory oracle;
these pure checks prevent reintroducing split C++ ABI names or mismatched
compiler profiles. No newly exact function is credited here.
"""

import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location('runecl_timer_guard', ROOT/'scripts/check-boss-frontiers.py')
BOSS = importlib.util.module_from_spec(sp)
sp.loader.exec_module(BOSS)


class GenuineEclTimerABI(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT/'src/EclOpcodes.hpp').read_text()
        cls.manager = (ROOT/'src/EclManager.cpp').read_text()
        cls.timer = (ROOT/'src/ZunTimer.hpp').read_text()
        cls.binder = (ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
        cls.biased = (ROOT/'scripts/inspect-ecl-biased-movement.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as src:
            cls.units = tomllib.load(src)['units']

    def test_one_real_12_byte_timer_layout_and_scalar_member(self):
        self.assertIn('#include "ZunTimer.hpp"',self.header)
        self.assertIn('typedef ZunTimer Th09EclTimerStorageView;',self.header)
        self.assertNotIn('struct Th09EclTimerStorageView',self.header)
        self.assertIn('Th09EclContextSizeIs22C',self.header)
        self.assertIn('Th09EclTimerStorageSizeIs0C',self.header)
        self.assertIn('ZunTimerSizeIs0C',self.timer)
        self.assertIn('int current;',self.timer)
        self.assertIn('time008.current =',self.manager)
        self.assertNotIn('time008.current08',self.manager)
        self.assertIn('Th09EclTimerStorageView timer04;',self.header)
        self.assertIn('Th09EclTimerStorageView time008;',self.header)
        self.assertIn('Th09EclTimerStorageView secondaryTime094;',self.header)

    def test_no_source_mangled_fake_timer_remains_in_callsite_audits(self):
        self.assertNotIn('Th09EclTimerStorageView@@',self.binder)
        self.assertNotIn('Th09EclTimerStorageView@@',self.biased)
        self.assertIn("'??4ZunTimer@@QAEXH@Z': 0x00401500",self.biased)
        self.assertIn("(0x1EE, 'REL32', '??4ZunTimer@@QAEXH@Z')",self.biased)
        self.assertIn("'??BZunTimer@@QAEHXZ': 0x435f00",self.binder)

    def test_real_authentic_three_compiler_carriers_cover_eight_timer_leaves(self):
        table = BOSS.RUNECL_REAL_TIMER_EXACT_CARRIERS
        self.assertEqual(len(table),3)
        self.assertEqual([len(owners) for _,owners in table],[6,1,1])
        self.assertEqual(len(set(name for _,owners in table for name,_,_ in owners)),8)
        self.assertEqual(table[0][0],'src/ZunTimer.cpp')
        self.assertEqual(table[1][0],'src/BackgroundStageScript.cpp')
        self.assertEqual(table[2][0],'src/EnemyScheduleRuntimeAdvance.cpp')
        for source,group in table:
            actual = [self.units[name] for name,_,_ in group]
            obj,flags = actual[0]['object'],actual[0]['profile']
            for (name,va,symbol),spec in zip(group,actual):
                with self.subTest(name=name):
                    self.assertEqual((spec['source'],spec['object'],spec['profile'],
                                      spec['target_address'],spec['symbol']),
                                     (source,obj,flags,va,symbol))
        self.assertIn('/Ob0',self.units['zun-timer-greater-equal']['profile'])
        self.assertNotIn('/Ob0',self.units['zun-timer-assign-int']['profile'])

    def test_full_timer_carrier_replays_each_exact_body_once(self):
        with patch.object(BOSS,'build') as build,patch.object(
                BOSS,'canonical',side_effect=lambda name:{'unit':name}) as exact:
            results = BOSS.replay_runecl_real_timer_exact()
        self.assertEqual(len(results),8)
        self.assertEqual(len({r['unit'] for r in results}),8)
        self.assertEqual([c.args[0] for c in build.call_args_list],
                         ['zun-timer-assign-int','zun-timer-greater-equal',
                          'zun-timer-equal-int'])
        self.assertEqual(exact.call_count,8)

    def test_renamed_exact_source_callees_retained_original_targets(self):
        expected={
            'ecl-start-timed-polar':[0xE2],
            'ecl-begin-boundary-aware-move':[0x1E6],
            'ecl-configure-polar-motion':[0x10C],
            'ecl-configure-relative-motion':[0xB4],
            'ecl-install-interpolation-slot':[0x35],
        }
        for name,offsets in expected.items():
            mapped={r['offset']:r for r in self.units[name]['relocations']}
            for offset in offsets:
                with self.subTest(name=name,offset=hex(offset)):
                    self.assertEqual((mapped[offset]['symbol'],mapped[offset]['target'],
                                      mapped[offset]['type']),
                                     ('??4ZunTimer@@QAEXH@Z',0x401500,'REL32'))
        self.assertEqual((self.units['ecl-compare-operands']['size'],
                          self.units['ecl-compare-operands']['compare_size']), (842,892))
        self.assertEqual((self.units['ecl-post-update-movement']['size'],
                          self.units['ecl-post-update-movement']['compare_size']), (967,992))

    def test_fake_source_target_and_profile_never_count_as_evidence(self):
        baseline = self.units
        for name,key,bad in (
            ('zun-timer-assign-int','symbol','??4Th09EclTimerStorageView@@QAEXH@Z'),
            ('zun-timer-post-decrement','target_address',0x401510),
            ('zun-timer-add-assign-float','object','build/matching/Wrong.obj'),
            ('zun-timer-greater-equal','profile',['/Ob1']),
            ('zun-timer-equal-int','source','src/Wrong.cpp'),
        ):
            candidate=copy.deepcopy(baseline)
            candidate[name][key]=bad
            with self.subTest(name=name,key=key),patch.object(
                    BOSS.tomllib,'load',return_value={'units':candidate}),patch.object(
                    BOSS,'build'),patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
                BOSS.replay_runecl_real_timer_exact()


if __name__=='__main__':
    unittest.main()
