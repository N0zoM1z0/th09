"""Real-owner context splicing and bounded native-output safety checks."""
import importlib.util
from pathlib import Path
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_context_probe', ROOT / 'scripts/probe-title-replay-menu-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)

TITLE = '''#include "ReplayDataOriginalView.hpp"
struct TitleScreenView {
    int real_owner_field;
    static void __cdecl TitleSetupThread(void *unused);
};
void __cdecl TitleScreenView::TitleSetupThread(void *unused)
{
    native_setup_body();
}
'''
MENU = '''#include "ReplayDataOriginalView.hpp"
struct TitleScreenView { int provisional_owner; int DrawReplayMenu(); };
extern AsciiManager g_AsciiManager;
static int real_selector() { return 1; }
int TitleScreenView::DrawReplayMenu()
{
    complete_menu_body();
    return 1;
}
'''


class TitleReplayContextProbe(unittest.TestCase):
    def test_contexts_reuse_real_owner_and_complete_original_menu_body(self):
        rows = probe.make_contexts(TITLE, MENU)
        self.assertEqual(rows['standalone'], MENU)
        for name in ('declarations', 'native'):
            text = rows[name]
            self.assertIn('int real_owner_field;', text)
            self.assertNotIn('provisional_owner', text)
            self.assertEqual(text.count('    int DrawReplayMenu();'), 1)
            self.assertEqual(text.count('complete_menu_body();'), 1)
            self.assertEqual(text.count('static int real_selector()'), 1)
            self.assertTrue(text.endswith(MENU[MENU.index(probe.MENU_GLOBALS):]))

    def test_declarations_and_native_body_visibility_are_distinct(self):
        rows = probe.make_contexts(TITLE, MENU)
        self.assertNotIn('native_setup_body();', rows['declarations'])
        self.assertIn('native_setup_body();', rows['native'])
        self.assertEqual(set(probe.make_contexts(TITLE, MENU, 'native')),
                         {'standalone', 'native'})
        self.assertEqual(set(probe.make_contexts(TITLE, MENU, 'declarations')),
                         {'standalone', 'declarations'})

    def test_missing_duplicate_or_already_integrated_definitions_fail_closed(self):
        for title, menu in (
            (TITLE.replace(probe.SETUP_DECLARATION, ''), MENU),
            (TITLE + probe.SETUP_DECLARATION, MENU),
            (TITLE.replace(probe.SETUP_DEFINITION, '\nvoid OtherSetup('), MENU),
            (TITLE + probe.SETUP_DEFINITION, MENU),
            (TITLE + 'int DrawReplayMenu();', MENU),
            (TITLE, MENU.replace(probe.MENU_GLOBALS, '')),
            (TITLE, MENU + probe.MENU_GLOBALS),
            (TITLE, MENU.replace(probe.MENU_DEFINITION, 'int OtherMenu()')),
            (TITLE, MENU + probe.MENU_DEFINITION),
            (TITLE, MENU.replace('#include "ReplayDataOriginalView.hpp"', '')),
        ):
            with self.subTest(title=title[-60:], menu=menu[-60:]), self.assertRaises(ValueError):
                probe.make_contexts(title, menu)
        with self.assertRaises(ValueError):
            probe.make_contexts(TITLE, MENU, 'unknown')

    def test_output_escape_and_symlink_are_rejected_before_compilation(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / 'repository'
            analysis = root / '.analysis'
            analysis.mkdir(parents=True)
            outside = Path(directory) / 'outside'
            outside.mkdir()
            (analysis / 'escape').symlink_to(outside, target_is_directory=True)
            accepted = probe.checked_output(analysis / 'new-run', root=root)
            self.assertEqual(accepted, analysis / 'new-run')
            self.assertFalse(accepted.exists())
            for path in (outside, analysis / 'escape' / 'run', analysis / '..' / 'run'):
                with self.subTest(path=path), self.assertRaises(ValueError):
                    probe.checked_output(path, root=root)
