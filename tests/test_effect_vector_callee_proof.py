"""SDK alias binding requires actual complete relocation-free COFF bodies."""
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('effect_vector_proof', ROOT/'scripts/inspect-effect-type34.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


class CompleteVectorCalleeProof(unittest.TestCase):
    def test_wrong_size_bytes_or_relocations_cannot_bind_an_alias(self):
        original = bytes(range(26))
        audit.require_local_callee_bytes(original, [], original, 26)
        for raw, fields, target, size in (
                (original[:-1], [], original, 26),
                (original+b'\x90', [], original, 26),
                (original, [], original[:-1], 26),
                (b'X'+original[1:], [], original, 26),
                (original, [{'offset': 2, 'type': 'DIR32'}], original, 26)):
            with self.subTest(raw=raw, fields=fields), self.assertRaises(ValueError):
                audit.require_local_callee_bytes(raw, fields, target, size)

    def test_same_object_definition_is_read_before_alias_binding(self):
        symbol = '??0D3DXVECTOR3@@QAE@MMM@Z'
        with patch.object(audit.c, 'object_function', return_value=(bytes(range(26)), [])) as read, \
                patch.object(audit.c, 'pe_bytes_at', return_value=bytes(range(26))) as target:
            row = audit.local_vector_callee(Path('candidate.obj'), symbol, b'image')
        read.assert_called_once_with(Path('candidate.obj'), symbol)
        target.assert_called_once_with(b'image', 0x4010B0, 26)
        self.assertTrue(row['same_object_definition'])
        self.assertEqual(row['exactness_credit'], 'none')
        with patch.object(audit.c, 'object_function', side_effect=ValueError('undefined')):
            with self.assertRaisesRegex(ValueError, 'undefined'):
                audit.local_vector_callee(Path('candidate.obj'), symbol, b'image')
