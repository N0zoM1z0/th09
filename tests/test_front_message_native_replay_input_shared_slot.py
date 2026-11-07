"""The TH09 FrontMessage script consumes the shared third replay-input slot.

The exact original PE/COFF comparison proves the complete script bytes.
A changed COFF symbol/addend is allowed only when the original effective
0x4ACF34 address and exact IsHeld/WasPressed call targets are retained.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
GLOBAL='?g_ReplayInputStates@@3PAUReplayInputState@@A'

class FrontMessageNativeReplayInputSharedSlot(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as fp:
            cls.units=tomllib.load(fp)['units']

    def test_shared_third_native_input_storage_not_another_global(self):
        source=(ROOT/'src/FrontMessageUpdate.cpp').read_text()
        state=(ROOT/'src/ReplayInputState.hpp').read_text()
        cap=(ROOT/'src/ReplayNativeInputGateCapacity.hpp').read_text()
        self.assertNotIn('struct FrontMessageInputView',source)
        self.assertNotIn('g_FrontMessageInput',source)
        self.assertIn('#include "ReplayNativeInputGateCapacity.hpp"',source)
        self.assertIn('FrontMessageNativeReplayInputSlot',source)
        self.assertIn('extern ReplayInputState g_ReplayInputStates[3];',state)
        self.assertIn('ReplayInputStateSizeIs8E',state)
        self.assertIn('SharedInputStateIndex = StateCount - 1',cap)
        self.assertIn('StateCount == 3',source)
        self.assertIn('SharedInputStateIndex == 2',source)
        self.assertIn('== 0x11C',source)
        view='g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex]'
        self.assertEqual(source.count(view+'.IsHeld('),4)
        self.assertEqual(source.count(view+'.WasPressed('),1)
        self.assertIn('case 4:',source)
        self.assertIn('skippable1D6C',source)

    def test_original_operand_relocation_equivalence_and_method_leaf_targets(self):
        script=self.units['front-message-update']
        self.assertEqual((script['target_address'],script['size'],script['compare_size'],len(script['relocations'])),
                         (0x00416590,2852,2968,167))
        global_fields=[r for r in script['relocations'] if r['symbol']==GLOBAL]
        self.assertEqual(len(global_fields),5)
        self.assertEqual([r['offset'] for r in global_fields],
                         [0x3C,0x75E,0x773,0xA43,0xA8A])
        for r in global_fields:
            self.assertEqual(r['type'],'DIR32')
            self.assertEqual(r['target'],0x004ACE18)
            self.assertEqual(r.get('addend'),0x11C)
            self.assertEqual(r['target']+r['addend'],0x004ACF34)
        for method,unit,at,offsets in (
            ('IsHeld','replay-input-is-held',0x00415CB0,[0x41,0x763,0xA48,0xA8F]),
            ('WasPressed','replay-input-was-pressed',0x00415CD0,[0x778])):
            with self.subTest(method=method):
                leaf=self.units[unit]
                symbol=leaf['symbol']
                self.assertEqual(leaf['target_address'],at)
                self.assertEqual(leaf['size'],18)
                self.assertIn('@ReplayInputState@@',symbol)
                fields=[r for r in script['relocations'] if r['symbol']==symbol]
                self.assertEqual([r['offset'] for r in fields],offsets)
                self.assertTrue(all(r['type']=='REL32' and r['target']==at and r.get('addend',0)==0 for r in fields))
        independent=self.units['replay-manager-capture-frame-sync-state']
        self.assertTrue(any(r['symbol']==GLOBAL and r['target']==0x004ACE18
                            for r in independent['relocations']))
        self.assertEqual(len([r for r in script['relocations'] if r['symbol'].startswith('$')]),30)

if __name__=='__main__':unittest.main()
