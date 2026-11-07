"""Exact Title AddedCallback uses genuine Supervisor effect/thread leaves."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeSupervisorEffectAndThread(unittest.TestCase):
    def test_shared_storage_method_hierarchy(self):
        s=(ROOT/'src/TitleScreen.cpp').read_text()
        t=(ROOT/'src/SupervisorThreadStartView.hpp').read_text()
        e=(ROOT/'src/SupervisorStartEffectView.hpp').read_text()
        self.assertIn('#include "SupervisorThreadStartView.hpp"',s)
        self.assertIn('struct TitleSupervisorView : SupervisorThreadStartView',(ROOT/'src/TitleSupervisorRuntimeView.hpp').read_text())
        self.assertIn('struct SupervisorThreadStartView : SupervisorStartEffectView',t)
        self.assertIn('struct SupervisorStartEffectView : ScoreSupervisorView',e)
        self.assertIn('g_TitleSupervisor.StartEffect(0);',s)
        self.assertIn('g_TitleSupervisor.ThreadStart(',s)
        self.assertNotIn('    void StartEffect(i32 index);',s)
        self.assertNotIn('    int ThreadStart(LPTHREAD_START_ROUTINE',s)

    def test_original_caller_has_exact_native_callee_identities(self):
        with (ROOT/'config/match-units.toml').open('rb') as f: u=tomllib.load(f)['units']
        owner=u['title-screen-actual-added-callback']
        self.assertEqual((owner['size'],len(owner['relocations'])),(335,27))
        for offset,leafname,size in (
            (277,'supervisor-start-effect-leaf',47),
            (324,'supervisor-thread-start-leaf',69),
        ):
            with self.subTest(offset=offset):
                leaf=u[leafname]
                row=next(r for r in owner['relocations'] if r['offset']==offset)
                self.assertEqual(leaf['size'],size)
                self.assertEqual((row['symbol'],row['type'],row['target']),
                                 (leaf['symbol'],'REL32',leaf['target_address']))

if __name__=='__main__': unittest.main()
