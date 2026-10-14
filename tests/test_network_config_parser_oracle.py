"""No-target malformed input tests for the 243-byte network parser oracle."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'network_parser_oracle', ROOT / 'scripts/inspect-network-config-parser.py')
parser = importlib.util.module_from_spec(spec)
spec.loader.exec_module(parser)


class NetworkParserOriginalTests(unittest.TestCase):
    @staticmethod
    def target_model():
        raw = bytearray(parser.SIZE)
        struct.pack_into('<I', raw, parser.TARGET_FIELD[0],
                         parser.encoding(parser.TARGET_FIELD[0]))
        return bytes(raw)

    @staticmethod
    def candidate_rows():
        return [{'offset': 45, 'symbol': parser.SOURCE_SYMBOL,
                 'type': 'REL32', 'addend': 0}]

    def test_original_manifest_rejects_wrong_hash_extent_or_call_operand(self):
        code = self.target_model()
        parser.require_target(code, expected_sha256=hashlib.sha256(code).hexdigest())
        with self.assertRaisesRegex(ValueError, 'SHA-256'):
            parser.require_target(code)
        with self.assertRaisesRegex(ValueError, 'SHA-256'):
            parser.require_target(code[:-1], expected_sha256=hashlib.sha256(code[:-1]).hexdigest())
        wrong = bytearray(code)
        wrong[parser.TARGET_FIELD[0]] ^= 1
        with self.assertRaisesRegex(ValueError, 'call field'):
            parser.require_target(bytes(wrong), expected_sha256=hashlib.sha256(wrong).hexdigest())

    def test_actual_single_source_field_binds_call_without_target_byte_fitting(self):
        raw = bytes(parser.SIZE)
        bound, fields = parser.link_source(raw, self.candidate_rows())
        self.assertEqual(fields, [parser.TARGET_FIELD])
        self.assertEqual(struct.unpack_from('<I', bound, parser.TARGET_FIELD[0])[0],
                         parser.encoding(parser.TARGET_FIELD[0]))
        self.assertEqual(len(raw), len(bound))

    def test_missing_extra_ambiguous_and_wrong_source_field_all_rejected(self):
        baseline = self.candidate_rows()
        bad = [[], baseline+baseline,
               [dict(baseline[0], symbol='other-symbol')],
               [dict(baseline[0], type='DIR32')],
               [dict(baseline[0], addend=4)],
               [dict(baseline[0], offset=-1)],
               [dict(baseline[0], offset=242)],
               [dict(baseline[0], offset=900)]]
        for candidate in bad:
            with self.subTest(candidate=candidate), self.assertRaises(ValueError):
                parser.link_source(bytes(parser.SIZE), candidate)
        with self.assertRaisesRegex(ValueError, 'candidate code'):
            parser.link_source(bytes(99), baseline)

    def test_source_other_relocation_offsets_follow_real_relative_encoding(self):
        raw = bytes(parser.SIZE)
        relocated, fields = parser.link_source(raw, [dict(self.candidate_rows()[0], offset=55)])
        self.assertEqual(fields, [(55, 'REL32', parser.TARGET_FIELD[2])])
        self.assertEqual(struct.unpack_from('<I', relocated, 55)[0], parser.encoding(55))
        self.assertNotEqual(parser.encoding(55), parser.encoding(45))


if __name__ == '__main__':
    unittest.main()
