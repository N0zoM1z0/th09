"""Cursor original-PE field guard accepts only the exact native ANM manager root."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('title_cursor_native_manager',ROOT/'scripts/inspect-title-character-cursors.py')
cursor=importlib.util.module_from_spec(spec);spec.loader.exec_module(cursor)

class TitleCursorNativeAnmGlobalOracle(unittest.TestCase):
    def test_all_original_41_owner_addresses_remain_fixed(self):
        self.assertEqual(cursor.MANAGER_SYMBOL,'?g_AnmManager@@3PAVAnmManager@@A')
        self.assertEqual(cursor.MANAGER_ADDRESS,0x004DC550)
        active=cursor.OWNERS['active']
        self.assertEqual((active['address'],active['size']),(0x00424EDD,211))
        self.assertEqual([(off,addr) for off,kind,sym,addr in active['fields']
                          if sym==cursor.MANAGER_SYMBOL],[(0x64,0x4DC550),(0xC0,0x4DC550)])

    def test_reject_legacy_or_fabricated_manager_source_symbols(self):
        owner=cursor.OWNERS['active']
        original=[{'offset':off,'type':kind,'symbol':sym,'addend':0}
                  for off,kind,sym,dest in owner['fields']]
        raw=b'\x90'*owner['size']
        self.assertEqual(len(cursor.bind_candidate(raw,original,owner)),owner['size'])
        for bogus in ('?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A',
                      '?g_AnmManager@@3UTitleAnmManagerView@@A'):
            forged=deepcopy(original)
            forged[next(i for i,r in enumerate(forged) if r['symbol']==cursor.MANAGER_SYMBOL)]['symbol']=bogus
            with self.subTest(symbol=bogus),self.assertRaises(ValueError):
                cursor.bind_candidate(raw,forged,owner)

if __name__=='__main__':unittest.main()
