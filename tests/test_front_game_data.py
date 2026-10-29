"""Guard affected-source coverage, target identity and complete physical replay."""
import csv
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import tomllib
import unittest
from unittest.mock import Mock, patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('front_game_test', ROOT/'scripts/check-front-game-data.py')
gate = importlib.util.module_from_spec(spec)
spec.loader.exec_module(gate)


class FrontGameDataTests(unittest.TestCase):
    def inventory(self):
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        with (ROOT/'config/matches.csv').open(newline='') as stream:
            matches = list(csv.DictReader(stream))
        return units, matches

    def test_reviewed_family_requires_shared_owner_and_rejects_missing_unit(self):
        units, matches = self.inventory()
        groups = gate.affected_groups(units, matches)
        self.assertEqual(len(groups), 11)
        self.assertEqual(sum(len(names) for _, names in groups), 24)
        replay = [names for (source, _), names in groups if source == 'src/ReplayInput.cpp']
        self.assertEqual(sorted(map(len, replay)), [2, 10])
        omitted = groups[0][1][0]
        with self.assertRaisesRegex(ValueError, 'inventory changed'):
            gate.affected_groups(units, [row for row in matches if row['unit'] != omitted])
        for names in replay:
            with self.subTest(omitted=names[0]), self.assertRaisesRegex(ValueError, 'inventory changed'):
                gate.affected_groups(units, [row for row in matches if row['unit'] != names[0]])

    def test_wrong_target_stops_before_any_compile(self):
        coff = SimpleNamespace(verified_target=Mock(side_effect=ValueError('wrong target')))
        with patch.object(gate, 'module', return_value=coff), \
                patch.object(gate.subprocess, 'run') as compile_, \
                self.assertRaisesRegex(ValueError, 'wrong target'):
            gate.run()
        compile_.assert_not_called()

    def test_all_groups_are_serial_complete_and_input_bound(self):
        def compare(name):
            return dict(unit=name, result='exact', size=5, matched_bytes=5,
                        compared_size=9, matched_compared_bytes=9)
        coff = SimpleNamespace(verified_target=lambda: b'original', compare_unit=Mock(side_effect=compare))
        share = SimpleNamespace(share_object=Mock())
        boss = SimpleNamespace(run_front_added=Mock(return_value=({'front': 'guarded'}, [])))
        providers = {'compare-coff-function.py': coff, 'check-enemy-player-state.py': share,
                     'check-boss-frontiers.py': boss}
        with patch.object(gate, 'module', side_effect=lambda _, file: providers[file]), \
                patch.object(gate.subprocess, 'run', return_value=SimpleNamespace(returncode=0)) as build, \
                patch.object(gate, 'inputs', return_value={'source': 'stable'}) as pins:
            result = gate.run()
            self.assertEqual((len(result['checked']), result['failures']), (24, []))
            self.assertEqual(build.call_count, 11)
            self.assertEqual(share.share_object.call_count, 11)
            self.assertTrue(all(call.args[0][:3] == ['nice', '-n', '10'] for call in build.call_args_list))
            boss.run_front_added.assert_called_once_with(protect_exact=False)
            self.assertEqual(result['exactness_credit'], 'no new credit')

            def truncated(name):
                return {**compare(name), 'matched_compared_bytes': 8}
            coff.compare_unit.side_effect = truncated
            rejected = gate.run()
            self.assertEqual((len(rejected['checked']), len(rejected['failures'])), (0, 24))
            coff.compare_unit.side_effect = compare
            pins.side_effect = [{'source': 'before'}, {'source': 'changed'}]
            with self.assertRaisesRegex(ValueError, 'inputs changed'):
                gate.run()


if __name__ == '__main__':
    unittest.main()
