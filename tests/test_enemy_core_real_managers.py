"""EnemyManager 3900-byte update uses shared real ANM, sound, ASCII owners.

This is a pure-input source/ledger ownership guard: original codegen requires
separate cold full PE/COFF replay with 97 true relocation fields, 69 ordered
calls, switch table and 3900 physical bytes. Cross-file declarations cannot
manufacture missing original data, nonexact promotion or native link success.
"""

import csv
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EnemyUpdateSharedRealOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.enemy = (ROOT/'src/EnemyManagerCore.cpp').read_text()
        cls.ascii_marker = (ROOT/'src/AsciiBossMarkerVm.cpp').read_text()
        cls.marker_header = (ROOT/'src/AsciiBossMarkerVmView.hpp').read_text()
        cls.sound_header = (ROOT/'src/SoundPlayer.hpp').read_text()
        cls.anm_header = (ROOT/'src/AnmManager.hpp').read_text()
        cls.ascii_header = (ROOT/'src/AsciiManager.hpp').read_text()
        cls.inspector = (ROOT/'scripts/inspect-enemy-manager-update.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']
        with (ROOT/'config/functions.csv').open(newline='') as file:
            cls.functions = {row['address']: row for row in csv.DictReader(file)}

    def test_true_sound_player_global_and_member_in_large_enemy_owner(self):
        self.assertEqual(self.enemy.count('#include "SoundPlayer.hpp"'), 1)
        self.assertEqual(self.enemy.count('g_SoundPlayer.PlaySoundPositionedByIdx('), 2)
        self.assertNotIn('EnemyCoreSoundPlayerView', self.enemy)
        self.assertNotIn('g_EnemyCoreSoundPlayer', self.enemy)
        self.assertIn('extern SoundPlayer g_SoundPlayer;', self.sound_header)
        self.assertIn('void PlaySoundPositionedByIdx(int index, float positionX);',
                      self.sound_header)
        unit = self.units['soundplayer-play-positioned-sound-by-idx']
        self.assertEqual((unit['target_address'], unit['size'], unit['symbol']),
                         (0x0043E380, 0x99,
                          '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z'))

    def test_true_anm_manager_and_vm_receiver_shim(self):
        self.assertEqual(self.enemy.count('#include "AnmManager.hpp"'), 1)
        self.assertEqual(self.enemy.count('g_AnmManager->ExecuteScript('), 2)
        self.assertNotIn('EnemyCoreAnmManagerView', self.enemy)
        self.assertNotIn('g_EnemyCoreAnmManager', self.enemy)
        self.assertIn('extern AnmManager *g_AnmManager;', self.anm_header)
        self.assertIn('int ExecuteScript(AnmVm *vm);', self.anm_header)
        self.assertIn('reinterpret_cast<AnmVm *>(&enemy->primaryVm008)',self.enemy)
        self.assertIn('reinterpret_cast<AnmVm *>(&enemy->secondaryVms2AC[vmIndex])',
                      self.enemy)
        self.assertIn('sizeof(EnemyCoreAnmVmView) == 0x2A4', self.enemy)
        unit = self.units['anm-manager-execute-script']
        self.assertEqual((unit['target_address'], unit['symbol']),
                         (0x00436F30,
                          '?ExecuteScript@AnmManager@@QAEHPAUAnmVm@@@Z'))

    def test_boss_marker_proven_original_receiver_shared_across_tus(self):
        self.assertEqual(self.enemy.count('#include "AsciiBossMarkerVmView.hpp"'), 1)
        self.assertIn('#include "AsciiBossMarkerVmView.hpp"', self.ascii_marker)
        self.assertNotIn('struct AsciiBossMarkerVmView\n', self.ascii_marker)
        self.assertEqual(self.marker_header.count('struct AsciiBossMarkerVmView\n'), 1)
        self.assertIn('offsetof(AsciiBossMarkerVmView, bossMarkerVms17C4) == 0x17C4',
                      self.marker_header)
        self.assertIn('sizeof(AsciiBossMarkerAnmVmView) == 0x2A4', self.marker_header)
        self.assertIn('void SetBossMarkerPosition(int slot, const Float3 *position);',
                      self.marker_header)
        self.assertEqual(self.enemy.count('reinterpret_cast<AsciiBossMarkerVmView *>(&g_AsciiManager)->SetBossMarkerPosition('), 1)
        self.assertEqual(self.enemy.count('g_AsciiManager.SetBossMarkerState('), 2)
        self.assertIn('extern AsciiManager g_AsciiManager;',self.enemy)
        self.assertIn('void SetBossMarkerState(int slot, int state);',self.ascii_header)
        self.assertNotIn('EnemyCoreUiView', self.enemy)
        self.assertNotIn('g_EnemyCoreUi', self.enemy)

    def test_real_original_callees_keep_distinct_original_exact_units(self):
        expected = {
            'ascii-boss-marker-set-position':
                (0x004067F0, 42, '?SetBossMarkerPosition@AsciiBossMarkerVmView@@QAEXHPBUFloat3@@@Z'),
            'ascii-boss-marker-set-interrupt':
                (0x004067D0, 32, '?SetBossMarkerInterrupt@AsciiBossMarkerVmView@@QAEXHF@Z'),
            'ascii-manager-set-boss-marker-state':
                (0x0040F890, 20, '?SetBossMarkerState@AsciiManager@@QAEXHH@Z'),
        }
        for name, (address, size, symbol) in expected.items():
            with self.subTest(unit=name):
                unit=self.units[name]
                self.assertEqual((unit['target_address'],unit['size'],unit['symbol']),
                                 (address,size,symbol))
        for real_symbol in ('?g_SoundPlayer@@3VSoundPlayer@@A',
                            '?g_AnmManager@@3PAVAnmManager@@A',
                            '?g_AsciiManager@@3VAsciiManager@@A'):
            self.assertIn(real_symbol, self.inspector)

    def test_boss_remains_nonexact_and_source_present_only(self):
        row = self.functions['0x00410730']
        self.assertEqual((row['owner'],row['status'],row['source_file'],int(row['size'])),
                         ('authored','unclassified','src/EnemyManagerCore.cpp',3883))
        self.assertIn('EnemyManager::OnUpdate PE/VC7.1 diagnostic',self.inspector)
        self.assertIn('owner-local label destinations and all 97',self.inspector)


if __name__=='__main__':
    unittest.main()
