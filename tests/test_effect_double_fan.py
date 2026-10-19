"""Guard the complete original Effect26 update while its eleven bytes stay open."""
import copy
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('double_fan_frontiers', ROOT/'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


def evidence():
    offsets = [0x38,0x39,0x3A,0x3B,0x3C,0x3E,0x56,0x59,0x63,0xB2,0xBB]
    calls = [['direct',0x42AED0],['direct',0x42AED0]]
    return dict(diagnostic_only=True, exactness_credit='none', target_bytes=209,
        candidate_bytes=209, fields=10, target_instructions=64,
        candidate_instructions=64, target_blocks=8, candidate_blocks=8,
        missing_bytes=0, extra_bytes=0, all_original_fields_agree=True,
        field_referent_order_agrees=True, ordered_calls_agree=True,
        direct_graphs_agree=True, target_calls=calls, candidate_calls=calls,
        target_sha256='e22152173d39a96b45457ee3d79faec4d4e53a8afa309dfad53cc3509419f7a4',
        difference_offsets=offsets, overlapping_differences=11,
        bindings=[dict(offset=0x34,type='DIR32',
            symbol='?g_GameManager@@3UAsciiGameManagerView@@A',
            target=0x4A7D90,addend=4,effective_target=0x4A7D94)])


class Effect26UpdateGuard(unittest.TestCase):
    def test_baseline_and_true_improvements_pass_without_exact_credit(self):
        for count in (11,6,0):
            r=evidence();r['difference_offsets']=r['difference_offsets'][:count]
            r['overlapping_differences']=count
            self.assertEqual(frontier.guard_effect_type26_update(r)['ordinary_differences'],count)

    def test_all_198_equal_positions_reject_same_count_exchanges(self):
        baseline=evidence()
        for offset in set(range(209))-set(baseline['difference_offsets']):
            r=copy.deepcopy(baseline);r['difference_offsets'][0]=offset
            with self.subTest(offset=offset),self.assertRaises(RuntimeError):
                frontier.guard_effect_type26_update(r)

    def test_extent_original_fields_calls_and_native_global_cannot_drift(self):
        corruptions=[('candidate_bytes',208),('target_bytes',208),('fields',9),
            ('target_sha256','0'*64),('diagnostic_only',False),
            ('exactness_credit','accepted'),('target_instructions',63),
            ('candidate_instructions',63),('candidate_blocks',9),
            ('all_original_fields_agree',False),('ordered_calls_agree',False),
            ('direct_graphs_agree',False),('candidate_calls',[['direct',0x42AED0]]),
            ('difference_offsets',[0x38]*11),('overlapping_differences',10)]
        for key,value in corruptions:
            with self.subTest(key=key),self.assertRaises(RuntimeError):
                frontier.guard_effect_type26_update({**evidence(),key:value})
        for key,value in [('offset',0x35),('type','REL32'),('target',0x4A7D94),
                ('addend',0),('effective_target',0x4A7D90),
                ('symbol','?g_GameManager@@3UGameManagerModeView@@A')]:
            r=evidence();r['bindings'][0][key]=value
            with self.subTest(field=key),self.assertRaises(RuntimeError):
                frontier.guard_effect_type26_update(r)
        for bindings in ([],evidence()['bindings']*2):
            with self.subTest(bindings=bindings),self.assertRaises(RuntimeError):
                frontier.guard_effect_type26_update({**evidence(),'bindings':bindings})
