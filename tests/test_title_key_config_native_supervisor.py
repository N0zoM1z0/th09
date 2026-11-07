"""Key-config 2803-byte original boss uses true Supervisor config at +0x388."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleKeyConfigNativeSupervisor(unittest.TestCase):
    def test_no_separate_config_owner_or_lifetime_shadow(self):
        source=(ROOT/'src/TitleScreenKeyConfig.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertNotIn('struct TitleKeySupervisorView',source)
        self.assertNotIn('extern TitleKeySupervisorView g_Supervisor;',source)
        self.assertIn('offsetof(Supervisor, config388) == 0x388',source)
        self.assertIn('GameConfiguration config388;',native)
        self.assertIn('extern Supervisor g_Supervisor;',native)
        self.assertIn('g_Supervisor.config388.sideFocusMode[keyConfigSide]',source)
        self.assertIn('g_Supervisor.config388.controllerMapping',source)

    def test_all_51_native_global_fields_keep_the_original_address(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:units=tomllib.load(f)['units']
        owner=units['title-screen-update-key-config']
        self.assertEqual((owner['size'],len(owner['relocations'])),(2803,118))
        native='?g_Supervisor@@3VSupervisor@@A'
        legacy='?g_Supervisor@@3UTitleKeySupervisorView@@A'
        rows=[r for r in owner['relocations'] if r['symbol']==native]
        self.assertEqual(len(rows),51)
        self.assertFalse(any(r['symbol']==legacy for r in owner['relocations']))
        for row in rows:
            self.assertEqual((row['type'],row['target'],row.get('addend',0)),
                             ('DIR32',0x004B3100,0))
        self.assertEqual(units['title-screen-swap-duplicate-character-index']['size'],106)

if __name__=='__main__':unittest.main()
