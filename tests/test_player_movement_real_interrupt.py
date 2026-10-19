"""Reject byte-equivalent fake C++ identities at Player's interrupt call."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'player_interrupt_frontier', ROOT/'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class PlayerMovementRealInterrupt(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    @staticmethod
    def fields():
        return {
            'target_fields': [{'offset': 0x31D, 'type': 'REL32',
                               'effective_destination': 0x00406790}],
            'actual_resolved_fields': [{
                'offset': 0x31D, 'type': 'REL32', 'addend': 0,
                'symbol': '?SetInterrupt@AnmVm@@QAEXF@Z',
                'destination': 0x00406790, 'effective_destination': 0x00406790,
                'symbol_section': 0, 'owner_local': False}],
        }

    def test_real_source_declaration_and_18_byte_callee(self):
        source = (ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertNotIn('PlayerMovementEffectVmView', source)
        self.assertIn('reinterpret_cast<AnmVm *>(\n'
                      '                PlayerMovementFocusEffect(player)->vms)', source)
        header = (ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('void SetInterrupt(short interrupt);', header)
        frontier.validate_player_movement_interrupt_abi(self.units)
        frontier.guard_player_movement_interrupt(self.fields())
        self.assertEqual(self.units['anm-vm-set-interrupt']['size'], 18)

    def test_numerically_equivalent_private_method_is_rejected(self):
        report = self.fields()
        report['actual_resolved_fields'][0]['symbol'] = (
            '?SetInterrupt@PlayerMovementEffectVmView@@QAEXF@Z')
        # The encoded call destination and every numeric property are unchanged.
        with self.assertRaises(RuntimeError):
            frontier.guard_player_movement_interrupt(report)

    def test_missing_duplicate_and_repointed_source_fields_are_rejected(self):
        for key, bad in (
            ('offset', 0x31E), ('type', 'DIR32'), ('addend', 1),
            ('destination', 0x00406791), ('effective_destination', 0x00406791),
            ('owner_local', True),
        ):
            report = self.fields()
            report['actual_resolved_fields'][0][key] = bad
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                frontier.guard_player_movement_interrupt(report)
        for field in ('target_fields', 'actual_resolved_fields'):
            for mode in ('missing', 'duplicate', 'absent'):
                report = self.fields()
                if mode == 'missing':
                    report[field] = []
                elif mode == 'duplicate':
                    report[field] *= 2
                else:
                    del report[field]
                with self.subTest(field=field, mode=mode), self.assertRaises(RuntimeError):
                    frontier.guard_player_movement_interrupt(report)
        for key, value in (('offset', 0x31E), ('type', 'DIR32'),
                           ('effective_destination', 0x00406791)):
            report = self.fields()
            report['target_fields'][0][key] = value
            with self.subTest(original=key), self.assertRaises(RuntimeError):
                frontier.guard_player_movement_interrupt(report)

    def test_wrong_callee_source_prototype_size_and_profile_are_rejected(self):
        for key, value in (
            ('source', 'src/PlayerMovement.cpp'),
            ('object', 'build/matching/Other.obj'),
            ('symbol', '?SetInterrupt@PlayerMovementEffectVmView@@QAEXF@Z'),
            ('target_address', 0x00406791), ('size', 8),
            ('relocations', [{'offset': 1}]), ('profile', ['/Ob0']),
        ):
            units = deepcopy(self.units)
            units['anm-vm-set-interrupt'][key] = value
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                frontier.validate_player_movement_interrupt_abi(units)
        for name in ('anm-vm-set-interrupt', 'player-on-update'):
            units = deepcopy(self.units)
            del units[name]
            with self.subTest(missing=name), self.assertRaises(RuntimeError):
                frontier.validate_player_movement_interrupt_abi(units)

    def test_exact_leaf_replay_uses_the_actual_separate_carrier(self):
        with (patch.object(frontier, 'build') as build,
              patch.object(frontier, 'canonical', side_effect=lambda name: {'unit': name})
              as canonical):
            result = frontier.replay_player_movement_interrupt_exact()
        self.assertEqual(result, [{'unit': 'anm-vm-set-interrupt'}])
        build.assert_called_once_with('anm-vm-set-interrupt')
        canonical.assert_called_once_with('anm-vm-set-interrupt')

    def test_bad_metadata_stops_before_build_or_comparison(self):
        units = deepcopy(self.units)
        units['anm-vm-set-interrupt']['size'] = 8
        with (patch.object(frontier.tomllib, 'load', return_value={'units': units}),
              patch.object(frontier, 'build') as build,
              patch.object(frontier, 'canonical') as canonical,
              self.assertRaises(RuntimeError)):
            frontier.replay_player_movement_interrupt_exact()
        build.assert_not_called()
        canonical.assert_not_called()


if __name__ == '__main__':
    unittest.main()
