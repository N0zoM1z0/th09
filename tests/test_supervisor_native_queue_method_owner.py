"""All four original network frame queue methods belong to one Supervisor.

Strict original PE/COFF compilation proves byte identity; these checks
prevent returning to a duplicate source-only method owner or moved call VA.
"""
from pathlib import Path
from collections import Counter
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
METHODS=(('InsertReceivedFrame','supervisor-frame-queue-insert-received',213,0x0042E9E0),
         ('InsertPredictedFrame','supervisor-frame-queue-insert-predicted',191,0x0042EAC0),
         ('PopFrame','supervisor-frame-queue-pop-frame',119,0x0042EB80),
         ('AreFrameQueuesSynchronized','supervisor-network-frame-queues-synchronized',49,0x0042ECF0))
CALL='?InsertReceivedFrame@Supervisor@@QAEXHHGF@Z'

class SupervisorNativeQueueMethodOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            cls.units=tomllib.load(f)['units']

    def test_true_supervisor_owns_four_queue_methods_and_physical_frames(self):
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertFalse((ROOT/'src/SupervisorFrameQueueView.hpp').exists())
        for name,_,_,_ in METHODS:
            with self.subTest(method=name):
                self.assertIn(name+'(',native)
        for part in ('SupervisorFrameEntryStorageView entries47C[20];',
                     'SupervisorFrameEntryStorageView lastReceived56C[2];',
                     'offsetof(Supervisor, entries47C) == 0x47C',
                     'offsetof(Supervisor, lastReceived56C) == 0x56C'):
            self.assertIn(part,native)
        self.assertIn('SupervisorSizeIs7B0',native)
        for file in ('SupervisorFrameQueueInsert.cpp','SupervisorFrameQueueOps.cpp',
                     'SupervisorNetworkQueue.cpp'):
            with self.subTest(source=file):
                src=(ROOT/'src'/file).read_text()
                self.assertIn('Supervisor::',src)
                self.assertNotIn('SupervisorFrameQueueView::',src)

    def test_independent_exact_functions_and_original_callee_sites(self):
        for method,unit,size,target in METHODS:
            with self.subTest(method=method):
                info=self.units[unit]
                self.assertEqual(info['size'],size)
                self.assertEqual(info['target_address'],target)
                self.assertIn('@Supervisor@@',info['symbol'])
        self.assertEqual(self.units['supervisor-reset-title-mode4']['size'],177)
        msg=self.units['supervisor-network-message-handler']
        self.assertEqual(msg['size'],768)
        self.assertEqual(msg['compare_size'],808)
        allcalls=[]
        for name in ('supervisor-reset-title-mode4',
                     'supervisor-network-message-handler'):
            allcalls.extend((name, row) for row in self.units[name]['relocations']
                            if row['symbol']==CALL)
        self.assertEqual(Counter(name for name,row in allcalls),
                         Counter({'supervisor-reset-title-mode4':2,
                                  'supervisor-network-message-handler':3}))
        self.assertTrue(all(r['target']==0x0042E9E0 and r['type']=='REL32' and
                            r.get('addend',0)==0 for _,r in allcalls))
        self.assertEqual(len([r for r in msg['relocations'] if r['symbol'].startswith('$L')]),8)

    def test_tracker_names_original_native_receiver(self):
        for file in ('functions.csv','implemented.csv','matches.csv','reccmp-functions.csv'):
            text=(ROOT/'config'/file).read_text()
            for name,_,_,_ in METHODS:
                with self.subTest(file=file,method=name):
                    self.assertIn('Supervisor::'+name,text)
                    self.assertNotIn('SupervisorFrameQueueView::'+name,text)

if __name__=='__main__':unittest.main()
