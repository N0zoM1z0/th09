"""Original exact 974-byte Enemy timeline uses true native pool and event slots."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EnemyTimelineNativePool(unittest.TestCase):
    def test_scheduler_uses_single_enemy_manager_and_enemy_allocation(self):
        source = (ROOT/'src/EnemyScheduleRuntimeAdvance.cpp').read_text()
        manager = (ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('struct EnemyTimelineEnemyView',source)
        self.assertNotIn('struct EnemyTimelineManagerView',source)
        self.assertIn('EnemyTimelineNativePoolAndEvents',source)
        self.assertIn('EnemyManagerView *manager1C;',source)
        self.assertIn('EnemyView *enemy17 =',source)
        self.assertIn('EnemyView *enemy;',source)
        self.assertIn('remoteSlots2AC388[args8[0]]',source)
        self.assertIn('->pendingSubroutineId2D70',source)
        self.assertIn('->secondaryFlags3380 |= 0x200u;',source)
        self.assertIn('->itemDropCount3360 = locals.args[4];',source)
        self.assertIn('->itemDropExtraCount3364 = locals.args[5];',source)
        for field,offset in (('remoteSlots2AC388','0x2AC388'),
                             ('timelineEventSlots2AC430','0x2AC430')):
            with self.subTest(field=field):
                self.assertIn('offsetof(EnemyManagerView, '+field+') == '+offset,source)
                self.assertIn(field,manager)
        self.assertIn('sizeof(EnemyManagerView) == 0x2AC450',source)

    def test_original_full_jump_table_and_42_fields_are_still_bound(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        u=units['enemy-schedule-runtime-run']
        self.assertEqual((u['target_address'],u['size'],u['compare_size'],len(u['relocations'])),
                         (0x40FCC0,974,1048,42))
        self.assertEqual(units['zun-timer-equal-int']['size'],20)
        self.assertTrue(all(not r['symbol'].startswith('?g_EnemyTimelineInstructionMask')
                            for r in u['relocations']))


if __name__=='__main__': unittest.main()
