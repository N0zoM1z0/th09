"""Native game-side subsystem pointers for all seven original GameManager creators.

Source-side layout/ABI contracts are supplemented by current VC7.1 target-PE
comparison of full GameManager OnUpdate, GameplaySetupThread diagnostics and
all affected exact linked callers; no matching credit from this test alone.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]

class NativeGameSubsystemSlots(unittest.TestCase):
    def test_source_uses_one_original_seven_subsystem_layout(self):
        header=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        for name,offset in (
            ('background00','0x00'),('player04','0x04'),
            ('etama08','0x08'),('effectManager0C','0x0C'),
            ('enemyManager10','0x10'),('cardAttack14','0x14'),
            ('frontSide18','0x18')):
            with self.subTest(name=name):
                self.assertIn('offsetof(AsciiGameManagerSideView, '+name+') == '+offset, header)
                self.assertIn('side->'+name,source)
        for text in ('Background *background00;', 'EnemyManagerView *enemyManager10;',
                     'CardAttack *cardAttack14;', 'FrontSide *frontSide18;'):
            self.assertIn(text,header)
        for term in ('side->background00 = Background::Create(sideIndex);',
                     'side->player04 = PlayerRegisterChain(0, sideIndex, side->selector);',
                     'side->etama08 = EtamaController::Register(sideIndex);',
                     'side->effectManager0C = EffectManager::Create(sideIndex, 0x100, 7);',
                     'side->enemyManager10 = EnemyManagerView::Create(sideIndex);',
                     'side->cardAttack14 = CardAttack::Create(sideIndex);',
                     'side->frontSide18 = FrontSide::Create(sideIndex);'):
            self.assertIn(term,source)
        for n in range(7):
            self.assertNotIn(f'side->subsystem{n} = ',source)
        self.assertNotIn('static_cast<PlayerLifecycleView *>(manager->sides[',source)
        self.assertNotIn('static_cast<PlayerLifecycleView *>(globalManager.sides[',source)
        self.assertIn('globalManager.sides[0].player04->header24.state7C',source)
        self.assertIn('manager->sides[1].player04->header24.state80',source)

    def test_original_creator_abi_and_large_owners_remain_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        expected=[
            ('background-create',307),
            ('player-register-chain',328),
            ('etama-controller-register',283),
            ('effect-manager-create',327),
            ('enemy-manager-create',328),
            ('card-attack-create',264),
            ('front-side-create',283),
        ]
        for name,size in expected:
            with self.subTest(name=name):
                self.assertEqual(units[name]['size'],size)
        self.assertEqual((units['game-manager-on-update']['size'],
                          len(units['game-manager-on-update']['relocations'])),(1230,87))
        self.assertEqual(units['game-manager-cleanup-gameplay-state']['size'],425)


if __name__ == '__main__':
    unittest.main()
