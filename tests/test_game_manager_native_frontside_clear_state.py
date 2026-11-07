"""Original GameManager OnUpdate calls true FrontSide::ClearState, not a fake Trigger.

The callsite's exact 1230-byte contract is checked separately against PE/COFF.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeFrontSideClearState(unittest.TestCase):
    def test_concrete_native_side_and_method(self):
        s=(ROOT/'src/GameManagerSetup.cpp').read_text()
        native=(ROOT/'src/GameManagerSideState.hpp').read_text()
        self.assertNotIn('struct SideAuxSetupView',s)
        self.assertNotIn('->Trigger(1)',s)
        for side in (0,1):
            self.assertIn(f'globalManager.sides[{side}].frontSide18->ClearState(1);',s)
        self.assertIn('FrontSide *frontSide18;',native)
        self.assertIn('offsetof(AsciiGameManagerSideView, frontSide18) == 0x18',native)

    def test_call_fields_bind_independent_exact_original_callee(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        callee=units['front-side-clear-state']
        update=units['game-manager-on-update']
        self.assertEqual((callee['size'],callee['target_address']),(56,0x0041A380))
        self.assertEqual(update['size'],1230)
        self.assertEqual(len(update['relocations']),87)
        by_offset={r['offset']:r for r in update['relocations']}
        for off in (0x3BD,0x3CA):
            r=by_offset[off]
            self.assertEqual(r['symbol'],callee['symbol'])
            self.assertEqual(r['target'],callee['target_address'])
            self.assertEqual(r['type'],'REL32')

if __name__=='__main__':unittest.main()
