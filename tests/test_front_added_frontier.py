"""Front field provenance, preserved bytes and native-callee replay boundaries."""
import copy
import importlib.util
from pathlib import Path
import struct
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]


def load(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/file)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


audit = load('front_added_inspector_test', 'inspect-front-added.py')
boss = load('front_added_gate_test', 'check-boss-frontiers.py')


class FrontAddedFieldTests(unittest.TestCase):
    def test_game_data_requires_native_root_and_five_original_member_addends(self):
        target, rows = bytearray(audit.SIZE), []
        aliases = {0xFC: '?g_FrontCharacterIndex@@3HA',
                   0x58: '?g_GameSide1Value20@@3HA', 0x20: '?g_GameSide0Value20@@3HA'}
        for offset, member in audit.GAME_MANAGER_DATA_FIELDS:
            struct.pack_into('<I', target, offset, audit.GAME_MANAGER_BASE+member)
            rows.append(dict(offset=offset, type='DIR32', symbol=audit.GAME_MANAGER_SYMBOL,
                             addend=member, symbol_section=0))
        self.assertEqual(audit.require_game_manager_fields(rows, target)['fields'], 5)
        for index, (_, member) in enumerate(audit.GAME_MANAGER_DATA_FIELDS):
            for changes in ({'symbol': aliases[member], 'addend': 0}, {'addend': member+4},
                            {'type': 'REL32'}, {'symbol_section': 1}):
                changed = copy.deepcopy(rows)
                changed[index].update(changes)
                with self.subTest(index=index, changes=changes), self.assertRaises(ValueError):
                    audit.require_game_manager_fields(changed, target)
        for changed in (rows[:-1], rows+[rows[0]], rows[::-1]):
            with self.assertRaises(ValueError):
                audit.require_game_manager_fields(changed, target)
        damaged = bytearray(target)
        damaged[audit.GAME_MANAGER_DATA_FIELDS[0][0]] ^= 1
        with self.assertRaises(ValueError):
            audit.require_game_manager_fields(rows, damaged)
        with self.assertRaises(ValueError):
            audit.require_game_manager_fields(rows, target[:-1])
        self.assertTrue(all(symbol not in audit.DESTINATIONS for symbol in aliases.values()))

    def fixture(self):
        target = bytearray(audit.SIZE)
        source, fields = [], []
        symbol = '?g_AnmManager@@3PAVAnmManager@@A'
        dest = audit.DESTINATIONS[symbol]
        for n in range(37):
            off, actual = 4 + 16*n, 7 + 16*n
            kind = 'REL32' if n % 2 else 'DIR32'
            value = dest - (audit.BASE + off + 4 if kind == 'REL32' else 0)
            struct.pack_into('<I', target, off, value & 0xFFFFFFFF)
            fields.append((off, kind, dest))
            source.append({'offset': actual, 'type': kind, 'symbol': symbol, 'addend': 0})
        return bytes(target), fields, source

    def test_independent_original_and_actual_coff_offsets(self):
        target, fields, source = self.fixture()
        result = audit.bind_candidate(bytes(audit.SIZE), source, target, fields)
        for row, (_, kind, dest) in zip(source, fields):
            expected = dest - (audit.BASE + row['offset'] + 4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', result, row['offset'])[0], expected & 0xFFFFFFFF)

    def test_reject_same_count_forged_referent_kind_addend_order_and_overlap(self):
        target, fields, source = self.fixture()
        for bad in [source[:-1], list(reversed(source)),
                    [dict(source[0], symbol='?Reset@FrontAddedMessageRuntimeView@@QAEHXZ')] + source[1:],
                    [dict(source[0], addend=4)] + source[1:],
                    [dict(source[0], type='REL32')] + source[1:],
                    source[:1] + [dict(source[1], offset=source[0]['offset'] + 1)] + source[2:]]:
            with self.subTest(bad=bad[:2]), self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), bad, target, fields)
        corrupted = bytearray(target)
        corrupted[fields[0][0]] ^= 1
        with self.assertRaises(ValueError):
            audit.bind_candidate(bytes(audit.SIZE), source, corrupted, fields)


