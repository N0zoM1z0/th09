"""One POD tracking allocation serves the full Enemy boss and Player shots.

Strict complete original PE/COFF tools, not these textual checks, establish
byte equality. The POD avoids extra Float3 auto-constructors in Player ctor.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class PlayerEnemyTrackingNativePod(unittest.TestCase):
    def test_native_tracking_allocation_has_no_implicit_float3_ctor(self):
        code=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        start=code.index('struct PlayerEnemyTrackingNativeView\n{')
        end=code.index('\n};',start)
        member=code[start:end]
        self.assertIn('float trackedX00;',member)
        self.assertIn('float trackedY04;',member)
        self.assertIn('float trackedZ08;',member)
        self.assertIn('unsigned char unknown0C[0x0C];',member)
        self.assertIn('EnemyView *homingTarget18;',member)
        self.assertIn('unsigned int trailingWord1C;',member)
        self.assertNotIn('Float3 ',member)
        self.assertNotIn('PlayerEnemyTrackingNativeView(',member)
        self.assertIn('PlayerEnemyTrackingNativeSizeIs20',code)
        self.assertIn('PlayerEnemyTrackingNativeView tracking30364;',code)
        self.assertNotIn('unsigned char unknown30364[',code)
        self.assertIn('offsetof(PlayerEnemyTrackingNativeView, homingTarget18) == 0x18',code)
        self.assertIn('offsetof(PlayerEnemyTrackingNativeView, trailingWord1C) == 0x1C',code)
        guard=(ROOT/'src/PlayerEnemyTrackingNative.hpp').read_text()
        self.assertIn('offsetof(PlayerLifecycleView, tracking30364) == 0x30364',guard)
        self.assertIn('== 0x3037C',guard)

    def test_full_enemy_boss_and_player_construction_use_same_tracking(self):
        enemy=(ROOT/'src/EnemyManagerCore.inl').read_text()
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        cleanup=(ROOT/'src/EnemyCleanupAfterDeactivation.cpp').read_text()
        self.assertIn('player->tracking30364.trackedX00',enemy)
        self.assertIn('player->tracking30364.homingTarget18',enemy)
        self.assertNotIn('player->unknown30364',enemy)
        self.assertIn('player->tracking30364.homingTarget18 == this',cleanup)
        self.assertIn('player->tracking30364.trailingWord1C = 0;',runtime)
        self.assertIn('player->tracking30364.unknown0C',runtime)
        self.assertIn('Float3(-999.0f, -999.0f, 0.0f)',runtime)
        for file in ('PlayerShotSpawnCallback7.cpp','PlayerShotUpdateCallback1.cpp',
                     'PlayerShotUpdateCallback3.cpp'):
            with self.subTest(shot_file=file):
                s=(ROOT/'src'/file).read_text()
                self.assertIn('player->tracking30364',s)
                self.assertNotIn('player->unknown30364',s)

    def test_existing_exact_owners_and_nonexact_boss_are_independent(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for unit, size in (('player-constructor',243),
                           ('player-on-update',522),
                           ('player-shot-spawn-callback-type7',139),
                           ('player-shot-update-callback-type1',471),
                           ('player-shot-update-callback-type3',248),
                           ('enemy-cleanup-after-deactivation',184)):
            with self.subTest(unit=unit):
                self.assertEqual(units[unit]['size'],size)
        self.assertFalse(any(u.get('symbol')=='?OnUpdate@EnemyManagerView@@SIHPAU1@@Z'
                             for u in units.values()))


if __name__=='__main__':
    unittest.main()
