"""ANM reader filename belongs to slot, not to the 0x1C resource."""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class NativeAnmReadSlotPath(unittest.TestCase):
    def test_slot_layout_and_shared_path_owner(self):
        source=(ROOT/'src/AnmManagerLoad.cpp').read_text()
        self.assertIn('AnmManagerReadSlotView *slot = &manager->slots[anmIdx];',source)
        self.assertIn('AnmLoaded *anmLoaded = &slot->loaded;',source)
        self.assertIn('strcpy(slot->path, filename);',source)
        self.assertNotIn('strcpy(reinterpret_cast<char *>(anmLoaded) + 0x20, filename)',source)
        self.assertIn('sizeof(AnmManagerReadSlotView) == 0x120',source)
        self.assertIn('offsetof(AnmManagerReadSlotView, releasePending) == 0x1C',source)
        self.assertIn('offsetof(AnmManagerReadSlotView, path) == 0x20',source)
        self.assertIn('sizeof(AnmLoaded) == 0x1C',source)

    def test_complete_target_owner_scope(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        reader=units['anm-manager-read-anm-entries']
        self.assertEqual((reader['target_address'],reader['size']),(0x43C610,390))
        self.assertEqual(len(reader['relocations']),17)
        source_units=[(name,u) for name,u in units.items()
                      if u.get('source')=='src/AnmManagerLoad.cpp']
        self.assertEqual(len(source_units),17)
        self.assertEqual(sum(u['object']=='build/matching/AnmManagerLoad.obj'
                             for _,u in source_units),16)
        self.assertEqual(sum(u['object']=='build/matching/AnmManagerCtor.obj'
                             for _,u in source_units),1)

if __name__=='__main__':
    unittest.main()
