"""Basic CI rejects invalid inputs without invoking a compiler or importing code."""
import contextlib
import io
import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('basic_ci', ROOT / 'scripts/ci.py')
ci = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ci)


class BasicCiTests(unittest.TestCase):
    def test_syntax_check_does_not_execute_python(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'script.py'
            path.write_text('raise RuntimeError("must not execute")\n')
            ci.check_file(path)

    def test_machine_input_syntax_is_checked(self):
        samples = {
            '.py': ('x = 1\n', 'return 1\n'),
            '.json': ('{"value": 1}', '{"value":'),
            '.toml': ('value = 1\n', 'value = [\n'),
            '.sh': ('true\n', 'if true; then\n'),
        }
        with tempfile.TemporaryDirectory() as folder:
            for suffix, (valid, invalid) in samples.items():
                path = Path(folder) / ('input' + suffix)
                with self.subTest(suffix=suffix):
                    path.write_text(valid)
                    ci.check_file(path)
                    path.write_text(invalid)
                    with self.assertRaises((ValueError, subprocess.CalledProcessError)):
                        ci.check_file(path)

    def test_deleted_tracked_file_is_ignored(self):
        ci.check_file(Path('/nonexistent/deleted-script.py'))

    def test_private_binary_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'private.exe'
            path.write_bytes(b'private fixture')
            with self.assertRaisesRegex(ValueError, 'private binary'):
                ci.check_file(path)

    def test_invalid_committed_input_is_checked_without_worktree_changes(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            subprocess.run(['git', 'init', '-q', str(root)], check=True)
            (root / 'old.py').write_text('def invalid(\n')
            subprocess.run(['git', 'add', '.'], cwd=root, check=True)
            subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                            'commit', '-qm', 'fixture'], cwd=root, check=True)
            output = io.StringIO()
            with patch.object(ci, 'ROOT', root), patch.object(ci.sys, 'argv', ['ci.py']), \
                    contextlib.redirect_stderr(output):
                self.assertEqual(ci.main(), 1)
            self.assertIn('old.py', output.getvalue())

    def test_staged_whitespace_is_checked(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            subprocess.run(['git', 'init', '-q', str(root)], check=True)
            path = root / 'input.py'
            path.write_text('value = 1\n')
            subprocess.run(['git', 'add', '.'], cwd=root, check=True)
            subprocess.run(['git', '-c', 'user.name=Test', '-c', 'user.email=test@example.invalid',
                            'commit', '-qm', 'fixture'], cwd=root, check=True)
            path.write_text('value = 1 \n')
            subprocess.run(['git', 'add', '.'], cwd=root, check=True)
            output = io.StringIO()
            with patch.object(ci, 'ROOT', root), patch.object(ci.sys, 'argv', ['ci.py']), \
                    contextlib.redirect_stderr(output):
                self.assertEqual(ci.main(), 1)
            self.assertIn('--check', output.getvalue())

    def test_unknown_flags_are_rejected(self):
        result = subprocess.run([ci.sys.executable, '-B', str(ROOT / 'scripts/ci.py'),
                                 '--full', '--unknown'], capture_output=True)
        self.assertEqual(result.returncode, 2)


if __name__ == '__main__':
    unittest.main()
