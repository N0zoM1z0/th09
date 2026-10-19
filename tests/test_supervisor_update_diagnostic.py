"""Target-independent operand binding guards; no private target or decoder."""

import importlib.util
import copy
from pathlib import Path
import struct
import tomllib
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "supervisor_update_diagnostic", ROOT / "scripts/inspect-supervisor-update.py"
)
DIAGNOSTIC = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(DIAGNOSTIC)
SPEC = importlib.util.spec_from_file_location(
    "supervisor_update_frontier", ROOT / "scripts/check-boss-frontiers.py"
)
BOSS = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(BOSS)


def local_field(offset=4, symbol_value=28):
    return dict(offset=offset, type="DIR32", symbol="$switch_table", addend=0,
                symbol_section=3, symbol_value=symbol_value,
                owner_section=3, owner_value=4)


class SupervisorBindingTests(unittest.TestCase):
    def test_local_destination_uses_actual_symbol_coordinate_not_field_offset(self):
        # Moving a field must not move the table that its actual symbol names.
        for offset in (4, 12):
            linked, rows, table = DIAGNOSTIC.bind(bytes(32), [local_field(offset)])
            self.assertEqual(table, 24)
            self.assertEqual(struct.unpack_from("<I", linked, offset)[0], DIAGNOSTIC.BASE + 24)
            self.assertEqual(rows[0]["destination"], DIAGNOSTIC.BASE + 24)

    def test_unknown_external_is_not_resolved_from_plausible_raw_address(self):
        raw = bytearray(32)
        struct.pack_into("<I", raw, 12, DIAGNOSTIC.BASE)
        unknown = dict(local_field(12), symbol="unreviewed_external", symbol_section=0,
                       symbol_value=0, addend=DIAGNOSTIC.BASE)
        with self.assertRaisesRegex(ValueError, "unreviewed external symbol"):
            DIAGNOSTIC.bind(raw, [local_field(), unknown])

    def test_partially_overlapping_fields_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "overlapping fields"):
            DIAGNOSTIC.bind(bytes(32), [local_field(4), local_field(6)])

    def test_title_registration_counts_actual_direct_calls_only(self):
        target = DIAGNOSTIC.TITLE_REGISTER_TARGET
        calls = [
            {"address": DIAGNOSTIC.BASE + 8, "destination": target},
            {"address": DIAGNOSTIC.BASE + 16, "indirect": "[ecx+0x14]"},
            {"address": DIAGNOSTIC.BASE + 24, "destination": 0x0041B9D0},
            {"address": DIAGNOSTIC.BASE + 32, "destination": target},
        ]
        self.assertEqual(DIAGNOSTIC.count_direct_calls_to(calls, target), 2)
        self.assertEqual(DIAGNOSTIC.count_direct_calls_to(calls, 0x0041B9D0), 1)
        self.assertEqual(DIAGNOSTIC.count_direct_calls_to([], target), 0)
        with self.assertRaisesRegex(ValueError, "unreviewed destination"):
            DIAGNOSTIC.count_direct_calls_to(calls, -1)
        for bad in ([{"destination": target}],
                    [{"address": DIAGNOSTIC.BASE, "unreviewed": 1}],
                    [{"address": DIAGNOSTIC.BASE, "destination": "unknown"}]):
            with self.subTest(bad=bad), self.assertRaises(ValueError):
                DIAGNOSTIC.count_direct_calls_to(bad, target)

    def test_field_and_label_must_belong_to_complete_owner(self):
        with self.assertRaisesRegex(ValueError, "field outside complete owner"):
            DIAGNOSTIC.bind(bytes(32), [local_field(30)])
        with self.assertRaisesRegex(ValueError, "local label outside owner"):
            DIAGNOSTIC.bind(bytes(32), [local_field(symbol_value=36)])

    def test_real_supervisor_addend_and_native_methods_reject_old_aliases(self):
        field = dict(local_field(12), symbol='?g_Supervisor@@3VSupervisor@@A',
                     symbol_section=0, symbol_value=0, addend=0x794)
        linked, rows, _ = DIAGNOSTIC.bind(bytes(32), [local_field(), field])
        self.assertEqual(struct.unpack_from('<I', linked, 12)[0], 0x004B3894)
        self.assertEqual(rows[1]['destination'], 0x004B3894)
        for alias in ('?g_SupervisorState794@@3HA',
                      '?g_SupervisorFrameOffsets@@3PAUSupervisorFrameOffset@?ANON@@A',
                      '?UpdateLoadingVms@AnmManagerSupervisorView@?ANON@@QAEXPAXH@Z',
                      '?UpdateSupervisorFrame@SoundPlayerSupervisorView@?ANON@@QAEXXZ',
                      '?CheckVersion@SupervisorMethodView@?ANON@@QAEHPADHH@Z'):
            with self.subTest(alias=alias), self.assertRaisesRegex(ValueError, 'unreviewed external'):
                DIAGNOSTIC.bind(bytes(32), [local_field(), dict(field, symbol=alias)])


