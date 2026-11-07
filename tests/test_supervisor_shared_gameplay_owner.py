"""One original-backed Supervisor runtime owner shared by gameplay Setup methods.

The former Setup-local full-size data projection is replaced with a
zero-offset nonvirtual base; its distinct Setup method ABI is retained.
The native TH09 PE byte proof is performed separately by exact units.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeSupervisorRuntimeInGameplay(unittest.TestCase):
    def test_setup_layout_uses_shared_supervisor_data_and_original_methods(self):
        shared=(ROOT/'src/Supervisor.hpp').read_text()
        source=(ROOT/'src/GameManagerSetup.cpp').read_text()
        self.assertIn('struct SupervisorSetupLayout : Supervisor',source)
        self.assertNotIn('unsigned char unknown_000[0x388]',source)
        self.assertIn('SupervisorNativeSetupFields',shared)
        self.assertIn('SupervisorSizeIs7B0',shared)
        for member,offset in (
            ('config388','0x388'),('state590','0x590'),
            ('value598','0x598'),('screenTransitionCountdown','0x5A4'),
            ('loadingAnm5D0','0x5D0'),('flags','0x5D4'),
            ('runningSubthreadHandle','0x6AC'),
            ('subthreadCloseRequestActive','0x6B4'),('subthreadActive','0x6B8'),
            ('loadingVmsHaveBeenSetup','0x740'),
            ('totalPlayTime','0x798'),('systemTime','0x79C')):
            with self.subTest(member=member):
                self.assertIn('offsetof(Supervisor, '+member+') == '+offset,shared)
        self.assertIn('volatile unsigned int flags5D4;',shared)
        self.assertIn('unsigned int flags;',shared)
        self.assertIn('supervisor->config388',source)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0',source)
        self.assertIn('offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740',source)
        for member in ('StartEffect','ThreadStart','PlayMusic','FadeOutMusic','UpdatePlayTime','UpdateGameTime'):
            self.assertIn(member+'(',source)

    def test_original_large_supervisor_and_setup_owners_remain_pinned(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        expected=[
            ('game-manager-on-update',1230,87),
            ('game-manager-cleanup-gameplay-state',425,43),
            ('supervisor-setup-loading-vms-and-capture',214,17),
            ('supervisor-startup-thread',882,106),
            ('supervisor-initialize-viewports',736,72),
            ('supervisor-constructor',62,3),
        ]
        for name,size,fields in expected:
            with self.subTest(name=name):
                unit=units[name]
                self.assertEqual((unit['size'],len(unit['relocations'])),(size,fields))


if __name__ == '__main__':
    unittest.main()
