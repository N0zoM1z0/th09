"""FrontMessage's complete original-exact script invokes true Supervisor audio.

Pinned VC7.1 full PE/COFF comparisons prove machine bytes; this test pins
native object size, original global address and four exact audio call VAs.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
GLOBAL='?g_Supervisor@@3VSupervisor@@A'
AUDIO=(
 ('StopAudio','supervisor-stop-audio',0x815,0x0042FD30),
 ('PlayMusic','supervisor-play-music',0x835,0x00431930),
 ('PlayMusic','supervisor-play-music',0x84F,0x00431930),
 ('FadeOutMusic','supervisor-fade-out-music',0x8A3,0x0042FE20),
)

class FrontMessageNativeSupervisorAudio(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_real_supervisor_object_and_audio_method_ownership(self):
        script=(ROOT/'src/FrontMessageUpdate.cpp').read_text()
        sup=(ROOT/'src/Supervisor.hpp').read_text()
        methods=(ROOT/'src/SupervisorAudio.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',script)
        self.assertIn('extern Supervisor g_Supervisor;',script)
        self.assertIn('FrontMessageNativeSupervisorAudio',script)
        self.assertIn('sizeof(Supervisor) == 0x7B0',script)
        self.assertNotIn('FrontMessageSupervisorView',script)
        self.assertIn('class Supervisor',sup)
        self.assertIn('SupervisorSizeIs7B0',sup)
        self.assertEqual(script.count('g_Supervisor.StopAudio()'),1)
        self.assertEqual(script.count('g_Supervisor.PlayMusic('),2)
        self.assertEqual(script.count('g_Supervisor.FadeOutMusic('),1)
        for method in ('StopAudio','PlayMusic','FadeOutMusic'):
            with self.subTest(method=method):
                self.assertIn('int '+method+'(',sup)
                self.assertIn('int Supervisor::'+method+'(',methods)

    def test_all_original_fields_and_independently_exact_audio_leaf_targets(self):
        script=self.units['front-message-update']
        self.assertEqual((script['target_address'],script['size'],script['compare_size'],len(script['relocations'])),
                         (0x00416590,2852,2968,167))
        global_fields=[r for r in script['relocations'] if r['symbol']==GLOBAL]
        self.assertEqual([r['offset'] for r in global_fields],[0x810,0x830,0x84A,0x89E])
        self.assertTrue(all(r['type']=='DIR32' and r['target']==0x004B3100 and r.get('addend',0)==0 for r in global_fields))
        for method,unit,offset,target in AUDIO:
            with self.subTest(method=method,offset=offset):
                leaf=self.units[unit]
                self.assertEqual(leaf['target_address'],target)
                symbol=leaf['symbol']
                self.assertIn('@Supervisor@@',symbol)
                found=[r for r in script['relocations'] if r['offset']==offset]
                self.assertEqual(len(found),1)
                self.assertEqual((found[0]['symbol'],found[0]['type'],found[0]['target'],found[0].get('addend',0)),
                                 (symbol,'REL32',target,0))
        self.assertEqual(len([r for r in script['relocations'] if r['symbol'].startswith('$')]),30)

if __name__=='__main__':unittest.main()
