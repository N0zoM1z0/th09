"""Original-bound VM rotation field reused by a leaf and the big ECL interpreter.

Tests guard source/manifest identities. The full original byte/COFF checks
are run separately via the focused owner and single match-unit entrypoints.
"""

from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SharedAnmVmRotationView(unittest.TestCase):
    def test_shared_prefix_has_both_independently_observed_fields(self):
        view = (ROOT / 'src/AnmVmRotationFieldView.hpp').read_text()
        self.assertIn('struct AnmVmRotationFieldView', view)
        self.assertIn('float rotationZ08;', view)
        self.assertIn('unsigned int flags1F8;', view)
        self.assertIn('offsetof(AnmVmRotationFieldView, rotationZ08) == 0x08', view)
        self.assertIn('offsetof(AnmVmRotationFieldView, flags1F8) == 0x1F8', view)
        self.assertNotIn('volatile', view)

    def test_exact_player_leaf_consumes_shared_field_not_a_duplicate(self):
        player = (ROOT / 'src/PlayerAnmVmRuntime.cpp').read_text()
        self.assertIn('#include "PlayerLifecycleView.hpp"', player)
        self.assertNotIn('#include "AnmVmRotationFieldView.hpp"', player)
        self.assertNotIn('struct PlayerAnmVmRotationView', player)
        self.assertIn('PlayerNativeAnmRotationAndFlags', player)
        self.assertIn('offsetof(AnmVm, flagsWord) == 0x1F8', player)
        self.assertIn('rotation.z = angle;', player)
        self.assertIn('flagsWord |= 4;', player)

        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            unit = tomllib.load(stream)['units']['player-anm-vm-set-z-rotation']
        self.assertEqual((unit['target_address'], unit['size'], unit['relocations']),
                         (0x0040F840, 20, []))
        self.assertEqual((unit['source'], unit['object']),
                         ('src/PlayerAnmVmRuntime.cpp',
                          'build/matching/PlayerAnmVmRuntime.obj'))

    def test_big_ecl_late_opcode_uses_same_native_vm_field(self):
        late = (ROOT / 'src/EclRunLate.inl').read_text()
        self.assertIn('typedef char Th09EclLateMainVmRotationAt10[',late)
        self.assertNotIn('#include "AnmVmRotationFieldView.hpp"',late)
        self.assertIn('Th09EclLateMainVmRotationAt10', late)
        self.assertIn('offsetof(EnemyView, primaryAnmVm0008)', late)
        self.assertIn('offsetof(AnmVm, rotation) + offsetof(Float3, z) == 0x0010', late)
        self.assertIn('return enemy->primaryAnmVm0008.rotation.z;',late)
        self.assertNotIn('reinterpret_cast<unsigned char *>(enemy) + 0x0010', late)
        self.assertIn('case TH09_ECL_OPCODE_SET_MAIN_ANM_ROTATION:', late)
        self.assertIn('Th09EclRunLate::MainAnmRotation(enemy) =', late)


if __name__ == '__main__':
    unittest.main()
