"""Pure-input guards for complete TH09 Effect AddedCallback comparison."""

import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'effect_added_inspector', ROOT/'scripts/inspect-effect-added.py')
EFFECT = importlib.util.module_from_spec(spec)
spec.loader.exec_module(EFFECT)


class EffectAddedInputTests(unittest.TestCase):
    def rows(self):
        return [dict(offset=off, type=kind, symbol=symbol, addend=0)
                for off, kind, symbol in (
                    (6, 'DIR32', list(EFFECT.DESTINATIONS)[0]),
                    (15, 'REL32', list(EFFECT.DESTINATIONS)[1]),
                    (27, 'DIR32', list(EFFECT.DESTINATIONS)[2]),
                    (36, 'REL32', list(EFFECT.DESTINATIONS)[3]),
                )]

    def test_canonical_four_fields_resolve_without_modifying_input(self):
        raw = bytes(EFFECT.SIZE)
        linked, fields = EFFECT.bind_candidate(raw, self.rows())
        self.assertEqual(len(fields), 4)
        self.assertEqual(raw, bytes(EFFECT.SIZE))
        for off, kind, symbol in (
            (6, 'DIR32', list(EFFECT.DESTINATIONS)[0]),
            (15, 'REL32', list(EFFECT.DESTINATIONS)[1]),
            (27, 'DIR32', list(EFFECT.DESTINATIONS)[2]),
            (36, 'REL32', list(EFFECT.DESTINATIONS)[3]),
        ):
            dest = EFFECT.DESTINATIONS[symbol]
            self.assertEqual(struct.unpack_from('<I', linked, off)[0],
                             (dest - (EFFECT.BASE+off+4 if kind=='REL32' else 0)) & 0xffffffff)

    def test_missing_duplicate_unreviewed_fields_are_rejected(self):
        data = self.rows()
        with self.assertRaisesRegex(ValueError, 'all four'):
            EFFECT.bind_candidate(bytes(EFFECT.SIZE), data[:3])
        for replacement in (
            dict(data[0], symbol='?FakeGlobal@@3H'),
            dict(data[0], type='REL32'),
            dict(data[0], addend=4),
            dict(data[0], offset=EFFECT.SIZE - 3),
            dict(data[0], offset=15),
        ):
            fields = [dict(x) for x in data]
            fields[0] = replacement
            with self.subTest(row=replacement), self.assertRaises(ValueError):
                EFFECT.bind_candidate(bytes(EFFECT.SIZE), fields)

    @unittest.skipUnless(importlib.util.find_spec('capstone') is not None,
                         'CI python -S intentionally omits Capstone')
    def test_reject_truncated_or_different_return(self):
        with self.assertRaises(ValueError):
            EFFECT.decode_owner(b'\x56\x57')
        with self.assertRaises(ValueError):
            EFFECT.decode_owner(b'\x56\x5e\xc2\x08\x00')
        with self.assertRaises(ValueError):
            EFFECT.decode_owner(b'\xff\xd0\xc3')


if __name__ == '__main__':
    unittest.main()
