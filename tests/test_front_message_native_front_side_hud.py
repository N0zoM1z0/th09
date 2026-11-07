"""FrontMessage's complete script and HUD LoadScript67 share real FrontSide.

Full PE/COFF original-bound comparisons establish exactness; source guards
protect the native side HUD pointer, 0xA678 ANM allocation and callee VA.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
LOAD='?LoadScript67@FrontSide@@QAEXXZ'

class FrontMessageNativeFrontSideHud(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_no_duplicate_hud_prefix_or_anm_resource_class(self):
        msg=(ROOT/'src/FrontMessageUpdate.cpp').read_text()
        hud=(ROOT/'src/FrontSide.hpp').read_text()
        self.assertIn('#include "FrontSide.hpp"',msg)
        self.assertNotIn('struct FrontMessageFrontSideView',msg)
        self.assertNotIn('struct FrontMessageFrontSideAuxView',msg)
        self.assertIn('FrontSide *frontSide18;', (ROOT/'src/GameManagerSideState.hpp').read_text())
        self.assertIn('FrontMessageNativeGameManagerOwners',msg)
        self.assertIn('sizeof(FrontSideAuxView) == 0x558',msg)
        self.assertIn('offsetof(FrontSide, auxA678) == 0xA678',msg)
        self.assertIn('offsetof(AsciiGameManagerSideView, frontSide18) == 0x18',msg)
        self.assertIn('FrontSideAuxView auxA678;',hud)
        self.assertIn('AnmLoaded *sideAnm00;',hud)
        self.assertIn('AnmLoaded *optionalAnm04;',hud)
        self.assertIn('void LoadScript67();',hud)
        self.assertEqual(msg.count('->LoadScript67();'),2)
        self.assertIn('PlayerLifecycleView *player04;', (ROOT/'src/GameManagerSideState.hpp').read_text())

    def test_complete_original_message_and_two_native_frontside_call_sites(self):
        msg=self.units['front-message-update']
        leaf=self.units['front-side-load-script-67']
        self.assertEqual((msg['size'],msg['compare_size'],len(msg['relocations'])),
                         (2852,2968,167))
        self.assertEqual((leaf['size'],leaf['target_address'],leaf['symbol']),
                         (21,0x0041A210,LOAD))
        rows=[r for r in msg['relocations'] if r['symbol']==LOAD]
        self.assertEqual([(r['offset'],r['type'],r['target'],r.get('addend',0))
                          for r in rows],
                         [(0x905,'REL32',0x0041A210,0),
                          (0x910,'REL32',0x0041A210,0)])
        self.assertEqual(len([r for r in msg['relocations'] if r['symbol'].startswith('$')]),30)
        self.assertTrue(any(r['symbol']=='?SetUpdateState@PlayerLifecycleView@@QAEXH@Z'
                            and r['target']==0x00403C20 for r in msg['relocations']))

if __name__=='__main__':
    unittest.main()
