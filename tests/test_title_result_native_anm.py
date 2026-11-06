"""Result ANM call contracts, independent of source spelling/private input."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec); spec.loader.exec_module(result)
    return result
A = module('native_result_audit', 'inspect-title-update-result.py')
B = module('native_result_frontier', 'check-boss-frontiers.py')
ROOT_SYMBOL = '?g_AnmManager@@3PAVAnmManager@@A'
CALLS = [('DIR32', ROOT_SYMBOL, 0x4DC550),
         ('REL32', '?LoadSurface@AnmManager@@QAEHHPBD@Z', 0x43CAE0),
         ('DIR32', ROOT_SYMBOL, 0x4DC550),
         ('REL32', '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z', 0x436DE0),
         ('DIR32', ROOT_SYMBOL, 0x4DC550),
         ('REL32', '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z', 0x439560)]
def report():
    return {'native_anm_fields': [dict(type=k, symbol=s, root=d, effective=d, addend=0)
                                  for k, s, d in CALLS]}

class NativeResultAnm(unittest.TestCase):
    def test_complete_native_provider_contract(self):
        B.guard_title_result_native_anm(report())

    def test_native_and_historical_symbols_encode_same_original_edges(self):
        raw = bytes(A.SIZE)
        legacy = [dict(offset=o, type=k, symbol=s, addend=0) for o, k, s, _ in A.FIELDS]
        native = deepcopy(legacy)
        replacement = {
            '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A': ROOT_SYMBOL,
            '?LoadSurface@TitleAnmManagerView@@QAEHHPBD@Z': CALLS[1][1],
            '?SetInterruptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@HH@Z': CALLS[3][1],
            '?ExecuteScriptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@H@Z': CALLS[5][1],
        }
        for r in native: r['symbol'] = replacement.get(r['symbol'], r['symbol'])
        self.assertEqual(A.bind_candidate(raw, legacy), A.bind_candidate(raw, native))

    def test_rejects_numeric_alias_with_wrong_provider(self):
        for i in range(6):
            r = report(); r['native_anm_fields'][i]['symbol'] = '?LegacyAlias'
            with self.subTest(i=i), self.assertRaises(RuntimeError):
                B.guard_title_result_native_anm(r)

    def test_rejects_wrong_kind_root_addend_or_destination(self):
        for i in range(6):
            for key, value in [('type', 'wrong'), ('root', 0x400000),
                               ('effective', 0x400001), ('addend', 1)]:
                r = report(); r['native_anm_fields'][i][key] = value
                with self.subTest(i=i, key=key), self.assertRaises(RuntimeError):
                    B.guard_title_result_native_anm(r)

    def test_rejects_missing_duplicate_and_absent_inventory(self):
        for mode in ['missing', 'duplicate', 'absent']:
            r = report()
            if mode == 'missing': r['native_anm_fields'].pop()
            elif mode == 'duplicate': r['native_anm_fields'].append(deepcopy(r['native_anm_fields'][0]))
            else: del r['native_anm_fields']
            with self.subTest(mode=mode), self.assertRaises(RuntimeError):
                B.guard_title_result_native_anm(r)

    def test_rejects_changed_call_order(self):
        r = report(); r['native_anm_fields'][1], r['native_anm_fields'][3] = r['native_anm_fields'][3], r['native_anm_fields'][1]
        with self.assertRaises(RuntimeError): B.guard_title_result_native_anm(r)

if __name__ == '__main__': unittest.main()