class FrontAddedGuardTests(unittest.TestCase):
    def fixture(self):
        offsets = [n for n in range(706) if boss.FRONT_ADDED_RESIDUAL_MASK[n//8] & (1 << (n % 8))]
        self.assertEqual(len(offsets), 222)
        return {'diagnostic_only': True, 'exactness_credit': 'none', 'target_bytes': 706,
                'candidate_bytes': 706, 'target_sha256': boss.FRONT_ADDED_SHA256,
                'target_instructions': 192, 'candidate_instructions': 189,
                'original_fields': 37, 'bound_fields': 37, 'ordered_calls': 21,
                'calls_agree': True, 'target_blocks': 19, 'candidate_blocks': 19,
                'direct_graphs_agree': True, 'differences': len(offsets),
                'difference_offsets': offsets, 'complete_bytes_equal': False}

    def test_same_count_may_not_damage_an_old_matching_byte(self):
        row = self.fixture()
        bad = next(n for n in range(706) if n not in row['difference_offsets'])
        row['difference_offsets'] = sorted([bad] + row['difference_offsets'][1:])
        with self.assertRaises(RuntimeError):
            boss.guard_front_added(row)

    def test_narrowing_is_allowed_but_forged_zero_or_counts_fail(self):
        row = self.fixture()
        narrowed = {**row, 'differences': 221, 'difference_offsets': row['difference_offsets'][:-1]}
        self.assertEqual(boss.guard_front_added(narrowed)['differences'], 221)
        for bad in [{**row, 'differences': True},
                    {**row, 'difference_offsets': row['difference_offsets'][::-1]},
                    {**row, 'differences': 0, 'difference_offsets': [], 'complete_bytes_equal': True},
                    {**row, 'target_sha256': '0'*64}, {**row, 'ordered_calls': 20}]:
            with self.subTest(bad=bad), self.assertRaises(RuntimeError):
                boss.guard_front_added(bad)
        zero = {**row, 'candidate_instructions': 192, 'differences': 0,
                'difference_offsets': [], 'complete_bytes_equal': True}
        self.assertEqual(boss.guard_front_added(zero)['exactness_credit'], 'none')

    def test_focused_replays_all_four_callees_and_all_mode_reuses_only_two(self):
        for shared, count in [(False, 4), (True, 2)]:
            with self.subTest(shared=shared), patch.object(boss, 'build_nonexact') as compile_, \
                    patch.object(boss, 'parsed', return_value=self.fixture()), \
                    patch.object(boss, 'build') as build, \
                    patch.object(boss, 'canonical', side_effect=lambda n: {'unit': n}):
                _, exact = boss.run_front_added(protect_exact=True, shared_callees_prechecked=shared)
            compile_.assert_called_once_with('src/FrontAddedCallback.cpp',
                'build/matching/FrontAddedCallback.obj', reference_unit='front-message-setup')
            self.assertEqual(build.call_count, count)
            self.assertEqual(len(exact), count)
            self.assertEqual(len({row['unit'] for row in exact}), count)

    def test_native_alias_address_source_or_profile_drift_is_rejected(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, field, bad in [
                ('front-message-runtime-reset', 'symbol', '?Reset@FrontAddedMessageRuntimeView@@QAEHXZ'),
                ('front-message-load-for-side-count', 'source', 'src/Other.cpp'),
                ('front-message-release-for-side-count', 'target_address', 0x417521),
                ('front-message-initialize-runtime', 'profile', ['/Ob0'])]:
            changed = copy.deepcopy(units)
            changed[name][field] = bad
            with self.subTest(name=name, field=field), patch.object(boss.tomllib, 'load', return_value={'units': changed}), \
                    patch.object(boss, 'build_nonexact'), patch.object(boss, 'parsed', return_value=self.fixture()), \
                    patch.object(boss, 'build'), patch.object(boss, 'canonical'), self.assertRaises(RuntimeError):
                boss.run_front_added(protect_exact=True)


if __name__ == '__main__':
    unittest.main()
