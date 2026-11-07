"""Original-bound native Supervisor network/frame-state source ownership."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SupervisorNetworkNativeOwnerTests(unittest.TestCase):
    def test_network_service_uses_native_supervisor_frame_and_queue_state(self):
        header = (ROOT / 'src/Supervisor.hpp').read_text()
        network = (ROOT / 'src/SupervisorNetwork.cpp').read_text()
        self.assertIn('Supervisor *s = supervisor;', network)
        self.assertNotIn('struct SupervisorNetworkView', network)
        self.assertIn('SupervisorNativeNetworkServiceFields', header)
        self.assertIn('SupervisorNetworkFrameTimingSizeIs20', header)
        for name, offset in (
            ('networkTiming458', '0x458'), ('networkBusy478', '0x478'),
            ('entries47C', '0x47C'), ('lastReceived56C', '0x56C'),
            ('networkMessageVm584', '0x584'),
            ('networkConnectStartTime588', '0x588'),
            ('flagsLow5D4', '0x5D4'),
        ):
            with self.subTest(field=name):
                self.assertIn('offsetof(Supervisor, ' + name + ') == ' + offset,
                              header)
        for name in ('frameCounter', 'confirmedFrame', 'frameMode',
                     'frameStartTime', 'lastFrameTime', 'waitTime',
                     'connectionTime'):
            with self.subTest(network_field=name):
                self.assertIn('s->networkTiming458.' + name, network)
        self.assertIn('s->networkMessageVm584', network)
        self.assertIn('s->flagsLow5D4', network)
        self.assertNotIn('SupervisorFrameQueueView', network)
        self.assertNotIn('frameQueues->', network)
        self.assertEqual(network.count('g_Supervisor.InsertReceivedFrame('), 2)
        self.assertIn('s->InsertPredictedFrame(',network)
        self.assertIn('s->PopFrame(',network)
        self.assertIn('s->AreFrameQueuesSynchronized(',network)
        self.assertNotIn('reinterpret_cast<SupervisorFrameQueueView *>(', network)

    def test_true_queue_producers_and_consumers_remain_independently_mapped(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        for name, size in (
            ('supervisor-frame-queue-insert-predicted', 191),
            ('supervisor-frame-queue-pop-frame', 119),
            ('supervisor-network-frame-queues-synchronized', 49),
            ('supervisor-reset-title-mode4', 177),
        ):
            with self.subTest(unit=name):
                self.assertEqual(units[name]['size'], size)
        reset = (ROOT / 'src/SupervisorMode4Reset.cpp').read_text()
        self.assertIn('struct TitleSupervisorView : Supervisor {};', reset)
        self.assertIn('TitleResetNativeSupervisorState', reset)
        self.assertNotIn('unsigned char unknown000[0x458];', reset)
        for field in ('frameCounter', 'confirmedFrame', 'frameMode',
                      'frameStartTime', 'lastFrameTime', 'connectionTime'):
            with self.subTest(mode4_field=field):
                self.assertIn('supervisor->networkTiming458.' + field, reset)
        self.assertIn('memset(supervisor->entries47C, 0,', reset)
        self.assertNotIn('SupervisorFrameQueueView', reset)
        self.assertEqual(reset.count('g_TitleSupervisor.InsertReceivedFrame('),2)
        self.assertNotIn('frameQueues->',reset)
        self.assertIn('void Supervisor::InsertReceivedFrame(',
                      (ROOT / 'src/SupervisorFrameQueueInsert.cpp').read_text())
        synchronized = (ROOT / 'src/SupervisorNetworkQueue.cpp').read_text()
        self.assertNotIn('struct SupervisorNetworkQueueSyncLayout', synchronized)
        self.assertIn('Supervisor *view = static_cast<Supervisor *>(this);',
                      synchronized)
        self.assertIn('view->entries47C[side * 10].frame04', synchronized)
        self.assertIn('view->entries47C[10].frame04 == view->entries47C[0].frame04',
                      synchronized)


if __name__ == '__main__':
    unittest.main()
