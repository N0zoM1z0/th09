import importlib.util
from pathlib import Path
import struct
import unittest
from unittest import mock


ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "coff_compare", ROOT / "scripts/compare-coff-function.py")
coff = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coff)


class EclInterpolationTableTests(unittest.TestCase):
    def test_all_slots_require_their_actual_callback_relocation(self):
        linear, hermite = coff.ECL_INTERPOLATION_UNITS
        units = {
            linear: {"symbol": "Linear", "target_address": 0x407450},
            hermite: {"symbol": "Hermite", "target_address": 0x4074A0},
        }
        table = struct.pack("<8I", *([0x407450] * 7 + [0x4074A0]))
        sections = [
            dict(raw_offset=1, size=32, flags=0x40, reloc_offset=33, reloc_count=8),
            dict(flags=0x20),
        ]
        symbols = {
            0: dict(name=coff.ECL_INTERPOLATION_TABLE, section=1, value=0),
            1: dict(name="Linear", section=2, value=0),
            2: dict(name="Hermite", section=2, value=0),
        }
        prefix = b"\0" * 33 + b"".join(struct.pack("<IIH", offset, 1, 6)
                                        for offset in range(0, 28, 4))
        for last, rejection in [
            ((28, 2, 6), None),
            ((28, 1, 6), "callback differs"),
            ((24, 2, 6), "one COFF relocation per"),
            ((28, 2, 0x14), "invalid ECL interpolation table relocation"),
        ]:
            with self.subTest(last=last), mock.patch.multiple(
                coff,
                read_coff_object=lambda path: (prefix + struct.pack("<IIH", *last),
                                               sections, symbols),
                pe_bytes_at=lambda image, address, size: table,
            ):
                if rejection:
                    with self.assertRaisesRegex(ValueError, rejection):
                        coff.verify_ecl_interpolation_table(Path("unused.obj"), b"", units)
                else:
                    report = coff.verify_ecl_interpolation_table(Path("unused.obj"), b"", units)
                    self.assertEqual(report["size"], 32)
                    self.assertEqual(report["relocations"], 8)
