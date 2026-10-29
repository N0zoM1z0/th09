"""A shared numeric address cannot replace the two real Enemy callback bodies."""
import copy
import struct
from unittest import TestCase, main

from test_enemy_manager_update_bindings import inspector


class EnemyFoldedCallbackTests(TestCase):
    def setUp(self):
        self.target = bytearray(inspector.OWNER_SIZE)
        self.rows = []
        self.definitions = {}
        for index, (offset, symbol) in enumerate(inspector.FOLDED_CALLBACK_CALLS):
            self.target[offset-1] = 0xE8
            struct.pack_into('<i', self.target, offset,
                             inspector.FOLDED_CALLBACK_ADDRESS-(inspector.BASE+offset+4))
            self.rows.append(dict(offset=offset, symbol=symbol, type='REL32', addend=0,
                                  symbol_section=index+2, symbol_value=0, owner_section=1))
            self.definitions[symbol] = dict(body=inspector.FOLDED_CALLBACK_BODY,
                                            fields=[], same_object=True)

    def check(self, *, rows=None, definitions=None, source=None, target=None, original=None):
        return inspector.require_folded_callback_definitions(
            self.rows if rows is None else rows,
            self.target if source is None else source,
            self.target if target is None else target,
            inspector.FOLDED_CALLBACK_BODY if original is None else original,
            self.definitions if definitions is None else definitions)

    def test_native_bodies_are_checked_without_borrowing_player_ownership(self):
        proof = self.check()
        self.assertEqual(proof['address'], 0x44AB20)
        self.assertEqual(len(proof['callbacks']), 2)
        self.assertEqual(proof['exactness_credit'], 'none')
        for row in self.rows:
            self.assertNotIn(row['symbol'], inspector.unit_aliases)
            address, basis = inspector.external_destination(row, {}, {}, {})
            self.assertEqual(address, 0x44AB20)
            self.assertIn('shared zero callback definition', basis)

    def test_explicit_external_definitions_preserve_visibility_distinction(self):
        rows = copy.deepcopy(self.rows)
        definitions = copy.deepcopy(self.definitions)
        rows[0]['symbol_section'] = 0
        symbol = rows[0]['symbol']
        with self.assertRaises(ValueError):
            self.check(rows=rows, definitions=definitions)
        definitions[symbol]['same_object'] = False
        proof = self.check(rows=rows, definitions=definitions)
        self.assertFalse(proof['callbacks'][0]['same_object_definition'])
        self.assertTrue(proof['callbacks'][1]['same_object_definition'])

    def test_real_shifted_call_is_reported_without_deriving_its_identity_from_target(self):
        rows = copy.deepcopy(self.rows)
        source = self.target.copy()
        rows[1]['offset'] += 2
        source[rows[1]['offset']-1] = 0xE8
        proof = self.check(rows=rows, source=source)
        self.assertEqual(proof['callbacks'][1]['call_offset'], 0x701)
        self.assertEqual(proof['callbacks'][1]['original_call_offset'], 0x6FF)

    def test_wrong_missing_or_incomplete_actual_callee_is_rejected(self):
        for symbol in self.definitions:
            for edits in ({'body': b'\x33\xc0'}, {'body': b'\x33\xc0\xc3\x90'},
                          {'body': b'\x33\xc9\xc3'}, {'fields': [{'offset': 0}]},
                          {'same_object': False}, {'same_object': 1}):
                definitions = copy.deepcopy(self.definitions)
                definitions[symbol].update(edits)
                with self.subTest(symbol=symbol, edits=edits), self.assertRaises(ValueError):
                    self.check(definitions=definitions)
            definitions = copy.deepcopy(self.definitions)
            del definitions[symbol]
            with self.assertRaises(ValueError):
                self.check(definitions=definitions)

    def test_call_identity_and_original_operands_are_independent(self):
        for index in range(2):
            for edits in ({'symbol': '?CheckState2@PlayerView@@QAEHXZ'},
                          {'offset': self.rows[index]['offset']+1}, {'type': 'DIR32'},
                          {'addend': 4}, {'addend': False}, {'symbol_section': -1},
                          {'symbol_section': True}, {'symbol_section': 1},
                          {'symbol_value': 1}, {'symbol_value': False},
                          {'owner_section': 0}, {'owner_section': True}):
                rows = copy.deepcopy(self.rows)
                rows[index].update(edits)
                with self.subTest(index=index, edits=edits), self.assertRaises(ValueError):
                    self.check(rows=rows)
        for rows in (self.rows[:-1], self.rows[::-1], self.rows+self.rows[:1]):
            with self.assertRaises(ValueError):
                self.check(rows=rows)
        for offset, _ in inspector.FOLDED_CALLBACK_CALLS:
            for changed_offset in (offset-1, offset):
                target = self.target.copy()
                target[changed_offset] ^= 1
                with self.assertRaises(ValueError):
                    self.check(target=target)
        with self.assertRaises(ValueError):
            self.check(target=self.target[:-1])
        with self.assertRaises(ValueError):
            self.check(original=b'\x33\xc9\xc3')


if __name__ == '__main__':
    main()
