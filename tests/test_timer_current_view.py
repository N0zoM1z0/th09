"""Keep the new native alias separate from existing physical exact credit."""
import copy
import csv
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('timer_current_view',
                                             ROOT/'scripts/check-timer-current-view.py')
CHECK = importlib.util.module_from_spec(spec)
spec.loader.exec_module(CHECK)


class TimerCurrentViewTests(unittest.TestCase):
    def setUp(self):
        self.units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        with (ROOT/'config/functions.csv').open() as stream:
            self.function = next(row for row in csv.DictReader(stream)
                                 if int(row['address'], 0) == CHECK.ADDRESS)
        with (ROOT/'config/matches.csv').open() as stream:
            self.matches = list(csv.DictReader(stream))

    def test_native_alias_requires_real_source_profile_abi_and_extent(self):
        CHECK.validate_contract(self.units, self.function, self.matches)
        for changes in ({'source': 'src/unrelated.cpp'}, {'object': 'build/other.obj'},
                        {'profile': ['/Ob0']}, {'functions': ['TextHelper::GetHeight']},
                        {'symbol': '?GuessedGetter@@QAEHXZ'}, {'size': 8},
                        {'target_address': CHECK.ADDRESS+1},
                        {'relocations': [{'offset': 0, 'type': 'DIR32'}]}):
            units = copy.deepcopy(self.units)
            units[CHECK.ALIAS].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                CHECK.validate_contract(units, self.function, self.matches)

    def test_alias_cannot_replace_or_duplicate_the_existing_text_credit(self):
        for changes in ({'proposed_name': 'ZunTimer::operator int'},
                        {'module': 'ZunTimer'},
                        {'source_file': 'src/ZunTimer.cpp'}, {'owner': 'unknown'},
                        {'status': 'unclassified'}, {'match_percent': '0.00'}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                CHECK.validate_contract(self.units, {**self.function, **changes}, self.matches)
        original = next(row for row in self.matches if row['unit'] == CHECK.CANONICAL)
        for matches in ([row for row in self.matches if row is not original],
                        self.matches + [dict(original, unit=CHECK.ALIAS)]):
            with self.assertRaises(ValueError):
                CHECK.validate_contract(self.units, self.function, matches)
        units = copy.deepcopy(self.units)
        units[CHECK.CANONICAL]['source'] = 'src/ZunTimer.cpp'
        with self.assertRaises(ValueError):
            CHECK.validate_contract(units, self.function, self.matches)

    def test_wrong_target_never_reaches_a_compiler(self):
        class WrongTarget:
            @staticmethod
            def verified_target():
                raise ValueError('wrong original target')

        class Loader:
            @staticmethod
            def exec_module(module):
                module.verified_target = WrongTarget.verified_target

        with (patch.object(CHECK.importlib.util, 'spec_from_file_location',
                           return_value=type('Spec', (), {'loader': Loader()})()),
              patch.object(CHECK.importlib.util, 'module_from_spec', return_value=WrongTarget()),
              patch.object(CHECK.subprocess, 'run') as compiler):
            with self.assertRaisesRegex(ValueError, 'wrong original target'):
                CHECK.run()
        compiler.assert_not_called()


if __name__ == '__main__':
    unittest.main()
