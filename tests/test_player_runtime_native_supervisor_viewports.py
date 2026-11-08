"""Player's large update/shots/draw owners call native Supervisor viewports.

Original equality comes from complete pinned-VC7.1 PE/COFF checks, not
these source assertions. The two shot-timer global fields carry a real
COFF +0x5B8 addend despite older manifests omitting its explicit spelling.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
GLOBAL='?g_Supervisor@@3VSupervisor@@A'

class PlayerRuntimeNativeSupervisorViewports(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_supervisor_and_no_duplicate_5b8_byte_prefix(self):
        src=(ROOT/'src/PlayerRuntime.cpp').read_text()
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',src)
        self.assertIn('PlayerRuntimeNativeSupervisorAt5B8',src)
        self.assertIn('sizeof(Supervisor) == 0x7B0',src)
        self.assertIn('offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8',src)
        self.assertNotIn('struct PlayerSupervisorRuntimeView',src)
        self.assertNotIn('g_PlayerSupervisorRuntime',src)
        self.assertNotIn('PreparePlayerDraw(',src)
        self.assertEqual(src.count('g_Supervisor.SelectSide('),4)
        self.assertEqual(src.count('g_Supervisor.ConfigureScreenViewport('),1)
        self.assertEqual(src.count('g_Supervisor.ConfigureGameplayViewport('),1)
        self.assertEqual(src.count('g_Supervisor.framerateMultiplier5B8'),2)
        for method in ('SelectSide','ConfigureScreenViewport','ConfigureGameplayViewport'):
            with self.subTest(method=method):
                self.assertIn('void '+method+'(',sup)
        self.assertIn('float framerateMultiplier5B8;',sup)

    def test_exact_full_update_and_render_call_sites(self):
        expected={
          'player-on-update':(522,[(0x46,GLOBAL,0x004B3100),
                                   (0x4B,'?SelectSide@Supervisor@@QAEXH@Z',0x00401440)]),
          'player-draw-high-prio':(166,[(0x8,GLOBAL,0x004B3100),
                   (0xD,'?ConfigureScreenViewport@Supervisor@@QAEXH@Z',0x00403D90)]),
          'player-draw-low-prio':(31,[(0x8,GLOBAL,0x004B3100),
                   (0xD,'?ConfigureGameplayViewport@Supervisor@@QAEXH@Z',0x00401390)]),
          'player-update-shots':(251,[(0x6A,GLOBAL,0x004B3100),
                                        (0x7A,GLOBAL,0x004B3100)]),
          'player-update-before-state':(814,[(0xF8,GLOBAL,0x004B3100),
                  (0xFD,'?SelectSide@Supervisor@@QAEXH@Z',0x00401440),
                  (0x15F,GLOBAL,0x004B3100),
                  (0x16B,'?SelectSide@Supervisor@@QAEXH@Z',0x00401440),
                  (0x28B,GLOBAL,0x004B3100),
                  (0x290,'?SelectSide@Supervisor@@QAEXH@Z',0x00401440)]),
        }
        for unit,(size,rows) in expected.items():
            with self.subTest(unit=unit):
                u=self.units[unit]
                self.assertEqual(u['size'],size)
                self.assertEqual(u['source'],'src/PlayerRuntime.cpp')
                actual=[(r['offset'],r['symbol'],r['target']) for r in u['relocations']
                        if 'Supervisor@@' in r['symbol']]
                self.assertEqual(actual,rows)
        self.assertEqual(self.units['player-update-before-state']['compare_size'],852)
        for unit,target in (('supervisor-select-side',0x00401440),
                            ('supervisor-configure-screen-viewport',0x00403D90),
                            ('supervisor-configure-gameplay-viewport',0x00401390)):
            with self.subTest(provider=unit):
                self.assertEqual(self.units[unit]['target_address'],target)

if __name__=='__main__':unittest.main()
