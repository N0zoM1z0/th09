"""Six exact TH09 ExAttack callbacks share genuine Player/GameManager owners.

Pinned VC7.1 original-bound function comparisons are the byte oracle;
this check protects the source-facing types and original linked destinations.
"""
from pathlib import Path
from collections import Counter
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
UNITS=(('exattack-type01-update','ExAttackUpdateType01.cpp'),
       ('exattack-type4-update','ExAttackUpdateType4.cpp'),
       ('exattack-type13-update','ExAttackUpdateType13.cpp'),
       ('exattack-type14-22-update','ExAttackUpdateType14Type22.cpp'),
       ('exattack-type16-23-update','ExAttackUpdateType16Type23.cpp'),
       ('exattack-type17-update','ExAttackUpdateType17.cpp'))
GM='?g_GameManager@@3UAsciiGameManagerView@@A'
PLAYER='?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z'
ANM='?SetInterrupt@AnmVm@@QAEXF@Z'

class NativeExAttackOwners(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units=tomllib.load(stream)['units']

    def test_native_objects_replace_six_independent_player_layouts(self):
        for name,filename in UNITS:
            with self.subTest(unit=name):
                source=(ROOT/'src'/filename).read_text()
                self.assertIn('#include "GameManagerRuntimeView.hpp"',source)
                self.assertIn('extern AsciiGameManagerView g_GameManager;',source)
                self.assertIn('->collisionQuery36C.AppendCircleRecord(',source)
                self.assertNotRegex(source,r'\bstruct\s+ExAttack\w*PlayerView\b')
                self.assertNotRegex(source,r'\bstruct\s+ExAttack\w*GameManagerView\b')
                if name=='exattack-type4-update':
                    self.assertIn('AnmVm *vms1C;',source)
                    self.assertNotIn('struct ExAttackType4VmView',source)
                else:
                    self.assertIn('->header24.anmFile98',source)
        native=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('AnmLoaded *anmFile98;',native)
        self.assertIn('PlayerHeaderNativeAnmAt98',native)
        self.assertIn('PlayerLifecycleHeaderAt24',native)

    def test_all_relocated_callees_keep_reviewed_original_targets(self):
        totals=Counter()
        for name,_ in UNITS:
            unit=self.units[name]
            for field in unit['relocations']:
                symbol=field['symbol']
                if symbol==GM:
                    self.assertEqual(field['type'],'DIR32')
                    self.assertEqual(field['target'],0x004A7D90)
                    totals['game_manager']+=1
                elif symbol==PLAYER:
                    self.assertEqual(field['type'],'REL32')
                    self.assertEqual(field['target'],0x0041DFF0)
                    self.assertEqual(field.get('addend',0),0)
                    totals['player_collision']+=1
                elif symbol==ANM:
                    self.assertEqual(field['type'],'REL32')
                    self.assertEqual(field['target'],0x00406790)
                    self.assertEqual(field.get('addend',0),0)
                    totals['anm_interrupt']+=1
        self.assertEqual(totals,Counter(game_manager=25,player_collision=6,
                                        anm_interrupt=5))
        exact=self.units['anm-vm-set-interrupt']
        self.assertEqual(exact['target_address'],0x00406790)
        self.assertEqual(exact['size'],18)

if __name__=='__main__':
    unittest.main()
