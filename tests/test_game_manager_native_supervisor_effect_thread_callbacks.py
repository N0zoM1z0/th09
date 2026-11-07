"""The exact gameplay AddedCallback calls genuine Supervisor leaf methods."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class GameManagerNativeSupervisorCallbackLeaves(unittest.TestCase):
    def test_native_effect_and_thread_inheritance(self):
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        thread=(ROOT/'src/SupervisorThreadStartView.hpp').read_text()
        effect=(ROOT/'src/SupervisorStartEffectView.hpp').read_text()
        score=(ROOT/'src/ScoreSupervisorView.hpp').read_text()
        self.assertIn('#include "SupervisorThreadStartView.hpp"',source)
        self.assertIn('struct SupervisorSetupLayout : SupervisorLoadingSetupView', (ROOT/'src/SupervisorSetupLayoutView.hpp').read_text())
        self.assertNotIn('void StartEffect(int index);',source)
        self.assertNotIn('int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);',source)
        self.assertIn('struct SupervisorThreadStartView : SupervisorStartEffectView',thread)
        self.assertIn('struct SupervisorStartEffectView : ScoreSupervisorView',effect)
        self.assertIn('struct ScoreSupervisorView : Supervisor',score)
        self.assertIn('supervisor->StartEffect(0);',source)
        self.assertIn('supervisor->ThreadStart(',source)

    def test_real_exact_original_callees_and_fields(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        added=units['game-manager-added-callback']
        self.assertEqual((added['size'],len(added['relocations'])),(187,13))
        for offset,leaf_name,size in ((0x66,'supervisor-start-effect-leaf',47),
                                      (0xA3,'supervisor-thread-start-leaf',69)):
            with self.subTest(offset=offset):
                leaf=units[leaf_name]
                row=next(x for x in added['relocations'] if x['offset']==offset)
                self.assertEqual(leaf['size'],size)
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (leaf['symbol'],leaf['target_address'],'REL32'))

if __name__=='__main__':unittest.main()
