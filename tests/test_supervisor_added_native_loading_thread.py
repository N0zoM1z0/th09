"""The exact 238-byte Supervisor AddedCallback invokes native loading/thread leaves."""
import tomllib
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]

class SupervisorAddedNativeLoadingThread(unittest.TestCase):
    def test_real_supervisor_methods_replace_local_shadow_methods(self):
        source=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        loader=(ROOT/'src/SupervisorLoadingSetupView.hpp').read_text()
        self.assertIn('struct SupervisorAddedLayout : SupervisorLoadingSetupView',source)
        self.assertIn('struct SupervisorLoadingSetupView : SupervisorThreadStartView',loader)
        self.assertNotIn('struct SupervisorAddedLayout : Supervisor\n{',source)
        self.assertNotIn('    int ThreadStart(LPTHREAD_START_ROUTINE',source)
        self.assertNotIn('    void SetupLoadingVms(Float3 *position);',source)
        self.assertIn('globalSupervisor->SetupLoadingVms(&position);',source)
        self.assertIn('globalSupervisor->ThreadStart(',source)

    def test_original_two_callee_addresses_are_independently_exact(self):
        with (ROOT/'config/match-units.toml').open('rb') as inp:units=tomllib.load(inp)['units']
        caller=units['supervisor-added-callback']
        self.assertEqual((caller['size'],len(caller['relocations'])),(238,27))
        for offset,leaf_name,size in ((0xC9,'supervisor-setup-loading-vms',173),
                                      (0xE3,'supervisor-thread-start-leaf',69)):
            with self.subTest(offset=offset):
                leaf=units[leaf_name]
                row=next(x for x in caller['relocations'] if x['offset']==offset)
                self.assertEqual(leaf['size'],size)
                self.assertEqual((row['symbol'],row['target'],row['type']),
                                 (leaf['symbol'],leaf['target_address'],'REL32'))

if __name__=='__main__':unittest.main()
