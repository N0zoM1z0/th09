"""Reject wrong native identities, incomplete slots and unsupported table provenance."""
import copy
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('interpolation_table', ROOT/'scripts/inspect-ecl-interpolation-table.py')
m = importlib.util.module_from_spec(spec)
spec.loader.exec_module(m)


def fixture():
    units = {}
    for i, (address, name) in enumerate(m.SLOTS):
        units[name] = dict(target_address=address, size=74 if i < 7 else 188,
                           source='src/EclHelpers.cpp',
                           symbol='?Interpolate'+('Linear' if i < 7 else 'Hermite')+'@?A0x1234@@YIXPAUEnemyView@@PAUTh09EclInterpolationSlotView@@M@Z')
    fields = [dict(offset=i*4, type_id=6, symbol=units[name]['symbol'], addend=0,
                   symbol_section=10 if i < 7 else 11, symbol_value=0,
                   symbol_type=0x20, storage=2) for i, (_, name) in enumerate(m.SLOTS)]
    return bytes(32), fields, units, struct.pack('<8I', *(x[0] for x in m.SLOTS))


class InterpolationTable(unittest.TestCase):
    def test_all_eight_reviewed_slots_require_same_object_native_functions(self):
        self.assertEqual(len(m.validate_binding(*fixture())), 8)

    def test_missing_extra_duplicate_shifted_or_wrong_kind_fields(self):
        payload, fields, units, original = fixture()
        cases = [fields[:-1], fields+fields[:1]]
        for change in (dict(offset=4), dict(type_id=20), dict(addend=1), dict(addend=False),
                       dict(symbol_section=0), dict(symbol_section=False), dict(symbol_value=4),
                       dict(symbol_type=0), dict(storage=3)):
            cases.append([{**fields[0], **change}, *fields[1:]])
        for changed in cases:
            with self.subTest(changed=changed[0]), self.assertRaises(ValueError):
                m.validate_binding(payload, changed, units, original)

    def test_correct_numeric_address_cannot_substitute_other_native_identity(self):
        payload, fields, units, original = fixture()
        for symbol in (fields[-1]['symbol'], '?PrivateFacade@@YIXPAX0M@Z'):
            changed = copy.deepcopy(fields); changed[0]['symbol'] = symbol
            with self.subTest(symbol=symbol), self.assertRaises(ValueError):
                m.validate_binding(payload, changed, units, original)
        for change in (dict(target_address=0x4074A0), dict(size=73), dict(source='src/Other.cpp')):
            changed = copy.deepcopy(units); changed[m.SLOTS[0][1]].update(change)
            with self.subTest(change=change), self.assertRaises(ValueError):
                m.validate_binding(payload, fields, changed, original)

    def test_changed_abi_in_both_manifest_and_fields_does_not_define_a_native_alias(self):
        payload, fields, units, original = fixture()
        wrong = fields[0]['symbol'].replace('@@YIX', '@@YIH')
        units[m.SLOTS[0][1]]['symbol'] = wrong
        for row in fields[:7]:
            row['symbol'] = wrong
        with self.assertRaises(ValueError):
            m.validate_binding(payload, fields, units, original)

    def test_payload_prefix_extent_and_target_order_cannot_be_normalized(self):
        payload, fields, units, original = fixture()
        for changed in (payload[:-1], payload+bytes(4), b'\x01'+payload[1:]):
            with self.assertRaises(ValueError):
                m.validate_binding(changed, fields, units, original)
        for changed in (original[:-1], original[4:]+original[:4], bytes(32)):
            with self.assertRaises(ValueError):
                m.validate_binding(payload, fields, units, changed)


if __name__ == '__main__':
    unittest.main()
