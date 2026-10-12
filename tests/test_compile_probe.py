from __future__ import annotations

import hashlib
import os
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]


class CompilerCorePinTests(unittest.TestCase):
    def setUp(self) -> None:
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        (self.root / "scripts").mkdir()
        (self.root / "config").mkdir()
        self.wrapper = self.root / "scripts" / "compile-probe.sh"
        self.wrapper.write_bytes((ROOT / "scripts" / "compile-probe.sh").read_bytes())
        self.tools = self.root / "toolchain"
        self.binary = self.tools / "Vc7" / "bin"
        self.binary.mkdir(parents=True)
        components = {
            "cl.exe": "compiler_sha256",
            "c1.dll": "c_frontend_sha256",
            "c1xx.dll": "cpp_frontend_sha256",
            "c2.dll": "backend_sha256",
        }
        lock = ["[msvc71]"]
        for name, key in components.items():
            data = ("synthetic component " + name).encode()
            (self.binary / name).write_bytes(data)
            digest = hashlib.sha256(data).hexdigest()
            lock.append(f'{key} = "{digest}"')
        (self.root / "config" / "tools.lock.toml").write_text("\n".join(lock) + "\n")
        self.source = self.root / "probe.cpp"
        self.source.write_text("void probe();\n")
        self.output = self.root / "build" / "probe.obj"
        self.log = self.root / "wine-called.txt"
        commands = self.root / "commands"
        commands.mkdir()
        scripts = {
            "winepath": '#!/bin/sh\nprintf "%s\\n" "$2"\n',
            "wine": '#!/bin/sh\nprintf "%s|%s\\n" "${CL-unset}" "${_CL_-unset}" > "$COMPILER_TEST_LOG"\n',
        }
        for name, text in scripts.items():
            path = commands / name
            path.write_text(text)
            path.chmod(0o755)
        self.environment = {
            **os.environ,
            "PATH": str(commands) + os.pathsep + os.environ["PATH"],
            "TH09_MSVC71_ROOT": str(self.tools),
            "COMPILER_TEST_LOG": str(self.log),
        }

    def run_probe(self) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            ["bash", str(self.wrapper), str(self.source), str(self.output), "/O2"],
            env=self.environment,
            capture_output=True,
            text=True,
        )

    def test_backend_drift_is_rejected_with_unchanged_driver(self) -> None:
        (self.binary / "c2.dll").write_bytes(b"different optimizer backend")
        result = self.run_probe()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("component hash mismatch: c2.dll", result.stderr)
        self.assertFalse(self.log.exists())
        self.assertFalse(self.output.parent.exists())

    def test_missing_cpp_frontend_is_rejected_before_wine(self) -> None:
        (self.binary / "c1xx.dll").unlink()
        result = self.run_probe()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("missing pinned VC7.1 component: c1xx.dll", result.stderr)
        self.assertFalse(self.log.exists())

    def test_missing_backend_pin_is_not_an_optional_check(self) -> None:
        lock = self.root / "config" / "tools.lock.toml"
        lock.write_text("\n".join(line for line in lock.read_text().splitlines()
                                  if not line.startswith("backend_sha256")) + "\n")
        result = self.run_probe()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("missing or invalid pinned VC7.1 component hash: c2.dll", result.stderr)
        self.assertFalse(self.log.exists())

    def test_explicit_profile_cannot_inherit_cl_option_variables(self) -> None:
        self.environment.update(CL="/O1", _CL_="/Ob2")
        result = self.run_probe()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(self.log.read_text(), "|\n")


if __name__ == "__main__":
    unittest.main()
