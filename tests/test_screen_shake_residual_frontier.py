"""Complete 336-byte original residual protection for TH09 CalcShakeEnvelope.

No private PE is required for these negative tests. The actual cold
VC7.1 PE + COFF check is separately run by check-boss-frontiers.py.
"""
import importlib.util
from pathlib import Path
from unittest import TestCase

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('screen_envelope_residual',
    ROOT/'scripts/check-boss-frontiers.py')
front=importlib.util.module_from_spec(spec);spec.loader.exec_module(front)

def current():
    return {
        'diagnostic_only':True,
        'exactness_credit':'none; original-linked full-owner diagnostic only',
        'target_bytes':336,
        'candidate_bytes':336,
        'target_sha256':front.SCREEN_SHAKE_ORIGINAL_SHA256,
        'target_instructions':132,
        'candidate_instructions':132,
        'reviewed_target_fields':20,
        'bound_relocations':20,
        'target_calls':8,
        'candidate_calls':8,
        'ordered_calls_agree':True,
        'target_blocks':26,
        'candidate_blocks':26,
        'complete_direct_graphs_agree':True,
        'absent_target_bytes':0,
        'extra_candidate_bytes':0,
        'complete_bytes_agree':False,
        'overlapping_differences':26,
        'difference_offsets':list(front.SCREEN_SHAKE_RESIDUAL_OFFSETS),
    }

class OriginalSHAAndWholeResidualGuard(TestCase):
    def test_exactly_twenty_six_current_residual_offsets(self):
        report=current()
        self.assertEqual(len(front.SCREEN_SHAKE_RESIDUAL_OFFSETS),26)
        self.assertNotIn(0x5c,report['difference_offsets'])
        self.assertNotIn(0x5f,report['difference_offsets'])
        self.assertIn(0x5b,report['difference_offsets'])
        self.assertIn(0x5e,report['difference_offsets'])
        front.guard_screen_shake_frontier(report)
        self.assertTrue(report['exactness_credit'].startswith('none'))
        source=(ROOT/'src/ScreenEffect.cpp').read_text()
        before=source.index('const volatile int &fade = screenEffect->rawParameter2;')
        after=source.index('const volatile int &hold = screenEffect->rawParameter1;')
        self.assertLess(before,after)
        self.assertIn('int update = hold + fade;',source)

    def test_narrower_valid_source_never_earns_local_exact_credit(self):
        report=current()
        smaller={**report,
                 'difference_offsets':report['difference_offsets'][:-3],
                 'overlapping_differences':23}
        front.guard_screen_shake_frontier(smaller)
        self.assertTrue(smaller['exactness_credit'].startswith('none'))
        apparent_zero={**report,'difference_offsets':[],
                       'overlapping_differences':0,'complete_bytes_agree':True}
        front.guard_screen_shake_frontier(apparent_zero)
        self.assertTrue(apparent_zero['exactness_credit'].startswith('none'))

    def test_any_newly_broken_original_byte_fails(self):
        report=current();current_set=set(report['difference_offsets'])
        for offset in (0,0x5c,0x5f,0x14c,335):
            self.assertNotIn(offset,current_set)
            new=sorted((current_set-{max(current_set)})|{offset})
            with self.subTest(offset=offset),self.assertRaises(RuntimeError):
                front.guard_screen_shake_frontier({
                    **report,'difference_offsets':new,
                })

    def test_reject_fake_or_unordered_residual_inventory(self):
        report=current();diff=report['difference_offsets']
        tests=(
            {'difference_offsets':diff[:-1]},
            {'difference_offsets':list(reversed(diff))},
            {'difference_offsets':diff[:13]+[diff[0]]+diff[14:]},
            {'difference_offsets':[str(x) for x in diff]},
            {'difference_offsets':diff[:-1]+[True]},
            {'difference_offsets':diff[:-1]+[-1]},
            {'difference_offsets':diff[:-1]+[336]},
            {'difference_offsets':None},
            {'overlapping_differences':'26'},
            {'overlapping_differences':27},
            {'complete_bytes_agree':True},
            {'exactness_credit':'accepted'},
            {'diagnostic_only':False},
        )
        for change in tests:
            with self.subTest(change=str(change)[:90]),self.assertRaises(RuntimeError):
                front.guard_screen_shake_frontier({**report,**change})

    def test_reject_changed_original_field_call_graph_identity(self):
        report=current()
        invalid=(
            {'target_sha256':'0'*64}, {'target_bytes':335},
            {'candidate_bytes':335}, {'target_instructions':131},
            {'candidate_instructions':133}, {'reviewed_target_fields':19},
            {'bound_relocations':19}, {'target_calls':7},
            {'candidate_calls':7}, {'ordered_calls_agree':False},
            {'target_blocks':25}, {'candidate_blocks':25},
            {'complete_direct_graphs_agree':False},
            {'absent_target_bytes':1}, {'extra_candidate_bytes':1}
        )
        for change in invalid:
            with self.subTest(change=change),self.assertRaises(RuntimeError):
                front.guard_screen_shake_frontier({**report,**change})
