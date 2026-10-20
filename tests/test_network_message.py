"""Network owner boundaries, switch ownership and producer bindings."""
import copy
import importlib.util
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('network_message_audit', ROOT/'scripts/inspect-network-message.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def rows():
    result = []
    for off, kind, va in audit.TARGET_FIELDS:
        local = audit.BASE <= va < audit.BASE+audit.SIZE
        symbol = '$actual_coff_label' if local else next(n for n, d in audit.REFERENTS.items() if d == va)
        result.append(dict(offset=off, type=kind, symbol=symbol, addend=0,
                           owner_section=1, symbol_section=1 if local else 0,
                           owner_value=0, symbol_value=va-audit.BASE if local else 0))
    return result


def frontier():
    return dict(diagnostic_only=True, exactness_credit='none', target_bytes=808,
                target_code_bytes=768, candidate_bytes=808, candidate_code_bytes=768,
                target_sha256=audit.TARGET_SHA256, target_instructions=235,
                candidate_instructions=234, bound_fields=46, target_fields=46,
                ordered_call_operands_agree=True, direct_calls=17, virtual_calls=2,
                target_blocks=34, candidate_blocks=34, direct_graphs_agree=True,
                switch_destinations=6, switch_map_bytes=16, missing_target_bytes=0,
                extra_candidate_bytes=0, overlap_differences=111,
                difference_offsets=sorted(audit.RESIDUAL_OFFSETS), complete_bytes_equal=False)


class NetworkMessageAuditTests(unittest.TestCase):
    def test_complete_fields_and_real_section_relative_labels(self):
        linked, fields = audit.bind_candidate(bytes(audit.SIZE), rows())
        self.assertEqual(tuple(fields), audit.TARGET_FIELDS)
        for off, kind, destination in fields:
            expected = destination-(audit.BASE+off+4 if kind == 'REL32' else 0)
            self.assertEqual(struct.unpack_from('<I', linked, off)[0], expected & 0xFFFFFFFF)
        original = rows()
        external = next(i for i, r in enumerate(original) if r['symbol_section'] == 0)
        for changes in ({'symbol': '?invented@@3HA'}, {'addend': 1}, {'type': 'DIR16'},
                        {'owner_section': 0}, {'offset': -1}):
            changed = copy.deepcopy(original)
            changed[external].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), changed)
        local = next(i for i, r in enumerate(original) if r['symbol_section'] == 1)
        for changes in ({'symbol_value': audit.SIZE}, {'symbol_value': -1},
                        {'symbol_section': 2}, {'type': 'REL32'}):
            changed = copy.deepcopy(original)
            changed[local].update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), changed)
        for changed in (original[:-1], list(reversed(original)), [original[0]]+original[:-1]):
            with self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), changed)

    def test_switch_map_and_six_distinct_actual_destinations(self):
        data = bytearray(audit.SIZE)
        entries = tuple(audit.BASE+i for i in range(6))
        struct.pack_into('<6I', data, audit.CODE_SIZE, *entries)
        data[-16:] = audit.BYTE_MAP
        self.assertEqual(audit.switch_entries(data, audit.CODE_SIZE), entries)
        for i in range(16):
            changed = bytearray(data)
            changed[-16+i] ^= 1
            with self.subTest(index=i), self.assertRaises(ValueError):
                audit.switch_entries(changed, audit.CODE_SIZE)
        struct.pack_into('<I', data, audit.CODE_SIZE+4, entries[0])
        with self.assertRaises(ValueError):
            audit.switch_entries(data, audit.CODE_SIZE)
        with self.assertRaises(ValueError):
            audit.switch_entries(data[:-1], audit.CODE_SIZE)

    def test_original_identity_and_table_bytes_are_not_optional(self):
        for data in (b'', bytes(audit.SIZE), bytes(audit.CODE_SIZE), bytes(audit.SIZE+1)):
            with self.assertRaises(ValueError):
                audit.require_original(data)

    def test_same_count_mismatch_move_is_rejected_but_real_improvement_is_allowed(self):
        original = frontier()
        self.assertEqual(len(original['difference_offsets']), 111)
        audit.guard(original)
        bad = copy.deepcopy(original)
        outside = next(i for i in range(audit.SIZE) if i not in audit.RESIDUAL_OFFSETS)
        bad['difference_offsets'] = sorted(bad['difference_offsets'][1:]+[outside])
        with self.assertRaises(ValueError):
            audit.guard(bad)
        audit.guard(dict(original, difference_offsets=[], overlap_differences=0,
                         candidate_instructions=235, complete_bytes_equal=True))
        for changes in ({'candidate_instructions': 233}, {'candidate_blocks': 33},
                        {'switch_map_bytes': 15}, {'bound_fields': 45},
                        {'complete_bytes_equal': True}, {'exactness_credit': 'exact'},
                        {'candidate_code_bytes': 784}, {'virtual_calls': 1}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audit.guard(dict(original, **changes))

    def test_build_attests_target_first_and_detects_mid_compile_header_changes(self):
        with patch.object(audit, 'module', return_value=SimpleNamespace(
                verified_target=lambda: (_ for _ in ()).throw(ValueError('wrong target')))), patch.object(
                audit.subprocess, 'run') as compiler:
            with self.assertRaisesRegex(ValueError, 'wrong target'):
                audit.build()
            compiler.assert_not_called()
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            for folder in ('src', 'scripts', 'config'):
                (root/folder).mkdir()
            (root/audit.SOURCE).write_text('#include "message.hpp"\n')
            (root/'src/message.hpp').write_text('int fixture;\n')
            (root/'scripts/compile-probe.sh').write_text('provider fixture\n')
            (root/'config/tools.lock.toml').write_text('pinned fixture\n')
            with patch.object(audit, 'ROOT', root), patch.object(audit, 'module', return_value=SimpleNamespace(
                    verified_target=lambda: b'verified')), patch.object(audit.subprocess, 'run', return_value=SimpleNamespace(
                    returncode=0)) as compiler:
                inputs = audit.build()
                self.assertIn('src/message.hpp', inputs)
                self.assertEqual(compiler.call_args.args[0][:3], ['nice', '-n', '10'])
                def mutate(*args, **kwargs):
                    (root/'src/message.hpp').write_text('changed fixture;\n')
                    return SimpleNamespace(returncode=0)
                compiler.side_effect = mutate
                with self.assertRaisesRegex(ValueError, 'inputs changed'):
                    audit.build()


if __name__ == '__main__':
    unittest.main()
