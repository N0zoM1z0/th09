"""Native Player update, drawing and shots use the genuine Supervisor.

The original VC7.1 complete PE/COFF oracles prove exactness independently;
these constraints prevent second 0x5B8-byte shadow Supervisor receivers.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SUP='?g_Supervisor@@3VSupervisor@@A'

class PlayerRuntimeNativeSupervisorOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_one_supervisor_allocation_for_player_motion_and_draw(self):
        runtime=(ROOT/'src/PlayerRuntime.cpp').read_text()
        header=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',runtime)
        self.assertIn('PlayerRuntimeNativeSupervisorAt5B8',runtime)
        self.assertIn('sizeof(Supervisor) == 0x7B0',runtime)
        self.assertIn('offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8',runtime)
        self.assertNotIn('struct PlayerSupervisorRuntimeView',runtime)
        self.assertNotIn('g_PlayerSupervisorRuntime',runtime)
        self.assertEqual(runtime.count('g_Supervisor.SelectSide('),4)
        self.assertEqual(runtime.count('g_Supervisor.ConfigureScreenViewport('),1)
        self.assertEqual(runtime.count('g_Supervisor.ConfigureGameplayViewport('),1)
        self.assertEqual(runtime.count('g_Supervisor.framerateMultiplier5B8'),2)
        self.assertIn('union { float framerateMultiplier5B8; float gameSpeed; };',header)

    def test_original_supervisor_callees_and_addresses(self):
        expected={
            'player-on-update':{0x46:(SUP,0x004B3100),0x4B:('?SelectSide@Supervisor@@QAEXH@Z',0x00401440)},
            'player-draw-high-prio':{0x08:(SUP,0x004B3100),0x0D:('?ConfigureScreenViewport@Supervisor@@QAEXH@Z',0x00403D90)},
            'player-draw-low-prio':{0x08:(SUP,0x004B3100),0x0D:('?ConfigureGameplayViewport@Supervisor@@QAEXH@Z',0x00401390)},
            'player-update-shots':{0x6A:(SUP,0x004B3100),0x7A:(SUP,0x004B3100)},
            'player-update-before-state':{0xF8:(SUP,0x004B3100),0xFD:('?SelectSide@Supervisor@@QAEXH@Z',0x00401440),0x15F:(SUP,0x004B3100),0x16B:('?SelectSide@Supervisor@@QAEXH@Z',0x00401440),0x28B:(SUP,0x004B3100),0x290:('?SelectSide@Supervisor@@QAEXH@Z',0x00401440)},
        }
        for unit,fields in expected.items():
            with self.subTest(unit=unit):
                spec=self.units[unit]
                at={r['offset']:r for r in spec['relocations']}
                for off,(symbol,target) in fields.items():
                    self.assertEqual((at[off]['symbol'],at[off]['target'],at[off].get('addend',0)),
                                     (symbol,target,0))
        self.assertEqual(self.units['player-update-before-state']['compare_size'],852)
        self.assertEqual(self.units['player-initialize-added-state']['compare_size'],572)

if __name__=='__main__':unittest.main()
