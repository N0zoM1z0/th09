"""The exact GameManager AddedCallback uses actual Supervisor effect/thread methods."""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class GameManagerNativeSupervisorThreadAndEffect(unittest.TestCase):
    def test_zero_extra_storage_original_bound_method_chain(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        start=(ROOT/'src/SupervisorThreadStartView.hpp').read_text()
        effect=(ROOT/'src/SupervisorStartEffectView.hpp').read_text()
        self.assertIn('struct SupervisorSetupLayout : SupervisorThreadStartView',source)
        self.assertIn('struct SupervisorThreadStartView : SupervisorStartEffectView',start)
        self.assertIn('struct SupervisorStartEffectView : ScoreSupervisorView',effect)
        self.assertIn('supervisor->StartEffect(0);',source)
        self.assertIn('supervisor->ThreadStart(',source)
        self.assertNotIn('    void StartEffect(int index);',source)
        self.assertNotIn('    int ThreadStart(LPTHREAD_START_ROUTINE',source)

    def test_original_callees_and_addded_callback_fields_are_exact_identity(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream: units=tomllib.load(stream)['units']
        caller=units['game-manager-added-callback']
        self.assertEqual((caller['size'],len(caller['relocations'])),(187,13))
        by_offset={r['offset']:r for r in caller['relocations']}
        for off,name,size in ((0x66,'supervisor-start-effect-leaf',47),
                              (0xA3,'supervisor-thread-start-leaf',69)):
            with self.subTest(offset=off):
                leaf=units[name]
                self.assertEqual(leaf['size'],size)
                self.assertEqual((by_offset[off]['symbol'],by_offset[off]['type'],by_offset[off]['target']),
                                 (leaf['symbol'],'REL32',leaf['target_address']))

if __name__=='__main__':unittest.main()
