"""Fail-closed boundaries for the bounded external-source audit."""

import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("cirno_audit", ROOT / "scripts/audit-cirno-reference.py")
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def field(offset=0, symbol="external", kind="DIR32", **kwargs):
    return dict(offset=offset, symbol=symbol, type=kind, addend=kwargs.get("addend", 0),
                symbol_section=kwargs.get("section", 0), symbol_value=kwargs.get("value", 0),
                owner_section=1, owner_value=16)


class ReferenceAuditTests(unittest.TestCase):
    def test_named_relocations_preserve_addends_and_real_destinations(self):
        code, bound, missing = audit.replay(
            bytes(8), [field(addend=4), field(4, "callee", "REL32")],
            0x401000, 8, {"external": 0x4A0000, "callee": 0x402000})
        self.assertEqual(struct.unpack("<II", code), (0x4A0004, 0xFF8))
        self.assertEqual(len(bound), 2)
        self.assertFalse(missing)

    def test_internal_target_uses_actual_definition_even_if_binding_lies(self):
        code, bound, missing = audit.replay(
            bytes(8), [field(symbol="$L1", section=1, value=20)],
            0x401000, 8, {"$L1": 0xDEADBEEF})
        self.assertEqual(struct.unpack_from("<I", code)[0], 0x401004)
        self.assertEqual(bound[0]["target"], 0x401004)
        self.assertFalse(missing)

    def test_unknown_reference_is_not_recovered_from_raw_bytes(self):
        raw = struct.pack("<I", 0x4A1234)
        code, bound, missing = audit.replay(raw, [field()], 0x401000, 4, {})
        self.assertEqual(code, raw)
        self.assertFalse(bound)
        self.assertEqual(len(missing), 1)

    def test_label_outside_complete_owner_is_unresolved(self):
        _, bound, missing = audit.replay(
            bytes(4), [field(symbol="$L1", section=1, value=20)], 0x401000, 4, {})
        self.assertFalse(bound)
        self.assertEqual(len(missing), 1)

    def test_rejects_partial_overlapping_and_unsupported_fields(self):
        for rows in ([field(1)], [field(), field()], [field(kind="OTHER")]):
            with self.subTest(rows=rows), self.assertRaises(ValueError):
                audit.replay(bytes(4), rows, 0x401000, 4, {"external": 0x4A0000})

    def test_extent_includes_switch_labels_and_table_bytes(self):
        owner = dict(section=1, value=0, kind=0x20)
        sections = [dict(size=32, raw=0)]
        symbols = [owner, dict(section=1, value=8, kind=0),
                   dict(section=1, value=24, kind=0x20)]
        extent, padding = audit.physical_extent(bytes(32), sections, symbols, owner, 16)
        self.assertEqual((extent, padding), (24, 0))

    def test_only_short_alignment_can_be_discarded(self):
        owner = dict(section=1, value=0, kind=0x20)
        for extra, expected in [(4, (16, 4)), (16, (32, 0))]:
            raw = bytes(16) + bytes([0xCC]) * extra
            self.assertEqual(audit.physical_extent(
                raw, [dict(size=len(raw), raw=0)], [owner], owner, 16), expected)

    def test_relocation_in_apparent_padding_cannot_be_truncated(self):
        owner = dict(section=1, value=0, kind=0x20)
        raw = bytes(16) + bytes([0xCC]) * 4
        sections = [dict(size=20, raw=0, relocation_offsets=[16])]
        self.assertEqual(audit.physical_extent(raw, sections, [owner], owner, 16), (20, 0))

    def test_changed_object_or_profile_invalidates_build_proof(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            obj = root / "mainmenu.obj"
            obj.write_bytes(b"original object")
            inputs = {"source": "pinned"}
            receipt = dict(inputs, profile=audit.PROFILES["mainmenu"],
                           object_sha256=audit.sha(obj.read_bytes()))
            (root / "mainmenu-build.json").write_text(json.dumps(receipt))
            with patch.object(audit, "PACKET", root), patch.object(audit, "OBJECTS", root):
                self.assertEqual(audit.verify_build("mainmenu", inputs), obj)
                with self.assertRaises(ValueError):
                    audit.verify_build("mainmenu", {"source": "changed"})
                obj.write_bytes(b"changed object")
                with self.assertRaises(ValueError):
                    audit.verify_build("mainmenu", inputs)

    def test_wrong_reference_revision_is_rejected(self):
        with patch.object(audit, "git", return_value="different-commit"):
            with self.assertRaisesRegex(ValueError, "reference HEAD"):
                audit.source_inputs(Path("unused"))

    def test_cp932_trail_backslash_is_preserved_for_legacy_compiler(self):
        self.assertEqual(audit.cp932_source('"表"'), b'"\x95\\\\"')
        self.assertEqual(audit.cp932_source("ordinary\\n"), b"ordinary\\n")


if __name__ == "__main__":
    unittest.main()
