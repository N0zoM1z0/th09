"""The exact loading transition leaves use the real native AnmVm interrupt API."""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]

class SupervisorNativeAnmInterrupts(unittest.TestCase):
    def test_no_vm_shadow_allocations_and_real_member_calls(self):
        source=(ROOT/'src/SupervisorLoadingTransitions.cpp').read_text()
        header=(ROOT/'src/AnmVmLifecycle.hpp').read_text()
        self.assertIn('#include "AnmVmLifecycle.hpp"',source)
        self.assertNotIn('struct SupervisorLoadingTransitionVmView',source)
        self.assertIn('extern AnmVm g_SupervisorLoadingVms[3];',source)
        self.assertIn('sizeof(AnmVm) == 0x2A4',source)
        self.assertIn('offsetof(AnmVm, pendingInterrupt) == 0x1FE',source)
        self.assertIn('void SetInterrupt(short interrupt);',header)
        for i in (0,1,2):
            self.assertEqual(source.count('g_SupervisorLoadingVms['+str(i)+'].SetInterrupt('),2)

    def test_twelve_true_original_referents_are_exact(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:u=tomllib.load(f)['units']
        method=u['anm-vm-set-interrupt']
        self.assertEqual((method['target_address'],method['size']),(0x00406790,18))
        native_symbol='?g_SupervisorLoadingVms@@3PAUAnmVm@@A'
        for name in ('supervisor-hide-loading-vms','supervisor-finish-loading'):
            with self.subTest(name=name):
                v=u[name];rows={r['offset']:r for r in v['relocations']}
                self.assertEqual((v['size'],len(rows)),(85,9))
                for i,(glob_offset,method_offset) in enumerate(((0x0F,0x14),(0x1B,0x20),(0x27,0x2C))):
                    with self.subTest(index=i):
                        self.assertEqual((rows[glob_offset]['symbol'],rows[glob_offset]['target'],rows[glob_offset].get('addend',0)),
                                         (native_symbol,0x004B38B0,i*0x2A4))
                        self.assertEqual((rows[method_offset]['symbol'],rows[method_offset]['target'],rows[method_offset]['type']),
                                         (method['symbol'],method['target_address'],'REL32'))

if __name__=='__main__':unittest.main()
