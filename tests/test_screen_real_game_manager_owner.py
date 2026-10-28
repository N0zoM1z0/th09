"""ScreenEffect's target GameManager address has one native C++ owner.

The nonexact 336-byte shake envelope stays noncrediting. Actual original PE
relocations and all 18 exact ScreenEffect owners are cold rechecked by the
separate check-boss-frontiers.py --owner screen replay.
"""
from pathlib import Path
import importlib.util
import tomllib
import unittest
from copy import deepcopy

ROOT=Path(__file__).resolve().parents[1]
REAL='?g_GameManager@@3UAsciiGameManagerView@@A'
OLD='?g_GameManager@@3UScreenEffectGameManagerView@@A'

sp=importlib.util.spec_from_file_location('screen_effect_attested',ROOT/'scripts/inspect-screen-shake-envelope.py')
ORACLE=importlib.util.module_from_spec(sp)
sp.loader.exec_module(ORACLE)


class SharedScreenGameManager(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.oracle=(ROOT/'scripts/inspect-screen-shake-envelope.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as src:
            cls.units=tomllib.load(src)['units']

    def test_true_original_envelope_global_with_real_coff_addend(self):
        self.assertEqual((ORACLE.BASE,ORACLE.SIZE),(0x4227A0,336))
        self.assertEqual(len(ORACLE.FIELDS),20)
        self.assertEqual(ORACLE.FIELDS[0],(0x6,'DIR32',REAL,0x4A7D90,0x348))
        self.assertNotIn(OLD,self.oracle)
        self.assertEqual(ORACLE.TARGET_SHA256,
                         '1d33957d51f8cc170471b9673a938899281cfa07fbed746eb0843e62e2597472')

    def test_exact_screen_callers_refer_to_real_global(self):
        self.assertEqual(self.units['screen-effect-calc-shake']['size'],316)
        self.assertEqual(self.units['screen-effect-calc-fade-out']['size'],123)
        expected={'screen-effect-calc-shake':{0x10,0x18},
                  'screen-effect-calc-fade-out':{0x57,0x60}}
        for name,offsets in expected.items():
            v=self.units[name]
            globals_={r['offset']:r for r in v['relocations'] if 'g_GameManager' in r['symbol']}
            self.assertEqual(set(globals_),offsets)
            for offset,row in globals_.items():
                with self.subTest(owner=name,offset=offset):
                    self.assertEqual((row['symbol'],row['target'],row['type']),
                                     (REAL,0x4A7D90,'DIR32'))
        for v in self.units.values():
            for row in v.get('relocations',[]):
                self.assertNotEqual(row['symbol'],OLD)

    def test_exact_register_chain_private_labels_remain_original_address(self):
        v=self.units['screen-effect-register-chain']
        self.assertEqual((v['size'],v['compare_size'],len(v['relocations'])),(447,480,55))
        original={0x68:0x422EE0,0x1C0:0x422D8C,0x1C4:0x422D93,
                  0x1C8:0x422D9D,0x1CC:0x422DED,0x1D0:0x422DC5,
                  0x1D4:0x422E12,0x1D8:0x422E37,0x1DC:0x422E5C}
        local={r['offset']:r for r in v['relocations'] if r['symbol'].startswith('$')}
        self.assertEqual(set(local),set(original))
        for off,va in original.items():
            with self.subTest(offset=hex(off)):
                self.assertEqual((local[off]['target'],local[off]['type']),(va,'DIR32'))

    def test_original_field_identity_gate_rejects_false_native_type(self):
        # No synthetic alias to ScreenEffectGameManagerView should pass the
        # original address/field-order auditor, even when VA and addend agree.
        rows=[]
        for offset,kind,symbol,va,addend in ORACLE.FIELDS:
            rows.append({'offset':offset,'type':kind,'symbol':symbol,'addend':addend})
        ORACLE.require_candidate(rows,ORACLE.SIZE)
        tampered=deepcopy(rows)
        tampered[0]['symbol']=OLD
        with self.assertRaisesRegex(ValueError,'identity|name|addend'):
            ORACLE.require_candidate(tampered,ORACLE.SIZE)
        tampered=deepcopy(rows)
        tampered[0]['addend']=0x344
        with self.assertRaises(ValueError):
            ORACLE.require_candidate(tampered,ORACLE.SIZE)


if __name__=='__main__':
    unittest.main()
