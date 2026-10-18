"""Genuine ZunTimer 0x0C class unifies the TH09 RunEcl source timer ABI.

Pure-input test: only the pinned full-source cold VC7.1 original PE/COFF
comparison can prove codegen exactness. Source type replacement adds no credit.
"""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[1]
loader=importlib.util.spec_from_file_location('ecl_zun_real',ROOT/'scripts/check-boss-frontiers.py')
BOSS=importlib.util.module_from_spec(loader)
loader.loader.exec_module(BOSS)


class SharedEclZunTimer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header=(ROOT/'src/EclOpcodes.hpp').read_text()
        cls.timer=(ROOT/'src/ZunTimer.hpp').read_text()
        cls.interpreter=(ROOT/'src/EclManager.cpp').read_text()
        cls.audit=(ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
        cls.biased=(ROOT/'scripts/inspect-ecl-biased-movement.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_single_canonical_0x0c_timer_in_ecl_context(self):
        self.assertIn('#include "ZunTimer.hpp"',self.header)
        self.assertIn('typedef ZunTimer Th09EclTimerStorageView;',self.header)
        self.assertNotIn('struct Th09EclTimerStorageView',self.header)
        for signature in ('Th09EclTimerStorageSizeIs0C',
                          'offsetof(Th09EclTimerStorageView, previous) == 0x00',
                          'offsetof(Th09EclTimerStorageView, subFrame) == 0x04',
                          'offsetof(Th09EclTimerStorageView, current) == 0x08',
                          'Th09EclTimerStorageView time008;',
                          'Th09EclTimerStorageView secondaryTime094;',
                          'Th09EclTimerStorageView timer04;',
                          'Th09EclContextSizeIs22C'):
            with self.subTest(signature=signature):self.assertIn(signature,self.header)
        for member in ('int previous;', 'float subFrame;', 'int current;',
                       'ZunTimerSizeIs0C', 'void operator=(int value);'):
            self.assertIn(member,self.timer)
        self.assertIn('time008.current =',self.interpreter)
        self.assertNotIn('time008.current08',self.interpreter)

    def test_real_source_symbols_and_no_old_fabricated_timer_callee(self):
        self.assertNotIn('Th09EclTimerStorageView@@',self.audit)
        self.assertNotIn('Th09EclTimerStorageView@@',self.biased)
        self.assertIn('??4ZunTimer@@QAEXH@Z',self.biased)
        self.assertIn('??BZunTimer@@QAEHXZ',self.audit) # source-only folded getter
        self.assertIn('0x435f00',self.audit) # original folded four-byte getter VA
        self.assertIn("(0x1EE, 'REL32', '??4ZunTimer@@QAEXH@Z')",self.biased)
        for spec in self.units.values():
            if spec.get('source')=='src/EclManager.cpp':
                for row in spec['relocations']:
                    self.assertNotIn('Th09EclTimerStorageView@@',row['symbol'])

    def test_cold_run_ecl_now_guards_eight_genuine_full_timer_owners(self):
        groups=BOSS.RUNECL_REAL_TIMER_EXACT_CARRIERS
        self.assertEqual([len(a) for _,a in groups],[6,1,1])
        self.assertEqual(sum(len(a) for _,a in groups),8)
        self.assertEqual(len({self.units[items[0][0]]['object'] for _,items in groups}),3)
        self.assertIn('/Ob0',self.units['zun-timer-greater-equal']['profile'])
        self.assertNotIn('/Ob0',self.units['zun-timer-assign-int']['profile'])
        with patch.object(BOSS,'build') as build,patch.object(
            BOSS,'canonical',side_effect=lambda name:{'unit':name}):
            result=BOSS.replay_runecl_real_timer_exact()
        self.assertEqual(len(result),8)
        self.assertEqual(len({z['unit'] for z in result}),8)
        self.assertEqual([call.args[0] for call in build.call_args_list],
                         ['zun-timer-assign-int','zun-timer-greater-equal','zun-timer-equal-int'])

    def test_mismatched_type_profile_compiler_object_or_address_fails_closed(self):
        bad_cases=(
            ('zun-timer-assign-int','symbol','??4Th09EclTimerStorageView@@QAEXH@Z'),
            ('zun-timer-add-assign-float','target_address',0x406651),
            ('zun-timer-post-decrement','object','build/matching/Other.obj'),
            ('zun-timer-equal-int','source','src/ZunTimer.cpp'),
            ('zun-timer-greater-equal','profile',['/Ob1']),
            ('zun-timer-float-conversion','target_address',0x401500),
        )
        for name,key,bad in bad_cases:
            units=copy.deepcopy(self.units)
            units[name][key]=bad
            with self.subTest(name=name,key=key),patch.object(
                BOSS.tomllib,'load',return_value={'units':units}),patch.object(
                BOSS,'build'),patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
                BOSS.replay_runecl_real_timer_exact()
        bad=copy.deepcopy(self.units)
        del bad['zun-timer-equal-int']
        with patch.object(BOSS.tomllib,'load',return_value={'units':bad}),patch.object(
            BOSS,'build'),patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
            BOSS.replay_runecl_real_timer_exact()


if __name__=='__main__':
    unittest.main()
