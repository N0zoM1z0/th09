"""Native code and definition provenance remain independent of a good caller."""
import copy
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'pause_native_audio', ROOT/'scripts/probe-pause-native-audio.py')
PROBE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PROBE)


class PauseNativeAudioTests(unittest.TestCase):
    def test_control_context_rejects_changed_animation_count(self):
        source = (ROOT/'src/AsciiManagerMenu.cpp').read_text()
        changed = source.replace(
            'for (i = 0; i < 7; i++) {\n        g_AnmManager->ExecuteScript',
            'for (i = 0; i < 8; i++) {\n        g_AnmManager->ExecuteScript')
        for case in ('embedded-footer', 'animate-cursor'):
            with self.subTest(case=case), self.assertRaises(ValueError):
                PROBE.control_context(changed, case)
        with self.assertRaises(ValueError):
            PROBE.control_context(source, 'unknown-control')

    def setUp(self):
        self.raw = b'\xe8\0\0\0\0\xc3'
        self.unit = dict(size=6, target_address=0x420000, relocations=[
            dict(offset=1, type='REL32', symbol='?Native@@YAXXZ', target=0x430000)])
        self.fields = [dict(offset=1, type='REL32', symbol='?Native@@YAXXZ',
                            addend=0, symbol_section=4, symbol_value=0, owner_section=2)]
        self.target = b'\xe8'+struct.pack('<i', 0x430000-0x420005)+b'\xc3'

    def test_native_bytes_cannot_be_hidden_by_matching_caller(self):
        self.assertTrue(PROBE.compare_native(
            self.raw, self.fields, self.unit, self.target)['complete_equal'])
        for raw in (self.raw[:-1], self.raw+b'\x90', b'\xe9'+self.raw[1:]):
            with self.subTest(raw=raw), self.assertRaises(ValueError):
                PROBE.compare_native(raw, self.fields, self.unit, self.target)

    def test_actual_field_and_original_operand_are_independent(self):
        for changes in ({'symbol': '?Other@@YAXXZ'}, {'addend': 4},
                        {'offset': 2}, {'type': 'DIR32'}):
            fields = copy.deepcopy(self.fields)
            fields[0].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                PROBE.compare_native(self.raw, fields, self.unit, self.target)
        redirected = copy.deepcopy(self.unit)
        redirected['relocations'][0]['target'] += 4
        with self.assertRaisesRegex(ValueError, 'original field'):
            PROBE.compare_native(self.raw, self.fields, redirected, self.target)
        with self.assertRaises(ValueError):
            PROBE.compare_native(self.raw, [], self.unit, self.target)

    def test_only_generated_namespace_token_may_differ(self):
        fields, unit = copy.deepcopy(self.fields), copy.deepcopy(self.unit)
        old = '?StopPlayback@SupervisorAudioMidiOutputView@?A0x123abc@@QAEXXZ'
        unit['relocations'][0]['symbol'] = old
        fields[0]['symbol'] = old.replace('123abc', '456def')
        PROBE.compare_native(self.raw, fields, unit, self.target)
        for changed in ('?Other@SupervisorAudioMidiOutputView@?A0x456def@@QAEXXZ',
                        '?StopPlayback@OtherView@?A0x456def@@QAEXXZ'):
            fields[0]['symbol'] = changed
            with self.subTest(changed=changed), self.assertRaises(ValueError):
                PROBE.compare_native(self.raw, fields, unit, self.target)

    def test_opaque_or_inconsistent_definitions_cannot_claim_visibility(self):
        symbol = self.fields[0]['symbol']
        self.assertEqual(PROBE.definition_references(self.fields, symbol, 1)['calls'], 1)
        for changes in ({'symbol_section': 0}, {'symbol_section': 2},
                        {'symbol_value': 1}, {'addend': 4}):
            fields = copy.deepcopy(self.fields)
            fields[0].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                PROBE.definition_references(fields, symbol, 1)
        inconsistent = copy.deepcopy(self.fields)*2
        inconsistent[1] = dict(inconsistent[1], symbol_section=5)
        with self.assertRaises(ValueError):
            PROBE.definition_references(inconsistent, symbol, 2)


if __name__ == '__main__':
    unittest.main()
