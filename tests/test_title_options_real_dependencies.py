"""TH09 Title Options uses proven shared ANM/SoundPlayer members, not aliases.

Pure-input regression guards. Full original codegen and complete 135-field
binding remain separate VC7.1 cold checks; no local exactness is claimed.
"""

import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]

spec = importlib.util.spec_from_file_location(
    'title_options_real_dependencies_oracle', ROOT/'scripts/inspect-title-options.py')
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


def manifest():
    with (ROOT/'config/match-units.toml').open('rb') as handle:
        return tomllib.load(handle)['units']


class RealTitleOptionDependencies(TestCase):
    def test_maintained_gameplay_code_uses_shared_owners(self):
        s = (ROOT/'src/TitleScreenOptions.cpp').read_text()
        for expected in (
            '#include "AnmManager.hpp"', '#include "AsciiManager.hpp"',
            '#include "SoundPlayer.hpp"',
            'extern const char *g_TitleOptionsHelpText[9];',
            'AnmLoaded *titleAnm;', 'g_AnmManager->SetInterruptArray(',
            'g_AnmManager->ExecuteScriptArray(',
            'g_SoundPlayer.PlaySoundByIdx(', 'g_SoundPlayer.QueueCommand(',
            'OptionsRealBgmVolumeAt6218',
            'OptionsRealSfxVolumeAt621C', 'OptionsRealAnmVmSizeIs2A4',
        ):
            with self.subTest(expected=expected):
                self.assertIn(expected, s)
        self.assertEqual(s.count('titleAnm->SetSprite('), 8)
        self.assertEqual(s.count('reinterpret_cast<AnmVm *>(&vms['), 8)
        for legacy in ('struct SoundPlayerView', 'struct TitleAnmView',
                       'struct TitleAnmManagerView {',
                       'extern TitleAnmManagerView *g_TitleAnmManager;'):
            self.assertNotIn(legacy, s)

    def test_each_actual_method_has_an_independent_original_exact_owner(self):
        owners = {
            0x00436DE0: 'AnmManager::SetInterruptArray',
            0x00439560: 'AnmManager::ExecuteScriptArray',
            0x00436AC0: 'AnmLoaded::SetSprite',
            0x0043E2F0: 'SoundPlayer::PlaySoundByIdx',
            0x0043EE40: 'SoundPlayer::QueueCommand',
            0x0043F1F0: 'SoundPlayer::ProcessQueues',
        }
        with (ROOT/'config/functions.csv').open(newline='') as stream:
            current = {int(r['address'], 16): r for r in csv.DictReader(stream)}
        for address, name in owners.items():
            with self.subTest(name=name):
                self.assertEqual(current[address]['proposed_name'], name)
                self.assertEqual(current[address]['status'], 'matching')
                self.assertTrue(current[address]['source_file'].startswith('src/'))

    def test_original_field_owner_names_link_to_real_physical_addresses(self):
        expected = {
            '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
            '?g_SoundPlayer@@3VSoundPlayer@@A': 0x004DC698,
            '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z': 0x00436DE0,
            '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z': 0x00439560,
            '?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z': 0x00436AC0,
            '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': 0x0043E2F0,
            '?QueueCommand@SoundPlayer@@QAEXHHPAD@Z': 0x0043EE40,
        }
        for symbol, address in expected.items():
            with self.subTest(symbol=symbol):
                self.assertEqual(check.DESTINATIONS[symbol], address)
        for legacy in (
            '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A',
            '?g_SoundPlayer@@3USoundPlayerView@@A',
            '?SetSprite@TitleAnmView@@QAEXPAUAnmVmView@@H@Z',
            '?PlaySoundByIdx@SoundPlayerView@@QAEXHH@Z',
            '?QueueCommand@SoundPlayerView@@QAEXHHPAD@Z',
        ):
            self.assertNotIn(legacy, check.DESTINATIONS)

    def test_existing_exact_title_menu_sound_uses_same_true_receiver(self):
        unit = manifest()['title-screen-play-menu-sound']
        self.assertEqual(unit['source'], 'src/TitleScreenOptions.cpp')
        self.assertEqual(unit['target_address'], 0x00422F17)
        self.assertEqual(unit['size'], 34)
        self.assertEqual(
            [(r['offset'], r['type'], r['symbol'], r['target']) for r in unit['relocations']],
            [
                (8, 'DIR32', '?g_SoundPlayer@@3VSoundPlayer@@A', 0x004DC698),
                (0x12, 'REL32', '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 0x0043E2F0),
                (0x19, 'REL32', '?ProcessQueues@SoundPlayer@@QAEHXZ', 0x0043F1F0),
            ])
        self.assertEqual(check.SIZE, 2045)
        self.assertEqual(check.ORIGINAL_FIELDS, 135)
        self.assertEqual(check.ORIGINAL_CALLS, 51)
        self.assertEqual(check.ORIGINAL_BLOCKS, 121)
