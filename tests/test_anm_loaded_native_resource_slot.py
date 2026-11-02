"""Original-bound 0x1C AnmLoaded resource slot shared by SetSprite and ANM.

Source-level identity checks complement current VC7.1 / original PE COFF
comparison on the complete 9776-byte ExecuteScript and exact callers.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeAnmLoadedResourceSlot(unittest.TestCase):
    def test_native_slot_members_are_original_bound(self):
        header = (ROOT/'src/AnmManager.hpp').read_text()
        set_sprite = (ROOT/'src/AnmLoadedSetSprite.cpp').read_text()
        self.assertIn('class AnmLoaded', header)
        self.assertIn('AnmLoadedNativeSizeIs1C', header)
        for field in ('rawData04','totalEntries08','sprites0C','scripts10',
                      'textures14','postloadEntryNumber18','numberEntriesToBeLoaded18'):
            self.assertIn(field, header)
        for field,offset in (
            ('rawData04','0x04'),('sprites0C','0x0C'),
            ('numberEntriesToBeLoaded18','0x18')):
            self.assertIn('offsetof(AnmLoaded, '+field+') == '+offset,set_sprite)
        self.assertNotIn('struct AnmSetSpriteLoadedView',set_sprite)
        self.assertIn('AnmLoaded *self = this;',set_sprite)
        self.assertIn('self->rawData04 == NULL',set_sprite)
        self.assertIn('self->numberEntriesToBeLoaded18 != 0',set_sprite)
        self.assertIn('target->loadedSprite = &self->sprites0C[spriteIndex];',set_sprite)

    def test_full_original_interpreter_and_leaf_pins_are_unchanged(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units = tomllib.load(f)['units']
        pins = {
            'anm-loaded-set-sprite': (0x00436AC0,189,189,4),
            'anm-manager-execute-script': (0x00436F30,9358,9776,368),
            'anm-manager-draw-player-bullet': (None,84,108,13),
        }
        for unit,(address,size,extent,fields) in pins.items():
            with self.subTest(unit=unit):
                item=units[unit]
                self.assertEqual(item['size'],size)
                self.assertEqual(item.get('compare_size',size),extent)
                self.assertEqual(len(item['relocations']),fields)
                if address is not None:
                    self.assertEqual(item['target_address'],address)


if __name__ == '__main__':
    unittest.main()
