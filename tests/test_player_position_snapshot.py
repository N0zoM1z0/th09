"""Provenance tests for the source-cold Player context experiment."""
from pathlib import Path
import hashlib
import importlib.util
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'position_snapshot', ROOT/'scripts/probe-player-position-visibility.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class PlayerPositionSnapshot(unittest.TestCase):
    def test_recovery_snapshot_includes_new_source_and_skips_deleted_old_path(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root/'src').mkdir()
            (root/'src/new.inl').write_bytes(b'new maintained source\n')
            with patch.object(probe, 'ROOT', root), patch.object(
                    probe.subprocess, 'check_output',
                    return_value=b'src/deleted.cpp\0src/new.inl\0') as git:
                snapshot = probe.source_snapshot()
            self.assertEqual(snapshot, {'src/new.inl': hashlib.sha256(
                b'new maintained source\n').hexdigest()})
            command = git.call_args.args[0]
            self.assertIn('--cached', command)
            self.assertIn('--others', command)
            self.assertIn('--exclude-standard', command)

    def test_snapshot_binds_compiler_bytes_including_line_endings(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root/'src').mkdir()
            source = root/'src/input.inl'
            with patch.object(probe, 'ROOT', root), patch.object(
                    probe.subprocess, 'check_output', return_value=b'src/input.inl\0'):
                source.write_bytes(b'line\r\n')
                before = probe.source_snapshot()
                source.write_bytes(b'line\n')
                after = probe.source_snapshot()
            self.assertNotEqual(before, after)
            self.assertEqual(after['src/input.inl'], hashlib.sha256(b'line\n').hexdigest())


if __name__ == '__main__':
    unittest.main()
