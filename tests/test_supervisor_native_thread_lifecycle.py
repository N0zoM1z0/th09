"""Two exact Supervisor thread leafs share real +0x6AC/+0x6B4/+0x6B8.

The original ThreadClose member's class spelling remains intact for real
linker ABI; source-byte claims rely on independent current VC7.1 PE replays.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorThreadNativeOwner(unittest.TestCase):
    def test_running_predicate_uses_actual_supervisor(self):
        src=(ROOT/'src/SupervisorSubthread.cpp').read_text()
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct SupervisorSubthreadView',src)
        self.assertIn('offsetof(Supervisor, runningSubthreadHandle) == 0x6AC',src)
        self.assertIn('return supervisor->runningSubthreadHandle != 0;',src)
        self.assertIn('int runningSubthreadHandle;',sup)

    def test_exact_close_preserves_native_class_symbol_and_values(self):
        src=(ROOT/'src/SupervisorThreadClose.cpp').read_text()
        self.assertIn('struct SupervisorThreadCloseView : Supervisor',src)
        self.assertNotIn('unsigned char unknown000[0x6AC]',src)
        self.assertIn('void SupervisorThreadCloseView::ThreadClose()',src)
        for field in ('runningSubthreadHandle','subthreadCloseRequestActive','subthreadActive'):
            self.assertIn('this->'+field+' = ',src)
        self.assertIn('offsetof(SupervisorThreadCloseView, runningSubthreadHandle) == 0x6AC',src)
        self.assertIn('offsetof(SupervisorThreadCloseView, subthreadActive) == 0x6B8',src)

    def test_original_pe_leaf_extents_remain_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        for name,size in (('supervisor-subthread-is-running',14),
                          ('supervisor-thread-close',21)):
            with self.subTest(name=name):
                self.assertEqual(units[name]['size'],size)
                self.assertEqual(len(units[name]['relocations']),0)

if __name__=='__main__':
    unittest.main()
