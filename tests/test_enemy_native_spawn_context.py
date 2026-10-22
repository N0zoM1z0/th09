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
    def test_scalar_copies_keep_real_widths_and_later_draw_read(self):
        source = (ROOT/'src/EnemyManagerCore.inl').read_text()
        result = probe.scalar_copy_context(source, 'standalone-scalar-all')
        for declaration in ('unsigned char drawGroup;', 'int specialAttackThreshold;',
                            'EnemyCoreSideStateView *spawnSide;', 'int characterIndex;'):
            self.assertIn(declaration, result)
        self.assertEqual(result.count('memcpy(')-source.count('memcpy('), 4)
        self.assertEqual(result.count('&manager->drawGroupHeads2AC410[enemy->drawGroup3387]'), 1)
        self.assertNotIn('volatile', result)
        self.assertIn('SpawnBulletPatternPrimary(', result)
        for case in probe.SCALAR_COPY_CASES:
            self.assertEqual(probe.CASE_OWNERS[case], probe.CASE_OWNERS['standalone'])
            self.assertEqual(probe.standalone_source(case, ''),
                             probe.standalone_source('standalone', ''))
            with self.assertRaises(ValueError):
                probe.standalone_source(case, 'unexpected_definition')

    def test_scalar_source_drift_or_duplicate_read_is_rejected(self):
        source = (ROOT/'src/EnemyManagerCore.inl').read_text()
        for changed in (source+source, source.replace('specialAttackThreshold2AC44C', 'changedField')):
            with self.assertRaises(ValueError):
                probe.scalar_copy_context(changed, 'standalone-scalar-threshold')
        with self.assertRaises(ValueError):
            probe.scalar_copy_context(source, 'unknown')

    def test_primary_body_requires_complete_unique_source_boundaries(self):
        source = (ROOT/'src/BulletManager.cpp').read_bytes().replace(b'\r\n', b'\n')
        selected = probe.bullet_primary_body(source)
        self.assertIn('Bullet *EtamaController::SpawnBulletPatternPrimary(', selected)
        self.assertNotIn('Bullet *EtamaController::SpawnBulletPatternSecondary(', selected)
        self.assertIn('struct BulletCancelCollisionView', selected)
        self.assertIn('struct BulletPlayerView', selected)
        for marker in (b'struct BulletPlayerView\n',
                       b'Bullet *EtamaController::SpawnBulletPatternSecondary('):
            for changed in (source.replace(marker, b'changed marker'), source+marker):
                with self.subTest(marker=marker), self.assertRaises(ValueError):
                    probe.bullet_primary_body(changed)
        owners = probe.CASE_OWNERS['standalone-bullet-primary']
        self.assertEqual(owners, probe.CASE_OWNERS['standalone']+('bullet-pattern-primary',))

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
        self.assertEqual(probe.native_bodies('standalone', {}), '')

    def test_standalone_keeps_all_five_real_native_contracts(self):
        expected = {
            'enemy-collision-append-box': (0x410250, 82, 2),
            'float3-divide': (0x40F5A0, 56, 2),
            'player-collision-append-box': (0x40F8E0, 205, 1),
            'enemy-attached-effect-update': (0x40F4C0, 151, 1),
            'zun-timer-greater': (0x403DE0, 20, 4),
        }
        self.assertEqual(set(probe.CASE_OWNERS['standalone']), set(expected))
        for name, (address, size, references) in expected.items():
            with self.subTest(name=name):
                actual = probe.NATIVE_OWNERS[name]
                self.assertEqual((actual[0], actual[1], actual[3]),
                                 (address, size, references))

    def test_periodic_standalone_requires_the_extra_actual_definition(self):
        base = probe.CASE_OWNERS['standalone']
        periodic = 'zun-timer-has-ticked-every'
        for case in ('standalone-periodic-first', 'standalone-periodic-last'):
            self.assertEqual(probe.CASE_OWNERS[case], base+(periodic,))
            self.assertEqual(probe.native_bodies(case, {
                'ZunTimerHasTickedEvery.cpp': b'actual_periodic_body\n'}),
                'actual_periodic_body\n\n')
            with self.assertRaises(KeyError):
                probe.native_bodies(case, {})
        self.assertEqual(probe.NATIVE_OWNERS[periodic],
                         (0x404920, 33, '?HasTickedEvery@ZunTimer@@QAEHH@Z', 1))

    def test_standalone_body_position_keeps_the_real_declaration_chain(self):
        prefix = '#include "EclManager.hpp"\n#include "EclRunState.inl"\n'
        owner = '#include "EnemyManagerCore.inl"\n'
        self.assertEqual(probe.standalone_source('standalone', ''), prefix+owner)
        self.assertEqual(probe.standalone_source('standalone-periodic-first', 'actual_body'),
                         prefix+'actual_body\n'+owner)
        self.assertEqual(probe.standalone_source('standalone-periodic-last', 'actual_body'),
                         prefix+owner+'actual_body\n')
        for case, body in (('spawn', 'actual_body'), ('standalone', 'unexpected'),
                           ('standalone-periodic-first', ''), ('standalone-periodic-last', '')):
            with self.subTest(case=case, body=body), self.assertRaises(ValueError):
                probe.standalone_source(case, body)

    def test_pointer_context_requires_the_actual_folded_definition(self):
        case = 'standalone-pointer-view'
        name = 'float3-pointer-identity-view'
        self.assertEqual(probe.CASE_OWNERS[case],
                         probe.CASE_OWNERS['standalone']+(name,))
        self.assertEqual(probe.NATIVE_OWNERS[name],
                         (0x4343D0, 3, '??BFloat3@@QAEPAMXZ', 1))
        actual = (ROOT/'src/VectorPointerViews.cpp').read_bytes()
        bodies = probe.native_bodies(case, {'VectorPointerViews.cpp': actual})
        self.assertEqual(bodies, actual.decode()+'\n')
        carrier = probe.standalone_source(case, bodies)
        self.assertLess(carrier.index('Float3::operator float *()'),
                        carrier.index('#include "EnemyManagerCore.inl"'))
        with self.assertRaises(KeyError):
            probe.native_bodies(case, {})
        with self.assertRaises(ValueError):
            probe.standalone_source(case, '')

    def test_float3_selection_rejects_ambiguous_or_reversed_boundaries(self):
        constructor = 'Float3::Float3(float x) { constructor_body; }\n'
        addition = 'Float3 Float3::operator+(int x) { addition_body; }\n'
        subtraction = 'Float3 Float3::operator-(int x) { unselected_body; }\n'
        inputs = {'ZunTimerHasTickedEvery.cpp': b'periodic_body\n',
                  'Float3Core.inl': (constructor+addition+subtraction).encode()}
        for case in ('standalone-periodic-add', 'standalone-periodic-add-constructor'):
            body = probe.native_bodies(case, inputs)
            self.assertIn('addition_body', body)
            self.assertNotIn('unselected_body', body)
            self.assertEqual('constructor_body' in body, case.endswith('-constructor'))
            carrier = probe.standalone_source(case, body)
            self.assertLess(carrier.index('addition_body'),
                            carrier.index('#include "EnemyManagerCore.inl"'))
            for bad in (addition+subtraction, constructor+addition+addition+subtraction,
                        constructor+subtraction+addition):
                with self.subTest(case=case, bad=bad), self.assertRaises(ValueError):
                    probe.native_bodies(case, dict(inputs, **{'Float3Core.inl': bad.encode()}))

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
