"""Pure-input checks for TH09 RunEcl's real shared RNG source ownership.

Exact physical PE/COFF validation, handler tables and private label target VAs
are checked separately in the cold full-owner ECL audits and canonical units.
"""

import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]


def units():
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        return tomllib.load(stream)['units']


class RunEclNativeRngSource(TestCase):
    def test_shared_real_rng_declaration_and_source_are_consistent(self):
        control = (ROOT / 'src/EclRunControl.inl').read_text()
        runtime = (ROOT / 'src/RngRuntimeLeaves.hpp').read_text()
        self.assertIn('#include "RngRuntimeLeaves.hpp"', control)
        self.assertIn('extern RngRuntimeView g_Rng;', control)
        self.assertIn('using ::g_Rng;', control)
        self.assertNotIn('struct RngView', control)
        self.assertNotIn('extern RngView g_Rng;', control)
        for role in ('NextU16()', 'GetRandomU32InRange(',
                     'GetRandomF32InRange(', 'GetRandomF32SignedInRange('):
            self.assertIn(role, runtime)
        self.assertIn('Th09EclRunControl::g_Rng.GetRandomF32InRange(',
                      (ROOT / 'src/EclManager.cpp').read_text())

    def test_renamed_call_symbol_addresses_are_independent_existing_exact_leaves(self):
        spec = importlib.util.spec_from_file_location(
            'ecl_shared_rng_xrefs', ROOT/'scripts/audit-ecl-callsite-identities.py')
        audit = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(audit)
        bound = {
            '?NextU16@RngRuntimeView@@QAEGXZ': 0x0042AE20,
            '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z': 0x004048E0,
            '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x00404910,
        }
        for symbol, address in bound.items():
            with self.subTest(symbol=symbol):
                self.assertEqual(audit.ALIASES[symbol], address)
                expected, provenance, agrees = audit.direct_identity(
                    symbol, {}, audit.ALIASES, address)
                self.assertEqual(expected, address)
                self.assertTrue(agrees)
                self.assertIn('independent_alias_review', provenance)
        for legacy in ('?NextU16@RngView@Th09EclRunControl@@QAEGXZ',
                       '?GetRandomF32InRange@RngView@Th09EclRunControl@@QAEMM@Z'):
            self.assertNotIn(legacy, audit.ALIASES)
            expected, _, agrees = audit.direct_identity(legacy, {}, audit.ALIASES, 0)
            self.assertIsNone(expected)
            self.assertIsNone(agrees)

    def test_original_code_addresses_are_anchored_to_authored_exact_rng(self):
        with (ROOT / 'config/functions.csv').open(newline='') as source:
            rows = {row['address']: row for row in csv.DictReader(source)}
        for address, role in ((0x0042AE20, 'NextU16'),
                              (0x004048E0, 'GetRandomU32InRange'),
                              (0x00404910, 'GetRandomF32InRange')):
            key=f'0x{address:08X}'
            self.assertIn(key, rows)
            self.assertEqual(rows[key]['status'], 'matching')
            self.assertIn('RngRuntimeView::'+role, rows[key]['proposed_name'])

    def test_exact_ecl_movement_unit_has_same_three_original_fields(self):
        entry=units()['ecl-begin-boundary-aware-move']
        self.assertEqual(entry['target_address'], 0x00407C30)
        self.assertEqual(entry['size'], 510)
        actual={r['offset']:(r['type'],r['symbol'],r['target'])
                for r in entry['relocations']}
        for offset,kind,symbol,address in (
                (0x25, 'DIR32', '?g_Rng@@3URngRuntimeView@@A', 0x004ACE0C),
                (0x38, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z', 0x00404910),
                (0x53, 'REL32', '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z', 0x00404910)):
            with self.subTest(offset=hex(offset)):
                self.assertEqual(actual[offset],(kind,symbol,address))
        self.assertEqual(len(entry['relocations']),27)

    def test_biased_movement_inspector_rejects_old_private_rng_names(self):
        inspector=(ROOT/'scripts/inspect-ecl-biased-movement.py').read_text()
        names={
            '?g_Rng@@3URngRuntimeView@@A':0x004ACE0C,
            '?GetRandomU32InRange@RngRuntimeView@@QAEII@Z':0x004048E0,
            '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z':0x00404910,
            '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z':0x00406200,
        }
        for name,address in names.items():
            with self.subTest(name=name):
                self.assertIn("'"+name+"': 0x%08X"%address,inspector)
        self.assertNotIn('RngView@Th09EclRunControl',inspector)
