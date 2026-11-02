"""Complete original deleted callback consumes native Supervisor lifetime.

The exact 405-byte callback still invokes the real DeletedDummyMidiTimer,
DeletedMidiOutput and thread cleanup member ABIs, not renamed runtime methods.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeSupervisorDeleted(unittest.TestCase):
    def test_native_supervisor_carries_each_deleted_owner_field(self):
        s=(ROOT/'src/SupervisorDeletedCallback.cpp').read_text()
        h=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('SupervisorNativeDeletedFields',h)
        self.assertIn('offsetof(Supervisor, versionData7AC) == 0x7AC',h)
        self.assertIn('offsetof(GameConfiguration, musicMode) == 0x436',s)
        self.assertNotIn('struct SupervisorDeletedGlobalLayout',s)
        self.assertNotIn('struct SupervisorDeletedArgumentLayout',s)
        self.assertIn('Supervisor *globalSupervisor = &g_Supervisor;',s)
        self.assertIn('Supervisor *callbackSupervisor = supervisor;',s)
        for member,offset in (('dummyMidiTimer384','0x384'),
                              ('startupVm584','0x584'),('midiOutput5BC','0x5BC'),
                              ('versionData7AC','0x7AC'),
                              ('directInput0C','0x0C'),('keyboard10','0x10'),
                              ('controller014','0x14'),('controller118','0x18')):
            self.assertIn('offsetof(Supervisor, '+member+') == '+offset,s)
        for name in ('versionData7AC','startupVm584','config388.musicMode',
                     'midiOutput5BC','dummyMidiTimer384'):
            self.assertIn('globalSupervisor->'+name,s)
        for name in ('keyboard10','controller014','controller118','directInput0C'):
            self.assertIn('callbackSupervisor->'+name,s)

    def test_true_original_release_method_symbols_survive(self):
        s=(ROOT/'src/SupervisorDeletedCallback.cpp').read_text()
        self.assertIn('reinterpret_cast<DeletedSupervisorThreadView *>(&g_Supervisor)->ThreadClose();',s)
        self.assertIn('reinterpret_cast<DeletedMidiOutputView *>(globalSupervisor->midiOutput5BC)->PlayFile(30);',s)
        self.assertIn('reinterpret_cast<DeletedDummyMidiTimerView *>(globalSupervisor->dummyMidiTimer384)->StopTimer();',s)
        self.assertIn('reinterpret_cast<DeletedDummyMidiTimerView *>(globalSupervisor->dummyMidiTimer384)->DeleteObject(1);',s)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            u=tomllib.load(f)['units']
        self.assertEqual((u['supervisor-deleted-callback']['size'],
                          len(u['supervisor-deleted-callback']['relocations'])),(405,44))

if __name__=='__main__':
    unittest.main()
