"""Keep native serialized Replay codec member and signed offset ownership."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class ReplayNativeCodecWire(unittest.TestCase):
    def test_native_wire_cursors_and_signed_counts(self):
        source = (ROOT / 'src/ReplayInput.cpp').read_text()
        header = (ROOT / 'src/ReplayDataOriginalView.hpp').read_text()
        self.assertIn('ReplayCodecNativeWireHeaderOffsets', source)
        self.assertIn('offsetof(ReplayDataView, obfuscationKey) == 0x15', source)
        self.assertIn('offsetof(ReplayDataView, compressedSize) == 0x18', source)
        self.assertIn('offsetof(ReplayDataView, randomPayloadByte) == 0xC0', source)
        self.assertIn('obfuscateCursor = reinterpret_cast<u8 *>(&replayData->compressedSize);', source)
        self.assertIn('checksumCursor = &replayData->obfuscationKey;', source)
        for name in ('obfuscationKey', 'compressedSize'):
            self.assertIn('static_cast<i32>(offsetof(ReplayDataView, '+name+'))', source)
            self.assertIn(name, header)
        self.assertIn('&replayData->randomPayloadByte, replayData->compressedSize', source)
        self.assertIn('&decodedReplay->randomPayloadByte, replayData->decompressedSize', source)
        self.assertNotIn('(u8 *)replayData + 0x18', source)
        self.assertNotIn('(u8 *)replayData + 0x15', source)

    def test_existing_complete_replay_carriers_are_not_relabelled(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        members = [unit for unit in units.values()
                   if unit.get('source') == 'src/ReplayInput.cpp']
        self.assertEqual(len(members), 12)
        self.assertEqual(sum(unit.get('compare_size', unit['size']) for unit in members), 4093)
        self.assertEqual(sum(len(unit['relocations']) for unit in members), 232)
        self.assertEqual(units['replay-manager-load-replay-data']['size'], 591)
        self.assertEqual(units['replay-manager-begin-recording-stage']['size'], 1323)


if __name__ == '__main__':
    unittest.main()
