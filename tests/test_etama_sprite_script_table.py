"""Original Etama sprite-template data and relocation shape regressions.

These are pure-input negative controls. A genuine compiled object is checked
against all original PE bytes by scripts/inspect-etama-sprite-script-table.py.
"""
import hashlib
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'etama_sprite_original_table', ROOT / 'scripts/inspect-etama-sprite-script-table.py')
assert spec and spec.loader
reader = importlib.util.module_from_spec(spec)
spec.loader.exec_module(reader)


class MockOriginal:
    @staticmethod
    def pe_bytes_at(image, address, size):
        assert size == 4
        for offset, addend in reader.TABLE_FIELD_SITES:
            if address == reader.TARGET_CALLER + offset:
                return (reader.TABLE_VA + addend).to_bytes(4, 'little')
        raise AssertionError(f'new or missing original table field {address:#x}')


class EtamaOriginalSpriteScriptTable(unittest.TestCase):
    @staticmethod
    def rows():
        return [dict(offset=offset, type='DIR32', addend=addend,
                     symbol=reader.TABLE_SYMBOL, symbol_section=3, symbol_value=0)
                for offset, addend in reader.TABLE_FIELD_SITES]

    def test_all_seven_original_pointer_roles_are_checked(self):
        rows = self.rows()
        self.assertEqual(len(reader.verify_references(rows, None, MockOriginal)), 7)
        for index in range(7):
            for field, corrupt in (('offset', 0x600), ('addend', 0x100),
                                   ('type', 'REL32'), ('symbol', '?Unreviewed'),
                                   ('symbol_section', 0), ('symbol_value', -1)):
                with self.subTest(index=index, field=field), self.assertRaises(ValueError):
                    candidate = [dict(row) for row in rows]
                    candidate[index][field] = corrupt
                    reader.verify_references(candidate, None, MockOriginal)
        with self.assertRaises(ValueError):
            reader.verify_references(rows[:-1], None, MockOriginal)

    def test_every_original_table_byte_and_extent_are_required(self):
        base = bytes((i * 13) & 255 for i in range(reader.TABLE_SIZE))
        digest = hashlib.sha256(base).hexdigest()
        reader.verify_payload(base, base, original_hash=digest)
        for i in (0, 79, 300, reader.TABLE_SIZE - 1):
            with self.subTest(byte=i), self.assertRaises(ValueError):
                candidate = bytearray(base)
                candidate[i] ^= 1
                reader.verify_payload(base, bytes(candidate), original_hash=digest)
        for payload in (base[:-1], base + b'\x00'):
            with self.assertRaises(ValueError):
                reader.verify_payload(base, payload, original_hash=digest)
        with self.assertRaises(ValueError):
            reader.verify_payload(base, base)

    def test_actual_native_table_has_23_rows_and_controller_allocates_32_slots(self):
        source = (ROOT / 'src/EtamaControllerDraw.cpp').read_text()
        native = (ROOT / 'src/BulletManager.hpp').read_text()
        self.assertIn('kBulletSpriteScripts[23][5]', source)
        self.assertIn('BulletTypeSprites bulletTypeSprites[32]', native)
        self.assertIn('sizeof(kBulletSpriteScripts) / sizeof(kBulletSpriteScripts[0])', source)
        self.assertNotIn('for (unsigned int i = 0; i < 23; ++i)', source)
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        unit = units['etama-controller-added-callback']
        self.assertEqual((unit['target_address'], unit['size'], unit['compare_size']),
                         (reader.TARGET_CALLER, 598, reader.CALLER_PHYSICAL_SIZE))
        self.assertEqual([r['offset'] for r in unit['relocations']
                          if r['symbol'] == reader.TABLE_SYMBOL],
                         [offset for offset, _ in reader.TABLE_FIELD_SITES])


if __name__ == '__main__':
    unittest.main()
