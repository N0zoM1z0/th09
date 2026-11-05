"""Unsplit Supervisor::OnUpdate consumes the native 0x7B0 Supervisor owner.

The six frame offsets stored within the separately constructed ctor-array
remain a *bounded* local view; the queue/state/lifetime source records are
not duplicated. Exactness requires focused original-PE/COFF replays.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class NativeSupervisorUpdateOwner(unittest.TestCase):
    def test_unified_update_uses_native_supervisor_and_bounded_frames(self):
        source=(ROOT/'src/SupervisorUpdate.cpp').read_text()
        header=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertNotIn('struct SupervisorUpdateLayout',source)
        self.assertNotIn('reinterpret_cast<SupervisorUpdateLayout *>(',source)
        self.assertIn('Supervisor::OnUpdate(Supervisor *',source)
        self.assertIn('SupervisorUpdateNativeFrameOffsets',source)
        self.assertIn('SupervisorCtorArrayElement',source)
        self.assertIn('SupervisorUpdateNativeQueueAt47C',source)
        for index in range(3):
            for component in ('frameOffsetX','frameOffsetY'):
                with self.subTest(index=index,component=component):
                    self.assertIn('ctorArray078[%d].%s = 0.0f;'
                                  % (index, component),source)
        self.assertIn('offsetof(SupervisorCtorArrayElement, frameOffsetX) == 0xE8',source)
        self.assertIn('offsetof(SupervisorCtorArrayElement, frameOffsetY) == 0xEC',source)
        self.assertIn('sizeof(SupervisorFrameEntryStorageView)', (ROOT/'src/SupervisorFrameQueueStorageView.hpp').read_text())
        for name,off in (
            ('calcCount454','0x454'),('field478','0x478'),('entries47C','0x47C'),
            ('lastReceived56C','0x56C'),('wantedState58C','0x58C'),
            ('curState590','0x590'),('wantedState2','0x594'),
            ('flagsLow5D4','0x5D4'),('startupThreadState6BC','0x6BC'),
            ('frameState794','0x794'),('exeChecksum7A0','0x7A0'),
            ('exeSize7A4','0x7A4')):
            with self.subTest(name=name):
                self.assertIn('offsetof(Supervisor, '+name+') == '+off,header)
        self.assertIn('SupervisorSizeIs7B0',header)

    def test_true_supervisor_callees_preserve_original_profiles(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for name,size in (('supervisor-subthread-is-running',14),
                          ('supervisor-frame-queue-pop-frame',119),
                          ('supervisor-frame-queue-insert-predicted',191),
                          ('supervisor-constructor',62)):
            with self.subTest(name=name):
                self.assertEqual(units[name]['size'],size)
        self.assertIn('Supervisor::OnUpdate(', (ROOT/'src/SupervisorUpdate.cpp').read_text())

if __name__ == '__main__':
    unittest.main()
