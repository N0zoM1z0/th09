"""Pure-input checks for TH09 RunEcl's shared RNG call identities.

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
