"""Original-bound RunEcl native Enemy ownership guards; byte proof is scoped replay."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]

class RunEclNativeCoreOwner(unittest.TestCase):
    def test_main_context_and_world_position_use_native_enemy_storage(self):
        source=(ROOT/'src/EclManager.cpp').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertRegex(source,r'int\s+EclManager::RunEcl\(EnemyView\s*\*\s*\w+\)')
        self.assertNotIn('Th09EclRunState::EnemyStateView *',source)
        self.assertRegex(source,r'->manager00->primaryEclManager000\s*\.\s*InitializeSubroutine\s*\(')
        self.assertNotIn('->manager0000',source)
        self.assertIn('->activeContext2CE0->time008.current =',source)
        self.assertIn('->activeContext2CE0->interpolationSlots0A0',source)
        for field in ('positionOffset2D80','position2D74','worldPosition2DD4'):
            self.assertIn('->'+field,source)
            self.assertIn('EnemyFloat3 '+field+';',native)

    def test_post_ecl_native_manager_and_draw_link(self):
        state=(ROOT/'src/EclRunState.inl').read_text()
        post=(ROOT/'src/EclPostRuntime.inl').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('manager0000', state + post)
        self.assertIn('return enemy->manager00->sideState320->background00->pendingStageLabel18;', state)
        self.assertIn('enemy->manager00->enemyAnm2AC420->SetAndExecuteScriptIdx(', post)
        self.assertIn('enemy->manager00->opposingEnemyAnm2AC424', post)
        self.assertIn('EnemyManagerView *manager00;', native)

    def test_death_animation_byte_opcodes_share_native_enemy_initializer(self):
        bullet=(ROOT/'src/EclRunBullet.inl').read_text()
        init=(ROOT/'src/EnemyManagerLifecycle.cpp').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('Th09EclBulletNativeDeathAnmScriptsAt3368',bullet)
        for idx,field in enumerate(('deathEffectVariant3368',
                                    'deathEffectVariant3369','deathEffectVariant336A')):
            with self.subTest(field=field):
                self.assertIn('enemy->'+field+' =',bullet)
                self.assertIn('enemy->'+field+' = 0;',init)
                self.assertIn('offsetof(EnemyView, '+field+') == '+format(0x3368+idx, '#06X').replace('0X','0x'),bullet)
                self.assertIn(field,native)
        self.assertNotIn('Th09EclRunState::View(enemy)->deathAnmScripts3368',bullet)

if __name__=='__main__':unittest.main()
