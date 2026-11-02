"""Native Supervisor startup and AddedCallback share the same original 0x7B0 ABI.

Separate StartupAnmLoaded/StartupAnmVm and SupervisorAddedLayout external
method names remain intact. Current-source VC7.1 replays establish exactness.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeSupervisorStartupAdded(unittest.TestCase):
    def test_startup_uses_real_supervisor_and_original_named_callees(self):
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        startup=(ROOT/'src/SupervisorStartup.cpp').read_text()
        self.assertIn('typedef Supervisor SupervisorStartupLayout;',startup)
        self.assertNotIn('struct SupervisorStartupLayout\n{',startup)
        self.assertIn('return &g_Supervisor;',startup)
        for term in ('StartupLayout()->dummyMidiTimer384',
                     'StartupLayout()->startupVm584',
                     'StartupLayout()->textAnm5CC',
                     'StartupLayout()->midiOutput5BC',
                     'StartupLayout()->config388.musicVolume',
                     'StartupLayout()->config388.sfxVolume',
                     'StartupLayout()->startupThreadState'):
            self.assertIn(term,startup)
        self.assertIn('struct StartupAnmVm',startup)
        self.assertIn('struct StartupAnmLoaded',startup)
        for member,offset in (
            ('dummyMidiTimer384','0x384'),('startupVm584','0x584'),
            ('startupField5A4','0x5A4'),('startupField5A8','0x5A8'),
            ('suppressFpsDisplay','0x5A8'),('disableVsync','0x5AC'),
            ('gameSpeed','0x5B8'),('midiOutput5BC','0x5BC'),
            ('textAnm5CC','0x5CC'),('loadingAnm','0x5D0'),
            ('startupThreadState','0x6BC')):
            with self.subTest(member=member):
                self.assertIn('offsetof(Supervisor, '+member+') == '+offset,sup)

    def test_added_preserves_original_abi_as_nonvirtual_native_derived(self):
        src=(ROOT/'src/SupervisorAddedCallback.cpp').read_text()
        self.assertIn('struct SupervisorAddedLayout : Supervisor',src)
        self.assertNotIn('unsigned char unknown000[0x5A8]',src)
        self.assertIn('void SetupLoadingVms(Float3 *position);',src)
        self.assertIn('int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);',src)
        for name in ('suppressFpsDisplay','disableVsync','gameSpeed',
                     'loadingAnm','startupThreadState'):
            self.assertIn(name,src)

    def test_original_boss_and_exact_callee_extents(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            u=tomllib.load(stream)['units']
        for name,size,fields in (
            ('supervisor-startup-thread',882,106),
            ('supervisor-added-callback',238,27),
            ('supervisor-constructor',62,3),
            ('supervisor-setup-loading-vms',173,15),
        ):
            with self.subTest(name=name):
                item=u[name]
                self.assertEqual((item['size'],len(item['relocations'])),(size,fields))


if __name__=='__main__':
    unittest.main()
