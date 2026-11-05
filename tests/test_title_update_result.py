"""Independent input gates for source-present TH09 Title OnUpdateResult."""

import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:  # python -S public CI deliberately excludes site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_result_update_inspector', ROOT / 'scripts/inspect-title-update-result.py')
result = importlib.util.module_from_spec(spec)
spec.loader.exec_module(result)


class TitleResultUpdateGates(unittest.TestCase):
    @staticmethod
    def synthetic():
        data = bytearray(result.SIZE)
        for off, kind, _, address in result.FIELDS:
            struct.pack_into('<I', data, off, result.encode(kind, address, off))
        return bytes(data), hashlib.sha256(data).hexdigest()

    @staticmethod
    def rows(shift=0):
        return [dict(offset=off + shift, type=kind, symbol=symbol, addend=0)
                for off, kind, symbol, _ in result.FIELDS]

    def test_original_has_34_fields_and_nine_calls(self):
        self.assertEqual(len(result.FIELDS), 34)
        self.assertEqual(len(result.CALLS), 9)
        self.assertEqual([off for off, _, _, _ in result.FIELDS],
                         sorted(off for off, _, _, _ in result.FIELDS))
        data, digest = self.synthetic()
        result.validate_original(data, target_hash=digest)

    def test_original_hash_and_each_field_encoding_are_independently_required(self):
        data, digest = self.synthetic()
        for off, _, _, _ in result.FIELDS:
            bad = bytearray(data)
            bad[off] ^= 0x80
            with self.subTest(off=hex(off)), self.assertRaises(ValueError):
                result.validate_original(bytes(bad), target_hash=hashlib.sha256(bad).hexdigest())
        with self.assertRaises(ValueError):
            result.validate_original(data, target_hash='0' * 64)
        with self.assertRaises(ValueError):
            result.validate_original(data[:-1], target_hash=digest)

    def test_source_field_payload_uses_each_real_candidate_offset(self):
        for shift in (0, 1):
            rows = self.rows(shift=shift)
            linked = result.bind_candidate(bytes(result.SIZE), rows)
            self.assertEqual(len(linked), result.SIZE)
            for row, (_, kind, _, target) in zip(rows, result.FIELDS):
                self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                                 result.encode(kind, target, row['offset']))
        self.assertEqual(len(result.bind_candidate(bytes(483), self.rows())), 483)

    def test_wrong_or_fabricated_source_fields_fail_closed(self):
        rows = self.rows(shift=1)
        tests = [rows[:-1], list(reversed(rows)),
                 [dict(rows[0], type='REL32')] + rows[1:],
                 [dict(rows[0], symbol='?injectedUnknown@@3HA')] + rows[1:],
                 [dict(rows[0], addend=4)] + rows[1:],
                 rows[:1] + [dict(rows[1], offset=rows[0]['offset'] + 1)] + rows[2:],
                 rows[:-1] + [dict(rows[-1], offset=result.SIZE - 2)]]
        for candidate in tests:
            with self.subTest(candidate=candidate[:2]), self.assertRaises(ValueError):
                result.bind_candidate(bytes(result.SIZE), candidate)
        for extent in (449, 521):
            with self.assertRaises(ValueError):
                result.bind_candidate(bytes(extent), rows)

    def test_native_storage_roots_preserve_original_effective_operands(self):
        rows = self.rows()
        replacements = {
            '?g_TitleInputFlags@@3IA':
                ('?g_ReplayInputStates@@3PAUReplayInputState@@A', 0x122),
            '?g_TitleCharacterUnlocked@@3PAEA':
                ('?g_PlayStatsRecord@@3UPlayStatsRecordView@@A', 0x4C),
            '?g_TitleCharacterUnlockedNormal@@3PAEA':
                ('?g_PlayStatsRecord@@3UPlayStatsRecordView@@A', 0x5C),
            '?g_TitleCharacterUnlockedMode4@@3PAEA':
                ('?g_PlayStatsRecord@@3UPlayStatsRecordView@@A', 0x6C),
        }
        for row in rows:
            if row['symbol'] in replacements:
                row['symbol'], row['addend'] = replacements[row['symbol']]
        self.assertEqual(result.bind_candidate(bytes(result.SIZE), rows),
                         result.bind_candidate(bytes(result.SIZE), self.rows()))
        for index, row in enumerate(rows):
            if row['symbol'] not in result.NATIVE_ADDENDS:
                continue
            damaged = [dict(item) for item in rows]
            damaged[index]['addend'] += 1
            with self.subTest(index=index), self.assertRaises(ValueError):
                result.bind_candidate(bytes(result.SIZE), damaged)

    def test_literal_payload_is_checked_even_when_names_and_fields_are_unchanged(self):
        literal = b'title/result00.png\0'
        symbol = '??_C@_0BD@JGJJAHCD@title?1result00?4png?$AA@'
        data = bytearray(60 + len(literal))
        struct.pack_into('<HH', data, 0, 0x14C, 1)
        struct.pack_into('<II', data, 36, len(literal), 60)
        struct.pack_into('<I', data, 56, 0x40000040)
        data[60:] = literal
        rows = [dict(symbol=symbol, symbol_section=1, symbol_value=0,
                     offset=0x156, addend=0)]

        class Original:
            @staticmethod
            def pe_bytes_at(image, address, size):
                if address != 0x48F640:
                    raise ValueError('unexpected literal root')
                return literal + bytes(size - len(literal))

        proof = result.require_literal_definitions(bytes(data), rows, b'', Original())
        self.assertEqual((proof[0]['target'], proof[0]['size']), (0x48F640, 19))
        data[60] ^= 1
        with self.assertRaisesRegex(ValueError, 'literal definition differs'):
            result.require_literal_definitions(bytes(data), rows, b'', Original())

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_decoder_distinguishes_jump_displacements_from_external_referents(self):
        # mov eax,[absolute], call next, jnz next, ret
        code = bytes.fromhex('a1ac1d4a00e8000000007500c3')
        ins, fields, calls = result.decoded_fields(code)
        self.assertEqual(sum(i.size for i in ins), len(code))
        self.assertEqual(fields, [(1, 'DIR32', 0x4A1DAC),
                                  (6, 'REL32', result.BASE + 10)])
        self.assertEqual(calls, [result.BASE + 10])

    @unittest.skipIf(capstone is None, 'Capstone unavailable under python -S')
    def test_wrong_abi_or_indirect_call_does_not_pass(self):
        for code in (bytes.fromhex('ffd0c3'), bytes.fromhex('c21000')):
            with self.assertRaises(ValueError):
                result.decoded_fields(code)


if __name__ == '__main__':
    unittest.main()
