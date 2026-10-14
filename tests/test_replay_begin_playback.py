"""No-target, fail-closed checks for TH09's complete replay playback Oracle."""

import hashlib
import importlib.util
from pathlib import Path
import struct
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
mod = importlib.util.spec_from_file_location(
    'replay_playback_inspector', ROOT / 'scripts/inspect-replay-begin-playback.py')
playback = importlib.util.module_from_spec(mod)
mod.loader.exec_module(playback)


class ReplayPlaybackOriginalManifestTests(TestCase):
    @staticmethod
    def original_bytes():
        code = bytearray(playback.SIZE)
        for off, kind, dest in playback.TARGET_FIELDS:
            struct.pack_into('<I', code, off, playback.encoded(kind, dest, off))
        return bytes(code)

    @staticmethod
    def sample_known():
        return {(symbol): {dest}
                for symbol, kind, dest, addend in playback.SOURCE_FIELDS}

    @staticmethod
    def sample_rows():
        # 32 nonoverlapping COFF-style fields, not target-byte substitutions.
        return [{'offset': 0x10 + i * 7, 'symbol': symbol, 'type': kind,
                 'addend': addend}
                for i, (symbol, kind, dest, addend)
                in enumerate(playback.SOURCE_FIELDS)]

    def test_full_original_manifest_has_all_32_physical_operands(self):
        self.assertEqual(len(playback.TARGET_FIELDS), 32)
        self.assertEqual(len(playback.SOURCE_FIELDS), 32)
        self.assertEqual(len(playback.CALL_ORDER), 5)
        self.assertEqual(playback.TARGET_FIELDS[19], (0x14D, 'DIR32', 0x004A810C))
        self.assertEqual(playback.TARGET_FIELDS[20], (0x15D, 'DIR32', 0x004A7DAC))
        data = self.original_bytes()
        playback.validate_original(data, sha256=hashlib.sha256(data).hexdigest())

    def test_target_original_hash_and_target_field_bits_both_required(self):
        data = self.original_bytes()
        with self.assertRaisesRegex(ValueError, 'physical owner SHA'):
            playback.validate_original(data)
        for off, _, _ in playback.TARGET_FIELDS:
            broken = bytearray(data)
            broken[off] ^= 1
            with self.subTest(offset=off), self.assertRaisesRegex(ValueError, 'destination/encoding'):
                playback.validate_original(bytes(broken),
                    sha256=hashlib.sha256(broken).hexdigest())
        with self.assertRaises(ValueError):
            playback.validate_original(data[:-1], sha256=hashlib.sha256(data[:-1]).hexdigest())

    def test_candidate_can_follow_retained_or_corrected_original_field_order(self):
        rows = self.sample_rows()
        linked, fields = playback.link_candidate(bytes(538), rows, self.sample_known())
        self.assertEqual(len(fields), 32)
        for row, (_, kind, address, addend) in zip(rows, playback.SOURCE_FIELDS):
            self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                             playback.encoded(kind, address + addend, row['offset']))
        rows[19], rows[20] = (
            dict(rows[20], offset=rows[19]['offset']),
            dict(rows[19], offset=rows[20]['offset']))
        linked, fields = playback.link_candidate(bytes(538), rows, self.sample_known())
        self.assertEqual(len(fields), 32)
        self.assertEqual(fields[19][2], 0x004A7DAC)
        self.assertEqual(fields[20][2], 0x004A810C)

    def test_candidate_wrong_count_alias_addend_kind_or_other_reorder_fails(self):
        baseline = self.sample_rows()
        variants = [baseline[:-1], baseline + [baseline[-1]],
                    [dict(baseline[0], symbol='?notOriginal@@3HA')] + baseline[1:],
                    [dict(baseline[0], type='REL32')] + baseline[1:],
                    [dict(baseline[0], addend=0)] + baseline[1:],
                    [dict(baseline[0], offset=baseline[1]['offset'])] + baseline[1:],
                    baseline[:5] + [baseline[6], baseline[5]] + baseline[7:],
                    baseline[:-1] + [dict(baseline[-1], offset=538)],
                    ]
        for rows in variants:
            with self.subTest(rows=rows[:2]), self.assertRaises(ValueError):
                playback.link_candidate(bytes(538), rows, self.sample_known())
        with self.assertRaisesRegex(ValueError, 'unreviewed or ambiguous'):
            playback.link_candidate(bytes(538), baseline,
                                    {key: value for key, value in self.sample_known().items()
                                     if key != baseline[4]['symbol']})

    def test_changed_verified_referent_cannot_make_an_exact_match(self):
        known = self.sample_known()
        known[playback.SOURCE_FIELDS[1][0]] = {0x12345678}
        with self.assertRaisesRegex(ValueError, 'substituted, duplicated or omitted'):
            playback.link_candidate(bytes(538), self.sample_rows(), known)
