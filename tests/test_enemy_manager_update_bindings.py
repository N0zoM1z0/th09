"""Target-independent fail-closed checks for the large Enemy update inspector."""
import importlib.util
from pathlib import Path
import struct
import sys
from types import ModuleType
from unittest import TestCase, main
from unittest.mock import patch

SOURCE = Path(__file__).resolve().parents[1] / 'scripts/inspect-enemy-manager-update.py'
with patch.dict(sys.modules, {'capstone': ModuleType('capstone')}), \
        patch.object(sys, 'path', [str(SOURCE.parent), *sys.path]):
    spec = importlib.util.spec_from_file_location('enemy_update_inspector_tests', SOURCE)
    inspector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(inspector)


class TargetPayload:
    def __init__(self, payload):
        self.payload = payload

    def pe_bytes_at(self, target, address, length):
        return self.payload


class EnemyUpdateBindingTests(TestCase):
    def row(self, **edits):
        base = {
            'symbol': 'checked_external', 'symbol_section': 0,
            'owner_section': 1, 'symbol_value': 0, 'owner_value': 0,
            'offset': 0x14, 'type': 'REL32', 'addend': 0,
        }
        base.update(edits)
        return base

    def test_timer_and_descriptor_calls_require_canonical_link_symbols(self):
        units, functions, registry = inspector.reference_index()
        cases = (
            ('?IsAfter@EnemyCoreTimerView@@QAEHH@Z', '??OZunTimer@@QAEIH@Z', 0x00403DE0),
            ('?Set@EnemyCoreTimerView@@QAEXH@Z', '??4ZunTimer@@QAEXH@Z', 0x00401500),
            ('??EEnemyCoreTimerView@@QAEXH@Z', '??EZunTimer@@QAEXH@Z', 0x00401510),
            ('?HasTicked@EnemyCoreTimerView@@QAEHH@Z', '?HasTickedEvery@ZunTimer@@QAEHH@Z', 0x00404920),
            ('??FEnemyCoreTimerView@@QAEXH@Z', '??FZunTimer@@QAEXH@Z', 0x00406670),
            ('?Current@EnemyCoreTimerView@@QAEHXZ', '??BZunTimer@@QAEHXZ', 0x00435F00),
            ('??0EnemyCoreBulletSpawnDescriptorView@@QAE@XZ', '??0BulletSpawnDescriptorCtorView@@QAE@XZ', 0x0040D500),
        )
        for legacy, canonical, expected in cases:
            with self.subTest(symbol=canonical):
                destination, proof = inspector.external_destination(
                    self.row(symbol=canonical), units, functions, registry)
                self.assertEqual(destination, expected)
                self.assertIn('canonical exact unit identity', proof)
                with self.assertRaisesRegex(ValueError, 'absent or ambiguous'):
                    inspector.external_destination(
                        self.row(symbol=legacy), units, functions, registry)

    def test_actual_etama_dispatch_uses_existing_exact_214_byte_descriptor(self):
        text = (SOURCE.parents[1] / 'src/EnemyManagerCore.inl').read_text()
        self.assertIn('#include "BulletManager.hpp"', text)
        self.assertIn('EtamaController *etama08;', text)
        self.assertIn('EnemyCoreBulletDescriptorMatchesEtamaFields', text)
        self.assertIn('sizeof(EnemyCoreBulletSpawnDescriptorView) == sizeof(BulletSpawnDescriptor)', text)
        self.assertIn('reinterpret_cast<BulletSpawnDescriptor *>(&descriptor)', text)
        self.assertNotIn('EnemyCoreEtamaControllerView', text)
        unit_name = 'bullet-pattern-primary'
        units, functions, registry = inspector.reference_index()
        method = units[unit_name]['symbol']
        self.assertEqual(method,
            '?SpawnBulletPatternPrimary@EtamaController@@QAEPAUBullet@@PAUBulletSpawnDescriptor@@@Z')
        self.assertEqual(units[unit_name]['target_address'], 0x004130F0)
        self.assertEqual(units[unit_name]['size'], 203)
        self.assertEqual(units['bullet-spawn-descriptor-constructor']['target_address'],
                         0x0040D500)
        self.assertEqual(units['bullet-spawn-descriptor-constructor']['size'], 28)
        target, provenance = inspector.external_destination(
            self.row(symbol=method), units, functions, registry)
        self.assertEqual(target, 0x004130F0)
        self.assertIn('canonical exact unit identity', provenance)
        # Synthetic receiver ABI is not backed by a physical C++ owner. It
        # may not continue to pass via a legacy fake-symbol alias.
        self.assertNotIn(
            '?SpawnBulletPatternPrimary@EnemyCoreEtamaControllerView@@QAEPAXPAUEnemyCoreBulletSpawnDescriptorView@@@Z',
            inspector.unit_aliases)
        with self.assertRaisesRegex(ValueError, 'absent or ambiguous'):
            inspector.external_destination(
                self.row(symbol='?SpawnBulletPatternPrimary@EnemyCoreEtamaControllerView@@QAEPAXPAUEnemyCoreBulletSpawnDescriptorView@@@Z'),
                units, functions, registry)

    def test_owner_local_label_uses_actual_coff_position(self):
        row = self.row(symbol='$Lowner', symbol_section=1,
                       symbol_value=0x240, owner_value=0x30, type='DIR32')
        value, address, reason = inspector.bind_field(
            row, {}, {}, {}, TargetPayload(b''), b'')
        self.assertEqual(address, inspector.BASE + 0x210)
        self.assertEqual(value, address)
        self.assertIn('actual same-section', reason)

    def test_owner_local_label_outside_physical_extent_is_rejected(self):
        row = self.row(symbol='$Lbad', symbol_section=1,
                       symbol_value=inspector.OWNER_SIZE, type='DIR32')
        with self.assertRaisesRegex(ValueError, 'leaves reviewed extent'):
            inspector.bind_field(row, {}, {}, {}, TargetPayload(b''), b'')

    def test_absent_or_conflicting_external_symbol_fails_closed(self):
        row = self.row()
        for registry in ({}, {'checked_external': [
                {'target': 0x411000}, {'target': 0x412000}]}):
            with self.assertRaisesRegex(ValueError, 'absent or ambiguous'):
                inspector.bind_field(row, {}, {}, registry, TargetPayload(b''), b'')

    def test_external_relative_call_uses_independent_destination(self):
        row = self.row()
        destination = 0x413000
        value, resolved, proof = inspector.bind_field(
            row, {}, {}, {'checked_external': [{'target': destination}]},
            TargetPayload(b''), b'')
        self.assertEqual(resolved, destination)
        self.assertEqual(value,
                         (destination - (inspector.BASE + row['offset'] + 4)) & 0xFFFFFFFF)
        self.assertIn('canonical exact unit identity', proof)

    def test_runecl_reference_is_source_present_without_claiming_callee_exactness(self):
        units, functions, registry = inspector.reference_index()
        destination, proof = inspector.external_destination(
            self.row(symbol=inspector.RUNECL_SYMBOL), units, functions, registry)
        self.assertEqual(destination, 0x004086C0)
        self.assertIn('source-present non-exact owner', proof)
        self.assertNotIn('canonical exact', proof)
        for old in ('?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z',
                    '?RunEcl@EnemySpawnEclManagerBoundary@@QAEHPAUEnemyView@@@Z',
                    '?RunEcl@EnemySpawnRecordEclManagerBoundary@@QAEHPAUEnemyView@@@Z'):
            with self.subTest(symbol=old), self.assertRaisesRegex(ValueError, 'absent or ambiguous'):
                inspector.external_destination(self.row(symbol=old), units, functions, registry)

    def test_native_runecl_call_must_resolve_to_the_actual_definition(self):
        call = self.row(symbol=inspector.RUNECL_SYMBOL, offset=0x397,
                        owner_section=5, symbol_section=8, symbol_value=0x10)
        definitions = [{'owner_section': 8, 'owner_value': 0x10} for _ in range(598)]
        report = inspector.require_runecl_definition([call], bytes(15564), definitions)
        self.assertTrue(report['same_object_definition'])
        for fields, raw, defined in (
                ([], bytes(15564), definitions),
                ([call, call], bytes(15564), definitions),
                ([self.row(**{**call, 'symbol_section': 0})], bytes(15564), definitions),
                ([self.row(**{**call, 'symbol_value': 0})], bytes(15564), definitions),
                ([self.row(**{**call, 'type': 'DIR32'})], bytes(15564), definitions),
                ([self.row(**{**call, 'addend': 4})], bytes(15564), definitions),
                ([self.row(**{**call, 'offset': 0x396})], bytes(15564), definitions),
                ([call], bytes(15563), definitions),
                ([call], bytes(15564), definitions[:-1]),
                ([call], bytes(15564), [dict(definitions[0], owner_value=0)] + definitions[1:])):
            with self.subTest(fields=fields[:1], size=len(raw)), self.assertRaises(ValueError):
                inspector.require_runecl_definition(fields, raw, defined)

    def test_literal_payload_is_verified_against_pe_bytes(self):
        row = self.row(symbol='__real@3f000000', type='DIR32')
        refs = {'__real@3f000000': [{'target': 0x490F54}]}
        payload = struct.pack('<I', 0x3F000000)
        _, destination, _ = inspector.bind_field(
            row, {}, {}, refs, TargetPayload(payload), b'')
        self.assertEqual(destination, 0x490F54)
        with self.assertRaisesRegex(ValueError, 'float literal'):
            inspector.bind_field(row, {}, {}, refs,
                                 TargetPayload(struct.pack('<I', 0x3F800000)), b'')

    def test_unreviewed_relocation_kind_rejected(self):
        row = self.row(type='REL16')
        with self.assertRaisesRegex(ValueError, 'relocation kind'):
            inspector.bind_field(
                row, {}, {}, {'checked_external': [{'target': 0x413000}]},
                TargetPayload(b''), b'')


if __name__ == '__main__':
    main()
