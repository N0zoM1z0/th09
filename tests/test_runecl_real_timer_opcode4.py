"""TH09 RunEcl opcode-4 timer binding: genuine original-leaf ABI, no credit.

These checks are independent of the proprietary target. Complete code+table
bytes and every relocation are verified by source-cold RunEcl/24-unit oracles.
"""

import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]


class RealTimerOpcode4Binding(TestCase):
    def test_actual_zuntimer_assignment_not_fictitious_second_callee(self):
        text = (ROOT / 'src/EclRunControl.inl').read_text()
        self.assertIn('#include "ZunTimer.hpp"', text)
        start = text.index('case TH09_ECL_OPCODE_JUMP:')
        end = text.index('case TH09_ECL_OPCODE_SET_INT:', start)
        opcode = text[start:end]
        self.assertIn('const volatile int &rawTime =', opcode)
        self.assertIn('int value = rawTime;', opcode)
        self.assertIn('ZunTimer &timer = *reinterpret_cast<ZunTimer *>(&context->time008);', opcode)
        self.assertIn('timer = value;', opcode)
        self.assertNotIn('context->time008 = value;', opcode)
        self.assertIn('th09_ecl_redispatch_instruction', opcode)
        self.assertIn('TH09_ECL_OPCODE_SET_TIMEOUT_SPELL', (ROOT/'src/EclRunState.inl').read_text())

    def test_independent_exact_leaf_matches_original_address_and_method(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        entry = units['zun-timer-assign-int']
        self.assertEqual(entry['source'], 'src/ZunTimer.cpp')
        self.assertEqual(entry['target_address'], 0x00401500)
        self.assertEqual(entry['size'], 9)
        self.assertEqual(entry['symbol'], '??4ZunTimer@@QAEXH@Z')
        with (ROOT / 'config/functions.csv').open(newline='') as f:
            rows = [x for x in csv.DictReader(f) if x['address']=='0x00401500']
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['status'], 'matching')
        self.assertEqual(rows[0]['proposed_name'], 'ZunTimer::operator=')

    def test_real_time_storage_has_same_12_byte_current_at_eight(self):
        cpp = (ROOT/'src/EclOpcodes.hpp').read_text()
        timer = (ROOT/'src/ZunTimer.hpp').read_text()
        self.assertIn('Th09EclTimerStorageSizeIs0C', cpp)
        self.assertIn('Th09EclTimerStorageView time008;', cpp)
        self.assertIn('typedef ZunTimer Th09EclTimerStorageView;', cpp)
        self.assertNotIn('struct Th09EclTimerStorageView', cpp)
        self.assertIn('time008.current =', (ROOT/'src/EclManager.cpp').read_text())
        self.assertIn('ZunTimerSizeIs0C', timer)
        self.assertIn('int previous;', timer)
        self.assertIn('float subFrame;', timer)
        self.assertIn('int current;', timer)
        self.assertIn('Th09EclContextSizeIs22C', cpp)

    def test_exact_leaf_is_independently_canonical_not_assumed_from_alias(self):
        spec = importlib.util.spec_from_file_location(
            'ecl_timer_identity_guard', ROOT/'scripts/audit-ecl-callsite-identities.py')
        audit = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(audit)
        target = 0x00401500
        canonical = {'??4ZunTimer@@QAEXH@Z': [(target, 'zun-timer-assign-int')]}
        expected, provenance, agrees = audit.direct_identity(
            '??4ZunTimer@@QAEXH@Z', canonical, audit.ALIASES, target)
        self.assertEqual(expected, target)
        self.assertTrue(agrees)
        self.assertEqual(provenance['canonical_units'], ['zun-timer-assign-int'])
        _, _, not_matching = audit.direct_identity(
            '??4ZunTimer@@QAEXH@Z', canonical, audit.ALIASES, target+4)
        self.assertFalse(not_matching)

    def test_interpreter_remains_nonexact_original_bound_complete_owner(self):
        source = (ROOT/'scripts/check-boss-frontiers.py').read_text()
        self.assertIn('RUNECL_TARGET_OWNER_SHA',source)
        self.assertIn('RUNECL_WINDOWS = {4: (0x277, 29, 7, 8)', source)
        self.assertIn('155: (0x30B0, 45, 23, 12)',source)
        self.assertIn('original identity/code alignment/fields/table regressed', source)
