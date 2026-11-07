"""Original Title methods share the genuine Supervisor allocation and FPS words."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class TitleNativeSupervisorStorage(unittest.TestCase):
    def test_true_supervisor_prefix_and_fps_state(self):
        title=(ROOT/'src/TitleScreen.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('struct TitleSupervisorView : Supervisor',title)
        self.assertIn('sizeof(TitleSupervisorView) == sizeof(Supervisor)',title)
        self.assertNotIn('u8 unknown000[8];\n    IDirect3DDevice8 *d3dDevice;',title)
        self.assertIn('g_TitleSupervisor.d3dDevice08->ResourceManagerDiscardBytes(0);',title)
        self.assertIn('g_TitleSupervisor.framerateMultiplier5B8 = 1.0f;',title)
        self.assertIn('g_TitleSupervisor.totalPlayTime == 0',title)
        self.assertIn('g_TitleSupervisor.value594',title)
        self.assertIn('unsigned char unknown744[0x794 - 0x744];',native)
        for field,offset in (
                ('recordingFpsWarning77C','0x77C'),
                ('playbackFpsWarning780','0x780'),
                ('recordingFpsState784','0x784'),
                ('recordingFpsState788','0x788'),
                ('recordingFpsState78C','0x78C'),
                ('recordingFpsState790','0x790')):
            with self.subTest(field=field):
                self.assertIn('int '+field+';',native)
                self.assertIn('offsetof(TitleSupervisorView, '+field+') == '+offset,title) if field in ('recordingFpsWarning77C','playbackFpsWarning780','recordingFpsState790') else None
        self.assertIn('TitleSupervisorView::ClearRecordingFpsWarningState()',title)

    def test_original_bound_title_big_and_small_owners_remain_stable(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        for unit,size,fields in (
            ('title-screen-start-menu',1560,107),
            ('title-screen-setup-thread',534,41),
            ('title-screen-actual-added-callback',335,27),
            ('title-screen-deleted-callback',96,11)):
            with self.subTest(unit=unit):
                self.assertEqual(u[unit]['size'],size)
                self.assertEqual(len(u[unit]['relocations']),fields)
                self.assertEqual(u[unit]['source'],'src/TitleScreen.cpp')

if __name__=='__main__':unittest.main()
