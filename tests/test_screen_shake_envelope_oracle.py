"""Target-independent fail-closed checks for the ScreenEffect large owner."""
import importlib.util
import tomllib
from pathlib import Path
from types import SimpleNamespace
from unittest import TestCase, main

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/inspect-screen-shake-envelope.py'
spec = importlib.util.spec_from_file_location('shake_envelope_inspector', SOURCE)
inspector = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inspector)


class ShakeEnvelopeOracleTests(TestCase):
    def setUp(self):
        self.example = [dict(offset=i * 5, type=kind, symbol=symbol, addend=addend)
                        for i, (_, kind, symbol, _, addend)
                        in enumerate(inspector.FIELDS)]

    def test_manifest_has_twenty_nonoverlapping_fields(self):
        inspector.validate_manifest()
        self.assertEqual(len(inspector.FIELDS), 20)
        self.assertEqual(sum(k == 'REL32' for _, k, _, _, _ in inspector.FIELDS), 8)
        self.assertEqual(sum(k == 'DIR32' for _, k, _, _, _ in inspector.FIELDS), 12)

    def test_target_addresses_bound_independently(self):
        self.assertEqual(inspector.target_encoding(0x4227A0, 0x006, 'DIR32',
                                                  0x4A7D90, 0x348), 0x4A80D8)
        addr = inspector.target_encoding(0x4227A0, 0x021, 'REL32', 0x401510, 0)
        self.assertEqual((addr + 0x4227A0 + 0x021 + 4) & 0xFFFFFFFF, 0x401510)

    def test_shared_runtime_rng_is_bound_to_actual_original_method(self):
        repo = SOURCE.parent.parent
        cpp = (repo / 'src/ScreenEffect.cpp').read_text()
        self.assertIn('#include "RngRuntimeLeaves.hpp"', cpp)
        self.assertIn('extern RngRuntimeView g_ReplayRng;', cpp)
        self.assertNotIn('ScreenEffectReplayRngView', cpp)
        rng_fields = [record for record in inspector.FIELDS
                      if 'ReplayRng' in record[2] or 'GetRandomU32InRange' in record[2]]
        self.assertEqual([(off, kind, destination) for off, kind, _, destination, _
                          in rng_fields],
                         [(0xA9, 'DIR32', 0x004ACE0C),
                          (0xB5, 'REL32', 0x004048E0),
                          (0xEE, 'DIR32', 0x004ACE0C),
                          (0xF3, 'REL32', 0x004048E0)])
        for _, _, symbol, _, _ in rng_fields:
            self.assertIn('RngRuntimeView', symbol)
        with (repo / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        unit = units['screen-effect-calc-shake']
        self.assertEqual(unit['source'], 'src/ScreenEffect.cpp')
        linked = [(r['offset'], r['type'], r['symbol'], r['target'])
                  for r in unit['relocations'] if 'ReplayRng' in r['symbol']
                  or 'GetRandomU32InRange' in r['symbol']]
        self.assertEqual([(off, kind, address) for off, kind, _, address in linked],
                         [(0x7E, 'DIR32', 0x004ACE0C),
                          (0x8C, 'REL32', 0x004048E0),
                          (0xD6, 'DIR32', 0x004ACE0C),
                          (0xDB, 'REL32', 0x004048E0)])
        self.assertTrue(all('RngRuntimeView' in name for _, _, name, _ in linked))
        previous = self.example
        for original in ('?g_ReplayRng@@3UScreenEffectReplayRngView@@A',
                         '?GetRandomU32InRange@ScreenEffectReplayRngView@@QAEII@Z'):
            corrupted = [dict(row) for row in previous]
            index = next(i for i, row in enumerate(corrupted)
                         if 'RngRuntimeView' in row['symbol'])
            corrupted[index]['symbol'] = original
            with self.subTest(original=original), self.assertRaisesRegex(
                    ValueError, 'field type/name/addend'):
                inspector.require_candidate(corrupted, 336)

    def test_reloc_type_mutation_fails_closed(self):
        self.example[0]['type'] = 'REL32'
        with self.assertRaisesRegex(ValueError, 'field type/name/addend'):
            inspector.require_candidate(self.example, 336)

    def test_reloc_symbol_mutation_fails_closed(self):
        self.example[1]['symbol'] = '?UnknownOrForgedMethod@@QAEHXZ'
        with self.assertRaisesRegex(ValueError, 'field type/name/addend'):
            inspector.require_candidate(self.example, 336)

    def test_reloc_addend_mutation_fails_closed(self):
        self.example[0]['addend'] = 0
        with self.assertRaisesRegex(ValueError, 'field type/name/addend'):
            inspector.require_candidate(self.example, 336)

    def test_reloc_order_mutation_fails_closed(self):
        self.example[4], self.example[5] = self.example[5], self.example[4]
        with self.assertRaisesRegex(ValueError, 'field type/name/addend'):
            inspector.require_candidate(self.example, 336)

    def test_reloc_missing_or_extra_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'relocation count'):
            inspector.require_candidate(self.example[:-1], 336)
        with self.assertRaisesRegex(ValueError, 'relocation count'):
            inspector.require_candidate(self.example + [self.example[0]], 336)

    def test_function_size_outside_reviewed_region_fails_closed(self):
        with self.assertRaisesRegex(ValueError, 'function size'):
            inspector.require_candidate(self.example, 299)
        with self.assertRaisesRegex(ValueError, 'function size'):
            inspector.require_candidate(self.example, 401)

    def test_overlapping_field_span_fails_closed(self):
        self.example[1]['offset'] = self.example[0]['offset'] + 2
        with self.assertRaisesRegex(ValueError, 'overlapping'):
            inspector.require_candidate(self.example, 336)

    def test_out_of_function_reloc_fails_closed(self):
        self.example[-1]['offset'] = 333
        with self.assertRaisesRegex(ValueError, 'outside owner'):
            inspector.require_candidate(self.example, 336)

    def test_only_reviewed_fields_and_branch_displacements_are_masked(self):
        def ins(offset, raw, mnemonic, immediate_size=0, immediate_offset=0):
            return SimpleNamespace(
                address=inspector.BASE + offset, size=len(raw), bytes=raw,
                mnemonic=mnemonic, imm_size=immediate_size,
                imm_offset=immediate_offset,
            )
        stream = [ins(0, bytes.fromhex('E8AABBCCDD'), 'call', 4, 1),
                  ins(5, bytes.fromhex('7506'), 'jne', 1, 1),
                  ins(7, bytes.fromhex('25FF000000'), 'and', 4, 1)]
        normalized = inspector.normalized_instructions(stream, [1])
        self.assertEqual(normalized, [(0, bytes.fromhex('E800000000')),
                                      (5, bytes.fromhex('7500')),
                                      (7, bytes.fromhex('25FF000000'))])

    def test_field_cannot_escape_instruction_boundary(self):
        inst = SimpleNamespace(address=inspector.BASE, size=5,
                               bytes=bytes.fromhex('E800000000'),
                               mnemonic='call', imm_size=4, imm_offset=1)
        with self.assertRaisesRegex(ValueError, 'crosses instruction'):
            inspector.normalized_instructions([inst], [2])
        with self.assertRaisesRegex(ValueError, 'does not correspond'):
            inspector.normalized_instructions([inst], [7])

    def test_manifest_rejected_on_duplicate_target_field(self):
        original = inspector.FIELDS
        try:
            inspector.FIELDS = original + [original[0]]
            with self.assertRaises(ValueError):
                inspector.validate_manifest()
        finally:
            inspector.FIELDS = original


if __name__ == '__main__':
    main()
