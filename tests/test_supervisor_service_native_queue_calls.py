"""Full original nonexact network-service boss calls real Supervisor queue methods.

The complete i386 PE/COFF comparison is separate; source assertions prevent
accidentally reinstating the empty queue receiver or changing native call VAs.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
QUEUE_METHODS=('InsertPredictedFrame','InsertReceivedFrame',
               'PopFrame','AreFrameQueuesSynchronized')

class SupervisorServiceNativeQueueCalls(unittest.TestCase):
    def test_complete_network_boss_uses_one_supervisor_queue_owner(self):
        source=(ROOT/'src/SupervisorNetwork.cpp').read_text()
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('int __fastcall SupervisorServiceUpdate(Supervisor *supervisor)',source)
        self.assertIn('Supervisor *s = supervisor;',source)
        self.assertNotIn('SupervisorFrameQueueView',source)
        self.assertNotIn('frameQueues->',source)
        self.assertNotIn('reinterpret_cast<SupervisorFrameQueueView',source)
        self.assertEqual(source.count('g_Supervisor.InsertReceivedFrame('),2)
        self.assertEqual(source.count('s->InsertPredictedFrame('),2)
        self.assertEqual(source.count('s->InsertReceivedFrame('),1)
        self.assertEqual(source.count('s->PopFrame('),4)
        self.assertEqual(source.count('s->AreFrameQueuesSynchronized('),1)
        self.assertIn('SupervisorFrameEntryStorageView entries47C[20];',native)
        self.assertIn('SupervisorFrameEntryStorageView lastReceived56C[2];',native)
        for name in QUEUE_METHODS:
            self.assertIn(name+'(',native)

    def test_ten_queue_callees_have_original_bound_targets(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        targets={'InsertPredictedFrame':0x0042EAC0,
                 'InsertReceivedFrame':0x0042E9E0,
                 'PopFrame':0x0042EB80,
                 'AreFrameQueuesSynchronized':0x0042ECF0}
        expected=(('InsertPredictedFrame',2),('InsertReceivedFrame',3),
                  ('PopFrame',4),('AreFrameQueuesSynchronized',1))
        # The exact method owners are independently pinned. The nonexact
        # service's 106 fields are audited by the separate whole-boss tool.
        for name,count in expected:
            with self.subTest(method=name):
                self.assertEqual(len([1 for unit in units.values()
                                      if unit['target_address']==targets[name] and
                                      ('@Supervisor@@' in unit['symbol'])]),1)
                service=(ROOT/'src/SupervisorNetwork.cpp').read_text()
                self.assertEqual(service.count('->'+name+'(')+
                                 service.count('g_Supervisor.'+name+'('),count)
        self.assertEqual(units['supervisor-on-update']['size'],844)
        self.assertEqual(units['supervisor-on-update']['compare_size'],996)
        self.assertEqual(len(units['supervisor-on-update']['relocations']),92)
        self.assertTrue(any(r['target']==0x00430AA0
                            for r in units['supervisor-on-update']['relocations']))

if __name__=='__main__':unittest.main()
