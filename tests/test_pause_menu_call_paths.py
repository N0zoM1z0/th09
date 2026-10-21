"""Bounded call sharing must include reachable IAT sites and preserve loops."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'pause_call_paths', ROOT/'scripts/inspect-pause-menu.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class PauseCallPaths(unittest.TestCase):
    def test_wrong_import_slot_register_index_or_operand_width_is_rejected(self):
        args = [7, 'memory', 0, 0, 4, 0x48e238]
        self.assertEqual(audit.timestamp_call(*args),
                         dict(offset=7, iat=0x48e238, symbol='__imp__timeGetTime@0'))
        for index, value in [(0,True),(0,-1),(1,'register'),(2,1),(3,1),
                             (4,2),(5,0x48e23c)]:
            changed = list(args)
            changed[index] = value
            with self.subTest(index=index, value=value), self.assertRaises(ValueError):
                audit.timestamp_call(*changed)

    def test_distinct_cases_share_one_footer_and_keep_separate_early_return(self):
        # Abstract CFG fixture, containing no private executable bytes.
        graph = {0:[10], 1:[10], 2:[10,20], 10:[11], 11:[], 20:[]}
        self.assertEqual(audit.reachable_calls(graph, [0,1,2], [10,20]),
                         [[10],[10],[10,20]])

    def test_candidate_duplicate_footer_is_visible_despite_same_call_identity(self):
        graph = {0:[10], 1:[10], 2:[12,20], 10:[], 12:[], 20:[]}
        self.assertEqual(audit.reachable_calls(graph, [0,1,2], [10,12,20]),
                         [[10],[10],[12,20]])

    def test_loop_does_not_drop_call_or_infer_a_dynamic_execution_count(self):
        graph = {0:[1], 1:[0,2], 2:[], 9:[]}
        self.assertEqual(audit.reachable_calls(graph, [0,9], [1,2]), [[1,2],[]])

    def test_bad_entry_edge_call_and_boolean_boundary_fail_closed(self):
        for graph, entries, calls in (
                ({0:[]}, [1], []), ({0:[1]}, [0], []),
                ({0:[]}, [0], [1]), ({False:[]}, [0], []),
                ({0:[False]}, [0], []), ({0:[]}, [False], []),
                ({0:[]}, [0], [False])):
            with self.subTest(graph=graph, entries=entries, calls=calls), \
                    self.assertRaises(ValueError):
                audit.reachable_calls(graph, entries, calls)
