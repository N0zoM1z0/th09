"""No-target input guards for the complete TH09 1,689-byte gameplay worker.

These pure-input tests do not claim original-byte acceptance. The required
actual Japanese PE/COFF check runs separately in check-boss-frontiers.py.
"""

import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'gameplay_setup_frontier', ROOT / 'scripts/check-boss-frontiers.py')
front = importlib.util.module_from_spec(spec)
spec.loader.exec_module(front)


def baseline():
    differences = [i for i in range(1689)
                   if front.GAMEPLAY_SETUP_RESIDUAL_MASK[i // 8] & (1 << (i % 8))]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': front.GAMEPLAY_SETUP_TARGET_SHA,
        'target_bytes': 1689, 'candidate_bytes': 1682,
        'target_instructions': 413, 'candidate_instructions': 413,
        'original_fields': 172, 'bound_fields': 172,
        'ordered_calls': 35, 'candidate_calls': 35,
        'ordered_calls_agree': True,
        'native_replay_calls': [
                dict(symbol='?Create@ReplayManagerView@@SIPAU1@HPBD@Z',
                     target=0x421670, count=2),
                dict(symbol='?DispatchReplayStage@@YIXPAUReplayStageDispatchView@@H@Z',
                     target=0x4217E0, count=2)],
        'native_opponent_selection_calls': dict(
                symbol='?SelectOpponentConfiguration@GameManagerOpponentSelectionView@@QAEXXZ',
                target=0x415910, count=4),
        'native_creator_calls': [
            dict(unit=unit, symbol=symbol, target=target, count=count)
            for unit, symbol, target, count in (
                ('background-create', '?Create@Background@@SIPAU1@H@Z', 0x403AE0, 1),
                ('player-register-chain', '?PlayerRegisterChain@@YIPAUPlayerLifecycleView@@HHH@Z', 0x41F170, 1),
                ('etama-controller-register', '?Register@EtamaController@@SIPAU1@H@Z', 0x4150D0, 1),
                ('effect-manager-create', '?Create@EffectManager@@SIPAU1@HHH@Z', 0x40D320, 2),
                ('enemy-manager-create', '?Create@EnemyManagerView@@SIPAU1@H@Z', 0x412240, 1),
                ('card-attack-create', '?Create@CardAttack@@SIPAU1@H@Z', 0x404600, 1),
                ('front-side-create', '?Create@FrontSide@@SIPAU1@H@Z', 0x41A5D0, 1),
                ('exattack-register', '?Register@ExAttackController@@SIPAU1@XZ', 0x4157D0, 1))],
        'original_blocks': 78, 'candidate_blocks': 78,
        'direct_graphs_agree': False,
        'aligned_field_identities': 171, 'field_change_spans': 2,
        # The real full-owner decoded 172/171-field *referent multisets*
        # are part of the source-cold guard, not optional diagnostic data.
        'original_only_field_referents': [
            dict(type=t, address=address, count=count)
            for t, address, count in front.GAMEPLAY_SETUP_TARGET_ONLY_REFERENTS],
        'source_only_field_referents': [
            dict(type=t, address=address, count=count)
            for t, address, count in front.GAMEPLAY_SETUP_SOURCE_ONLY_REFERENTS],
        'aligned_instructions': 412,
        'overlap_differences': len(differences),
        'difference_offsets': differences, 'complete_bytes_equal': False,
        'missing_target_bytes': 7, 'extra_candidate_bytes': 0,
    }


class GameplaySetupOriginalBoundFrontier(TestCase):
    def test_complete_candidate_accounts_for_694_differences_and_seven_missing_bytes(self):
        report = baseline()
        self.assertEqual(len(front.GAMEPLAY_SETUP_RESIDUAL_MASK), 212)
        self.assertEqual(report['overlap_differences'], 694)
        self.assertEqual(report['missing_target_bytes'], 7)
        self.assertNotIn(0, report['difference_offsets'])
        self.assertTrue(all(off < 1682 for off in report['difference_offsets']))
        front.guard_gameplay_setup_thread(report)

    def test_genuinely_narrower_full_extent_still_receives_no_local_exact_credit(self):
        report = baseline()
        narrower = {**report,
                    'difference_offsets': report['difference_offsets'][:-10],
                    'overlap_differences': report['overlap_differences'] - 10}
        front.guard_gameplay_setup_thread(narrower)
        self.assertEqual(narrower['exactness_credit'], 'none')
        apparent_zero = {**report, 'difference_offsets': [],
                         'overlap_differences': 0, 'complete_bytes_equal': False}
        front.guard_gameplay_setup_thread(apparent_zero)
        self.assertEqual(apparent_zero['exactness_credit'], 'none')

    def test_reject_any_newly_different_byte_anywhere_in_original_owner(self):
        report = baseline()
        allowed = report['difference_offsets']
        outside = [i for i in range(1689) if i not in set(allowed)]
        for off in (0, 1, 0x100, outside[-1]):
            with self.subTest(offset=hex(off)), self.assertRaises(RuntimeError):
                candidate = sorted(allowed[:-1] + [off])
                front.guard_gameplay_setup_thread({**report,
                    'difference_offsets': candidate})

    def test_reject_old_aliases_or_incomplete_native_replay_calls(self):
        report = baseline()
        calls = report['native_replay_calls']
        invalid = (None, [], calls[::-1],
                   [{**calls[0], 'symbol': '?CreateStageObject@@YIPAXHPAX@Z'}, calls[1]],
                   [calls[0], {**calls[1], 'target': 0x420C10}],
                   [{**calls[0], 'count': 1}, calls[1]])
        for value in invalid:
            with self.subTest(value=value), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, 'native_replay_calls': value})

    def test_native_creator_contracts_are_required_even_with_identical_bytes(self):
        report = baseline()
        creators = report['native_creator_calls']
        invalid = [None, [], creators[::-1], creators[:-1]]
        for key, value in [('symbol', '?RegisterSubsystem1@@YIPAXHHH@Z'),
                           ('target', 0x420C10), ('count', 2), ('count', True), ('unit', 'invented')]:
            invalid.append([{**creators[0], key: value}, *creators[1:]])
        for value in invalid:
            with self.subTest(value=value), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, 'native_creator_calls': value})

    def test_reject_malformed_byte_inventory_and_forged_acceptance(self):
        report = baseline()
        differences = report['difference_offsets']
        invalid = (
            {'difference_offsets': None},
            {'difference_offsets': differences[:-1]},
            {'difference_offsets': list(reversed(differences))},
            {'difference_offsets': [differences[0]] * len(differences)},
            {'difference_offsets': ['0'] + differences[1:]},
            {'difference_offsets': differences[:-1] + [1689]},
            {'difference_offsets': differences[:-1] + [-1]},
            {'difference_offsets': differences[:-1] + [True]},
            {'overlap_differences': 695},
            {'overlap_differences': '694'},
            {'complete_bytes_equal': True},
            {'missing_target_bytes': 0}, {'missing_target_bytes': 6},
            {'missing_target_bytes': True}, {'extra_candidate_bytes': 1},
            {'exactness_credit': 'accepted'},
            {'diagnostic_only': False},
            {'original_only_field_referents': None},
            {'source_only_field_referents': None},
            {'original_only_field_referents': [dict(type='DIR32',
                address=0x4A7D90, count=4),
                *report['original_only_field_referents'][1:]]},
            {'source_only_field_referents': [
                dict(type='DIR32',address=0x4A7DA0,count=1),
                *report['source_only_field_referents'][1:]]},
        )
        for change in invalid:
            with self.subTest(change=str(change)[:60]), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, **change})

    def test_reject_missing_or_surrogate_opponent_selection_contract(self):
        report = baseline()
        call = report['native_opponent_selection_calls']
        invalid = (None, {},
                   {**call, 'symbol': '?UpdateProgress@GameManagerSetupLayout@@QAEXXZ'},
                   {**call, 'target': 0x4159A0},
                   {**call, 'count': 3}, {**call, 'count': 5})
        for value in invalid:
            with self.subTest(value=value), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report,
                    'native_opponent_selection_calls': value})

    def test_reject_original_target_and_field_call_graph_regressions(self):
        report = baseline()
        invalid = (
            {'target_sha256': '0' * 64},
            {'target_bytes': 1688}, {'candidate_bytes': 1690},
            {'target_instructions': 412}, {'candidate_instructions': 412},
            {'original_fields': 171}, {'bound_fields': 170},
            {'ordered_calls': 34}, {'candidate_calls': 34},
            {'ordered_calls_agree': False},
            {'original_blocks': 77}, {'candidate_blocks': 77},
            {'direct_graphs_agree': True},
            {'aligned_field_identities': 170},
            {'field_change_spans': 3},
            {'aligned_instructions': 411},
        )
        for change in invalid:
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                front.guard_gameplay_setup_thread({**report, **change})
