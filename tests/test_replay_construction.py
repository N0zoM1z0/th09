"""Constructor origin, complete extents and selective replay policy guards."""
import copy
import hashlib
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('replay_construction_audit', ROOT/'scripts/check-replay-data-layout.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def units():
    return tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']


def complete(name, size):
    return dict(unit=name, size=size, compared_size=size,
                matched_bytes=size, matched_compared_bytes=size, result='exact')


class ReplayConstructionPolicyTests(unittest.TestCase):
    def test_constructor_source_profile_full_extent_and_native_callee(self):
        original = units()
        audit.validate_models(original)
        for name in audit.MODELS:
            for key, value in (('size', original[name]['size']-1),
                               ('source', 'src/ReplayInput.cpp'), ('profile', ['/O1']),
                               ('relocations', [])):
                changed = copy.deepcopy(original)
                changed[name][key] = value
                with self.subTest(unit=name, field=key), self.assertRaises(ValueError):
                    audit.validate_models(changed)
        changed = copy.deepcopy(original)
        changed['game-configuration-constructor']['target_address'] += 1
        with self.assertRaises(ValueError):
            audit.validate_models(changed)

    def test_complete_byte_equality_never_promotes_unknown_origin(self):
        functions, origins, matches = (audit.ledger(n) for n in
                                      ('functions.csv', 'function-origins.csv', 'matches.csv'))
        audit.validate_origin_policy(functions, origins, matches)
        for address, *_ in audit.MODELS.values():
            for table, field, value in (('function', 'owner', 'authored'),
                                        ('function', 'status', 'matching'),
                                        ('origin', 'origin', 'compiler'),
                                        ('origin', 'disposition', 'exclude')):
                f, o = copy.deepcopy(functions), copy.deepcopy(origins)
                row = next(r for r in (f if table == 'function' else o)
                           if int(r['address'], 0) == address)
                row[field] = value
                with self.subTest(address=hex(address), field=field), self.assertRaises(ValueError):
                    audit.validate_origin_policy(f, o, matches)
            with self.assertRaises(ValueError):
                audit.validate_origin_policy(functions, origins, matches+[dict(address=hex(address))])

    def test_dependent_replay_covers_each_of_61_accepted_units_once(self):
        manifest, matches = units(), audit.ledger('matches.csv')
        groups = audit.dependent_groups(manifest, matches)
        names = [name for source, profile, group in groups for name in group]
        self.assertEqual(len(names), 61)
        self.assertEqual(len(set(names)), 61)
        self.assertEqual(len(groups), 7)
        self.assertNotIn('replay-payload-constructor', names)
        missing = [r for r in matches if r['unit'] != names[0]]
        with self.assertRaises(ValueError):
            audit.dependent_groups(manifest, missing)

    def test_exact_check_includes_physical_table_tail(self):
        report = complete('full-owner', 1560)
        report.update(compared_size=1592, matched_compared_bytes=1592)
        audit.require_exact(report)
        for changes in (dict(matched_compared_bytes=1591), dict(matched_bytes=1559),
                        dict(result='near'), dict(compared_size=1559, matched_compared_bytes=1559)):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.require_exact(dict(report, **changes))

    def test_wrong_target_is_rejected_before_compiler(self):
        fake = SimpleNamespace(verified_target=lambda: (_ for _ in ()).throw(ValueError('wrong target')))
        with patch.object(audit, 'module', return_value=fake), patch.object(audit, 'build') as compiler:
            with self.assertRaisesRegex(ValueError, 'wrong target'):
                audit.run()
            compiler.assert_not_called()

    def test_default_build_is_serial_scoped_and_detects_input_race(self):
        models = {name: (address, size, hashlib.sha256(bytes(size)).hexdigest(), *rest)
                  for name, (address, size, _, *rest) in audit.MODELS.items()}
        fake = SimpleNamespace(verified_target=lambda: b'target fixture',
                               pe_bytes_at=lambda target, address, size: bytes(size),
                               compare_unit=lambda name: complete(name, models[name][1]))
        with patch.object(audit, 'MODELS', models), patch.object(audit, 'module', return_value=fake), \
                patch.object(audit, 'build') as compiler, \
                patch.object(audit, 'input_hashes', return_value={'fixture': 'before'}) as inputs:
            report = audit.run()
            compiler.assert_called_once_with('replay-payload-constructor')
            self.assertEqual(inputs.call_args.args[0], [audit.SOURCE])
            self.assertEqual(report['constructor_bytes'], 33)
            self.assertEqual(report['authored_exact_delta'], 0)
            self.assertTrue(report['diagnostic_only'])
            self.assertEqual(report['dependent_units'], [])
            inputs.side_effect = [{'fixture': 'before'}, {'fixture': 'after'}]
            with self.assertRaisesRegex(ValueError, 'inputs changed'):
                audit.run()


if __name__ == '__main__':
    unittest.main()
