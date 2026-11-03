"""Original Enemy spawn helpers and six CardAttack callbacks share real manager slots."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeEnemySpawnReceiver(unittest.TestCase):
    def test_spawn_method_receiver_has_no_second_manager_allocation(self):
        header=(ROOT/'src/EnemyManagerCardAttackSpawn.hpp').read_text()
        spawn=(ROOT/'src/EnemyManagerSpawnRecord.cpp').read_text()
        self.assertIn('struct EnemyManagerCardAttackSpawnView : EnemyManagerView',header)
        self.assertIn('EnemyCardAttackSpawnNativeReceiver',header)
        self.assertIn('sizeof(EnemyManagerCardAttackSpawnView) == sizeof(EnemyManagerView)',header)
        self.assertIn('offsetof(EnemyManagerCardAttackSpawnView, enemies5758) == 0x5758',header)
        self.assertIn('offsetof(EnemyManagerCardAttackSpawnView, spawnFailure2AC42C) == 0x2AC42C',header)
        self.assertIn('EnemyManagerView *manager = static_cast<EnemyManagerView *>(this);',spawn)
        self.assertNotIn('reinterpret_cast<EnemyManagerView *>(this)',spawn)
        self.assertIn('char defeatTokenType,',header)

    def test_six_cardattack_entrypoints_use_native_game_side_not_private_global(self):
        source=(ROOT/'src/CardAttackBeginCallbacks.cpp').read_text()
        self.assertNotIn('struct CardAttackBeginGameManagerView',source)
        self.assertNotIn('extern CardAttackBeginGameManagerView g_GameManager;',source)
        self.assertIn('CardAttackBeginNativeEnemySideOwner',source)
        self.assertEqual(source.count('static_cast<EnemyManagerCardAttackSpawnView *>('),6)
        self.assertEqual(source.count('g_GameManager.sides[cardAttack->sideIndex00].enemyManager10'),6)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        self.assertEqual((units['enemy-manager-spawn-record']['size'],
                          units['enemy-manager-card-attack-spawn']['size']),(364,38))
        orig='?g_GameManager@@3UAsciiGameManagerView@@A'
        callee=units['enemy-manager-card-attack-spawn']
        for player in ('pl00','pl01'):
            for kind in range(3):
                name=f'card-attack-begin-{player}-type{kind}'
                with self.subTest(unit=name):
                    u=units[name]
                    self.assertEqual(len(u['relocations']),5)
                    gm=[r for r in u['relocations'] if r['symbol']==orig]
                    self.assertEqual(gm,[dict(offset=0x33,type='DIR32',symbol=orig,
                                              target=0x4A7D90,addend=0x10)])
                    self.assertTrue(any(r['symbol']==callee['symbol'] and
                                        r['target']==callee['target_address']==0x440C80
                                        for r in u['relocations']))

    def test_timeline_int_callsite_is_not_misdeclared_as_native_byte_argument(self):
        # Exact 1048-byte timeline source still calls the observed 32-bit
        # caller signature; changing it to byte expands the function by 8 bytes.
        source=(ROOT/'src/EnemyScheduleRuntimeAdvance.cpp').read_text()
        self.assertIn('struct EnemyTimelineSpawnManagerView',source)
        self.assertIn('int defeatTokenType,',source)
        self.assertEqual(source.count('reinterpret_cast<EnemyTimelineSpawnManagerView *>('),6)


if __name__=='__main__':unittest.main()
