"""Manifest-precise source scopes without accessing the private TH09 target."""
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('scoped_exact_replay', ROOT/'scripts/replay-scoped-exact.py')
scoped = importlib.util.module_from_spec(spec)
spec.loader.exec_module(scoped)


class ScopedReplayTests(unittest.TestCase):
    def setUp(self):
        self.units = {
            'one': dict(source='src/Owner.cpp', object='build/matching/Owner.obj',
                        profile=['/O2', '/Ob1'], functions=['One'], target_address=0x400000, size=30),
            'two': dict(source='src/Owner.cpp', object='build/matching/Owner.obj',
                        profile=['/O2', '/Ob1'], functions=['Two'], target_address=0x400030, size=20),
            'alt': dict(source='src/Owner.cpp', object='build/matching/Owner-Ob0.obj',
                        profile=['/O2', '/Ob0'], functions=['Alt'], target_address=0x400060, size=8),
            'other': dict(source='src/Other.cpp', object='build/matching/Other.obj',
                          profile=['/O2'], functions=['Other'], target_address=0x401000, size=10),
        }
        self.matched = {key:value['source'] for key,value in self.units.items()}

    def test_exact_unit_expands_all_same_source_profiles_only(self):
        plan = scoped.plan(self.units, self.matched, [], ['two'])
        self.assertEqual(len(plan), 2)
        self.assertEqual(sum(len(x['units']) for x in plan), 3)
        self.assertEqual({x['object']:x['units'] for x in plan}, {
            'build/matching/Owner.obj': ['one','two'],
            'build/matching/Owner-Ob0.obj': ['alt']})
        self.assertEqual(scoped.plan(self.units,self.matched,['src/Owner.cpp'],[]),plan)

    def test_ambiguous_or_missing_requested_scope_is_rejected(self):
        for sources,names in [([],[]),([],['not-an-exact-unit']),(['src/None.cpp'],[])]:
            with self.subTest(sources=sources,names=names),self.assertRaises(ValueError):
                scoped.plan(self.units,self.matched,sources,names)
        broken = dict(self.units)
        broken['alt'] = dict(broken['alt'], object='build/matching/Owner.obj')
        with self.assertRaisesRegex(ValueError,'conflicting source/compiler profiles'):
            scoped.plan(broken,self.matched,[],['one'])

    def test_load_requires_every_canonical_ledger_row_to_bind_actual_unit(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            cfg = root/'config'
            cfg.mkdir()
            payload = '''schema_version = 1
[units.good]
source="src/Foo.cpp"
object="build/matching/Foo.obj"
profile=["/O2"]
functions=["Foo"]
symbol="?Foo@@YAXXZ"
target_address=0x00401000
size=20
'''
            (cfg/'match-units.toml').write_text(payload)
            head='address,name,size,status,match_percent,unit,evidence\n'
            row='0x00401000,Foo,20,matching,100.00,good,replayed\n'
            (cfg/'matches.csv').write_text(head+row)
            units, matched = scoped.load_carriers(root)
            self.assertEqual(set(units), {'good'})
            self.assertEqual(matched,{'good':'src/Foo.cpp'})
            for bad in (head+row+row,head+row.replace('20,matching','21,matching'),
                        head+row.replace('good,replayed','missing,replayed'),
                        head+row.replace('100.00','99.00')):
                (cfg/'matches.csv').write_text(bad)
                with self.subTest(bad=bad[-45:]),self.assertRaises(ValueError):
                    scoped.load_carriers(root)

    def test_changed_header_follows_reverse_local_includes_transitively(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            src=root/'src'
            src.mkdir()
            (src/'Owner.cpp').write_text('#include "Inner.hpp"\n')
            (src/'Nested.cpp').write_text('#include "Shared.inl"\n')
            (src/'Inner.hpp').write_text('#include "Shared.inl"\n')
            (src/'Shared.inl').write_text('#include "Inner.hpp"\n')  # cycle
            (src/'Independent.cpp').write_text('#include "Independent.hpp"\n')
            (src/'Independent.hpp').write_text('int value;\n')
            expected={'src/Owner.cpp','src/Nested.cpp'}
            self.assertEqual(scoped.resolve_changed_sources(['src/Shared.inl'],root=root),expected)
            self.assertEqual(scoped.resolve_changed_sources(['src/Inner.hpp'],root=root),expected)
            self.assertEqual(scoped.resolve_changed_sources(['src/Owner.cpp'],root=root),{'src/Owner.cpp'})
            for bad in ('../src/Shared.inl','/tmp/Shared.inl','src/none.inl',
                        'src/Independent.svg'):
                with self.subTest(bad=bad),self.assertRaisesRegex(ValueError,'absent or unsafe'):
                    scoped.resolve_changed_sources([bad],root=root)

    def test_changed_header_with_no_real_cpp_consumer_is_not_assumed_safe(self):
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            src=root/'src'
            src.mkdir()
            (src/'Unused.inl').write_text('int unused;\n')
            (src/'Visible.cpp').write_text('int visible;\n')
            with self.assertRaisesRegex(ValueError,'no lexical .cpp consumers'):
                scoped.resolve_changed_sources(['src/Unused.inl'],root=root)

    def test_default_budget_prevents_accidental_broad_cold_replay(self):
        scoped.enforce_carrier_budget([{}]*5,8)
        with self.assertRaisesRegex(ValueError,'refusing 9 carrier jobs'):
            scoped.enforce_carrier_budget([{}]*9,8)
        with self.assertRaises(ValueError):
            scoped.enforce_carrier_budget([{}],0)
        with self.assertRaises(ValueError):
            scoped.enforce_carrier_budget([{}],65)

    def test_one_compile_per_real_carrier_all_exact_references_checked(self):
        groups=scoped.plan(self.units,self.matched,[],['one'])
        history=[]
        def invoke(argv, *, cwd):
            history.append(argv)
            if argv[-1]=='--json':
                return json.dumps(dict(result='exact',size=20,matched_bytes=20,
                                       compared_size=20,matched_compared_bytes=20,
                                       relocations=[{'symbol':'Real'}]))
            return 'OK'
        reports=scoped.run(groups,invoke=invoke)
        self.assertEqual(len(reports),2)
        self.assertEqual(sum(len(x['passed']) for x in reports),3)
        self.assertEqual(sum('--unit' in argv and '--json' not in argv for argv in history),2)
        self.assertEqual(sum(argv[-1]=='--json' for argv in history),3)
        self.assertFalse(any('--owner' in argv or '--all' in argv for argv in history))
        self.assertEqual(sum(any(str(item).endswith('/verify-target.py') for item in argv) for argv in history),1)

    def test_wrong_comparison_never_grants_exact_match(self):
        groups=scoped.plan(self.units,self.matched,[],['one'])
        for bad in (dict(result='near',size=20,compared_size=20,matched_bytes=20,
                         matched_compared_bytes=20,relocations=[]),
                    dict(result='exact',size=20,compared_size=20,matched_bytes=20,
                         matched_compared_bytes=19,relocations=[]),
                    dict(result='exact',size=20,compared_size=19,matched_bytes=20,
                         matched_compared_bytes=19,relocations=[])):
            with self.subTest(bad=bad):
                def invoke(argv, *, cwd):
                    return json.dumps(bad) if argv[-1]=='--json' else 'OK'
                with self.assertRaisesRegex(ValueError,'exact regression'):
                    scoped.run(groups,invoke=invoke)


if __name__=='__main__':
    unittest.main()
