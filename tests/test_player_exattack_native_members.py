"""Keep recovered native Player timer/header and ExAttack alpha storage."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativePlayerExAttackMembers(unittest.TestCase):
    def test_original_native_storage_paths(self):
        register = (ROOT / 'src/PlayerLifecycle.cpp').read_text()
        added = (ROOT / 'src/PlayerAddedState.cpp').read_text()
        attack = (ROOT / 'src/ExAttackDrawType19_21.cpp').read_text()
        self.assertIn('player->timer303C8 = 0;', register)
        self.assertIn('offsetof(PlayerLifecycleView, timer303C8) == 0x303C8', register)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x303C8', register)
        self.assertIn('this->header24.Reset();', added)
        self.assertNotIn('PlayerAddedHeaderResetView', added)
        self.assertIn('offsetof(PlayerLifecycleView, header24) == 0x24', added)
        self.assertNotIn('reinterpret_cast<unsigned char *>(player) + 0x24', added)
        self.assertIn('record->vms1C->color1Script.a', attack)
        self.assertIn('offsetof(AnmVmScriptColor, a) == 0x1F3', attack)
        self.assertNotIn('reinterpret_cast<unsigned char *>(record->vms1C) + 0x1F3', attack)

    def test_canonical_owner_extents_and_internal_label_referents(self):
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        for name, extent in (
            ('player-register-chain', 328),
            ('player-release', 86),
            ('player-release-owned-resources', 75),
            ('player-initialize-added-state', 572),
            ('player-added-header-reset', 29),
            ('player-added-collision-reset', 23),
            ('player-added-owner-reset', 41),
            ('exattack-type19-21-draw', 318),
        ):
            with self.subTest(unit=name):
                item = units[name]
                self.assertEqual(item.get('compare_size', item['size']), extent)
        initializer = units['player-initialize-added-state']
        self.assertEqual(len(initializer['relocations']), 25)
        self.assertEqual([(e['offset'], e['target']) for e in initializer['relocations']
                          if e['symbol'].startswith('$L')],
                         [(0x1ad, 0x41ede8), (0x228, 0x41ed71),
                          (0x22c, 0x41ed71), (0x230, 0x41ed74),
                          (0x234, 0x41ed79), (0x238, 0x41ed7e)])


if __name__ == '__main__':
    unittest.main()
