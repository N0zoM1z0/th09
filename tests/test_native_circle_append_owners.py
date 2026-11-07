"""TH09 Player collision circle append has one real source method and exact callers.

The physical PE/COFF comparator proves byte identity; these invariants
prevent reintroducing independent method overlays with the same numeric VA.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
CALL_SYMBOL = '?AppendCircleRecord@PlayerCollisionQueryStateView@@QAEXPBUPlayerPositionView@@MH@Z'
CALL_UNITS = (
    ('exattack-type01-update', 'ExAttackUpdateType01.cpp', 1),
    ('exattack-type4-update', 'ExAttackUpdateType4.cpp', 5),
    ('exattack-type13-update', 'ExAttackUpdateType13.cpp', 1),
    ('exattack-type14-22-update', 'ExAttackUpdateType14Type22.cpp', 1),
    ('exattack-type16-23-update', 'ExAttackUpdateType16Type23.cpp', 1),
    ('exattack-type17-update', 'ExAttackUpdateType17.cpp', 2),
    ('exattack-type18-24-update', 'ExAttackUpdateType18Type24.cpp', 1),
)

class NativeCircleAppendOwnerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            cls.units = tomllib.load(stream)['units']

    def test_single_native_receiver_owns_all_three_append_variants(self):
        source = (ROOT/'src/PlayerCollisionAppendCircle.cpp').read_text()
        header = (ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('void PlayerCollisionQueryStateView::AppendCircleRecord(', source)
        self.assertNotIn('struct PlayerCollisionCircleAppendView', source)
        self.assertIn('void AppendCircleRecord(', header)
        self.assertIn('void AppendLaserRecord(', header)
        self.assertIn('void AppendBoxRecord(', header)
        self.assertIn('PlayerCollisionQueryRecordView records[128]', header)
        unit = self.units['player-collision-append-circle']
        self.assertEqual(unit['symbol'], CALL_SYMBOL)
        self.assertEqual(unit['target_address'], 0x00440D30)
        self.assertEqual(unit['size'], 95)
        self.assertEqual(unit['relocations'], [])

    def test_seven_full_exattack_functions_bind_all_eleven_native_calls(self):
        total = 0
        for name, filename, count in CALL_UNITS:
            with self.subTest(unit=name):
                code = (ROOT/'src'/filename).read_text()
                self.assertIn('extern AsciiGameManagerView g_GameManager;',code)
                self.assertIn('#include "GameManagerRuntimeView.hpp"',code)
                self.assertNotIn('struct ExAttackType4PlayerView',code)
                self.assertNotIn('struct ExAttackType13PlayerView',code)
                self.assertNotIn('struct ExAttackType1422PlayerView',code)
                self.assertNotIn('struct ExAttackType16PlayerView',code)
                self.assertNotIn('struct ExAttackType17PlayerView',code)
                self.assertNotIn('struct ExAttackUpdatePlayerView',code)
                self.assertNotIn('struct ExAttackType1824PlayerView',code)
                self.assertNotIn('CircleAppendView',code)
                self.assertNotIn('ExAttackCollisionCircleView',code)
                # Type17's natural radius branch has two C++ sites but
                # VC7.1 merges their target call into one physical REL32.
                self.assertEqual(code.count('.AppendCircleRecord('), count)
                actual = [row for row in self.units[name]['relocations']
                          if 'AppendCircleRecord@' in row['symbol']]
                self.assertEqual(len(actual), 1 if name=='exattack-type17-update' else count)
                self.assertTrue(all(row['symbol']==CALL_SYMBOL and
                                    row['type']=='REL32' and
                                    row['target']==0x00440D30 and
                                    row.get('addend',0)==0 for row in actual))
                total += len(actual)
        self.assertEqual(total, 11)

    def test_tracking_has_no_legacy_circle_method_alias(self):
        for filename in ('implemented.csv','reccmp-functions.csv',
                         'matches.csv','functions.csv'):
            with self.subTest(tracking=filename):
                content=(ROOT/'config'/filename).read_text()
                self.assertIn('PlayerCollisionQueryStateView::AppendCircleRecord',content)
                self.assertNotIn('PlayerCollisionCircleAppendView::AppendCircleRecord',content)

    def test_type4_native_player_and_anm_vm_identity(self):
        source = (ROOT/'src/ExAttackUpdateType4.cpp').read_text()
        self.assertIn('extern AsciiGameManagerView g_GameManager;',source)
        self.assertIn('PlayerLifecycleView *player04;',
                      (ROOT/'src/GameManagerSideState.hpp').read_text())
        self.assertIn('AnmVm *vms1C;',source)
        self.assertIn('ExAttackType4NativePlayerQueryLayout',source)
        self.assertIn('ExAttackType4NativeVmLayout',source)
        self.assertNotIn('struct ExAttackType4PlayerView',source)
        self.assertNotIn('struct ExAttackType4VmView',source)
        unit = self.units['exattack-type4-update']
        self.assertEqual(unit['size'],739)
        real_vm=self.units['anm-vm-set-interrupt']
        self.assertEqual(real_vm['target_address'],0x00406790)
        self.assertEqual(real_vm['size'],18)
        calls=[r for r in unit['relocations']
               if 'SetInterrupt@' in r['symbol']]
        self.assertEqual([r['offset'] for r in calls],
                         [0x296,0x2A6,0x2B6,0x2C6,0x2D6])
        for row in calls:
            self.assertEqual(row['symbol'],real_vm['symbol'])
            self.assertEqual(row['target'],real_vm['target_address'])
            self.assertEqual(row['type'],'REL32')
            self.assertEqual(row.get('addend',0),0)
        self.assertEqual(len([r for r in unit['relocations']
                              if 'AppendCircleRecord@' in r['symbol']]),5)

    def test_six_updates_use_real_game_manager_and_player_callers(self):
        native_gm='?g_GameManager@@3UAsciiGameManagerView@@A'
        native_player='?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z'
        audited=('exattack-type01-update','exattack-type13-update',
                 'exattack-type14-22-update','exattack-type16-23-update',
                 'exattack-type17-update','exattack-type4-update')
        gm_fields=player_fields=0
        for name in audited:
            with self.subTest(unit=name):
                unit=self.units[name]
                source=(ROOT/unit['source']).read_text()
                self.assertIn('extern AsciiGameManagerView g_GameManager;',source)
                self.assertIn('#include "GameManagerRuntimeView.hpp"',source)
                self.assertNotIn('struct ExAttackType4UpdateGameManagerView',source)
                fields=[r for r in unit['relocations']
                        if 'g_GameManager@@' in r['symbol']]
                self.assertTrue(fields)
                gm_fields+=len(fields)
                for row in fields:
                    self.assertEqual(row['symbol'],native_gm)
                    self.assertEqual(row['target'],0x004A7D90)
                    self.assertEqual(row['type'],'DIR32')
                calls=[r for r in unit['relocations']
                       if 'CheckBulletCollision@' in r['symbol']]
                player_fields+=len(calls)
                for row in calls:
                    self.assertEqual(row['symbol'],native_player)
                    self.assertEqual(row['target'],0x0041DFF0)
                    self.assertEqual(row['type'],'REL32')
        self.assertEqual(gm_fields,25)
        self.assertEqual(player_fields,6)

    def test_type18_original_global_field_addends_are_not_zeroed(self):
        # The actual Type18 COFF uses GameManager+4 at these fields even
        # though the historical TOML omits an explicit addend. This test
        # guards preserving native side indexing rather than injecting a
        # fabricated zero-addend binding in an auxiliary comparator.
        unit = self.units['exattack-type18-24-update']
        fields = [row for row in unit['relocations']
                  if row['symbol']=='?g_GameManager@@3UAsciiGameManagerView@@A']
        self.assertEqual(len(fields), 7)
        self.assertEqual({r['target'] for r in fields},{0x004A7D90})
        self.assertEqual(unit['size'],1436)


if __name__ == '__main__':
    unittest.main()
