"""Native cursor observer rejects unproven sprite ABI changes, not real native spelling."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('title_cursor_native',(ROOT/'scripts/inspect-title-character-cursors.py'))
reader=importlib.util.module_from_spec(spec)
spec.loader.exec_module(reader)

class TitleNativeCursorCalleeIdentity(unittest.TestCase):
    def test_true_sprite_original_bound_producer(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        self.assertEqual(reader.SPRITE_SYMBOL,u['anm-loaded-set-sprite']['symbol'])
        self.assertEqual(reader.SPRITE_ADDRESS,0x00436AC0)
        reader.require_native_sprite_leaf(u)
        for name,model in reader.OWNERS.items():
            with self.subTest(owner=name):
                count=sum(1 for o,kind,sym,addr in model['fields'] if sym==reader.SPRITE_SYMBOL)
                self.assertEqual(count,2)
                self.assertEqual({addr for o,kind,sym,addr in model['fields'] if sym==reader.SPRITE_SYMBOL},{0x00436AC0})

    def test_corrupt_real_callee_identity_fails_closed(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for key,bad in (('symbol','?FakeSetSprite@TitleAnmView@@QAEXXZ'),
                        ('target_address',0x00436AC4),('size',188),
                        ('source','src/FakeAnmSprite.cpp')):
            forged=deepcopy(u)
            forged['anm-loaded-set-sprite'][key]=bad
            with self.subTest(field=key),self.assertRaises(ValueError):
                reader.require_native_sprite_leaf(forged)

if __name__=='__main__':unittest.main()
