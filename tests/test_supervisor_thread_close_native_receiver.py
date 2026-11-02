"""Both original ThreadClose receiver types are zero-offset Supervisor views.

Their distinct decorated C++ member names remain intact because both
historical callsites target the same native Supervisor thread-state words.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeThreadCloseReceivers(unittest.TestCase):
    def test_startup_and_deleted_receiver_types_share_supervisor(self):
        startup=(ROOT/'src/SupervisorStartup.cpp').read_text()
        deleted=(ROOT/'src/SupervisorDeletedCallback.cpp').read_text()
        implementation=(ROOT/'src/SupervisorThreadClose.cpp').read_text()
        self.assertIn('struct SupervisorThreadCloseView : Supervisor',startup)
        self.assertIn('struct DeletedSupervisorThreadView : Supervisor',deleted)
        self.assertIn('struct SupervisorThreadCloseView : Supervisor',implementation)
        self.assertNotIn('struct SupervisorThreadCloseView\n{',startup)
        self.assertNotIn('struct DeletedSupervisorThreadView\n{',deleted)
        self.assertIn('reinterpret_cast<SupervisorThreadCloseView *>(&g_Supervisor)->ThreadClose();',startup)
        self.assertIn('reinterpret_cast<DeletedSupervisorThreadView *>(&g_Supervisor)->ThreadClose();',deleted)
        for field,offset in (('runningSubthreadHandle','0x6AC'),
                             ('subthreadCloseRequestActive','0x6B4'),
                             ('subthreadActive','0x6B8')):
            self.assertIn('offsetof(SupervisorThreadCloseView, '+field+') == '+offset,implementation)

    def test_original_boss_and_callee_sizes_stay_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        for name,size,fields in (
            ('supervisor-startup-thread',882,106),
            ('supervisor-deleted-callback',405,44),
            ('supervisor-thread-close',21,0)):
            with self.subTest(name=name):
                self.assertEqual((u[name]['size'],len(u[name]['relocations'])),(size,fields))

if __name__=='__main__':
    unittest.main()
