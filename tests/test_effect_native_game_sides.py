"""EffectManager's exact Create uses real two-side GameManager storage.

Target and compiler checks, not a duplicated magic-side shim, establish
physical ownership. The third shared-effect slot is present in setup only.
"""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class EffectNativeGameSides(unittest.TestCase):
    def test_two_playable_side_references_and_untyped_third_record(self):
        src = (ROOT / 'src/EffectManager.cpp').read_text()
        header = (ROOT / 'src/GameplaySetupNativeSides.hpp').read_text()
        self.assertIn('GameplaySetupNativeSides::PlayableCount', src)
        self.assertIn('effectManager->sideState = &g_GameManager.sides[sideIndex]', src)
        self.assertIn('effectManager->opposingSideState = &g_GameManager.sides[1 - sideIndex]', src)
        self.assertIn('sideIndex * sizeof(AsciiGameManagerSideView)', src)
        self.assertNotIn('g_GameManager.sides[GameplaySetupNativeSides::SharedEffectIndex]', src)
        self.assertIn('PlayableCount = sizeof(((AsciiGameManagerView *)0)->sides)', header)
        self.assertIn('SharedEffectIndex = PlayableCount', header)
        self.assertIn('PhysicalCount = sizeof(((GameManagerSetupLayout *)0)->sides)', header)
        self.assertIn('0x7C', header)

    def test_existing_oracle_checks_original_field_bindings(self):
        # Deliberately keep the full target-bound source/profile/COFF auditor
        # as the exactness authority: it validates all 327 Create bytes,
        # original destinations including the +0x38 side offset, and the
        # complete same-source exact cohort. This unit is only the source
        # allocation/ownership cross-check.
        import importlib.util
        import tomllib
        import sys
        path = ROOT / 'scripts/check-boss-frontiers.py'
        spec = importlib.util.spec_from_file_location('th09_frontier_sides', path)
        mod = importlib.util.module_from_spec(spec)
        old_path = list(sys.path)
        try:
            sys.path.insert(0, str(ROOT/'scripts'))
            spec.loader.exec_module(mod)
        finally:
            sys.path[:] = old_path
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        mod.validate_effect_real_game_manager_abi(units)
        self.assertEqual(units['effect-manager-create']['size'], 327)
        sites = mod.EFFECT_GAME_MANAGER_SITES['effect-manager-create'][2]
        self.assertEqual([site[-1] for site in sites], [0x38, 0, 0])


if __name__ == '__main__':
    unittest.main()
