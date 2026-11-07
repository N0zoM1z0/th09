"""Type18/24's real Player/VM/angle owners retain original PE call roles.

These source/ABI checks accompany the complete original-bound 1,436-byte
COFF comparison and must never substitute for that whole-owner oracle.
"""
from collections import Counter
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
GM = '?g_GameManager@@3UAsciiGameManagerView@@A'
PLAYER = '?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z'
ROTATE = '?SetZRotation@PlayerAnmVmView@@QAEXM@Z'
INTERRUPT = '?SetInterrupt@AnmVm@@QAEXF@Z'
ANGLE = '?AngleToPoint@PlayerAngleView@@QAEMPAUPlayerAngleFloat3View@@@Z'

class ExAttackType18NativeOwnershipTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT/'src/ExAttackUpdateType18Type24.cpp').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_true_native_player_header_vm_and_angle_projection(self):
        s=self.source
        self.assertIn('extern AsciiGameManagerView g_GameManager;',s)
        self.assertIn('PlayerAngleView *',s)
        self.assertIn('PlayerAngleFloat3View *',s)
        self.assertIn('PlayerAnmVmView *',s)
        self.assertIn('AnmVm, pos2) == 0x288',s)
        self.assertIn('ExAttackType1824NativePlayerFields',s)
        self.assertIn('ExAttackType1824NativeAngleStorage',s)
        self.assertIn('offsetof(PlayerAngleView, position1B88)',s)
        self.assertIn('sizeof(PlayerAngleFloat3View) == sizeof(PlayerPositionView)',s)
        self.assertEqual(s.count('->header24.anmFile98'),4)
        self.assertIn('->collisionQuery36C.AppendCircleRecord(',s)
        for old in ('struct ExAttackType1824PlayerView',
                    'struct ExAttackType1824VmView',
                    'struct ExAttackType1824VmPositionView',
                    'struct ExAttackType1824GameManagerView',
                    '->anmFileBC'):
            self.assertNotIn(old,s)
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('PlayerHeaderNativeAnmAt98',player)
        self.assertIn('PlayerLifecycleCollisionQueryAt36C',player)

    def test_whole_owner_fields_and_independent_callees(self):
        owner=self.units['exattack-type18-24-update']
        self.assertEqual(owner['size'],1436)
        self.assertEqual(len(owner['relocations']),44)
        counts=Counter()
        sites={GM:[],PLAYER:[],ROTATE:[],INTERRUPT:[],ANGLE:[]}
        expected_targets={GM:0x004A7D90,PLAYER:0x0041DFF0,
                          ROTATE:0x0040F840,INTERRUPT:0x00406790,
                          ANGLE:0x0041BB70}
        for field in owner['relocations']:
            sym=field['symbol']
            if sym in sites:
                sites[sym].append(field['offset'])
                self.assertEqual(field['target'],expected_targets[sym])
                self.assertEqual(field['type'],'DIR32' if sym==GM else 'REL32')
                # Seven DIR32 GameManager fields use the original +4 addend.
                if sym==GM:
                    self.assertEqual(field.get('addend',4),4)
                else:
                    self.assertEqual(field.get('addend',0),0)
        self.assertEqual(sites[GM],[0x245,0x347,0x3BD,0x3DA,0x3FC,0x41E,0x463])
        self.assertEqual(sites[PLAYER],[0x24B])
        self.assertEqual(sites[ROTATE],[0x151,0x18C,0x1C7,0x205])
        self.assertEqual(sites[INTERRUPT],[0x266,0x276,0x286,0x296])
        self.assertEqual(sites[ANGLE],[0x469])
        for unit,addr,size in (('player-angle-to-point',0x0041BB70,101),
                               ('player-anm-vm-set-z-rotation',0x0040F840,20),
                               ('anm-vm-set-interrupt',0x00406790,18)):
            with self.subTest(provider=unit):
                self.assertEqual(self.units[unit]['target_address'],addr)
                self.assertEqual(self.units[unit]['size'],size)

if __name__=='__main__':
    unittest.main()
