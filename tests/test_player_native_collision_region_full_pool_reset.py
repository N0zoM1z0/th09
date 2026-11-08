"""Exact 512 native collision regions share their real Reset and allocator.

Strict original PE/COFF compares protect the whole 23-byte Reset, 110-byte
allocator and 572-byte initializer including its six local table fields.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALLEE='?Reset@PlayerCollisionRegionCreateRegionView@@QAEHXZ'

class PlayerNativeCollisionRegionFullPoolReset(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_full_512_record_real_player_pool_with_constructor_free_regions(self):
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        header=(ROOT/'src/PlayerCollisionRegionCreate.hpp').read_text()
        source=(ROOT/'src/PlayerAddedState.cpp').read_text()
        self.assertIn('PlayerCollisionRegionCreateRegionView collisionRegions28BC[512];',native)
        self.assertIn('PlayerLifecycleNativeFullRegionPool',native)
        self.assertNotIn('unsigned char unknown28BC[0xB0BC - 0x28BC];',native)
        self.assertIn('offsetof(PlayerLifecycleView, collisionRegions28BC) == 0x28BC',native)
        self.assertIn('sizeof(((PlayerLifecycleView *)0)->collisionRegions28BC) == 512 * 0x44',native)
        self.assertIn('PlayerCollisionRegionCreateRegionView collisionRegionB0BC;',native)
        self.assertIn('PlayerCollisionRegionCreateRegionView *activeB100[514];',native)
        self.assertIn('PlayerCollisionRegionCreateRegionView *releasedB908[514];',native)
        self.assertIn('PlayerCollisionRegionCreateRegionView embeddedCollisionB0BC;',source)
        self.assertNotIn('PlayerAddedCollisionRegionView',source)
        self.assertIn('struct PlayerCollisionRegionCreateRegionView',header)
        self.assertIn('int interval34;',header)
        self.assertIn('int Reset();',header)
        self.assertIn('PlayerCreateRegionSizeIs44',header)

    def test_exact_reset_and_full_initialize_use_native_pool_members(self):
        src=(ROOT/'src/PlayerAddedState.cpp').read_text()
        self.assertIn('int PlayerCollisionRegionCreateRegionView::Reset()',src)
        self.assertIn('memset(this, 0, sizeof(*this));',src)
        self.assertIn('this->interval34 = 1;',src)
        self.assertIn('sizeof(this->collisionRegions28BC) /',src)
        self.assertIn('sizeof(this->collisionRegions28BC[0])',src)
        self.assertIn('this->collisionRegions28BC[i].Reset();',src)
        self.assertIn('this->releasedB908[i] = &this->collisionRegions28BC[i];',src)
        self.assertIn('this->activeB100[i] = NULL;',src)
        self.assertIn('this->collisionRegionActiveCountC110 = 0;',src)
        self.assertIn('this->collisionRegionReleasedCountC114 = 512;',src)
        self.assertNotIn('player->collisionRegions28BC[i].Reset();',src)
        self.assertIn('this->ownerState30410.Reset();',src)
        self.assertIn('sizeof(PlayerAddedStateLayout) == 0x30F70',src)

    def test_original_reset_and_two_independent_exact_callers_share_target_va(self):
        leaf=self.units['player-added-collision-reset']
        self.assertEqual((leaf['target_address'],leaf['size'],len(leaf['relocations'])),
                         (0x0041BDA0,23,0))
        self.assertEqual(leaf['symbol'],CALLEE)
        for unit,offset,size in (('player-initialize-added-state',0x5A,552),
                                 ('player-allocate-collision-region',0x46,110)):
            with self.subTest(unit=unit):
                u=self.units[unit]
                self.assertEqual(u['size'],size)
                refs=[r for r in u['relocations'] if r['symbol']==CALLEE]
                self.assertEqual([(r['offset'],r['type'],r['target']) for r in refs],
                                 [(offset,'REL32',0x0041BDA0)])
        init=self.units['player-initialize-added-state']
        self.assertEqual((init['compare_size'],len(init['relocations'])),(572,25))
        self.assertEqual(len([r for r in init['relocations'] if r['symbol'].startswith('$L')]),6)
        self.assertIn('PlayerCollisionRegionCreateRegionView::Reset',(ROOT/'config/functions.csv').read_text())
        self.assertNotIn('PlayerAddedCollisionRegionView::Reset',(ROOT/'config/functions.csv').read_text())

if __name__=='__main__':unittest.main()
