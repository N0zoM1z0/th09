"""A source alias cannot assign the shared physical getter a unique origin."""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'player_getter_policy', ROOT/'scripts/check-player-state-getter.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class PlayerGetterAlias(unittest.TestCase):
    def setUp(self):
        self.units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        self.function = next(r for r in audit.read_rows('functions.csv')
                             if int(r['address'], 0) == audit.ADDRESS)
        self.origin = next(r for r in audit.read_rows('function-origins.csv')
                           if int(r['address'], 0) == audit.ADDRESS)
        self.matches = audit.read_rows('matches.csv')

    def validate(self, units=None, function=None, origin=None, matches=None):
        audit.validate_contract(self.units if units is None else units,
                                self.function if function is None else function,
                                self.origin if origin is None else origin,
                                self.matches if matches is None else matches)

    def test_existing_native_interface_is_defined_without_physical_origin_credit(self):
        self.validate()
        self.assertNotIn(audit.ADDRESS, {int(r['address'], 0) for r in self.matches})

    def test_wrong_source_abi_profile_body_extent_and_fields_are_rejected(self):
        for change in ({'source':'src/PlayerRuntime.cpp'}, {'symbol':'fake-getter'},
                       {'size':2}, {'target_address':0x435EC1}, {'profile':['/O1']},
                       {'relocations':[{'offset':0, 'type':'DIR32'}]},
                       {'functions':['unrelated getter']}):
            units = copy.deepcopy(self.units)
            units[audit.UNIT].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate(units=units)

    def test_byte_equality_cannot_promote_authorship_or_unique_owner(self):
        for change in ({'proposed_name':'PlayerLifecycleView::GetUpdateState'},
                       {'module':'Player'}, {'owner':'authored'},
                       {'status':'matching'}, {'match_percent':'100.00'}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate(function={**self.function, **change})
        for change in ({'origin':'authored_game'}, {'origin':'compiler_generated'},
                       {'disposition':'authored'}, {'disposition':'exclude'}):
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate(origin={**self.origin, **change})

    def test_duplicate_exact_credit_for_shared_address_is_rejected(self):
        with self.assertRaises(ValueError):
            self.validate(matches=[*self.matches,
                                   {'address':'0x00435EC0', 'unit':audit.UNIT}])
