"""Original Player main and shot VM owners share actual AnmVm storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class PlayerNativeAnmVmOwner(unittest.TestCase):
    def test_source_vm_is_zero_extra_storage_abi_receiver(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        native=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        source=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertIn('struct PlayerAnmVmView : AnmVm',header)
        self.assertIn('PlayerAnmVmSizeIs2A4',header)
        self.assertIn('sizeof(PlayerAnmVmView) == sizeof(AnmVm)',header)
        self.assertNotIn('struct PlayerAnmVmView\n{',header)
        for field,off in (('color1Script','0x1F0'),('type','0x1FC'),
                          ('pos','0x208'),('loadedSprite','0x224'),('pos2','0x288')):
            with self.subTest(field=field):
                self.assertIn('offsetof(PlayerAnmVmView, '+field+') == '+off,header)
        self.assertIn('AnmVmScriptColor color1Script;',native)
        self.assertIn('PlayerAnmVmView mainVm;',header)
        self.assertIn('PlayerAnmVmView vm;',header)
        self.assertIn('player->mainVm.color1',source)
        self.assertIn('shot->vm.color1Script.r',source)
        self.assertIn('shot->vm.pos.x',source)
        self.assertIn('shot->vm.loadedSprite->widthPx',source)
        self.assertNotIn('shot->vm.color1F0',source)
        self.assertNotIn('shot->vm.position208',source)

    def test_constructor_and_rotation_physical_calls_remain_target_backed(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        ctor=u['anm-vm-constructor']
        self.assertEqual((ctor['target_address'],ctor['size']),(0x403A10,45))
        player=u['player-constructor']
        self.assertTrue(any(r['symbol']=='??0PlayerAnmVmView@@QAE@XZ'
                            and r['target']==0x403A10 for r in player['relocations']))
        rot=u['player-anm-vm-set-z-rotation']
        self.assertEqual((rot['target_address'],rot['size'],rot['symbol']),
                         (0x40F840,20,'?SetZRotation@PlayerAnmVmView@@QAEXM@Z'))
        self.assertEqual(u['player-update-shots']['size'],251)


if __name__=='__main__':unittest.main()
