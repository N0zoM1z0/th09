"""Ensure Title DrawResult's field inventory never mistakes jump targets for data."""

import importlib.util
from pathlib import Path
import unittest

try:
    import capstone
except ImportError:  # Target-independent public CI excludes site packages.
    capstone = None

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location(
    'title_result_original_fields', ROOT / 'scripts/inspect-title-result-draw.py')
# This inspector uses an existing helper installed on the ordinary script path.
import sys
sys.path.insert(0, str(ROOT / 'scripts'))
inspection = importlib.util.module_from_spec(sp) if capstone is not None else None
if inspection is not None:
    sp.loader.exec_module(inspection)


@unittest.skipIf(capstone is None, "Capstone unavailable under python -S")
class DrawResultOriginalFieldTests(unittest.TestCase):
    def test_absolute_indexed_field_and_direct_call_but_not_near_jump(self):
        # MOV ECX,[0x4a8380]; CALL next; JZ next; MOV EAX,[EAX*4+0x4a8380]; RET
        raw = bytes.fromhex('8b0d80834a00e8000000000f84000000008b048580834a00c3')
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(raw, inspection.BASE))
        self.assertEqual(sum(i.size for i in instructions), len(raw))
        fields = inspection.original_address_fields(instructions)
        self.assertEqual(fields, [
            (2, 'DIR32', 0x004A8380),
            (7, 'REL32', inspection.BASE + 11),
            (20, 'DIR32', 0x004A8380),
        ])

    def test_indirect_call_has_no_verified_target_identity(self):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        instructions = list(decoder.disasm(b'\xff\xd0\xc3', inspection.BASE))
        with self.assertRaises(ValueError):
            inspection.original_address_fields(instructions)


if __name__ == '__main__':
    unittest.main()
