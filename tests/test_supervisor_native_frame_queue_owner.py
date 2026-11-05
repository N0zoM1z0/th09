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
        queue = (ROOT/'src/SupervisorFrameQueueView.hpp').read_text()
        storage = (ROOT/'src/SupervisorFrameQueueStorageView.hpp').read_text()
        self.assertIn('struct SupervisorFrameQueueView : Supervisor',queue)
        self.assertIn('SupervisorQueueNativeReceiverLayout',queue)
        self.assertNotIn('struct SupervisorFrameQueueStorageView',storage)
        self.assertNotIn('SupervisorFrameQueueInsertLayout',storage)
        self.assertNotIn('SupervisorFrameQueueOpsLayout',storage)
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertIn('#include "Supervisor.hpp"',received)
        self.assertIn('Supervisor *view = static_cast<Supervisor *>(this);',source)
        self.assertIn('Supervisor *view = static_cast<Supervisor *>(this);',received)
        self.assertNotIn('SupervisorFrameQueueOpsLayout *view =',source)
        self.assertNotIn('SupervisorFrameQueueInsertLayout *view =',received)
        self.assertNotIn('unsigned char *sideBase =',received)
        self.assertIn('view->lastReceived56C[side]',source)
        self.assertIn('view->lastReceived56C[side]',received)

    def test_exact_consumer_reads_original_native_queue_elements(self):
        consumer=(ROOT/'src/SupervisorFrameQueueOps.cpp').read_text()
        self.assertIn('entries47C[side * 10].predicted08',consumer)
        self.assertIn('entries47C[side * 10].packedInput00',consumer)
        self.assertIn('entries47C[side * 10].seed02',consumer)
        self.assertIn('&static_cast<Supervisor *>(this)->entries47C[side * 10]',consumer)
        self.assertNotIn('side * 0x78 + 0x47C',consumer)
        self.assertNotIn('side * 0x78 + 0x484',consumer)

    def test_pop_frame_uses_the_exact_native_replay_input_array(self):
        source=(ROOT/'src/SupervisorFrameQueueOps.cpp').read_text()
        header=(ROOT/'src/ReplayInputState.hpp').read_text()
        self.assertIn('#include "ReplayInputState.hpp"',source)
        self.assertNotIn('struct SupervisorReplayInputHeadOpsView',source)
        self.assertNotIn('currentInput00',source)
        self.assertIn('g_ReplayInputStates[side].currentInput;',source)
        self.assertIn('SupervisorNativeFrameInputState',source)
        self.assertIn('sizeof(ReplayInputState) == 0x8E',source)
        self.assertIn('offsetof(ReplayInputState, currentInput) == 0x00',source)
        self.assertIn('extern ReplayInputState g_ReplayInputStates[3];',header)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        original=units['supervisor-frame-queue-pop-frame']
        self.assertEqual((original['target_address'],original['size']),(0x42EB80,119))
        self.assertEqual([(r['offset'],r['type'],r['symbol'],r['target'])
                          for r in original['relocations']],
                         [(0x29,'DIR32','?g_ReplayInputStates@@3PAUReplayInputState@@A',0x4ACE18)])
        replay=units['replay-manager-playback-input']
        self.assertTrue(any(r['symbol']=='?g_ReplayInputStates@@3PAUReplayInputState@@A'
                            and r['target']==0x4ACE18 for r in replay['relocations']))

    def test_exact_canonical_queue_methods_remain_frozen(self):
        with (ROOT/'config/match-units.toml').open('rb') as fd:
            units=tomllib.load(fd)['units']
        for name,size,fields in (
            ('supervisor-frame-queue-insert-received',213,0),
            ('supervisor-frame-queue-pop-frame',119,1),
            ('supervisor-frame-queue-insert-predicted',191,0),
            ('supervisor-network-frame-queues-synchronized',49,0)):
            with self.subTest(name=name):
                unit=units[name]
                self.assertEqual((unit['size'],len(unit['relocations'])),(size,fields))


if __name__ == '__main__':
    unittest.main()
