"""Full charge operand binding rejects false provenance without private inputs."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'charge_complete', ROOT / 'scripts/inspect-player-charge-update.py')
oracle = importlib.util.module_from_spec(spec)
spec.loader.exec_module(oracle)


class ChargeCompleteTests(unittest.TestCase):
    def fixture(self, shift=0):
        target, source, rows = bytearray(oracle.SIZE), bytearray(oracle.SIZE + shift), []
        for off, kind, destination in oracle.TARGET_FIELDS:
            names = [name for name, root in oracle.REFERENTS.items()
                     if root + oracle.source_addend(name) == destination]
            self.assertEqual(len(names), 1)
            name = names[0]
            addend = oracle.source_addend(name)
            value = destination - (oracle.BASE + off + 4 if kind == 'REL32' else 0)
            struct.pack_into('<I', target, off, value & 0xFFFFFFFF)
            struct.pack_into('<I', source, off + shift, addend)
            rows.append(dict(offset=off + shift, type=kind,
                             type_id=6 if kind == 'DIR32' else 20,
                             symbol=name, addend=addend))
        return source, rows, target

    def test_complete_inventory_and_rebasing_use_source_operand_positions(self):
        source, rows, target = self.fixture(8)
        with patch.object(oracle, 'TARGET_SHA256', hashlib.sha256(target).hexdigest()):
            linked = oracle.bind_candidate(source, rows, target)
        self.assertEqual(len(rows), 70)
        self.assertEqual(sum(r['type'] == 'REL32' for r in rows), 58)
        for row, (_, kind, destination) in zip(rows, oracle.TARGET_FIELDS):
            value = destination - (oracle.BASE + row['offset'] + 4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', linked, row['offset'])[0],
                             value & 0xFFFFFFFF)

    def test_wrong_target_extent_or_digest_stops_binding(self):
        source, rows, target = self.fixture()
        for invalid in (target, target[:-1], target + b'\0'):
            with self.subTest(bytes=len(invalid)), self.assertRaisesRegex(ValueError, 'original'):
                oracle.bind_candidate(source, rows, invalid)

    def test_missing_duplicate_mistyped_retargeted_or_misencoded_fields_fail(self):
        source, rows, target = self.fixture()
        changes = [rows[:-1]]
        for change in (
            {'symbol': '?g_SoundPlayer@@3VSoundPlayer@@A'},
            {'type': 'REL32'}, {'type_id': 20}, {'addend': 1},
            {'offset': True}, {'offset': len(source)},
        ):
            copy = [dict(r) for r in rows]
            copy[0].update(change)
            changes.append(copy)
        duplicate = [dict(r) for r in rows]
        duplicate[1]['offset'] = duplicate[0]['offset']
        changes.append(duplicate)
        with patch.object(oracle, 'TARGET_SHA256', hashlib.sha256(target).hexdigest()):
            for changed in changes:
                with self.subTest(changed=changed[0]), self.assertRaises(ValueError):
                    oracle.bind_candidate(source, changed, target)
            wrong_raw = bytearray(source)
            wrong_raw[rows[0]['offset']] ^= 1
            with self.assertRaisesRegex(ValueError, 'raw field'):
                oracle.bind_candidate(wrong_raw, rows, target)

    def test_original_encoded_operand_is_checked_separately_from_digest(self):
        source, rows, target = self.fixture()
        target[oracle.TARGET_FIELDS[0][0]] ^= 1
        with (patch.object(oracle, 'TARGET_SHA256', hashlib.sha256(target).hexdigest()),
              self.assertRaisesRegex(ValueError, 'original operand')):
            oracle.bind_candidate(source, rows, target)

    def test_void_pointer_return_aliases_cannot_replace_native_effect_returns(self):
        source, rows, target = self.fixture()
        for effect in (13, 14, 15):
            native = f'?ConfigureState3Effect{effect}@PlayerStateEffectView@@QAEPAUEffect@@XZ'
            changed = [dict(row) for row in rows]
            calls = [row for row in changed if row['symbol'] == native]
            self.assertEqual(len(calls), 1)
            calls[0]['symbol'] = native.replace('PAUEffect@@', 'PAX')
            with (self.subTest(effect=effect),
                  patch.object(oracle, 'TARGET_SHA256', hashlib.sha256(target).hexdigest()),
                  self.assertRaisesRegex(ValueError, 'symbol')):
                oracle.bind_candidate(source, changed, target)

    def test_literal_uses_real_bounded_section_definition(self):
        payload = struct.pack('<I', 0x42C80000)
        data = bytearray(64)
        struct.pack_into('<HH', data, 0, 0x14C, 1)
        struct.pack_into('<II', data, 36, 4, 60)
        data[60:64] = payload
        row = dict(symbol_section=1, symbol_value=0)
        self.assertEqual(oracle.literal_payload(data, row), payload)
        for invalid in (dict(symbol_section=0, symbol_value=0),
                        dict(symbol_section=2, symbol_value=0),
                        dict(symbol_section=1, symbol_value=1)):
            with self.subTest(row=invalid), self.assertRaises(ValueError):
                oracle.literal_payload(data, invalid)
        for cut in (0, 19, 59, 63):
            with self.subTest(cut=cut), self.assertRaises(ValueError):
                oracle.literal_payload(data[:cut], row)
        struct.pack_into('<I', data, 40, 0)
        with self.assertRaisesRegex(ValueError, 'section'):
            oracle.literal_payload(data, row)


if __name__ == '__main__':
    unittest.main()
