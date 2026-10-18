"""TH09 RunEcl's actual boss-marker interrupt is NOT AsciiManager state.

Pure source/manifest guard: complete 15,564-byte original/COFF review and
four true exact callees are cold-checked separately by the real factory.
"""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sp = importlib.util.spec_from_file_location(
    'ecl_original_identity', ROOT/'scripts/audit-ecl-callsite-identities.py')
AUDIT = importlib.util.module_from_spec(sp)
sp.loader.exec_module(AUDIT)
sp = importlib.util.spec_from_file_location(
    'ecl_complete_frontier', ROOT/'scripts/check-boss-frontiers.py')
BOSS = importlib.util.module_from_spec(sp)
sp.loader.exec_module(BOSS)


class RealEclBossSoundOwners(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.run_state = (ROOT/'src/EclRunState.inl').read_text()
        cls.post_runtime = (ROOT/'src/EclPostRuntime.inl').read_text()
        cls.legacy = (ROOT/'scripts/audit-ecl-callsite-identities.py').read_text()
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']

    @staticmethod
    def evidence():
        calls = []
        for off, (symbol, va) in AUDIT.RUNECL_REAL_RUNSTATE_CALLS.items():
            calls.append(dict(candidate_offset=off, symbol=symbol,
                              expected_destination=va, target_destination=va,
                              identity_agrees=True, provenance={'canonical_units':['proven-exact']}))
        fields = []
        for off, (symbol, va) in AUDIT.RUNECL_REAL_RUNSTATE_GLOBALS.items():
            fields.append(dict(candidate_offset=off, symbol=symbol,
                               expected_value=va, target_value=va,
                               identity_agrees=True,
                               provenance={'canonical_reference_units':['proven-exact']}))
        return calls, fields

    def test_precise_original_boss_interrupt_and_state_are_different_callees(self):
        self.assertEqual(self.units['ascii-boss-marker-set-interrupt']['target_address'], 0x4067D0)
        self.assertEqual(self.units['ascii-manager-set-boss-marker-state']['target_address'], 0x40F890)
        self.assertEqual(self.units['ascii-boss-marker-set-position']['target_address'], 0x4067F0)
        self.assertNotEqual(0x4067D0,0x40F890)
        self.assertEqual(self.run_state.count('->SetBossMarkerInterrupt('), 2)
        self.assertEqual(self.run_state.count('->SetBossMarkerPosition('), 1)
        self.assertNotIn('.SetBossMarkerState(', self.run_state)
        self.assertNotIn('struct BossUiView', self.run_state)
        self.assertNotIn('g_BossUi', self.run_state)
        self.assertNotIn('BossUiView@Th09EclRunState', self.legacy)

    def test_two_original_globals_and_three_sources_use_real_symbols(self):
        self.assertIn('#include "AsciiBossMarkerVmView.hpp"', self.run_state)
        self.assertIn('#include "SoundPlayer.hpp"', self.run_state)
        self.assertIn('extern AsciiManager g_AsciiManager;', self.run_state)
        self.assertEqual(self.run_state.count('::g_AsciiManager'), 3)
        self.assertEqual(self.run_state.count('::g_SoundPlayer.PlaySoundPositionedByIdx('), 1)
        self.assertIn('g_SoundPlayer.PlaySoundByIdx(', self.post_runtime)
        self.assertNotIn('EnemyEclSoundPlayerView', self.post_runtime)
        self.assertNotIn('extern EnemyEclSoundPlayerView g_SoundPlayer', self.post_runtime)
        self.assertNotIn('SoundPlayerView@Th09EclRunState', self.legacy)

    def test_four_distinct_exact_callees_stay_on_true_vas(self):
        expectations = {
            'ascii-boss-marker-set-interrupt': (0x4067D0, 32,
                '?SetBossMarkerInterrupt@AsciiBossMarkerVmView@@QAEXHF@Z'),
            'ascii-boss-marker-set-position': (0x4067F0, 42,
                '?SetBossMarkerPosition@AsciiBossMarkerVmView@@QAEXHPBUFloat3@@@Z'),
            'soundplayer-play-sound-by-idx': (0x43E2F0, 140,
                '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z'),
            'soundplayer-play-positioned-sound-by-idx': (0x43E380, 153,
                '?PlaySoundPositionedByIdx@SoundPlayer@@QAEXHM@Z'),
        }
        for name,(addr,size,symbol) in expectations.items():
            with self.subTest(name=name):
                actual=self.units[name]
                self.assertEqual((actual['target_address'],actual['size'],actual['symbol']),
                                 (addr,size,symbol))
        self.assertEqual(sum(s for _,s,_ in expectations.values()), 367)

    def test_post_ecl_exact_carrier_uses_original_sound_and_relocation_layout(self):
        member=self.units['ecl-post-update-movement']
        self.assertEqual((member['size'],member['compare_size'],len(member['relocations'])),
                         (967,992,41))
        mapping={r['offset']:r for r in member['relocations']}
        self.assertEqual((mapping[0x39D]['symbol'], mapping[0x39D]['target']),
                         ('?g_SoundPlayer@@3VSoundPlayer@@A', 0x4DC698))
        self.assertEqual((mapping[0x3A2]['symbol'], mapping[0x3A2]['target']),
                         ('?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 0x43E2F0))
        self.assertFalse(any('EnemyEclSoundPlayerView' in r['symbol']
                             for r in member['relocations']))
        self.assertEqual((self.units['ecl-compare-operands']['size'],
                          self.units['ecl-compare-operands']['compare_size']), (842,892))

    def test_explicit_eight_site_cold_owner_guard_is_noncrediting(self):
        a,b=self.evidence()
        report=AUDIT.validate_real_runstate_callees(a,b)
        self.assertEqual(report['original_bound_calls'],4)
        self.assertEqual(report['original_bound_globals'],4)
        self.assertEqual(report['exactness_credit'],'none')
        self.assertEqual(report['boss_marker_interrupt_va'],'0x4067d0')
        self.assertEqual(report['unrelated_ascii_boss_marker_state_va'],'0x40f890')

    def test_old_fake_state_method_wrong_actual_callee_or_missing_referent_rejected(self):
        a,b=self.evidence()
        wrongs = (
            ('symbol', '?SetBossMarkerState@AsciiManager@@QAEXHH@Z'),
            ('expected_destination',0x40F890),
            ('target_destination',0x40F890),
            ('identity_agrees',False),
            ('provenance',{'diagnostic_alias':True}),
        )
        for field,value in wrongs:
            damaged=copy.deepcopy(a)
            damaged[0][field]=value
            with self.subTest(field=field),self.assertRaises(ValueError):
                AUDIT.validate_real_runstate_callees(damaged,b)
        with self.assertRaises(ValueError):
            AUDIT.validate_real_runstate_callees(a[:-1],b)
        for field,value in (('symbol','?g_BossUi@Th09EclRunState@@3UBossUiView@1@A'),
                            ('expected_value',0x4DC698),
                            ('target_value',0x4DC698),
                            ('identity_agrees',False),
                            ('provenance',{'diagnostic_alias':True})):
            damaged=copy.deepcopy(b)
            damaged[0][field]=value
            with self.subTest(field=field),self.assertRaises(ValueError):
                AUDIT.validate_real_runstate_callees(a,damaged)
        with self.assertRaises(ValueError):
            AUDIT.validate_real_runstate_callees(a,b[:-1])

    def test_two_authentic_callee_carriers_never_duplicate_source_cold_build(self):
        with patch.object(BOSS,'build') as build,patch.object(
                BOSS,'canonical',side_effect=lambda name: {'unit': name}):
            full=BOSS.replay_runecl_real_state_callees(shared_with_enemy_prechecked=False)
        self.assertEqual([x['unit'] for x in full],[
            'ascii-boss-marker-set-interrupt','ascii-boss-marker-set-position',
            'soundplayer-play-sound-by-idx','soundplayer-play-positioned-sound-by-idx'])
        self.assertEqual([c.args[0] for c in build.call_args_list],[
            'ascii-boss-marker-set-interrupt','soundplayer-play-sound-by-idx'])
        with patch.object(BOSS,'build') as build,patch.object(
                BOSS,'canonical',side_effect=lambda name:{'unit':name}):
            all_mode=BOSS.replay_runecl_real_state_callees(shared_with_enemy_prechecked=True)
        self.assertEqual(all_mode,[{'unit':'soundplayer-play-sound-by-idx'}])
        build.assert_called_once_with('soundplayer-play-sound-by-idx')

    def test_faked_callee_original_location_or_profile_is_rejected(self):
        for name,field,value in (
            ('ascii-boss-marker-set-interrupt','symbol','?SetBossMarkerState@AsciiManager@@QAEXHH@Z'),
            ('ascii-boss-marker-set-interrupt','target_address',0x40F890),
            ('ascii-boss-marker-set-position','object','build/matching/Oops.obj'),
            ('soundplayer-play-positioned-sound-by-idx','profile',['/Ob0']),
            ('soundplayer-play-sound-by-idx','source','src/NotReal.cpp')):
            invalid=copy.deepcopy(self.units)
            invalid[name][field]=value
            with self.subTest(name=name,field=field),patch.object(
                BOSS.tomllib,'load',return_value={'units':invalid}),patch.object(
                BOSS,'build'),patch.object(BOSS,'canonical'),self.assertRaises(RuntimeError):
                BOSS.replay_runecl_real_state_callees(shared_with_enemy_prechecked=False)


if __name__=='__main__':
    unittest.main()
