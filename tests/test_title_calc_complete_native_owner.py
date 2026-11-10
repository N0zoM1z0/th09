"""Title calculation callback operates on the original complete TitleScreen."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeTitleCalcOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/TitleCalcCallback.cpp').read_text()
        cls.owner = (ROOT / 'src/TitleScreenReplayView.hpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as handle:
            cls.units = tomllib.load(handle)['units']

    def test_same_full_title_object_drives_all_update_branches(self):
        s = self.source
        self.assertIn('#include "TitleScreenReplayView.hpp"', s)
        self.assertNotIn('struct TitleScreenView\n{', s)
        self.assertIn('TitleScreenView *title = static_cast<TitleScreenView *>(object);', s)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388', s)
        for name in ('vms', 'currentHelpTextVm', 'vmCount', 'currentScreen', 'chainState'):
            self.assertIn('title->' + name, s)
        self.assertIn('title->OnUpdateKeyConfig();', s)
        self.assertIn('title->OnUpdateMusicRoom();', s)
        self.assertIn('int OnUpdateKeyConfig();', self.owner)
        self.assertIn('int OnUpdateMusicRoom();', self.owner)
        self.assertIn('AnmVmView embeddedVms[57];', self.owner)
        self.assertIn('g_AnmManager->ExecuteScriptArray(title->vms, title->vmCount);', s)

    def test_complete_original_bytes_calls_and_two_jump_tables(self):
        unit = self.units['title-screen-calc-callback']
        self.assertEqual((unit['target_address'], unit['size'], unit['compare_size'],
                          len(unit['relocations'])),
                         (0x0042A9CB, 264, 328, 36))
        fields = {r['offset']: r for r in unit['relocations']}
        for offset, method, target in (
                (0xA3, 'OnUpdateKeyConfig', 0x00427EE8),
                (0xBE, 'OnUpdateMusicRoom', 0x00426E05),
                (0xC7, 'OnUpdateResultNameEntry', 0x0042929C),
                (0xD0, 'OnUpdateResult', 0x004266B5)):
            with self.subTest(method=method):
                r = fields[offset]
                self.assertEqual((r['symbol'], r['target'], r['type']),
                                 ('?' + method + '@TitleScreenView@@QAEHXZ',
                                  target, 'REL32'))
        self.assertEqual(sum(r['type'] == 'DIR32' and r['symbol'].startswith('$')
                             for r in unit['relocations']), 17)


if __name__ == '__main__':
    unittest.main()
