"""Reject lost native Timer ownership even when a numeric call could bind."""
import ast
import copy
import hashlib
from pathlib import Path
import struct
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
tree = ast.parse((ROOT / 'scripts/inspect-enemy-manager-update.py').read_text())
names = {'BASE', 'TIMER_GREATER_SYMBOL', 'TIMER_GREATER_FIELDS', 'TIMER_GREATER_SHA256'}
nodes = [node for node in tree.body
         if isinstance(node, ast.FunctionDef) and node.name == 'require_timer_greater_definition'
         or isinstance(node, ast.Assign) and any(
             isinstance(target, ast.Name) and target.id in names for target in node.targets)]
scope = {'hashlib': hashlib, 'struct': struct}
exec(compile(ast.Module(body=nodes, type_ignores=[]), '<native-timer-guard>', 'exec'), scope)


class EnemyTimerGreaterContextTests(unittest.TestCase):
    def setUp(self):
        self.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
        self.target = bytearray(3900)
        self.rows = []
        for offset in scope['TIMER_GREATER_FIELDS']:
            self.target[offset-1] = 0xE8
            struct.pack_into('<i', self.target, offset, 0x403DE0-(scope['BASE']+offset+4))
            self.rows.append(dict(offset=offset, symbol=scope['TIMER_GREATER_SYMBOL'],
                                  type='REL32', addend=0, symbol_section=7,
                                  symbol_value=0, owner_section=9))
        # Synthetic body tests the contract without distributing target code.
        self.body = bytes(range(20))
        scope['TIMER_GREATER_SHA256'] = hashlib.sha256(self.body).hexdigest()

    def check(self, rows=None, units=None, body=None, fields=None, target=None):
        return scope['require_timer_greater_definition'](
            self.rows if rows is None else rows, self.units if units is None else units,
            self.body if body is None else body, [] if fields is None else fields,
            self.target if target is None else target)

    def test_complete_native_source_body_and_four_calls_are_required(self):
        proof = self.check()
        self.assertEqual((proof['calls'], proof['bytes'], proof['fields']), (4, 20, 0))
        self.assertTrue(proof['same_object_definition'])
        for body, fields in ((self.body[:-1], []), (self.body + b'\0', []),
                             (bytes(20), []), (self.body, [{'offset': 0}])):
            with self.subTest(body=body, fields=fields), self.assertRaises(ValueError):
                self.check(body=body, fields=fields)

    def test_call_aliases_offsets_and_definition_provenance_fail_closed(self):
        for index in range(4):
            for changes in ({'symbol': '??OGuessedTimer@@QAEIH@Z'},
                            {'offset': self.rows[index]['offset']+1},
                            {'type': 'DIR32'}, {'addend': 4},
                            {'symbol_section': 0}, {'symbol_section': 9},
                            {'symbol_section': 8}, {'symbol_value': 4}):
                rows = copy.deepcopy(self.rows)
                rows[index].update(changes)
                with self.subTest(index=index, changes=changes), self.assertRaises(ValueError):
                    self.check(rows=rows)
        for rows in (self.rows[:-1], self.rows[::-1], self.rows + [self.rows[0]]):
            with self.assertRaises(ValueError):
                self.check(rows=rows)

    def test_native_unit_and_original_call_operands_are_independent(self):
        for changes in ({'source': 'src/ZunTimer.cpp'}, {'size': 19},
                        {'target_address': 0x403DE1}, {'profile': ['/Ob0']},
                        {'symbol': '??OGuessedTimer@@QAEIH@Z'},
                        {'object': 'build/matching/ZunTimer.obj'}):
            units = copy.deepcopy(self.units)
            units['zun-timer-greater'].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                self.check(units=units)
        for offset in scope['TIMER_GREATER_FIELDS']:
            target = self.target.copy()
            target[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                self.check(target=target)

    def test_shifted_positive_call_is_not_a_missing_native_definition(self):
        rows = copy.deepcopy(self.rows)
        rows[-1]['offset'] -= 8
        with self.assertRaisesRegex(ValueError, 'original call offset changed'):
            self.check(rows=rows)


if __name__ == '__main__':
    unittest.main()