class SupervisorFrontierTests(unittest.TestCase):
    def fixture(self):
        offsets = [n for n in range(996)
                   if BOSS.SUPERVISOR_UPDATE_RESIDUAL_MASK[n//8] & (1 << (n % 8))]
        self.assertEqual(len(offsets), 835)
        return dict(diagnostic_only=True, exactness_credit='none', target_owned_bytes=996,
                    original_owned_sha256=BOSS.SUPERVISOR_UPDATE_SHA256,
                    target_authored_bytes=844, candidate_fields=92, target_fields=92,
                    target_instructions=217, candidate_instructions=205,
                    candidate_owned_bytes=952, candidate_pre_table_bytes=800,
                    target_calls=27, candidate_calls=27, target_title_register_calls=1,
                    candidate_title_register_calls=1, shared_title_register_call_agree=True,
                    external_field_inventory_equal=True, missing_external_fields=[],
                    extra_external_fields=[], target_table_counts=[10, 4, 6, 15],
                    candidate_table_counts=[10, 4, 6, 15], case_index_alignment_equal=True,
                    overlapping_differences=791, absent_bytes=44, excess_bytes=0,
                    original_difference_offsets=offsets, complete_equal=False)

    def test_same_count_exchange_may_not_damage_a_matching_original_byte(self):
        row = self.fixture()
        BOSS.guard_supervisor_update(row)
        good = next(n for n in range(952) if n not in row['original_difference_offsets'])
        row['original_difference_offsets'] = sorted([good] + row['original_difference_offsets'][1:])
        with self.assertRaises(RuntimeError):
            BOSS.guard_supervisor_update(row)

    def test_improvement_is_allowed_but_missing_tail_cannot_be_hidden(self):
        row = self.fixture()
        BOSS.guard_supervisor_update({**row, 'overlapping_differences': 790,
            'original_difference_offsets': row['original_difference_offsets'][1:]})
        for bad in ({**row, 'absent_bytes': 0},
                    {**row, 'overlapping_differences': 790,
                     'original_difference_offsets': row['original_difference_offsets'][:-1]},
                    {**row, 'overlapping_differences': True},
                    {**row, 'original_difference_offsets': row['original_difference_offsets'][::-1]},
                    {**row, 'candidate_calls': 26}, {**row, 'candidate_title_register_calls': 3},
                    {**row, 'candidate_table_counts': [10, 4, 5, 15]},
                    {**row, 'original_owned_sha256': '0'*64}):
            with self.subTest(bad=bad.keys()), self.assertRaises(RuntimeError):
                BOSS.guard_supervisor_update(bad)

    def test_zero_requires_complete_extent_and_instruction_census_and_stays_noncrediting(self):
        row = {**self.fixture(), 'original_difference_offsets': [],
               'overlapping_differences': 0, 'complete_equal': True}
        with self.assertRaises(RuntimeError):
            BOSS.guard_supervisor_update(row)
        row.update(candidate_owned_bytes=996, candidate_pre_table_bytes=844, absent_bytes=0)
        with self.assertRaises(RuntimeError):
            BOSS.guard_supervisor_update(row)
        row['candidate_instructions'] = 217
        self.assertEqual(BOSS.guard_supervisor_update(row)['exactness_credit'], 'none')

    def test_focused_owner_replays_all_six_actual_dependencies(self):
        with patch.object(BOSS, 'build_nonexact') as build_nonexact, \
                patch.object(BOSS, 'parsed', return_value=self.fixture()), \
                patch.object(BOSS, 'build') as build, \
                patch.object(BOSS, 'canonical', side_effect=lambda name: {'unit': name}):
            _, exact = BOSS.run_supervisor_update(protect_exact=True)
        build_nonexact.assert_called_once_with('src/SupervisorUpdate.cpp',
            'build/matching/SupervisorUpdate.obj', reference_unit='supervisor-register-chain')
        self.assertEqual(build.call_count, 6)
        self.assertEqual(len({x['unit'] for x in exact}), 6)

    def test_native_symbol_source_address_and_profile_substitution_fail(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, field, bad in (
                ('supervisor-check-version', 'symbol', '?CheckVersion@SupervisorMethodView@@QAEHPADHH@Z'),
                ('supervisor-load-version-data', 'target_address', 0x0042F101),
                ('soundplayer-update-fades', 'source', 'src/Other.cpp'),
                ('anm-manager-execute-script-array', 'profile', ['/Ob0'])):
            changed = copy.deepcopy(units)
            changed[name][field] = bad
            with self.subTest(name=name), patch.object(BOSS.tomllib, 'load', return_value={'units': changed}), \
                    patch.object(BOSS, 'build_nonexact'), \
                    patch.object(BOSS, 'parsed', return_value=self.fixture()), \
                    patch.object(BOSS, 'build'), patch.object(BOSS, 'canonical'), self.assertRaises(RuntimeError):
                BOSS.run_supervisor_update(protect_exact=True)


if __name__ == "__main__":
    unittest.main()
