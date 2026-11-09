"""Complete charge owner and its real visible Timer sibling fail closed."""
import importlib.util
import io
import json
from pathlib import Path
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]


def load(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / file)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


frontier = load('charge_frontier', 'check-boss-frontiers.py')
charge = load('charge_inspector', 'inspect-player-charge-update.py')


class ChargeFrontierTests(unittest.TestCase):
    def evidence(self):
        offsets = sorted(frontier.PLAYER_CHARGE_RESIDUAL_OFFSETS)
        return dict(diagnostic_only=True, exactness_credit='none', target_bytes=1210,
                    candidate_bytes=1210, target_sha256=frontier.PLAYER_CHARGE_TARGET_SHA,
                    target_instructions=350, candidate_instructions=350,
                    original_fields=70, bound_fields=70, ordered_calls=58,
                    ordered_calls_agree=True, target_blocks=53, candidate_blocks=53,
                    direct_graphs_agree=True, literal_definitions_checked=5,
                    same_carrier_timer_bytes_exact=20,
                    original_stack_byte_minus_one=frontier.PLAYER_CHARGE_ORIGINAL_MODE_BYTE,
                    candidate_stack_byte_minus_one=frontier.PLAYER_CHARGE_CANDIDATE_MODE_BYTE,
                    overlap_differences=len(offsets), difference_offsets=offsets,
                    missing_target_bytes=0, extra_candidate_bytes=0,
                    complete_bytes_equal=False)

    def test_full_extent_frontier_and_actual_reductions_remain_noncrediting(self):
        report = self.evidence()
        self.assertEqual(len(frontier.PLAYER_CHARGE_RESIDUAL_OFFSETS), 235)
        self.assertEqual(1210 - len(frontier.PLAYER_CHARGE_RESIDUAL_OFFSETS), 975)
        self.assertIs(frontier.guard_player_charge(report), report)
        report['difference_offsets'].pop()
        report['overlap_differences'] -= 1
        self.assertIs(frontier.guard_player_charge(report), report)
        self.assertEqual(report['exactness_credit'], 'none')

    def test_wrong_target_incomplete_census_or_forged_exactness_fails(self):
        for change in ({'target_sha256': '0' * 64}, {'target_bytes': 1197},
                       {'candidate_bytes': 1209}, {'candidate_bytes': 1211},
                       {'candidate_instructions': 349}, {'candidate_instructions': 351},
                       {'candidate_blocks': 52}, {'direct_graphs_agree': False},
                       {'bound_fields': 69}, {'original_fields': 69},
                       {'ordered_calls': 57}, {'ordered_calls_agree': False},
                       {'literal_definitions_checked': 4},
                       {'same_carrier_timer_bytes_exact': 19},
                       {'original_stack_byte_minus_one': []},
                       {'candidate_stack_byte_minus_one': None},
                       {'candidate_stack_byte_minus_one': []},
                       {'missing_target_bytes': 1}, {'extra_candidate_bytes': 1},
                       {'exactness_credit': 'accepted'}, {'complete_bytes_equal': True},
                       {'difference_offsets': [True]}, {'difference_offsets': [{}]}):
            with self.subTest(change=change), self.assertRaises(RuntimeError):
                frontier.guard_player_charge({**self.evidence(), **change})

    def test_same_count_exchange_cannot_damage_any_previously_equal_byte(self):
        matched = set(range(1210)) - frontier.PLAYER_CHARGE_RESIDUAL_OFFSETS
        for offset in matched:
            report = self.evidence()
            report['difference_offsets'][-1] = offset
            report['difference_offsets'].sort()
            with self.subTest(offset=offset), self.assertRaises(RuntimeError):
                frontier.guard_player_charge(report)

    def test_real_timer_body_extent_and_relocations_are_checked(self):
        original = b'\x90' * 19 + b'\xc3'  # synthetic private-free comparison fixture
        fake = SimpleNamespace(object_function=lambda *a: (original, []),
                               pe_bytes_at=lambda *a: original)
        self.assertEqual(charge.verify_timer_sibling(fake, Path('probe.obj'), b'image'), 20)
        for body, fields in ((original[:-1], []), (original + b'\x90', []),
                             (b'\x00' + original[1:], []), (original, [{}])):
            fake.object_function = lambda *a, body=body, fields=fields: (body, fields)
            with self.subTest(body=body, fields=fields), self.assertRaises(ValueError):
                charge.verify_timer_sibling(fake, Path('probe.obj'), b'image')

    def test_focused_entry_builds_one_fixed_profile_and_verifies_target_first(self):
        report = self.evidence()
        with patch.object(frontier, 'build_nonexact') as build, patch.object(
                frontier, 'parsed', return_value=report) as inspect:
            self.assertEqual(frontier.run_player_charge(), {'player_charge_update': report})
        build.assert_called_once_with('src/PlayerChargeAttackUpdate.cpp',
                                      'build/matching/PlayerChargeAttackUpdate.obj',
                                      reference_unit='player-on-update')
        inspect.assert_called_once_with(['scripts/inspect-player-charge-update.py',
                                        'build/matching/PlayerChargeAttackUpdate.obj'])
        order = []
        output = io.StringIO()
        with patch('sys.argv', ['check-boss-frontiers.py', '--owner', 'charge']), patch.object(
                frontier, 'execute', side_effect=lambda args: order.append(args)), patch.object(
                frontier, 'run_player_charge', side_effect=lambda: order.append('charge') or {
                    'player_charge_update': report}), patch.object(
                frontier, 'run_ecl') as ecl, patch.object(
                frontier, 'run_player_collision') as collision, patch('sys.stdout', output):
            self.assertEqual(frontier.main(), 0)
        self.assertEqual(order, [['scripts/verify-target.py'], 'charge'])
        self.assertEqual(list(json.loads(output.getvalue())['owners']), ['player_charge_update'])
        ecl.assert_not_called()
        collision.assert_not_called()


if __name__ == '__main__':
    unittest.main()
