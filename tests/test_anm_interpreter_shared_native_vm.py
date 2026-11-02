"""Original-pinned shared ANM VM storage for the full script interpreter.

Pure layout and source identity guards; execute and helper original-byte
equality is separately proven with pinned source/profile/PE COFF replays.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeAnmInterpreterVm(unittest.TestCase):
    def test_one_vm_class_across_interpreter_runtime_and_lifecycle(self):
        shared = (ROOT/'src/AnmVmLifecycle.hpp').read_text()
        script = (ROOT/'src/AnmManagerExecuteScript.cpp').read_text()
        runtime = (ROOT/'src/AnmManagerRuntime.cpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"', script)
        self.assertIn('#include "AnmVmLifecycle.hpp"', runtime)
        self.assertIn('struct AnmVm : AnmVmBase', shared)
        self.assertNotIn('struct AnmVm {', script)
        self.assertNotIn('struct AnmVm\n{', runtime)
        for term in ('AnmVmLifecycleSizeIs2A4', 'AnmVmBaseSizeIs228',
                     'AnmVmScriptScalarsAt100', 'AnmVmScriptTailAt294',
                     'AnmVmScriptColorSizeIs4', 'AnmVmScriptMatrixSizeIs40'):
            with self.subTest(term=term):
                self.assertIn(term, shared)

    def test_script_fields_have_independently_observed_physical_offsets(self):
        shared = (ROOT/'src/AnmVmLifecycle.hpp').read_text()
        exec_source = (ROOT/'src/AnmManagerExecuteScript.cpp').read_text()
        for obj,field,offset in (
            ('AnmVmBase','intVar0','0x100'), ('AnmVmBase','uvScrollVel','0x128'),
            ('AnmVmBase','matrix1','0x130'),('AnmVmBase','color1','0x1F0'),
            ('AnmVmBase','color2','0x1F4'),
            ('AnmVm','interruptReturnInstruction','0x234'),
            ('AnmVm','posInitial','0x238'),('AnmVm','color1Initial','0x278'),
            ('AnmVm','pos2','0x288'),('AnmVm','timeOfLastSpriteSet','0x294'),
        ):
            with self.subTest(field=field):
                self.assertIn('offsetof('+obj+', '+field+') == '+offset, shared)
        self.assertIn('AnmVmScriptColor color1Initial, color1Final;',shared)
        self.assertIn('AnmVmScriptColor color2Initial, color2Final;',shared)
        self.assertIn('color1Script.a',exec_source)
        self.assertIn('color2Script.r',exec_source)
        self.assertIn('vm->disableZWrite = instruction->intArgs[0];',exec_source)

    def test_original_full_interpreter_and_variable_helper_manifests(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual(units['anm-manager-execute-script']['target_address'],0x00436F30)
        self.assertEqual(units['anm-manager-execute-script']['size'],9358)
        self.assertEqual(units['anm-manager-execute-script']['compare_size'],9776)
        self.assertEqual(len(units['anm-manager-execute-script']['relocations']),368)
        expected = {
            'anm-vm-get-int-var': (144,184,15),
            'anm-vm-get-float-var-ptr':(116,132,6),
            'anm-vm-get-int-var-ptr':(106,148,11),
            'anm-vm-get-float-var':(203,256,21),
        }
        for name,(size,compare,relocs) in expected.items():
            with self.subTest(name=name):
                unit=units[name]
                self.assertEqual(unit['source'],'src/AnmManagerExecuteScript.cpp')
                self.assertEqual((unit['size'],unit['compare_size'],
                                  len(unit['relocations'])),(size,compare,relocs))


if __name__ == '__main__':
    unittest.main()
