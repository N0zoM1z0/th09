"""Player timer constructor and charge owner use the actual ZunTimer ABI."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class PlayerNativeTimerStorage(unittest.TestCase):
    def test_embedded_timers_share_exact_native_zun_timer_layout(self):
        owner=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        native=(ROOT/'src/ZunTimer.hpp').read_text()
        self.assertIn('typedef ZunTimer PlayerTimerCtorView;',owner)
        self.assertNotIn('struct PlayerTimerCtorView',owner)
        self.assertIn('PlayerTimerCtorNativeFields',owner)
        for name,off in (('previous','0x00'),('subFrame','0x04'),('current','0x08')):
            with self.subTest(field=name):
                self.assertIn('offsetof(PlayerTimerCtorView, '+name+') == '+off,owner)
                self.assertIn(' '+name+';',native)
        for name in ('timer1B74','timer3038C','timer30398','timer303A4',
                     'timer303B0','timer303BC','timer303C8','timer303D4'):
            with self.subTest(timer=name):self.assertIn('PlayerTimerCtorView '+name+';',owner)

    def test_complete_charge_owner_uses_direct_native_timer_storage(self):
        source=(ROOT/'src/PlayerChargeAttackUpdate.cpp').read_text()
        self.assertNotIn('reinterpret_cast<ZunTimer *>(&timer',source)
        for f in ('timer1B74 = 8;', 'timer303BC = -1;',
                  'timer3038C = 0;', 'timer30398 = 0;'):
            with self.subTest(storage=f):self.assertIn(f,source)
        self.assertIn('g_GameConfiguration->sideFocusMode',source)

    def test_runtime_collision_and_state_use_direct_timer_receivers(self):
        for filename in ('PlayerRuntime.cpp','PlayerCollisionHelpers.cpp',
                         'PlayerGameplayState.cpp'):
            with self.subTest(source=filename):
                source=(ROOT/'src'/filename).read_text()
                self.assertNotIn('reinterpret_cast<ZunTimer *>(&player->timer',source)
                self.assertIn('player->timer1B74',source)
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        self.assertIn('player->timer303C8.operator--(0)',runtime)
        self.assertIn('player->timer303C8.operator++(0)',runtime)
        self.assertIn('player->timer1B74.operator<=(0)',runtime)

    def test_original_eight_constructor_calls_and_shot_increment(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        ctor=units['zun-timer-constructor']
        self.assertEqual((ctor['symbol'],ctor['target_address'],ctor['size']),
                         ('??0ZunTimer@@QAE@XZ',0x403390,12))
        self.assertEqual(ctor['relocations'][0]['target'],units['zun-timer-initialize']['target_address'])
        pc=units['player-constructor']
        fields=[x for x in pc['relocations'] if x['symbol']==ctor['symbol']]
        self.assertEqual([r['offset'] for r in fields],[0x28,0x86,0x91,0x9C,0xA7,0xB2,0xBD,0xC8])
        self.assertTrue(all(r['target']==ctor['target_address'] and r['type']=='REL32' for r in fields))
        inc=units['zun-timer-post-increment']
        self.assertEqual((inc['target_address'],inc['size']), (0x401510,8))
        shot=units['player-update-shots']['relocations']
        self.assertEqual([(r['offset'],r['symbol'],r['target']) for r in shot
                          if r['symbol']==inc['symbol']],[(0xDD,inc['symbol'],inc['target_address'])])

if __name__=='__main__':unittest.main()
