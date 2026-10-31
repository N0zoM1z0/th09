"""Reject reset aliases and stale native-owner/actual-call identities."""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('cleanup_native',ROOT/'scripts/inspect-game-manager-cleanup.py')
audit=importlib.util.module_from_spec(spec);spec.loader.exec_module(audit)


def fixture():
    units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    rows=[dict(offset=off,type='REL32',symbol=symbol,addend=0)
          for _,symbol,_,_,off in audit.NATIVE_RESETS]
    original=[(off,'REL32',address) for _,_,address,_,off in audit.NATIVE_RESETS]
    return rows,original,units


class CleanupNativeResets(unittest.TestCase):
    def test_three_original_calls_bind_complete_native_implementations(self):
        rows,original,units=fixture()
        result=audit.require_native_resets(rows,original,units)
        self.assertEqual([r['target'] for r in result],[0x4011C0,0x403D80,0x41A230])
        caller=units['game-manager-cleanup-gameplay-state']
        for row in rows:
            actual=next(r for r in caller['relocations'] if r['offset']==row['offset'])
            self.assertEqual((actual['symbol'],actual['type']),(row['symbol'],'REL32'))

    def test_alias_wrong_abi_offset_kind_or_addend_is_rejected(self):
        rows,original,units=fixture()
        for change in [dict(symbol='?ClearSpellBackgroundState@SetupBackgroundResetView@@QAEXXZ'),
                       dict(symbol='?ClearSpellBackgroundState@Background@@SIXPAX@Z'),
                       dict(offset=0x38),dict(type='DIR32'),dict(addend=1),dict(addend=False)]:
            trial=copy.deepcopy(rows);trial[0].update(change)
            with self.subTest(change=change),self.assertRaises(ValueError):
                audit.require_native_resets(trial,original,units)
        for trial in [rows[:-1],rows+rows[:1]]:
            with self.assertRaises(ValueError):audit.require_native_resets(trial,original,units)
        for alias in ['?ClearSpellBackgroundState@SetupBackgroundResetView@@QAEXXZ',
                      '?RefreshResource@SetupCardAttackResetView@@QAEXXZ',
                      '?ResetTransitionState@SetupFrontSideResetView@@QAEXXZ']:
            self.assertNotIn(alias,audit.SOURCES)

    def test_original_call_geometry_is_independent_of_candidate_records(self):
        rows,original,units=fixture()
        for trial in [original[:-1],original+original[:1],
                      [(off+1,kind,va) for off,kind,va in original],
                      [(off,'DIR32',va) for off,_,va in original],
                      [(off,kind,va+1) for off,kind,va in original]]:
            with self.subTest(trial=trial),self.assertRaises(ValueError):
                audit.require_native_resets(rows,trial,units)

    def test_native_producer_symbol_extent_and_address_cannot_drift(self):
        rows,original,units=fixture();name=audit.NATIVE_RESETS[0][0]
        for key,value in [('symbol','wrong'),('size',10),('target_address',0x4011C1)]:
            trial=copy.deepcopy(units);trial[name][key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):
                audit.require_native_resets(rows,original,trial)
        del units[name]
        with self.assertRaises(ValueError):audit.require_native_resets(rows,original,units)
