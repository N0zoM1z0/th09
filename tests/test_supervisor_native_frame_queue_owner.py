"""Native Supervisor frame queue storage across boss and three queue methods.

The 213-byte InsertReceivedFrame remains unclassified and still has observed
differences; do not claim matching credit from its successful native compile.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class NativeSupervisorFrameQueues(unittest.TestCase):
    def test_shared_physical_queue_owned_by_supervisor(self):
        source=(ROOT/'src/SupervisorFrameQueueOps.cpp').read_text()
        received=(ROOT/'src/SupervisorFrameQueueInsert.cpp').read_text()
        header=(ROOT/'src/Supervisor.hpp').read_text()
        update=(ROOT/'src/SupervisorUpdate.cpp').read_text()
        for snippet in ('SupervisorFrameEntryStorageView entries47C[20];',
                        'SupervisorFrameEntryStorageView lastReceived56C[2];',
                        'offsetof(Supervisor, entries47C) == 0x47C',
                        'offsetof(Supervisor, lastReceived56C) == 0x56C'):
            self.assertIn(snippet,header)
        self.assertIn('SupervisorUpdateNativeQueueAt47C',update)
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertIn('#include "Supervisor.hpp"',received)
        self.assertIn('Supervisor *view = reinterpret_cast<Supervisor *>(this);',source)
        self.assertIn('Supervisor *view = reinterpret_cast<Supervisor *>(this);',received)
        self.assertNotIn('SupervisorFrameQueueOpsLayout *view =',source)
        self.assertNotIn('SupervisorFrameQueueInsertLayout *view =',received)
        self.assertIn('offsetof(Supervisor, entries47C)',received)
        self.assertIn('view->lastReceived56C[side]',source)
        self.assertIn('view->lastReceived56C[side]',received)

    def test_exact_canonical_queue_methods_remain_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        for name,size,fields in (
            ('supervisor-frame-queue-pop-frame',119,1),
            ('supervisor-frame-queue-insert-predicted',191,0),
            ('supervisor-network-frame-queues-synchronized',49,0)):
            with self.subTest(name=name):
                unit=units[name]
                self.assertEqual((unit['size'],len(unit['relocations'])),(size,fields))
        self.assertNotIn('supervisor-frame-queue-insert-received',
                         units, 'Unclassified InsertReceived must not be counted as exact')


if __name__ == '__main__':
    unittest.main()
