"""TH09 Supervisor queue complete native owner, with no duplicate class.

Original PE/COFF source-cold comparators protect the complete methods and
message switch tail; this checks source identity and preserved call locations.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorNoFrameQueueFacade(unittest.TestCase):
    def test_only_one_native_supervisor_receiver_and_two_physical_arrays(self):
        self.assertFalse((ROOT/'src/SupervisorFrameQueueView.hpp').exists())
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('SupervisorFrameEntryStorageView entries47C[20];',native)
        self.assertIn('SupervisorFrameEntryStorageView lastReceived56C[2];',native)
        self.assertIn('SupervisorSizeIs7B0',native)
        for name in ('InsertReceivedFrame','InsertPredictedFrame','PopFrame',
                     'AreFrameQueuesSynchronized'):
            self.assertIn(name+'(',native)
        for name in ('SupervisorFrameQueueInsert.cpp','SupervisorFrameQueueOps.cpp',
                     'SupervisorNetworkQueue.cpp','SupervisorMode4Reset.cpp',
                     'SupervisorNetworkMessageHandler.cpp','SupervisorNetwork.cpp'):
            with self.subTest(source=name):
                s=(ROOT/'src'/name).read_text()
                self.assertNotIn('SupervisorFrameQueueView',s)
                self.assertNotIn('SupervisorFrameQueueView.hpp',s)

    def test_mode4_preserves_native_title_supervisor_abi_and_original_calls(self):
        source=(ROOT/'src/SupervisorMode4Reset.cpp').read_text()
        self.assertIn('struct TitleSupervisorView : Supervisor {};',source)
        self.assertIn('TitleResetNativeSupervisorState',source)
        self.assertEqual(source.count('g_TitleSupervisor.InsertReceivedFrame('),2)
        self.assertIn('memset(supervisor->entries47C, 0, sizeof(supervisor->entries47C))',source)
        self.assertIn('memset(supervisor->lastReceived56C, 0, sizeof(supervisor->lastReceived56C))',source)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        title=units['supervisor-reset-title-mode4']
        self.assertEqual((title['size'],title['target_address']), (177,0x0042EC30))
        calls=[(r['offset'],r['target']) for r in title['relocations']
               if 'InsertReceivedFrame@Supervisor@@' in r['symbol']]
        self.assertEqual(calls,[(0x75,0x0042E9E0),(0x92,0x0042E9E0)])
        self.assertEqual(len(title['relocations']),12)

    def test_network_service_and_exact_message_share_this_physical_queue(self):
        source=(ROOT/'src/SupervisorNetwork.cpp').read_text()
        message=(ROOT/'src/SupervisorNetworkMessageHandler.cpp').read_text()
        self.assertEqual(source.count('g_Supervisor.InsertReceivedFrame('),2)
        self.assertEqual(message.count('g_Supervisor.InsertReceivedFrame('),3)
        self.assertNotIn('SupervisorFrameQueueView',source)
        self.assertNotIn('SupervisorFrameQueueView',message)
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        msg=units['supervisor-network-message-handler']
        self.assertEqual((msg['size'],msg['compare_size']), (768,808))
        self.assertEqual(len(msg['relocations']),46)

if __name__=='__main__':unittest.main()
