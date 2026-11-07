"""Exact Win32 thread start/join methods borrow the real Supervisor storage."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorNativeThreadLifecycle(unittest.TestCase):
    def test_real_handle_id_and_state_not_a_private_prefix(self):
        header=(ROOT/'src/SupervisorThreadStartView.hpp').read_text()
        src=(ROOT/'src/SupervisorThreadStart.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('struct SupervisorThreadStartView : SupervisorStartEffectView',header)
        self.assertIn('sizeof(SupervisorThreadStartView) == sizeof(Supervisor)',header)
        self.assertIn('offsetof(SupervisorThreadStartView, runningSubthreadHandlePtr) == 0x6AC',header)
        self.assertIn('offsetof(SupervisorThreadStartView, runningSubthreadId6B0) == 0x6B0',header)
        self.assertIn('offsetof(SupervisorThreadStartView, subthreadCloseRequestActive) == 0x6B4',header)
        self.assertIn('offsetof(SupervisorThreadStartView, subthreadActive) == 0x6B8',header)
        self.assertNotIn('unsigned char unknown000[0x6AC]',src)
        self.assertIn('WaitForSingleObject(this->runningSubthreadHandlePtr, 1000)',src)
        self.assertIn('&runningSubthreadId6B0',src)
        self.assertIn('subthreadActive = 1;',src)
        self.assertIn('unsigned long runningSubthreadId6B0;',native)

    def test_separate_original_method_abis(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for name,size,target in (
            ('supervisor-thread-start-leaf',69,0x00431AE0),
            ('supervisor-thread-join-close',108,0x00430230),
        ):
            with self.subTest(name=name):
                self.assertEqual((u[name]['size'],u[name]['target_address']), (size,target))
                self.assertEqual(u[name]['source'],'src/SupervisorThreadStart.cpp')

if __name__=='__main__':unittest.main()
