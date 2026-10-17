"""Pure-input safety checks for unknown-origin, byte-matched ExAttack ctors.

A complete target-byte match is useful evidence, but a compiler-synthesized
special member is not automatically an explicitly authored source function.
"""
import copy
import hashlib
import importlib.util
from pathlib import Path
import struct
import sys
import tomllib
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'constructor_diagnostic', ROOT / 'scripts/inspect-exattack-constructors.py')
ctor = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ctor)
fspec = importlib.util.spec_from_file_location(
    'constructor_frontier', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(fspec)
fspec.loader.exec_module(frontier)


class OriginalSpecialMemberBoundaries(unittest.TestCase):
    ORIGINAL = {
        'exattack_record_constructor': bytes.fromhex(
            '56 8bf1 8d4e10 e8e5dbfeff 8bc6 5e c3'),
        'exattack_controller_constructor': bytes.fromhex(
            '56 68a0574100 6801010000 8bf1 6a4c 8d461c 50 '
            'e8a8bcfeff 8bc6 5e c3'),
    }

    @classmethod
    def source_raw(cls, name):
        spec = ctor.OWNERS[name]
        body = bytearray(cls.ORIGINAL[name])
        rows = []
        for off, kind, symbol, destination in spec['fields']:
            struct.pack_into('<I', body, off, 0)
            rows.append({'offset': off, 'type': kind, 'symbol': symbol, 'addend': 0})
        return bytes(body), rows

    def test_original_15_and_28_bytes_have_independent_known_sha(self):
        expected = {'exattack_record_constructor': (0x4157A0, 15, 1, 'normal'),
                    'exattack_controller_constructor': (0x4157B0, 28, 2, 'size')}
        for name, (va, n, relos, profile) in expected.items():
            with self.subTest(name=name):
                original = self.ORIGINAL[name]
                spec = ctor.OWNERS[name]
                self.assertEqual(len(original), n)
                self.assertEqual(spec['size'], n)
                self.assertEqual(spec['address'], va)
                self.assertEqual(spec['profile'], profile)
                self.assertEqual(len(spec['fields']), relos)
                self.assertEqual(hashlib.sha256(original).hexdigest(), spec['sha256'])

    def test_both_originals_match_only_with_correct_source_relocations(self):
        for name in ctor.OWNERS:
            with self.subTest(name=name):
                compiled, fields = self.source_raw(name)
                outcome = ctor.bind_and_compare(self.ORIGINAL[name], compiled, fields,
                                                ctor.OWNERS[name])
                self.assertEqual(outcome['candidate_full_extent_bytes'],
                                 len(self.ORIGINAL[name]))
                self.assertEqual(outcome['original_physical_bytes_equal'], True)
                self.assertEqual(outcome['origin_classification'], 'unknown/review')
                self.assertEqual(outcome['exactness_credit'], 'none')
                self.assertEqual(outcome['bound_original_fields'], len(fields))
                self.assertEqual(outcome['diagnostic_only'], True)

    def test_original_sha_changed_extent_or_source_byte_rejected(self):
        for name in ctor.OWNERS:
            source, rows = self.source_raw(name)
            original = self.ORIGINAL[name]
            for altered in (original[:-1], bytes([original[0] ^ 1]) + original[1:]):
                with self.subTest(name=name, altered=altered[:3]), self.assertRaises(ValueError):
                    ctor.bind_and_compare(altered, source, rows, ctor.OWNERS[name])
            with self.subTest(name=name), self.assertRaisesRegex(ValueError,
                                                                 'physical size differs'):
                ctor.bind_and_compare(original, source + b'\x90', rows,
                                      ctor.OWNERS[name])
            corrupt_source = bytes([source[0] ^ 1]) + source[1:]
            with self.subTest(name=name), self.assertRaisesRegex(ValueError,
                                                                 'not byte-exact'):
                ctor.bind_and_compare(original, corrupt_source, rows, ctor.OWNERS[name])

    def test_target_relocation_counts_kinds_symbols_addends_and_offsets_bound(self):
        for name in ctor.OWNERS:
            raw, valid = self.source_raw(name)
            with self.subTest(name=name):
                ctor.validate_source_fields(valid, ctor.OWNERS[name])
                for bad in (valid[:-1], valid + [valid[-1]],
                            [dict(valid[0], symbol='?FakeCtor@@QAEXXZ')] + valid[1:],
                            [dict(valid[0], addend=4)] + valid[1:],
                            [dict(valid[0], offset=valid[0]['offset']+1)] + valid[1:],
                            [dict(valid[0], type='DIR32' if valid[0]['type']=='REL32' else 'REL32')] + valid[1:]):
                    with self.subTest(bad=bad), self.assertRaises(ValueError):
                        ctor.validate_source_fields(bad, ctor.OWNERS[name])

    def test_original_targets_are_real_ctor_and_eh_vector_helper(self):
        self.assertEqual(ctor.OWNERS['exattack_record_constructor']['fields'][0],
                         (7, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390))
        self.assertEqual(ctor.OWNERS['exattack_controller_constructor']['fields'], (
            (2, 'DIR32', '??0ExAttackRecord@@QAE@XZ', 0x004157A0),
            (20, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ))
        with (ROOT / 'config/match-units.toml').open('rb') as file:
            units = tomllib.load(file)['units']
        ctor.validate_origin_and_exact_callees(units)
        tampered = copy.deepcopy(units)
        tampered['zun-timer-constructor']['target_address'] = 0x403391
        with self.assertRaisesRegex(ValueError,'timer constructor'):
            ctor.validate_origin_and_exact_callees(tampered)

    def test_noncrediting_boss_guard_cannot_promote_origin_unknown_to_exact(self):
        sample = {}
        for name, spec in ctor.OWNERS.items():
            code, rows = self.source_raw(name)
            sample[name] = ctor.bind_and_compare(self.ORIGINAL[name], code, rows, spec)
            sample[name]['original_instructions'] = (7 if 'record' in name else 11)
        frontier.guard_exattack_constructor_codegen(sample)
        invalid = [
            {**sample, 'extra_owner': sample['exattack_record_constructor']},
            {'exattack_record_constructor': sample['exattack_record_constructor']},
        ]
        for name in sample:
            for key, value in (
                    ('origin_classification', 'authored'),
                    ('exactness_credit', 'accepted'),
                    ('diagnostic_only', False),
                    ('bound_original_fields', 0),
                    ('candidate_full_extent_bytes', 0),
                    ('original_instructions', 0),
                    ('relocation_target_addresses', [0x4157A1]),
                    ('original_physical_bytes_equal', False)):
                bad = copy.deepcopy(sample)
                bad[name][key] = value
                invalid.append(bad)
        for bad in invalid:
            with self.subTest(bad=str(bad)[:90]), self.assertRaises(RuntimeError):
                frontier.guard_exattack_constructor_codegen(bad)

    def test_original_compilation_profile_and_all_exact_siblings(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            units = tomllib.load(stream)['units']
        profile = units['exattack-release-records']['profile']
        self.assertEqual(profile, list(frontier._SCREEN_EFFECT_BASE_PROFILE))
        self.assertEqual(profile.count('/Ob1'),1)
        self.assertNotIn('/Os', profile)
        groups = frontier.exact_source_carriers(
            'src/ExAttackController.cpp', frontier.EXATTACK_CONTROLLER_EXACT_CARRIERS)
        self.assertEqual(groups, {'build/matching/ExAttackController.obj': [
            'exattack-release-records', 'exattack-release', 'exattack-register',
            'exattack-controller-spawn']})
        for unknown in ('??0ExAttackRecord@@QAE@XZ',
                        '??0ExAttackController@@QAE@XZ'):
            self.assertNotIn(unknown, [v['symbol'] for v in units.values()])


if __name__=='__main__':
    unittest.main()
