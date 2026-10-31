"""Creator identity comes from canonical contracts and independent PE call fields."""
import copy
import importlib.util
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT/'scripts'))
spec = importlib.util.spec_from_file_location('setup_creators', ROOT/'scripts/inspect-gameplay-setup.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def fixture():
    units, rows, original = {}, [], []
    offset = 0x100
    for name, symbol, address, size, count in audit.NATIVE_CREATORS:
        units[name] = dict(symbol=symbol, target_address=address, size=size)
        for _ in range(count):
            rows.append(dict(offset=offset+1, symbol=symbol, type='REL32', addend=0))
            original.append((offset, 'REL32', address))
            offset += 8
    return rows, original, units


class GameplaySetupCreatorContracts(unittest.TestCase):
    def test_nine_calls_use_eight_distinct_complete_native_contracts(self):
        rows, original, units = fixture()
        result = audit.require_native_creators(rows, original, units)
        self.assertEqual(len(result), 8)
        self.assertEqual(sum(row['count'] for row in result), 9)

    def test_original_call_census_is_independent_of_source_counts(self):
        rows, original, units = fixture()
        for changed in (original[:-1], original+original[:1],
                        [(off, 'DIR32', va) for off, _, va in original],
                        [(off, kind, va+4) for off, kind, va in original]):
            with self.subTest(changed=changed), self.assertRaises(ValueError):
                audit.require_native_creators(rows, changed, units)

    def test_old_alias_addend_or_noncall_field_cannot_replace_native_reference(self):
        rows, original, units = fixture()
        for change in (dict(symbol='?RegisterSubsystem1@@YIPAXHHH@Z'),
                       dict(addend=1), dict(addend=False), dict(type='DIR32')):
            changed = copy.deepcopy(rows)
            changed[0].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                audit.require_native_creators(changed, original, units)
        for changed in (rows[:-1], rows+rows[:1]):
            with self.assertRaises(ValueError):
                audit.require_native_creators(changed, original, units)
        for alias in ('?RegisterSubsystem0@@YIPAXH@Z', '?RegisterSubsystem5@@YIPAXH@Z',
                      '?RegisterSubsystem6@@YIPAXH@Z', '?RegisterSubsystem1@@YIPAXHHH@Z', '?RegisterSubsystem2@@YIPAXH@Z',
                      '?RegisterSubsystem3@@YIPAXHHH@Z', '?RegisterSubsystem4@@YIPAXH@Z',
                      '?RegisterSharedSubsystem@@YIPAXHHH@Z', '?RegisterSecondarySubsystem@@YIPAXXZ'):
            self.assertNotIn(alias, audit.DESTINATIONS)

    def test_changed_canonical_owner_cannot_be_hidden_by_matching_caller(self):
        rows, original, units = fixture()
        name = audit.NATIVE_CREATORS[0][0]
        for key, value in [('symbol', 'wrong'), ('target_address', 0x420C10), ('size', 1)]:
            changed = copy.deepcopy(units)
            changed[name][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                audit.require_native_creators(rows, original, changed)
        del units[name]
        with self.assertRaises(ValueError):
            audit.require_native_creators(rows, original, units)
