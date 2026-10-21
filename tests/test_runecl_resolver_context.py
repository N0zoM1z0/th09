"""Full native tables and actual definitions cannot be replaced by a near caller."""
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'resolver_context_tests', ROOT/'scripts/probe-runecl-resolver-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def external_fixture():
    # Six logical instruction bytes plus four synthetic owned tail bytes.
    raw = bytearray(b'\xe8\x00\x00\x00\x00\xc3tail')
    row = dict(offset=1, type='REL32', symbol='synthetic-callee', addend=0,
               owner_section=2, owner_value=0, symbol_section=0, symbol_value=0)
    unit = dict(size=6, compare_size=10, target_address=0x410000, relocations=[
        dict(offset=1, type='REL32', symbol='synthetic-callee', target=0x401000)])
    target = bytearray(raw)
    struct.pack_into('<I', target, 1, (0x401000-0x410005) & 0xffffffff)
    return raw, [row], unit, target


class RuneclResolverContextTests(unittest.TestCase):
    def test_complete_post_return_extent_cannot_be_replaced_by_logical_head(self):
        raw, rows, unit, target = external_fixture()
        proof = probe.compare_definition(raw, rows, unit, target)
        self.assertEqual((proof['logical_bytes'], proof['physical_bytes']), (6, 10))
        for offset in range(6, 10):
            changed = bytearray(raw)
            changed[offset] ^= 1
            with self.subTest(offset=offset), self.assertRaisesRegex(ValueError, 'bytes differ'):
                probe.compare_definition(changed, rows, unit, target)
        for changed in (raw[:6], raw[:-1], raw+b'!'):
            with self.assertRaisesRegex(ValueError, 'physical extent'):
                probe.compare_definition(changed, rows, unit, target)

    def test_local_label_names_may_change_but_actual_destinations_cannot(self):
        raw = bytearray(b'\xc3\x00\x00\x00\x00')
        row = dict(offset=1, type='DIR32', symbol='$Lnew', addend=0,
                   owner_section=3, owner_value=0, symbol_section=3, symbol_value=0)
        unit = dict(size=1, compare_size=5, target_address=0x410000, relocations=[
            dict(offset=1, type='DIR32', symbol='$Lold', target=0x410000)])
        target = b'\xc3'+struct.pack('<I', 0x410000)
        proof = probe.compare_definition(raw, [row], unit, target)
        self.assertEqual(proof['private_label_changes'][0]['relative'], 0)
        for change in (dict(symbol_value=1), dict(addend=1), dict(owner_value=1),
                       dict(offset=True), dict(offset=2), dict(type='REL32')):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.compare_definition(raw, [{**row, **change}], unit, target)

    def test_generated_namespace_hash_does_not_hide_other_identity_or_addend_changes(self):
        raw, rows, unit, target = external_fixture()
        old = '?Angle@KnownView@?A0x12345678@@QAEHXZ'
        new = '?Angle@KnownView@?A0xabcdef12@@QAEHXZ'
        unit['relocations'][0]['symbol'] = old
        rows[0]['symbol'] = new
        proof = probe.compare_definition(raw, rows, unit, target)
        self.assertEqual(len(proof['anonymous_namespace_changes']), 1)
        for change in (dict(symbol=new.replace('Angle', 'Other')),
                       dict(symbol=new.replace('KnownView', 'UnrelatedView')),
                       dict(symbol=new.replace('QAEH', 'QAEX')),
                       dict(symbol=new.replace('abcdef12', 'short')),
                       dict(addend=4), dict(type='DIR32')):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.compare_definition(raw, [{**rows[0], **change}], unit, target)

    def test_native_callers_must_resolve_to_the_extracted_actual_body(self):
        row = dict(symbol='resolver', type='REL32', addend=0, symbol_section=7,
                   symbol_value=0, owner_section=1)
        body = [dict(owner_section=7, owner_value=0)]
        proof = probe.require_definition_calls([row]*3, 'resolver', 3, body)
        self.assertEqual(proof['calls'], 3)
        for change in (dict(symbol_section=0), dict(symbol_section=True),
                       dict(symbol_section=1), dict(symbol_value=4),
                       dict(type='DIR32'), dict(addend=1)):
            with self.subTest(change=change), self.assertRaises(ValueError):
                probe.require_definition_calls([row]*2+[{**row, **change}], 'resolver', 3, body)
        for changed in ([], [dict(owner_section=8, owner_value=0)],
                        [dict(owner_section=7, owner_value=1)]):
            with self.assertRaises(ValueError):
                probe.require_definition_calls([row]*3, 'resolver', 3, changed)

    def test_selected_bodies_are_real_source_and_boundary_changes_fail_closed(self):
        source = (ROOT/'src/EclOperands.cpp').read_text()
        self.assertEqual(probe.selected_source(source, 'all-before'), source)
        self.assertEqual(probe.selected_source(source, 'all-after'), source)
        for case in probe.MODELS:
            selected = probe.selected_source(source, case)
            self.assertIn('#include "EclRunControl.inl"', selected)
            self.assertEqual(selected.count('} // namespace Th09EclRunControl'), 1)
            self.assertIn('return', selected)
        for changed in (source.replace('int __fastcall ResolveInt(', 'int other(', 1),
                        source+source):
            with self.assertRaisesRegex(ValueError, 'boundaries changed'):
                probe.selected_source(changed, 'int')

    def test_wrong_target_preserves_earlier_bound_report_without_compiling(self):
        fake = SimpleNamespace(verified_target=lambda: (_ for _ in ()).throw(ValueError('wrong target')))
        with tempfile.TemporaryDirectory(dir=ROOT/'.analysis') as td:
            output = Path(td)
            report = output/'all-before.json'
            report.write_text('previous source-bound evidence')
            with patch.object(probe, 'module', return_value=fake), \
                    patch.object(probe.subprocess, 'run') as compiler:
                with self.assertRaisesRegex(ValueError, 'wrong target'):
                    probe.run('all-before', output)
                compiler.assert_not_called()
            self.assertEqual(report.read_text(), 'previous source-bound evidence')

    def test_failed_selected_compile_invalidates_stale_success_and_cleans_staging(self):
        fixture = b'public target fixture'
        fake = SimpleNamespace(verified_target=lambda: fixture)
        with tempfile.TemporaryDirectory(dir=ROOT/'.analysis') as td:
            output = Path(td)
            report = output/'all-before.json'
            report.write_text(json.dumps(dict(state='complete-diagnostic')))
            with patch.object(probe, 'module', return_value=fake), \
                    patch.object(probe, 'TARGET_SHA256', probe.digest(fixture)), \
                    patch.object(probe.subprocess, 'run', return_value=SimpleNamespace(
                        returncode=1, stdout='', stderr='synthetic compiler failure')) as compiler:
                with self.assertRaisesRegex(ValueError, 'compiler failed'):
                    probe.run('all-before', output)
                self.assertEqual(compiler.call_count, 1)
                self.assertEqual(compiler.call_args.args[0][:3], ['nice', '-n', '10'])
            self.assertEqual(json.loads(report.read_text())['state'], 'rejected')
            self.assertEqual(list(output.iterdir()), [report])


if __name__ == '__main__':
    unittest.main()
