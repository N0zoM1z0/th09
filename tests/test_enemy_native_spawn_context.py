"""A matching caller cannot waive missing or corrupted native definitions."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'enemy_native_context_test', ROOT/'scripts/probe-enemy-native-spawn-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def definition():
    # Synthetic CALL/RET fixture, not bytes from the private executable.
    raw = bytearray(b'\xe8\x00\x00\x00\x00\xc3')
    field = dict(offset=1, type='REL32', symbol='synthetic-child', addend=0)
    unit = dict(size=6, target_address=0x410000, relocations=[
        dict(offset=1, type='REL32', symbol='synthetic-child', target=0x401000)])
    target = bytearray(raw)
    struct.pack_into('<I', target, 1, (0x401000-0x410005) & 0xffffffff)
    return raw, [field], unit, target


class EnemyNativeSpawnContext(unittest.TestCase):
    def test_selected_definition_recipes_preserve_original_two_contexts(self):
        # Distinct synthetic bodies make accidental exposure observable.
        spawn = 'Effect *EffectManager::SpawnEffect() { spawn_body; }\n'
        source = (spawn+'Effect *EffectManager::SpawnEffectWithVelocity() {}\n').encode()
        periodic = b'periodic_body\n'
        constructor = b'constructor_body\n'
        inputs = {'EffectManager.cpp':source, 'ZunTimerHasTickedEvery.cpp':periodic,
                  'BulletSpawnDescriptorCtor.cpp':constructor}
        old_spawn = '#include "EffectManager.hpp"\n'+spawn
        self.assertEqual(probe.native_bodies('spawn', inputs), old_spawn)
        self.assertEqual(probe.native_bodies('spawn-periodic', inputs),
                         periodic.decode()+'\n'+old_spawn)
        self.assertEqual(probe.native_bodies('constructor-periodic', inputs),
                         periodic.decode()+'\n'+constructor.decode()+'\n')
        self.assertEqual(probe.native_bodies('spawn-constructor-periodic', inputs),
                         periodic.decode()+'\n'+constructor.decode()+'\n'+old_spawn)
        inputs['EffectManager.cpp'] = source+source
        with self.assertRaises(ValueError):
            probe.native_bodies('spawn', inputs)
        # Unselected source is not silently inserted or required to be valid.
        self.assertNotIn('spawn_body', probe.native_bodies('constructor-periodic', inputs))

    def test_complete_native_code_and_all_actual_fields_required(self):
        raw, fields, unit, target = definition()
        result = probe.compare_native_definition(raw, fields, unit, target)
        self.assertEqual((result['bytes'], result['fields']), (6, 1))
        self.assertTrue(result['complete_bytes_equal'])
        self.assertEqual(result['linked_sha256'], result['target_sha256'])

    def test_same_caller_cannot_mask_native_body_or_extent_corruption(self):
        raw, fields, unit, target = definition()
        for data in (raw[:-1], raw+b'\x90', raw[:-1]+b'\xcc'):
            with self.subTest(data=data.hex()), self.assertRaises(ValueError):
                probe.compare_native_definition(data, fields, unit, target)
        with self.assertRaises(ValueError):
            probe.compare_native_definition(raw, fields, unit, target[:-1])

    def test_fields_and_original_operand_cannot_be_invented(self):
        raw, fields, unit, target = definition()
        for change in (dict(symbol='unrelated-child'), dict(type='DIR32'),
                       dict(addend=4), dict(offset=2), dict(offset=True)):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.compare_native_definition(raw, [{**fields[0], **change}], unit, target)
        for candidate in ([], fields+fields):
            with self.assertRaises(ValueError):
                probe.compare_native_definition(raw, candidate, unit, target)
        target[1] ^= 1
        with self.assertRaisesRegex(ValueError, 'original operand'):
            probe.compare_native_definition(raw, fields, unit, target)

    def test_actual_same_object_definition_references_required(self):
        row = dict(symbol='native-spawn', type='REL32', addend=0,
                   symbol_section=7, symbol_value=0, owner_section=1)
        result = probe.require_caller_definition([row]*4, 'native-spawn', 4)
        self.assertEqual(result, dict(calls=4, section=7, symbol_value=0))
        for change in (dict(symbol_section=0), dict(symbol_section=True),
                       dict(symbol_section=1), dict(symbol_value=1),
                       dict(type='DIR32'), dict(addend=4)):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.require_caller_definition([row]*3+[{**row, **change}], 'native-spawn', 4)
        for fields in ([row]*3, [row]*5, [row]*3+[{**row, 'symbol_section':8}]):
            with self.assertRaises(ValueError):
                probe.require_caller_definition(fields, 'native-spawn', 4)

    def test_target_or_source_rejection_does_not_leave_stale_success(self):
        with tempfile.TemporaryDirectory(prefix='native-report-test-', dir=ROOT/'.analysis') as td:
            output = Path(td)
            report = output/'spawn.json'
            report.write_text(json.dumps(dict(state='complete-diagnostic')))
            with patch.object(probe.sys, 'argv', ['probe', '--output', str(output)]), \
                    patch.object(probe, 'run', side_effect=ValueError('wrong target')), \
                    contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(probe.main(), 2)
            self.assertEqual(json.loads(report.read_text())['state'], 'incomplete')
