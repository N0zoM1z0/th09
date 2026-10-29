"""Target, mapped-byte and selection checks for direct Ghidra exports."""
import copy
import hashlib
import importlib.util
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
spec = importlib.util.spec_from_file_location('direct_ghidra', ROOT / 'scripts/ghidra-analysis.py')
ghidra = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ghidra)


def fixture():
    image = bytearray(0x220)
    image[:2] = b'MZ'
    struct.pack_into('<I', image, 0x3C, 0x80)
    image[0x80:0x84] = b'PE\0\0'
    struct.pack_into('<HH', image, 0x84, 0x14C, 1)
    struct.pack_into('<H', image, 0x94, 224)
    struct.pack_into('<H', image, 0x98, 0x10B)
    struct.pack_into('<I', image, 0x98 + 16, 0x1000)
    struct.pack_into('<I', image, 0x98 + 28, 0x400000)
    struct.pack_into('<I', image, 0x98 + 56, 0x2000)
    struct.pack_into('<I', image, 0x98 + 60, 0x200)
    image[0x178:0x180] = b'.text\0\0\0'
    struct.pack_into('<IIII', image, 0x180, 0x20, 0x1000, 0x20, 0x200)
    image[0x200:] = bytes(range(32))
    ranges = ghidra.file_ranges(image)
    for item in ranges:
        start, size = item['file_offset'], item['length']
        item['sha256'] = hashlib.sha256(image[start:start + size]).hexdigest()
    report = dict(binding=dict(target_sha256=ghidra.TARGET_SHA256,
                               program_sha256=ghidra.TARGET_SHA256,
                               image_base='00400000', entry_point='0047d45f',
                               language='x86:LE:32:default', file_ranges=ranges),
                  analysis_complete=True, requested_functions_complete=True,
                  exactness_credit='none',
                  functions=[dict(address='00401000', decompile_complete=True)])
    return bytes(image), report


class DirectGhidraTests(unittest.TestCase):
    def test_mapped_bytes_and_complete_function_selection(self):
        image, report = fixture()
        ghidra.verify_report(report, image, ['0x401000'])
        with self.assertRaisesRegex(ValueError, 'requested functions'):
            ghidra.verify_report(report, image, ['0x401010'])

    def test_missing_reordered_or_changed_mapped_range_is_rejected(self):
        image, report = fixture()
        for ranges in (report['binding']['file_ranges'][:-1],
                       list(reversed(report['binding']['file_ranges']))):
            changed = copy.deepcopy(report)
            changed['binding']['file_ranges'] = ranges
            with self.assertRaises(ValueError):
                ghidra.verify_report(changed, image, ['0x401000'])
        for mutation in ({'sha256': '0' * 64}, {'address': '0x401001'}, {'length': 31}):
            changed = copy.deepcopy(report)
            changed['binding']['file_ranges'][1].update(mutation)
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                ghidra.verify_report(changed, image, ['0x401000'])

    def test_wrong_identity_entry_architecture_and_partial_analysis_are_rejected(self):
        image, report = fixture()
        for mutation in ({'target_sha256': '0' * 64}, {'program_sha256': '0' * 64},
                         {'image_base': '00500000'}, {'entry_point': '00401000'},
                         {'language': 'x86:LE:64:default'}):
            changed = copy.deepcopy(report)
            changed['binding'].update(mutation)
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                ghidra.verify_report(changed, image, ['0x401000'])
        for mutation in ({'analysis_complete': False}, {'requested_functions_complete': False},
                         {'exactness_credit': 'exact'}, {'functions': []}):
            changed = {**report, **mutation}
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                ghidra.verify_report(changed, image, ['0x401000'])

    def test_wrong_disk_target_preserves_older_output_before_launch(self):
        with tempfile.TemporaryDirectory() as temporary:
            marker = Path(temporary) / 'older-proof.json'
            marker.write_text('preserved')
            with (patch.object(sys, 'argv', ['ghidra-analysis.py', '--output', temporary]),
                  patch.object(ghidra, 'verify_target', return_value=({}, ['wrong target'])),
                  patch.object(ghidra.subprocess, 'run') as run):
                with self.assertRaisesRegex(ValueError, 'target verification failed'):
                    ghidra.main()
                run.assert_not_called()
            self.assertEqual(marker.read_text(), 'preserved')


if __name__ == '__main__':
    unittest.main()
