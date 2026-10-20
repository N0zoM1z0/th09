"""Independent original-operand corruption tests for the large Player boss."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'player_cancel', ROOT / 'scripts/inspect-player-bullet-collision.py')
player = importlib.util.module_from_spec(spec)
spec.loader.exec_module(player)


class PlayerBulletOriginalTests(unittest.TestCase):
    @staticmethod
    def original_synthetic():
        data = bytearray(player.SIZE)
        for off, kind, dest in player.TARGET_FIELDS:
            struct.pack_into('<I', data, off, player.encode(kind, dest, off))
        return bytes(data)

    @staticmethod
    def source_rows():
        rows = []
        for n, (_, kind, dest) in enumerate(player.TARGET_FIELDS):
            matches = []
            for symbol, base in player.SOURCE_TARGETS.items():
                good_kind = 'DIR32' if symbol.startswith(('?g_', '__real@')) else 'REL32'
                addends = (0, 0x11C, 0x358) if symbol.startswith('?g_GameManager@@') else (0,)
                for addend in addends:
                    if good_kind == kind and base + addend == dest:
                        matches.append((symbol, addend))
            if len(matches) != 1:
                raise AssertionError('original Player PE identity is ambiguous')
            symbol, addend = matches[0]
            rows.append({'offset': 12 + 10 * n, 'type': kind,
                         'symbol': symbol, 'addend': addend})
        return rows

    def test_original_has_862_bytes_and_26_verified_operands(self):
        self.assertEqual(len(player.TARGET_FIELDS), 26)
        self.assertEqual(len(player.CALLS), 12)
        raw = self.original_synthetic()
        player.original_manifest(raw, expected_sha=hashlib.sha256(raw).hexdigest())
        with self.assertRaisesRegex(ValueError, 'SHA-256'):
            player.original_manifest(raw)
        for off, _, _ in player.TARGET_FIELDS:
            bad = bytearray(raw)
            bad[off] ^= 1
            with self.subTest(offset=off), self.assertRaisesRegex(ValueError, 'operand differs'):
                player.original_manifest(bytes(bad),
                    expected_sha=hashlib.sha256(bad).hexdigest())
        with self.assertRaises(ValueError):
            player.original_manifest(raw[:-1], expected_sha=hashlib.sha256(raw[:-1]).hexdigest())

    def test_full_original_multiset_is_preserved_at_real_source_offsets(self):
        source = self.source_rows()
        linked, bound = player.bind_candidate(bytes(849), source)
        self.assertEqual(len(bound), 26)
        for row, (_, kind, dest) in zip(source, player.TARGET_FIELDS):
            self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                             player.encode(kind, dest, row['offset']))
        self.assertEqual([(kind, dest) for _, kind, dest in bound],
                         [(kind, dest) for _, kind, dest in player.TARGET_FIELDS])

    def test_missing_or_new_field_wrong_symbol_type_addend_or_bytes_fail(self):
        source = self.source_rows()
        tests = [source[:-1], source + [source[-1]],
                 [dict(source[0], symbol='wrong-C++-callee')] + source[1:],
                 [dict(source[0], type='REL32')] + source[1:],
                 [dict(source[0], addend=4)] + source[1:],
                 [dict(source[0], offset=-1)] + source[1:],
                 [dict(source[0], offset=848)] + source[1:],
                 [dict(source[0], offset=source[1]['offset'])] + source[1:],
                 source[:2] + [source[3], source[2]] + source[4:]]
        for variant in tests:
            with self.subTest(variant=variant[:2]), self.assertRaises(ValueError):
                player.bind_candidate(bytes(849), variant)
        with self.assertRaisesRegex(ValueError, '26-field'):
            player.bind_candidate(bytes(770), source)

    def test_player_calls_have_reviewed_order_and_no_source_match_credit(self):
        self.assertEqual(player.CALLS[:3], (0x42AF40, 0x401680, 0x4016B0))
        self.assertEqual(player.CALLS[-2:], (0x41D150, 0x40F860))
        self.assertEqual(player.TARGET_FIELDS[0], (0x50, 'DIR32', 0x48E818))
        with self.assertRaises(ValueError):
            player.encode('unknown-field-type', 0x401680, 0x1c5)

    def test_private_service_aliases_and_unreviewed_game_offset_fail(self):
        rows = self.source_rows()
        aliases = {
            '?g_GameManager@@3UAsciiGameManagerView@@A':
                '?g_PlayerGameManagerRuntime@@3UPlayerCancelGameManagerView@@A',
            '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z':
                '?TransformPopupX@PlayerCancelGameManagerView@@QAEMM@Z',
            '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z':
                '?TransformPopupY@PlayerCancelGameManagerView@@QAEMM@Z',
            '?g_Supervisor@@3VSupervisor@@A':
                '?g_PlayerSupervisorRuntime@@3UPlayerCancelSupervisorView@@A',
            '?SelectSide@Supervisor@@QAEXH@Z':
                '?SelectSide@PlayerCancelSupervisorView@@QAEXH@Z',
            '?g_SoundPlayer@@3VSoundPlayer@@A':
                '?g_SoundPlayer@@3UPlayerCancelSoundView@@A',
            '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z':
                '?PlaySoundByIdx@PlayerCancelSoundView@@QAEXHH@Z',
        }
        self.assertEqual(set(aliases), set(aliases) & set(player.SOURCE_TARGETS))
        for index, row in enumerate(rows):
            if row['symbol'] in aliases:
                bad = [dict(r) for r in rows]
                bad[index]['symbol'] = aliases[row['symbol']]
                with self.subTest(symbol=row['symbol']), self.assertRaisesRegex(
                        ValueError, 'unreviewed Player collision'):
                    player.bind_candidate(bytes(849), bad)
        index = next(i for i, row in enumerate(rows)
                     if row['symbol'].startswith('?g_GameManager@@'))
        bad = [dict(r) for r in rows]
        bad[index]['addend'] = 0x354
        with self.assertRaisesRegex(ValueError, 'member offset'):
            player.bind_candidate(bytes(849), bad)
