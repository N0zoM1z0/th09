"""Synthetic CFGs test conservative result consumption without private bytes."""
import importlib.util
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('timer_cfg',ROOT/'scripts/audit-timer-result-cfg.py')
probe=importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def node(kind='ordinary',reads=0,writes=0,successors=()):
    return dict(kind=kind,reads=reads,writes=writes,successors=list(successors))


class TimerResultCFGTests(unittest.TestCase):
    def kinds(self,nodes,**kwargs):
        return {o['kind'] for o in probe.trace_paths(nodes,0,0,10,**kwargs)}

    def test_both_direct_paths_must_kill_the_result(self):
        nodes={0:node(successors=(1,2)),1:node(writes=0xffffffff),2:node(writes=0xffffffff)}
        self.assertEqual(self.kinds(nodes),{'full-eax-overwrite'})
        nodes[2]=node(reads=0xff)
        self.assertEqual(self.kinds(nodes),{'full-eax-overwrite','possible-explicit-eax-read'})

    def test_partial_write_does_not_hide_a_live_high_byte(self):
        nodes={0:node(writes=0xff,successors=(1,)),1:node(reads=0xff00)}
        self.assertEqual(self.kinds(nodes),{'possible-explicit-eax-read'})
        nodes[1]=node(reads=0xff,writes=0xffffff00)
        self.assertEqual(self.kinds(nodes),{'full-eax-overwrite'})

    def test_read_modify_write_is_a_read_before_a_kill(self):
        self.assertEqual(self.kinds({0:node(reads=0xffffffff,writes=0xffffffff)}),
                         {'possible-explicit-eax-read'})

    def test_calls_indirect_transfers_and_returns_remain_distinct(self):
        for kind,expected in (('call','callee-ABI-unknown'),('indirect','indirect-transfer-unknown'),
                              ('return','forwarded-register-return'),('unknown','instruction-unknown')):
            with self.subTest(kind=kind):
                self.assertEqual(self.kinds({0:node(kind)}),{expected})
        self.assertEqual(self.kinds({0:node('call',reads=0xffffffff)}),{'possible-explicit-eax-read'})

    def test_cycles_bounds_and_decode_gaps_never_become_nonuse_proof(self):
        self.assertEqual(self.kinds({0:node(successors=(0,))}),{'revisit-unknown'})
        self.assertEqual(self.kinds({0:node(successors=(10,))}),{'owner-boundary-unknown'})
        self.assertEqual(self.kinds({}),{'decode-gap-unknown'})
        self.assertEqual(self.kinds({0:node(successors=(1,))},budget=1),{'budget-unknown'})

    def test_decoder_zero_idiom_partial_write_and_address_read(self):
        try:
            import capstone
        except ImportError:
            self.skipTest('optional Capstone decoder is unavailable')
        # xor eax,eax; mov al,1; mov ecx,[eax]; ret
        nodes=probe.decode_nodes(bytes.fromhex('31c0b0018b08c3'),0)
        self.assertEqual((nodes[0]['reads'],nodes[0]['writes']),(0,0xffffffff))
        self.assertEqual(nodes[2]['writes'],0xff)
        self.assertEqual(nodes[4]['reads'],0xffffffff)
        self.assertEqual(nodes[6]['kind'],'return')
