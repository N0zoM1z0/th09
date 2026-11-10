"""The two exact Title music-room functions share the complete Title object."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class NativeTitleMusicRoomOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = (ROOT / 'src/TitleScreenMusicRoom.cpp').read_text()
        cls.header = (ROOT / 'src/TitleScreenReplayView.hpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as f:
            cls.units = tomllib.load(f)['units']

    def test_one_constructed_title_and_pod_music_records(self):
        s, h = self.source, self.header
        self.assertIn('#include "TitleScreenReplayView.hpp"', s)
        self.assertNotIn('struct TitleScreenView {', s)
        self.assertEqual(h.count('struct TitleScreenView {'), 1)
        self.assertIn('struct MusicRoomTrackDescriptor', h)
        self.assertIn('sizeof(MusicRoomTrackDescriptor) == 0x292', h)
        self.assertIn('MusicRoomTrackDescriptor musicTracks[32];', h)
        self.assertIn('sizeof(((TitleScreenView *)0)->musicTracks) == 0x5240', h)
        for name, offset in (('musicTracks','0xC934'),
                             ('musicSelectedSongIndex','0x11B74'),
                             ('musicTrackCount','0x11B78'),
                             ('musicListingOffset','0x11B7C'),
                             ('musicPaused','0x11B80')):
            with self.subTest(field=name):
                self.assertIn('offsetof(TitleScreenView, ' + name + ') == ' + offset, h)
        self.assertIn('TitleAnmView *titleAnm;', h)
        self.assertIn('titleAnm->SetAndExecuteScriptIdx(', s)
        self.assertNotIn('AnmLoaded *musicAnm;', s)
        self.assertIn('AnmVmView embeddedVms[57];', h)
        self.assertIn('sizeof(TitleScreenView) == 0x1B388', h)

    def test_two_original_complete_owners_and_indexed_globals(self):
        u = self.units['title-screen-music-room']
        draw = self.units['title-screen-draw-music-room']
        self.assertEqual((u['target_address'], u['size'], len(u['relocations'])),
                         (0x00426E05, 2258, 74))
        self.assertEqual((draw['target_address'], draw['size'],len(draw['relocations'])),
                         (0x004244A9, 209, 6))
        by_offset = {r['offset']: r for r in u['relocations']}
        for off, addr in ((0x91,0x004A1DC0),(0x1FF,0x004A1DD8),
                          (0x339,0x004ACF3A),(0x356,0x004ACF3B),
                          (0x394,0x004ACF3B),(0x3DC,0x004ACF3A)):
            with self.subTest(offset=hex(off)):
                row = by_offset[off]
                self.assertEqual((row['target'] + row.get('addend', 0), row['type']),
                                 (addr,'DIR32'))


if __name__ == '__main__':
    unittest.main()
