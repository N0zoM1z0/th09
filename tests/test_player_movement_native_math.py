"""Original call-edge identities, independent of source spelling/private input."""
from copy import deepcopy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('movement_math', ROOT/'scripts/check-boss-frontiers.py')
FRONT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(FRONT)


def report():
    # Synthetic relocation observations, never an exactness receipt.
    calls = [(0x59E, '??GFloat3@@QBE?AU0@ABU0@@Z', 0x401140),
             (0x5C0, '??HFloat3@@QBE?AU0@ABU0@@Z', 0x401100),
             (0x5E8, '??GFloat3@@QBE?AU0@ABU0@@Z', 0x401140),
             (0x60A, '??HFloat3@@QBE?AU0@ABU0@@Z', 0x401100),
             (0x632, '??GFloat3@@QBE?AU0@ABU0@@Z', 0x401140),
             (0x654, '??HFloat3@@QBE?AU0@ABU0@@Z', 0x401100)]
    target = [dict(offset=o, type='REL32', effective_destination=d) for o, _, d in calls]
    actual = [dict(offset=o, type='REL32', symbol=s, addend=0,
                   destination=d, effective_destination=d, owner_local=False)
              for o, s, d in calls]
    return dict(target_fields=target, actual_resolved_fields=actual)


class NativeMovementMath(unittest.TestCase):
    def test_complete_six_native_edges(self):
        FRONT.guard_player_movement_math(report())

    def test_rejects_private_method_with_unchanged_destination(self):
        for i in range(6):
            r = report()
            r['actual_resolved_fields'][i]['symbol'] = r['actual_resolved_fields'][i]['symbol'].replace('Float3', 'PlayerPositionView')
            with self.subTest(i=i), self.assertRaises(RuntimeError):
                FRONT.guard_player_movement_math(r)

    def test_rejects_changed_actual_field(self):
        for i in range(6):
            for key, value in [('offset', 0), ('type', 'DIR32'), ('addend', 1),
                               ('destination', 0x401141), ('effective_destination', 0x401141),
                               ('owner_local', True)]:
                r = report()
                r['actual_resolved_fields'][i][key] = value
                with self.subTest(i=i, key=key), self.assertRaises(RuntimeError):
                    FRONT.guard_player_movement_math(r)

    def test_rejects_changed_original_edge(self):
        for i in range(6):
            for key, value in [('offset', 0), ('type', 'DIR32'), ('effective_destination', 0x401141)]:
                r = report()
                r['target_fields'][i][key] = value
                with self.subTest(i=i, key=key), self.assertRaises(RuntimeError):
                    FRONT.guard_player_movement_math(r)

    def test_rejects_missing_duplicate_or_absent_inventory(self):
        for key in ['target_fields', 'actual_resolved_fields']:
            for mode in ['missing', 'duplicate', 'absent']:
                r = report()
                if mode == 'missing': r[key].pop()
                elif mode == 'duplicate': r[key].append(deepcopy(r[key][0]))
                else: del r[key]
                with self.subTest(key=key, mode=mode), self.assertRaises(RuntimeError):
                    FRONT.guard_player_movement_math(r)

    def test_rejects_unexpected_additional_math_reference(self):
        for key in ['target_fields', 'actual_resolved_fields']:
            r = report()
            extra = deepcopy(r[key][0]); extra['offset'] = 0x700
            r[key].append(extra)
            with self.subTest(key=key), self.assertRaises(RuntimeError):
                FRONT.guard_player_movement_math(r)


if __name__ == '__main__':
    unittest.main()
