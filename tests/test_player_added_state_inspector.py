"""No-target regressions for complete Player initialization evidence."""

import hashlib
import importlib.util
from pathlib import Path
import struct
from unittest import TestCase
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]


def load(name, relative):
    spec = importlib.util.spec_from_file_location(name, ROOT / relative)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


inspector = load('player_added_inspector', 'scripts/inspect-player-added-state.py')
frontiers = load('player_added_frontiers', 'scripts/check-boss-frontiers.py')


def fields():
    result = []
    for off, (kind, destination) in inspector.TARGET_FIELDS.items():
        symbols = [name for name, va in inspector.EXTERNALS.items() if va == destination]
        symbol = symbols[0] if symbols else '$L' + str(off)
        result.append(dict(offset=off, type=kind, symbol=symbol, addend=0,
                           symbol_section=0 if symbols else 1, owner_section=1,
                           symbol_value=0 if symbols else destination - inspector.BASE,
                           owner_value=0))
    return result


def evidence():
    return dict(
        diagnostic_only=True, exactness_credit='none',
        target_sha256=frontiers.PLAYER_ADDED_SHA256,
        target_bytes=572, candidate_bytes=572, code_bytes=552,
        bound_fields=25, fields_agree=25,
        target_instructions=136, candidate_instructions=136,
        target_calls=11, candidate_calls=11, ordered_calls_agree=True,
        target_blocks=20, candidate_blocks=20, direct_graph_agrees=True,
        switch_table_agrees=True, complete_bytes_equal=False,
        complete_differences=22,
        difference_offsets=[hex(x) for x in sorted(
            frontiers.PLAYER_ADDED_ORIGINAL_RESIDUAL_OFFSETS)])


class PlayerAddedInspectorTests(TestCase):
    def test_original_requires_complete_extent_and_digest(self):
        target = bytes(i % 256 for i in range(572))
        with patch.object(inspector, 'TARGET_SHA256', hashlib.sha256(target).hexdigest()):
            inspector.require_original(target)
            for wrong in (target[:-1], target + b'\0', target[:-1] + b'\0'):
                with self.subTest(size=len(wrong)), self.assertRaises(ValueError):
                    inspector.require_original(wrong)
        with self.assertRaises(ValueError):
            inspector.require_original(target)

    def test_complete_code_and_all_five_table_fields_are_linked(self):
        linked, bound = inspector.link_candidate(bytes(572), fields())
        self.assertEqual(len(bound), 25)
        self.assertEqual(struct.unpack_from('<5I', linked, 552), inspector.TARGET_TABLE)
        self.assertEqual(len(linked), 572)
        for size in (551, 552, 571, 573):
            with self.subTest(size=size), self.assertRaises(ValueError):
                inspector.link_candidate(bytes(size), fields())

    def test_unknown_symbol_and_wrong_local_definition_fail_closed(self):
        for off, change in ((0x30, {'symbol': '?invented_global'}),
                            (0x1AD, {'symbol_section': 0}),
                            (0x1AD, {'symbol_value': 0x221}),
                            (0x1AD, {'symbol_value': 572}),
                            (0x1AD, {'owner_section': 2})):
            rows = fields()
            row = next(r for r in rows if r['offset'] == off)
            row.update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                inspector.link_candidate(bytes(572), rows)

    def test_malformed_missing_duplicate_or_raw_fields_are_rejected(self):
        for change in ({'offset': 0x18}, {'offset': True}, {'type': 'DIR32'}, {'addend': 4}):
            rows = fields()
            rows[0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                inspector.link_candidate(bytes(572), rows)
        rows = fields()
        for wrong in (rows[:-1], rows + [rows[0]], rows[:-1] + [rows[0]]):
            with self.assertRaises(ValueError):
                inspector.link_candidate(bytes(572), wrong)
        raw = bytearray(572)
        raw[rows[0]['offset']] = 1
        with self.assertRaisesRegex(ValueError, 'raw field'):
            inspector.link_candidate(raw, rows)

    def test_no_nonrelocation_byte_is_masked_including_last_code_byte(self):
        rows = fields()
        covered = {off + i for off in inspector.TARGET_FIELDS for i in range(4)}
        for off in set(range(572)) - covered:
            raw = bytearray(572)
            raw[off] = 0xA5
            linked, _ = inspector.link_candidate(raw, rows)
            self.assertEqual(linked[off], 0xA5, hex(off))

    def test_every_previously_matching_byte_is_protected(self):
        baseline = evidence()
        expected = frontiers.PLAYER_ADDED_ORIGINAL_RESIDUAL_OFFSETS
        self.assertEqual(len(expected), 22)
        self.assertEqual(inspector.TARGET_SHA256, frontiers.PLAYER_ADDED_SHA256)
        self.assertEqual(frontiers.guard_player_added_state(baseline)['ordinary_differences'], 22)
        for off in set(range(572)) - expected:
            changed = (set(expected) - {min(expected)}) | {off}
            candidate = {**baseline, 'difference_offsets': [hex(x) for x in sorted(changed)]}
            with self.subTest(off=off), self.assertRaises(RuntimeError):
                frontiers.guard_player_added_state(candidate)

    def test_incomplete_or_contradictory_evidence_cannot_pass(self):
        baseline = evidence()
        for field, value in (
                ('target_sha256', '0' * 64), ('candidate_bytes', 552),
                ('code_bytes', 551), ('fields_agree', 24), ('bound_fields', 24),
                ('candidate_instructions', 135), ('target_calls', 10),
                ('candidate_calls', 12), ('ordered_calls_agree', False),
                ('candidate_blocks', 19), ('direct_graph_agrees', False),
                ('switch_table_agrees', False), ('complete_bytes_equal', True),
                ('complete_differences', 21), ('difference_offsets', ['0x3a'] * 22),
                ('exactness_credit', 'accepted'), ('diagnostic_only', False)):
            with self.subTest(field=field), self.assertRaises(RuntimeError):
                frontiers.guard_player_added_state({**baseline, field: value})

    def test_real_convergence_stays_noncrediting(self):
        baseline = evidence()
        for remaining in (21, 0):
            candidate = {**baseline, 'complete_differences': remaining,
                         'complete_bytes_equal': remaining == 0,
                         'difference_offsets': baseline['difference_offsets'][:remaining]}
            self.assertEqual(frontiers.guard_player_added_state(candidate)['ordinary_differences'], remaining)

    def test_focused_replay_reuses_the_one_actual_cold_carrier(self):
        for protect in (True, False):
            with self.subTest(protect=protect), patch.object(frontiers, 'build') as build, patch.object(
                    frontiers, 'parsed', return_value=evidence()), patch.object(
                    frontiers, 'cold_replay_exact_source', return_value=['three exact resets']) as replay:
                _, exact = frontiers.run_player_added_state(protect_exact=protect)
                build.assert_called_once_with('player-added-collision-reset')
                if protect:
                    replay.assert_called_once_with(
                        'src/PlayerAddedState.cpp', {'build/matching/PlayerAddedState.obj': 3},
                        already_built=('build/matching/PlayerAddedState.obj',))
                    self.assertEqual(exact, ['three exact resets'])
                else:
                    replay.assert_not_called()
                    self.assertEqual(exact, [])
