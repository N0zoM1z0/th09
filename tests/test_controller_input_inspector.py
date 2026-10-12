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
