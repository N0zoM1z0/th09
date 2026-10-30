"""A supplied Pause diagnostic cannot waive complete accepted byte equality."""
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('pause_boss_guard', ROOT/'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(frontier)

class PauseMenuFrontierTests(unittest.TestCase):
    @staticmethod
    def baseline():
        calls = [name for name, count in frontier.PAUSE_CALL_CENSUS.items() for _ in range(count)]
        timestamps = [dict(offset=x, iat=0x48E238, symbol='__imp__timeGetTime@0') for x in (0x3C1, 0x6B2)]
        target = dict(code_bytes=1734, alignment_hex='8bff', table_offset=1736,
                      instructions=449, fields=[(0,'DIR32')]*60+[(0,'REL32')]*46,
                      table_destinations=list(frontier.PAUSE_TARGET_TABLE),
                      ordered_direct_calls=calls, internal_branches=[None]*52,
                      imported_calls=timestamps)
        source = {**target, 'fields': [(0,'DIR32')]*106}
        return dict(diagnostic_only=True, exactness_credit='none',
                    target_owner_sha256=frontier.PAUSE_SHA256, physical_bytes=1776,
                    overlap_differences=0, missing_bytes=0, excess_bytes=0,
                    complete_equal=True, differences=[], fields=[{}]*106,
                    target=target, candidate=source)

    def test_complete_diagnostic_remains_noncrediting(self):
        result = self.baseline()
        checked = frontier.guard_pause_menu(result)
        self.assertEqual((checked['target_bytes'], checked['candidate_bytes']), (1776,1776))
        self.assertEqual((checked['overlap_differences'],checked['missing_target_bytes']), (0,0))
        self.assertEqual((checked['bound_fields'],checked['calls'],checked['switch_entries']), (106,46,10))
        self.assertEqual(result['exactness_credit'], 'none')

    def test_canonical_owner_and_separate_native_object_are_built(self):
        with (patch.object(frontier,'build') as build,
              patch.object(frontier,'build_nonexact') as nonexact,
              patch.object(frontier,'parsed',side_effect=[self.baseline(), {
                  'diagnostic_only':True,'exactness_credit':'none',
                  'native_resume':{'complete_equal':True,'bytes':15,'fields':2},
                  'resume_literal_bytes':8}]) as inspect,
              patch.object(frontier,'canonical',side_effect=lambda n: {'unit':n,'result':'exact'})):
            owners, exact = frontier.run_pause(protect_exact=True)
        nonexact.assert_not_called()
        self.assertEqual([x.args[0] for x in build.call_args_list],
                         ['pause-menu-update','anm-vm-is-visible']+
                         [r[0] for r in frontier.PAUSE_REAL_SOUND_CARRIERS])
        self.assertEqual([x['unit'] for x in exact], ['pause-menu-update']+
                         list(frontier.PAUSE_EXACT_SIBLINGS)+
                         [r[0] for r in frontier.PAUSE_REAL_SOUND_CARRIERS])
        self.assertEqual(inspect.call_args_list[0].args[0],
                         ['scripts/inspect-pause-menu.py','--json','build/matching/AsciiManagerMenu.obj'])
        self.assertEqual(owners['pause_menu_update']['overlap_differences'],0)

    def test_former_nonexact_frontier_is_rejected(self):
        e=self.baseline()
        old={**e,'physical_bytes':1732,'missing_bytes':44,'complete_equal':False,
             'overlap_differences':1453,'fields':[{}]*107,
             'differences':[dict(offset=i,candidate=1,target=2) for i in range(1453)],
             'candidate':{**e['candidate'],'code_bytes':1692,'alignment_hex':'',
                          'table_offset':1692,'instructions':454,'fields':[(0,'DIR32')]*107}}
        with self.assertRaises(RuntimeError): frontier.guard_pause_menu(old)

    def test_wrong_target_extent_field_order_timestamp_and_changed_byte_rejected(self):
        e=self.baseline()
        changes=[{'diagnostic_only':False},{'exactness_credit':'accepted'},
                 {'target_owner_sha256':'0'*64},{'physical_bytes':1775},
                 {'physical_bytes':1777},{'missing_bytes':1},{'excess_bytes':1},
                 {'complete_equal':False},{'fields':[{}]*105},
                 {'overlap_differences':1,'complete_equal':False,
                  'differences':[dict(offset=0x100,candidate=1,target=2)]},
                 {'differences':[dict(offset=0x100,candidate=1,target=2)]}]
        for side in ('target','candidate'):
            for key,value in [('code_bytes',1733),('alignment_hex','9090'),
                              ('table_offset',1735),('instructions',448),
                              ('fields',[]),('table_destinations',[]),
                              ('ordered_direct_calls',[]),('internal_branches',[]),
                              ('imported_calls',[])]:
                changes.append({side:{**e[side],key:value}})
        changes.append({'candidate':{**e['candidate'],'ordered_direct_calls':list(reversed(e['target']['ordered_direct_calls']))}})
        changes.append({'candidate':{**e['candidate'],'imported_calls':[
            dict(offset=0x3C1,iat=0x48E23C,symbol='__imp__timeGetTime@0'),e['candidate']['imported_calls'][1]]}})
        for change in changes:
            with self.subTest(change=str(change)[:100]), self.assertRaises(RuntimeError):
                frontier.guard_pause_menu({**e,**change})

if __name__=='__main__': unittest.main()
