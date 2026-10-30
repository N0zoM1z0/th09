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
    def test_assignment_calls_the_independently_exact_native_setter(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        assignment = units['zun-timer-assign-int']
        setter = units['zun-timer-set-current']
        self.assertEqual(setter['target_address'], 0x004014D0)
        self.assertEqual(setter['size'], 25)
        self.assertEqual(setter['source'], assignment['source'])
        self.assertEqual(setter['profile'], assignment['profile'])
        self.assertEqual(assignment['relocations'], [dict(
            offset=5, type='REL32', symbol=setter['symbol'],
            target=setter['target_address'])])

    def test_timeout_reads_one_wire_byte_without_qualified_access(self):
        text = (ROOT/'src/EclRunState.inl').read_text()
        start = text.index('case TH09_ECL_OPCODE_SET_TIMEOUT_SPELL:')
        end = text.index('case TH09_ECL_OPCODE_SET_SPECIAL_INTERACTION:', start)
        opcode = text[start:end]
        self.assertNotIn('volatile', opcode)
        self.assertIn('unsigned char value;', opcode)
        self.assertIn('memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));', opcode)
        self.assertIn('AssignTimeoutSpellBit(state.primaryFlags337C, value)', opcode)

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
