"""Native Supervisor queue calls in original-exact 808-byte message carrier.

The physical PE/COFF compare, not these source checks, proves the complete
768-byte function and its 40-byte jump-table/data suffix.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
CALL='?InsertReceivedFrame@Supervisor@@QAEXHHGF@Z'

class NetworkMessageNativeSupervisorCalls(unittest.TestCase):
    def test_one_real_supervisor_owns_network_message_frame_queue(self):
        source=(ROOT/'src/SupervisorNetworkMessageHandler.cpp').read_text()
        self.assertNotIn('SupervisorFrameQueueView',source)
        self.assertNotIn('static_cast<SupervisorFrameQueueView *>(',source)
        self.assertEqual(source.count('g_Supervisor.InsertReceivedFrame('),3)
        self.assertIn('this->lastReceivedC8 = *packet;',source)
        native=(ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('void InsertReceivedFrame(int side, int frame, unsigned short packedInput, short seed);',native)
        self.assertIn('SupervisorFrameEntryStorageView entries47C[20];',native)

    def test_all_three_original_call_operands_and_full_data_tail_are_bound(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            unit=tomllib.load(f)['units']['supervisor-network-message-handler']
        self.assertEqual(unit['size'],768)
        self.assertEqual(unit['compare_size'],808)
        self.assertEqual(unit['target_address'],0x00432B70)
        self.assertEqual(len(unit['relocations']),46)
        calls=[r for r in unit['relocations'] if r['symbol']==CALL]
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in calls],
                         [(0x89,'REL32',0x0042E9E0),
                          (0x156,'REL32',0x0042E9E0),
                          (0x16F,'REL32',0x0042E9E0)])
        labels=[r for r in unit['relocations'] if r['symbol'].startswith('$L')]
        self.assertEqual(len(labels),8)
        self.assertEqual(len([r for r in labels if r['offset']>=768]),6)

if __name__=='__main__':unittest.main()
