"""Fail-closed actual COFF scalar-definition checks for Enemy OnUpdate.

Synthetic COFF data allows corruption without access to the private TH09 PE.
The pinned complete retail owner still has to pass separately on the real ECL TU.
"""
from pathlib import Path
import importlib.util
import struct
import sys
from types import ModuleType
from unittest import TestCase
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'scripts/inspect-enemy-manager-update.py'
with patch.dict(sys.modules, {'capstone': ModuleType('capstone')}), \
        patch.object(sys, 'path', [str(SOURCE.parent), *sys.path]):
    spec = importlib.util.spec_from_file_location('enemy_literal_gate_tests', SOURCE)
    enemy = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(enemy)


class OriginalPayload:
    def __init__(self, data):
        self.by_va = {va: struct.pack('<I', int(name[7:], 16))
                      for _, name, va in enemy.ENEMY_LITERAL_FIELDS}

    def pe_bytes_at(self, image, va, count):
        assert count == 4
        return self.by_va[va]


class EnemyScalarDefinitionTests(TestCase):
    def fixtures(self):
        count = len(enemy.ENEMY_LITERAL_FIELDS)
        data = bytearray(80 + 4 * count)
        struct.pack_into('<HH', data, 0, 0x14C, 1)
        struct.pack_into('<II', data, 36, 4 * count, 80)
        struct.pack_into('<I', data, 56, 0x40000040)
        fields = []
        for i, (off, symbol, _) in enumerate(enemy.ENEMY_LITERAL_FIELDS):
            struct.pack_into('<I', data, 80 + 4*i, int(symbol[7:], 16))
            fields.append(dict(offset=off, symbol=symbol, addend=0,
                               symbol_section=1, symbol_value=4*i))
        return data, fields, OriginalPayload(data)

    def test_five_genuine_scalar_source_definitions_match(self):
        data, fields, original = self.fixtures()
        result = enemy.verify_enemy_literal_definitions(bytes(data), fields, None, original)
        self.assertEqual(len(result), 5)
        self.assertEqual([(x['references'], x['size']) for x in result],
                         [([off], 4) for off, _, _ in enemy.ENEMY_LITERAL_FIELDS])
        self.assertEqual(result[0]['symbol'], '__real@3dcccccd')
        self.assertEqual(result[-1]['symbol'], '__real@42000000')

    def test_flipped_data_and_corrupt_terminator_equivalents_are_rejected(self):
        data, fields, original = self.fixtures()
        for i in range(len(enemy.ENEMY_LITERAL_FIELDS)):
            tampered = bytearray(data)
            tampered[80 + 4*i] ^= 1
            with self.subTest(i=i), self.assertRaisesRegex(
                    ValueError, 'literal definition differs'):
                enemy.verify_enemy_literal_definitions(tampered, fields, None, original)

    def test_field_order_missing_retargeted_or_non_readonly_data_rejected(self):
        data, fields, original = self.fixtures()
        variants = [fields[:-1], fields + fields[:1],
                    [dict(fields[0], offset=fields[0]['offset']+1), *fields[1:]],
                    [dict(fields[0], symbol='__real@3dccccce'), *fields[1:]],
                    list(reversed(fields)),
                    [dict(fields[0], symbol_value=4), *fields[1:]]]
        for rows in variants:
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                enemy.verify_enemy_literal_definitions(data, rows, None, original)
        for flags in (0xC0000040, 0x40000020, 0, 0x80000040):
            broken = bytearray(data)
            struct.pack_into('<I', broken, 56, flags)
            with self.subTest(flags=hex(flags)), self.assertRaises(ValueError):
                enemy.verify_enemy_literal_definitions(broken, fields, None, original)

    def test_original_float_payload_matters_even_if_candidate_is_unchanged(self):
        data, fields, original = self.fixtures()
        original.by_va[enemy.ENEMY_LITERAL_FIELDS[2][2]] = bytes.fromhex('01000000')
        with self.assertRaises(ValueError):
            enemy.verify_enemy_literal_definitions(data, fields, None, original)
