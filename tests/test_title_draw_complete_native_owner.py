"""Title draw callback uses the complete original Title and ANM allocations."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeTitleDrawOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/TitleDrawCallback.cpp').read_text()
        cls.owner = (ROOT / 'src/TitleScreenReplayView.hpp').read_text()
        cls.anm = (ROOT / 'src/AnmManager.hpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_full_object_fields_and_real_texture_cache(self):
        s = self.source
        self.assertNotIn('struct TitleDrawView\n{', s)
        self.assertNotIn('AnmManagerBackgroundView', s)
        self.assertIn('TitleScreenView *title = static_cast<TitleScreenView *>(object);', s)
        self.assertIn('g_AnmManager->currentTexture12880 = NULL;', s)
        for member in ('vms', 'currentHelpTextVm', 'vmCount', 'currentScreen',
                       'chainState'):
            self.assertIn('title->' + member, s)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388', s)
        self.assertIn('offsetof(AnmManager, currentTexture12880) == 0x12880', s)
        self.assertIn('void *currentTexture12880;', self.anm)
        self.assertIn('AnmVmView embeddedVms[57];', self.owner)
        self.assertIn('int DrawMusicRoom();', self.owner)

    def test_exact_329_byte_owner_and_four_real_method_targets(self):
        unit = self.units['title-draw-callback']
        self.assertEqual((unit['target_address'], unit['size'], len(unit['relocations'])),
                         (0x00424898, 329, 20))
        rows = {r['offset']: r for r in unit['relocations']}
        expected = ((0x107, 'DrawReplayMenu', 0x004234F6),
                    (0x121, 'DrawReplaySave', 0x004239F6),
                    (0x12A, 'DrawResult', 0x00423D16),
                    (0x133, 'DrawMusicRoom', 0x004244A9))
        for offset, name, target in expected:
            with self.subTest(method=name):
                r = rows[offset]
                self.assertEqual((r['symbol'], r['target'], r['type']),
                                 ('?' + name + '@TitleScreenView@@QAEHXZ', target, 'REL32'))

    def test_start_menu_original_supervisor_member_operand(self):
        u = self.units['title-screen-start-menu']
        self.assertEqual(u['compare_size'], 1592)
        field = next(r for r in u['relocations'] if r['offset'] == 0x47)
        self.assertEqual((field['symbol'], field['target'], field.get('addend', 0)),
                         ('?g_Supervisor@@3VSupervisor@@A', 0x004B3100, 0x388))


if __name__ == '__main__':
    unittest.main()
