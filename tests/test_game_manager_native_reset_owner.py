"""Exact GameManager reset owner reuses canonical 0x390-byte storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class GameManagerNativeResetOwner(unittest.TestCase):
    def test_reset_class_is_zero_extra_storage_native_method_receiver(self):
        owner=(ROOT/'src/GameManagerResetView.hpp').read_text()
        native=(ROOT/'src/GameManagerRuntimeView.hpp').read_text()
        self.assertIn('struct GameManagerResetView : AsciiGameManagerView',owner)
        self.assertNotIn('unsigned char unknown000[0x13F];',owner)
        self.assertIn('sizeof(GameManagerResetView) == sizeof(AsciiGameManagerView)',owner)
        self.assertIn('sizeof(GameManagerResetView) == 0x390',owner)
        self.assertIn('union { unsigned char unknown13F; unsigned char resetFlag13F; };',native)
        self.assertIn('offsetof(GameManagerResetView, resetFlag13F) == 0x13F',owner)
        for field,offset in (('coordinateOffsetX','0x350'),('coordinateOffsetY','0x354'),
                             ('coordinate36C','0x36C'),('value378','0x378'),('value37C','0x37C')):
            with self.subTest(field=field):
                self.assertIn('offsetof(GameManagerResetView, '+field+') == '+offset,owner)

    def test_exact_original_two_member_owners_preserve_source_identity(self):
        setup=(ROOT/'src/GameManagerCoordinateBounds.cpp').read_text()
        reset=(ROOT/'src/GameManagerSupervisorReset.cpp').read_text()
        self.assertIn('void GameManagerResetView::InitializeCoordinateBounds()',setup)
        self.assertIn('void GameManagerResetView::ResetForSupervisorAdded()',reset)
        self.assertIn('coordinateOffsetX = -144.0f;',setup)
        self.assertIn('coordinateOffsetY = 0.0f;',setup)
        self.assertIn('resetFlag13F = 0;',reset)
        self.assertIn('memset(this, 0, sizeof(*this));',reset)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        for n,va,sz in (('game-manager-initialize-coordinate-bounds',0x41A9B8,79),
                        ('game-manager-reset-for-supervisor-added',0x41BA4F,47)):
            with self.subTest(unit=n):
                r=u[n]
                self.assertEqual((r['target_address'],r['size']),(va,sz))

if __name__=='__main__':unittest.main()
