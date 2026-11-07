"""The exact resource clamp consumes the shared native Player scalar."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativePlayerResourceSubtract(unittest.TestCase):
    def test_storage_and_class_abi(self):
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        source=(ROOT/'src/PlayerResourceSubtract.cpp').read_text()
        player=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertIn('void SubtractResourceClamped(float amount);',header)
        self.assertIn('PlayerLifecycleSizeIs30F70',header)
        self.assertIn('PlayerLifecycleNativeChargeResourceAt30388',header)
        self.assertFalse((ROOT/'src/PlayerResourceSubtractView.hpp').exists())
        self.assertNotIn('unknown00000[0x30388]',header)
        self.assertIn('float scalar30388;',player)
        self.assertIn('float value = scalar30388 - amount;',source)
        self.assertNotIn('resource30388',source)

    def test_exact_function_contract_stable(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        v=u['player-subtract-resource-clamped']
        self.assertEqual(v['target_address'],0x0041F310)
        self.assertEqual(v['size'],51)
        self.assertEqual(v['symbol'],'?SubtractResourceClamped@PlayerLifecycleView@@QAEXM@Z')

if __name__=='__main__':unittest.main()
