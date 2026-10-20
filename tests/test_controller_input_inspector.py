"""Target-independent guards for the full Controller input diagnostic."""

import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'controller_input_oracle', ROOT / 'scripts/inspect-controller-input.py')
INSPECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(INSPECT)


class ControllerInputManifestTests(unittest.TestCase):
    def native_rows(self):
        return [dict(offset=offset, symbol=symbol, addend=addend,
                     type='DIR32', symbol_section=0, symbol_value=0)
                for offset, symbol, addend in INSPECT.NATIVE_GLOBAL_FIELDS]

    def test_native_fields_require_complete_shared_symbol_identities(self):
        rows = self.native_rows()
        self.assertEqual(INSPECT.validate_native_globals(rows), 9)
        variants = [rows[:-1], rows + [rows[0]]]
        for key, value in (
                ('symbol', '?g_Supervisor@@3UControllerSupervisorInputView@@A'),
                ('offset', 0x0B), ('addend', 0x18)):
            variants.append([dict(rows[0], **{key: value})] + rows[1:])
        for bad in variants:
            with self.subTest(rows=bad):
                with self.assertRaisesRegex(ValueError, 'native global field identities'):
                    INSPECT.validate_native_globals(bad)

    def test_native_fields_require_actual_undefined_dir32_provenance(self):
        rows = self.native_rows()
        for key, value in (('type', 'REL32'), ('symbol_section', 1),
                           ('symbol_value', 4), ('symbol_section', None),
                           ('symbol_value', None)):
            with self.subTest(key=key, value=value):
                bad = [dict(rows[0], **{key: value})] + rows[1:]
                with self.assertRaisesRegex(ValueError, 'undefined DIR32'):
                    INSPECT.validate_native_globals(bad)

    def test_target_manifest_has_four_distinct_axis_members(self):
        INSPECT.validate_manifest()
        members = [(pos, addr) for pos, kind, addr in INSPECT.TARGET_FIELDS
                   if addr in (0x4AC900, 0x4AC8FC, 0x4AC908, 0x4AC904)]
        self.assertEqual(members, [
            (0x2AD, 0x4AC900), (0x2B3, 0x4AC8FC),
            (0x2DF, 0x4AC908), (0x2E5, 0x4AC904)])

    def test_truncated_or_invented_coff_fields_fail_closed(self):
        base = dict(offset=0, type='DIR32',
                    symbol='?g_JoystickCaps@Controller@@3PAUtagJOYCAPSA@@A',
                    addend=40)
        with self.assertRaisesRegex(ValueError, 'all 32 fields'):
            INSPECT.bind_candidate(bytes(200), [base] * 31)
        with self.assertRaisesRegex(ValueError, 'unknown Controller external'):
            INSPECT.bind_candidate(bytes(200),
                                   [dict(base, symbol='?Invented@@3PA') for _ in range(32)])
        with self.assertRaisesRegex(ValueError, 'out of bounds or overlapping|outside candidate extent'):
            INSPECT.bind_candidate(bytes(200), [base for _ in range(32)])

    def test_unreviewed_axis_field_fails_closed(self):
        row = dict(offset=0, type='DIR32',
                   symbol='?g_JoystickCaps@Controller@@3PAUtagJOYCAPSA@@A',
                   addend=0x34)
        fields = [dict(row, offset=4*i) for i in range(32)]
        with self.assertRaisesRegex(ValueError, 'unreviewed JOYCAPSA'):
            INSPECT.bind_candidate(bytes(132), fields)


if __name__ == '__main__':
    unittest.main()
