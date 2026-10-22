"""No-target tests: RunEcl first-include variants must actually reach VC7.1."""
from pathlib import Path
import hashlib
import importlib.util
import json
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('ecl_variant', ROOT/'scripts/probe-ecl-variant.py')
variant = importlib.util.module_from_spec(spec)
spec.loader.exec_module(variant)


class EclVariantFirstIncludeTests(unittest.TestCase):
    def test_every_lexical_first_include_fragment_is_copied(self):
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)
            manifest = variant.stage_variants(dst)
            self.assertEqual(set(manifest), set(variant.FRAGMENTS))
            self.assertEqual(len(manifest), 8)
            for name in variant.FRAGMENTS:
                original = (ROOT/'src'/name).read_bytes()
                self.assertEqual((dst/name).read_bytes(), original)
                self.assertEqual(manifest[name], hashlib.sha256(original).hexdigest())
            self.assertEqual((dst/'EclRunLate.inl').read_text().count('#include "EclRunState.inl"'), 1)
            self.assertIn('#include "EclRunControl.inl"', (dst/'EclRunMovement.inl').read_text())

    def test_shared_flags_variant_reaches_both_first_declarations(self):
        original = (ROOT/'src/EclEnemyPrimaryFlags.hpp').read_bytes()
        probe = original.replace(b'unsigned int boss01 : 1;',
                                 b'unsigned int reviewedBoss01 : 1;', 1)
        self.assertNotEqual(probe, original)
        with tempfile.TemporaryDirectory() as folder:
            tmp = Path(folder)
            header = tmp/'flags-variant.hpp'
            header.write_bytes(probe)
            manifest = variant.stage_variants(tmp/'carrier', primary_flags=header)
            self.assertEqual((tmp/'carrier/EclEnemyPrimaryFlags.hpp').read_bytes(), probe)
            self.assertEqual(manifest['EclEnemyPrimaryFlags.hpp'],
                             hashlib.sha256(probe).hexdigest())
            for consumer in ('EclRunMovement.inl', 'EclRunState.inl'):
                self.assertIn('#include "EclEnemyPrimaryFlags.hpp"',
                              (tmp/'carrier'/consumer).read_text())
            self.assertEqual((ROOT/'src/EclEnemyPrimaryFlags.hpp').read_bytes(), original)

    def test_changed_state_declaration_not_just_changed_late_body(self):
        before = (ROOT/'src/EclRunState.inl').read_text()
        probe = before.replace('inline unsigned int AssignTimeoutSpellBit(',
                               'inline unsigned long AssignTimeoutSpellBit(', 1)
        self.assertNotEqual(probe, before)
        with tempfile.TemporaryDirectory() as folder:
            base = Path(folder)
            candidate = base/'state-variant.inl'
            candidate.write_text(probe)
            manifest = variant.stage_variants(base/'carrier', state=candidate)
            self.assertEqual((base/'carrier/EclRunState.inl').read_text(), probe)
            self.assertNotEqual(manifest['EclRunState.inl'],
                                hashlib.sha256(before.encode()).hexdigest())
            self.assertEqual((ROOT/'src/EclRunState.inl').read_text(), before)

    def test_changed_control_fragment_staged_through_all_intermediate_files(self):
        original = (ROOT/'src/EclRunControl.inl').read_text()
        probe = original.replace('        int value = rawTime;',
                                 '        int value = static_cast<int>(rawTime);', 1)
        self.assertNotEqual(probe, original)
        with tempfile.TemporaryDirectory() as folder:
            tmp = Path(folder)
            control = tmp/'control-variant.inl'
            control.write_text(probe)
            manifest = variant.stage_variants(tmp/'carrier', control=control)
            self.assertEqual((tmp/'carrier/EclRunControl.inl').read_text(), probe)
            self.assertNotEqual(manifest['EclRunControl.inl'],
                                hashlib.sha256(original.encode()).hexdigest())
            for intermediate in ('EclRunLate.inl', 'EclRunState.inl',
                                 'EclRunBullet.inl', 'EclRunRemote.inl',
                                 'EclRunMovement.inl'):
                self.assertTrue((tmp/'carrier'/intermediate).is_file())

    def test_multiple_fragment_overrides_reach_the_actual_first_include_chain(self):
        names = ('EclManager.cpp', 'EclRunBullet.inl', 'EclRunRemote.inl')
        changed = {name: b'// reviewed context\n'+(ROOT/'src'/name).read_bytes()
                   for name in names}
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)/'carrier'
            manifest = variant.stage_variants(dst, overrides=changed)
            for name, data in changed.items():
                self.assertEqual((dst/name).read_bytes(), data)
                self.assertEqual(manifest[name], hashlib.sha256(data).hexdigest())
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)/'carrier'
            with self.assertRaisesRegex(ValueError, 'leaves'):
                variant.stage_variants(dst, overrides={'../outside.cpp':b''})
            self.assertFalse(dst.exists())
            with self.assertRaisesRegex(ValueError, 'conflicting'):
                variant.stage_variants(dst, state=ROOT/'src/EclRunState.inl',
                                       overrides={'EclRunState.inl':b''})
            with self.assertRaisesRegex(ValueError, 'non-byte'):
                variant.stage_variants(dst, overrides={'EclRunState.inl':'not bytes'})
            self.assertFalse(dst.exists())

    def test_first_include_chain_drift_fails_without_compilation(self):
        source = {name: (ROOT/'src'/name).read_bytes().replace(b'\r\n', b'\n')
                  for name in variant.FRAGMENTS}
        variant.attested_include_chain(source)
        for parent, old in (
                ('EclRunLate.inl', '#include "EclRunState.inl"'),
                ('EclRunState.inl', '#include "EclRunBullet.inl"'),
                ('EclRunBullet.inl', '#include "EclRunRemote.inl"'),
                ('EclRunRemote.inl', '#include "EclRunMovement.inl"'),
                ('EclRunMovement.inl', '#include "EclRunControl.inl"'),
                ('EclRunMovement.inl', '#include "EclEnemyPrimaryFlags.hpp"'),
                ('EclRunState.inl', '#include "EclEnemyPrimaryFlags.hpp"')):
            with self.subTest(parent=parent), self.assertRaisesRegex(ValueError, 'include chain|first declaration include'):
                candidate = dict(source)
                candidate[parent] = candidate[parent].replace(old.encode(), b'// replaced')
                variant.attested_include_chain(candidate)
        altered = dict(source)
        altered['EclManager.cpp'] = altered['EclManager.cpp'].replace(
            b'#define TH09_ECL_RUN_STATE_BODY\n#include "EclRunState.inl"',
            b'#define TH09_ECL_RUN_STATE_BODY\n// not the actual body include')
        with self.assertRaisesRegex(ValueError, 'include chain|state BODY'):
            variant.attested_include_chain(altered)

    def test_include_attestation_accepts_both_eols_without_weakening_body_check(self):
        source = {name: (ROOT/'src'/name).read_bytes().replace(b'\r\n', b'\n')
                  for name in variant.FRAGMENTS}
        for newline in (b'\n', b'\r\n'):
            data = {name: blob.replace(b'\n', newline) for name, blob in source.items()}
            before = dict(data)
            variant.attested_include_chain(data)
            self.assertEqual(data, before)
            actual = b'#define TH09_ECL_RUN_STATE_BODY' + newline + b'#include "EclRunState.inl"'
            data['EclManager.cpp'] = data['EclManager.cpp'].replace(
                actual, actual.replace(b'_STATE_BODY', b'_STATE_WRONG'))
            self.assertNotEqual(data, before)
            with self.subTest(newline=newline), self.assertRaisesRegex(ValueError, 'state BODY'):
                variant.attested_include_chain(data)

    def test_output_symlink_is_rejected_before_replacement(self):
        with tempfile.TemporaryDirectory() as folder:
            top = Path(folder)
            out = top/'carrier'
            out.mkdir()
            (out/'EclRunState.inl').symlink_to(ROOT/'src/EclRunState.inl')
            with self.assertRaisesRegex(ValueError, 'symlink'):
                variant.stage_variants(out)

    def test_symlinked_candidate_source_does_not_reach_carrier(self):
        with tempfile.TemporaryDirectory() as folder:
            tmp = Path(folder)
            source = tmp/'state-symlink.inl'
            source.symlink_to(ROOT/'src/EclRunState.inl')
            with self.assertRaisesRegex(ValueError, 'symlinked ECL variant source'):
                variant.stage_variants(tmp/'carrier', state=source)
            self.assertFalse((tmp/'carrier/EclManager.cpp').exists())

    def test_profile_carries_pinned_true_ecl_compiler_context(self):
        p = variant.pinned_profile()
        self.assertIn('/O2', p)
        self.assertIn('/Ob1', p)
        self.assertIn('/Gr', p)
        self.assertIn('/Oy-', p)

    def test_failed_compile_cannot_reuse_a_previous_trial_report(self):
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)
            for name in ('EclManager.obj', 'complete.json', 'replay-diagnostic.json', 'probe-result.json'):
                (dst/name).write_text('previous trial')
            with mock.patch.object(variant, 'command', side_effect=RuntimeError('compile failed')):
                with self.assertRaisesRegex(RuntimeError, 'compile failed'):
                    variant.replay(dst)
            for name in ('EclManager.obj', 'complete.json', 'replay-diagnostic.json', 'probe-result.json'):
                self.assertFalse((dst/name).exists())

    def test_shifted_fields_remain_rejected_but_keep_audited_handler_evidence(self):
        complete = dict(
            diagnostic_only=True, identity_audit_passed=True,
            physical_bytes=15564, relocations=598,
            ordinary_different_bytes=1912, field_different_bytes=201,
            table_different_bytes=25, raw_sha256='a'*64,
            target_owner_sha256='b'*64,
            object_source_binding={'status': 'test fixture'})
        shapes = dict(
            identity_audit_passed=True, matching_handler_shapes=144,
            unique_handlers=148, candidate_code_bytes=14791,
            target_code_bytes=14792,
            mismatches=[dict(opcode=156, candidate_bytes=51, target_bytes=52)])
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)

            def fake_command(args):
                if args[0].endswith('compile-probe.sh'):
                    (dst/'EclManager.obj').write_bytes(b'fixture only')
                    return ''
                return json.dumps(shapes)

            def fake_audit(*args, **kwargs):
                (dst/'complete.json').write_text(json.dumps(complete))
                return SimpleNamespace(returncode=1, stdout='', stderr='')

            with mock.patch.object(variant, 'command', side_effect=fake_command), \
                    mock.patch.object(variant.subprocess, 'run', side_effect=fake_audit):
                with self.assertRaisesRegex(ValueError, 'field positions changed: 201 field bytes'):
                    variant.replay(dst)
            diagnostic = json.loads((dst/'replay-diagnostic.json').read_text())
            self.assertFalse(diagnostic['field_positions_agree'])
            self.assertEqual(diagnostic['matching_handler_shapes'], 144)
            self.assertEqual(diagnostic['handler_mismatches'], shapes['mismatches'])
            self.assertEqual(diagnostic['object_source_binding'], complete['object_source_binding'])
            self.assertEqual(diagnostic['exactness_credit'], 'none')

    def test_replay_checks_all_output_symlinks_before_discarding_old_object(self):
        with tempfile.TemporaryDirectory() as folder:
            dst = Path(folder)
            obj = dst/'EclManager.obj'
            obj.write_bytes(b'previous object')
            (dst/'replay-diagnostic.json').symlink_to(ROOT/'src/EclManager.cpp')
            with mock.patch.object(variant, 'command') as compile_command:
                with self.assertRaisesRegex(ValueError, 'symlink ECL replay output'):
                    variant.replay(dst)
                compile_command.assert_not_called()
            self.assertEqual(obj.read_bytes(), b'previous object')


if __name__ == '__main__':
    unittest.main()
