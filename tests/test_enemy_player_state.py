"""Native state interfaces, complete comparisons and bounded replay scope."""
import ast
import copy
import csv
import importlib.util
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
tree = ast.parse((ROOT/'scripts/inspect-enemy-manager-update.py').read_text())
nodes = [node for node in tree.body
         if isinstance(node, ast.FunctionDef) and node.name in
            ('require_player_state_calls', 'require_player_damage_calls')
         or isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id in
            ('BASE', 'PLAYER_STATE_CALLS', 'PLAYER_DAMAGE_SYMBOL', 'PLAYER_DAMAGE_FIELDS')
            for t in node.targets)]
scope = {'struct': struct}
exec(compile(ast.Module(body=nodes, type_ignores=[]), '<state-identity-guard>', 'exec'), scope)
spec = importlib.util.spec_from_file_location('enemy_player_state_runner', ROOT/'scripts/check-enemy-player-state.py')
runner = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runner)


def manifest():
    return tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']


def fixture():
    target = bytearray(3900)
    rows = []
    for offset, symbol, destination in scope['PLAYER_STATE_CALLS']:
        target[offset-1] = 0xE8
        struct.pack_into('<i', target, offset, destination-(scope['BASE']+offset+4))
        rows.append(dict(offset=offset, symbol=symbol, type='REL32', addend=0, symbol_section=0))
    return rows, target


