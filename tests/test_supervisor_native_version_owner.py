"""Original exact Supervisor version loader/checker share canonical 0x7B0 bytes."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class SupervisorNativeVersionOwner(unittest.TestCase):
    def test_original_suffix_is_one_native_supervisor_allocation(self):
        native=(ROOT/'src/Supervisor.hpp').read_text()
        receiver=(ROOT/'src/SupervisorVersionView.hpp').read_text()
        source=(ROOT/'src/SupervisorVersion.cpp').read_text()
        update=(ROOT/'src/SupervisorUpdate.cpp').read_text()
        self.assertIn('unsigned int versionDataSize7A8;',native)
        self.assertIn('void *versionData7AC;',native)
        self.assertIn('SupervisorNativeVersionSuffix',native)
        self.assertIn('struct SupervisorVersionView : Supervisor',receiver)
        self.assertNotIn('unsigned char unknown000[0x7A8];',receiver)
        self.assertIn('sizeof(SupervisorVersionView) == sizeof(Supervisor)',receiver)
        self.assertIn('offsetof(SupervisorVersionView, versionDataSize7A8) == 0x7A8',receiver)
        self.assertIn('offsetof(SupervisorVersionView, versionData7AC) == 0x7AC',receiver)
        self.assertIn('static_cast<SupervisorVersionView *>(&g_Supervisor)',source)
        self.assertIn('versionState->versionDataSize7A8 = fileSize;',source)
        self.assertIn('versionState->versionData7AC',source)
        self.assertIn('versionDataSize = this->versionDataSize7A8;',source)
        self.assertIn('this->versionData7AC',source)
        self.assertIn('static_cast<SupervisorVersionView *>(globalSupervisor)->CheckVersion(',update)
        self.assertNotIn('reinterpret_cast<SupervisorVersionView *>(globalSupervisor)',update)

    def test_exact_version_and_loader_owners_keep_original_method_targets(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        for n,va,size in (('supervisor-check-version',0x430150,199),
                          ('supervisor-load-version-data',0x42F100,145)):
            with self.subTest(unit=n):
                u=units[n]
                self.assertEqual((u['target_address'],u['size']), (va,size))
        self.assertIn('CheckVersion@SupervisorVersionView',units['supervisor-check-version']['symbol'])
        self.assertEqual(len(units['supervisor-check-version']['relocations']),8)
        self.assertEqual(len(units['supervisor-load-version-data']['relocations']),14)
        assert units['supervisor-deleted-callback']['size']==405


if __name__=='__main__':unittest.main()