class EnemyPlayerStateTests(unittest.TestCase):
    def test_damage_interface_rejects_an_equal_address_private_alias(self):
        rows, target = [], bytearray(3900)
        for offset in scope['PLAYER_DAMAGE_FIELDS']:
            target[offset-1] = 0xE8
            struct.pack_into('<i', target, offset, 0x41FCD0-(scope['BASE']+offset+4))
            rows.append(dict(offset=offset, symbol=scope['PLAYER_DAMAGE_SYMBOL'],
                             type='REL32', addend=0, symbol_section=0))
        guard = scope['require_player_damage_calls']
        self.assertEqual(guard(rows, manifest(), target)['calls'], 2)
        for index in range(2):
            for changes in ({'symbol': '?CalcDamageToEnemy@EnemyCorePlayerView@@QAEHPAUFloat3@@0PAH11@Z'},
                            {'type': 'DIR32'}, {'addend': 4}, {'symbol_section': 1}):
                changed = copy.deepcopy(rows)
                changed[index].update(changes)
                with self.subTest(index=index, changes=changes), self.assertRaises(ValueError):
                    guard(changed, manifest(), target)
        for changed in (rows[:1], rows[::-1], rows+[rows[0]]):
            with self.assertRaises(ValueError):
                guard(changed, manifest(), target)
        wrong_target = bytearray(target)
        wrong_target[rows[1]['offset']] ^= 1
        with self.assertRaises(ValueError):
            guard(rows, manifest(), wrong_target)
        wrong_owner = manifest()
        wrong_owner['player-calc-damage-to-enemy']['size'] = 995
        with self.assertRaises(ValueError):
            guard(rows, wrong_owner, target)

    def test_damage_scope_covers_81_units_and_both_callee_bodies(self):
        with (ROOT/'config/matches.csv').open(newline='') as stream:
            matches = list(csv.DictReader(stream))
        groups = runner.groups(manifest(), matches, 'damage')
        names = [name for _, group in groups for name in group]
        self.assertEqual((len(groups), len(names), len(set(names))), (3, 81, 81))
        self.assertIn('player-build-aabb', names)
        self.assertIn('player-calc-damage-to-enemy', names)
        with self.assertRaises(ValueError):
            runner.groups(manifest(), [row for row in matches
                                      if row['unit'] != 'player-build-aabb'], 'damage')

    def test_native_names_are_required_even_at_the_same_numeric_destination(self):
        rows, target = fixture()
        report = scope['require_player_state_calls'](rows, manifest(), target)
        self.assertEqual(report['calls'], 3)
        self.assertTrue(report['getter_exact_credit'].startswith('none'))
        aliases = ('?GetState@EnemyCorePlayerView@@QAEHXZ',
                   '?GetState@EnemyCorePlayerView@@QAEHXZ',
                   '?SetState@EnemyCorePlayerView@@QAEXH@Z')
        for index in range(3):
            for changes in ({'symbol': aliases[index]}, {'addend': 1},
                            {'offset': rows[index]['offset']+1}, {'symbol_section': 1}):
                changed = copy.deepcopy(rows)
                changed[index].update(changes)
                with self.subTest(index=index, changes=changes), self.assertRaises(ValueError):
                    scope['require_player_state_calls'](changed, manifest(), target)
        changed = bytearray(target)
        changed[rows[0]['offset']] ^= 1
        with self.assertRaises(ValueError):
            scope['require_player_state_calls'](rows, manifest(), changed)
        wrong = manifest()
        wrong['player-set-update-state']['source'] = 'src/unrelated.cpp'
        with self.assertRaises(ValueError):
            scope['require_player_state_calls'](rows, wrong, target)

    def test_extra_native_state_calls_outside_original_offsets_are_not_ignored(self):
        rows, target = fixture()
        for original in rows:
            extra = {**original, 'offset': 0x400}
            changed = sorted(rows + [extra], key=lambda row: row['offset'])
            with self.subTest(symbol=extra['symbol']), self.assertRaisesRegex(
                    ValueError, 'state call inventory changed'):
                scope['require_player_state_calls'](changed, manifest(), target)

    def test_shifted_native_call_and_unreviewed_definition_have_distinct_errors(self):
        rows, target = fixture()
        for index in range(3):
            moved = copy.deepcopy(rows)
            moved[index]['offset'] -= 2
            with self.subTest(index=index), self.assertRaisesRegex(
                    ValueError, 'moved from original offset'):
                scope['require_player_state_calls'](moved, manifest(), target)
            defined = copy.deepcopy(rows)
            defined[index]['symbol_section'] = 7
            with self.subTest(index=index), self.assertRaisesRegex(
                    ValueError, 'unreviewed same-object definition'):
                scope['require_player_state_calls'](defined, manifest(), target)

    def test_scope_covers_all_104_units_once_in_six_serial_profiles(self):
        with (ROOT/'config/matches.csv').open(newline='') as stream:
            matches = list(csv.DictReader(stream))
        groups = runner.groups(manifest(), matches)
        names = [n for _, group in groups for n in group]
        self.assertEqual((len(groups), len(names), len(set(names))), (6, 104, 104))
        with self.assertRaises(ValueError):
            runner.groups(manifest(), [r for r in matches if r['unit'] != names[0]])
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            build = root/'build/matching'
            build.mkdir(parents=True)
            (build/'first.obj').write_bytes(b'freshly compiled fixture')
            units = {name: dict(source='src/one.cpp', profile=['/Ob1'],
                                object='build/matching/'+name+'.obj') for name in ('first', 'alias')}
            with patch.object(runner, 'ROOT', root):
                runner.share_object(units, ['first', 'alias'])
                self.assertEqual((build/'alias.obj').read_bytes(), (build/'first.obj').read_bytes())
                units['alias']['profile'] = ['/Ob0']
                with self.assertRaises(ValueError):
                    runner.share_object(units, ['first', 'alias'])

    def test_wrong_target_never_reaches_a_compiler(self):
        fake = SimpleNamespace(verified_target=lambda: (_ for _ in ()).throw(ValueError('wrong target')))
        with patch.object(runner, 'module', return_value=fake), patch.object(runner.subprocess, 'run') as compiler:
            with self.assertRaisesRegex(ValueError, 'wrong target'):
                runner.run()
            compiler.assert_not_called()

    def test_complete_physical_tail_and_input_races_fail_closed(self):
        units = manifest()
        def exact(name):
            u = units[name]
            size = u['size']
            physical = u.get('compare_size', size)
            return dict(unit=name, size=size, compared_size=physical, matched_bytes=size,
                        matched_compared_bytes=physical, result='exact')
        coff = SimpleNamespace(verified_target=lambda: b'target fixture', compare_unit=exact)
        enemy = SimpleNamespace(diagnose=lambda obj: {'fixture': True})
        ecl = SimpleNamespace(report=lambda obj: dict(physical_bytes=15564, relocations=598,
                            ordinary_different_bytes=30, field_different_bytes=0, table_different_bytes=0))
        guard = SimpleNamespace(guard_enemy_update=lambda report: None)
        modules = {'compare-coff-function.py': coff, 'inspect-enemy-manager-update.py': enemy,
                   'inspect-ecl-complete.py': ecl, 'check-boss-frontiers.py': guard}
        with patch.object(runner, 'module', side_effect=lambda name, file: modules[file]), \
                patch.object(runner, 'share_object'), \
                patch.object(runner, 'inputs', return_value={'fixture': 'same'}) as hashes, \
                patch.object(runner.subprocess, 'run', return_value=SimpleNamespace(returncode=0)) as compiler:
            report = runner.run()
            self.assertEqual(len(report['checked']), 104)
            self.assertEqual(compiler.call_count, 6)
            self.assertTrue(all(c.args[0][:3] == ['nice', '-n', '10'] for c in compiler.call_args_list))
            hashes.side_effect = [{'fixture': 'before'}, {'fixture': 'after'}]
            with self.assertRaisesRegex(ValueError, 'inputs changed'):
                runner.run()
            hashes.side_effect = None
            def partial(name):
                result = exact(name)
                result['matched_compared_bytes'] -= 1
                return result
            coff.compare_unit = partial
            with self.assertRaisesRegex(ValueError, 'complete equality'):
                runner.run()


if __name__ == '__main__':
    unittest.main()
