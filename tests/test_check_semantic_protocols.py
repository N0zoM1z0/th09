import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "scripts" / "check-semantic-protocols.py"


class CheckSemanticProtocolsTests(unittest.TestCase):
    def test_reject_pattern_matches_numeric_sound_and_ignores_comments(self):
        import importlib.util

        spec = importlib.util.spec_from_file_location("check_semantic_protocols", SCRIPT)
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            mod.ROOT = root
            good = root / "good.cpp"
            good.write_text(
                "// PlaySoundByIdx(10, 0)\n"
                "/* PlayMenuSound(12, 0) */\n"
                "g_SoundPlayer.PlaySoundByIdx(SOUND_SELECT, 0);\n",
                encoding="utf-8",
            )
            mod.reject_pattern(
                [good],
                "numeric fixed sound ID",
                r"\b(?:PlaySound(?:Positioned)?ByIdx|PlayMenuSound)\s*\(\s*"
                + mod.INTEGER_LITERAL
                + r"\s*,",
            )

            bad = root / "bad.cpp"
            bad.write_text("g_SoundPlayer.PlaySoundByIdx(10, 0);\n", encoding="utf-8")
            with self.assertRaises(SystemExit) as cm:
                mod.reject_pattern(
                    [bad],
                    "numeric fixed sound ID",
                    r"\b(?:PlaySound(?:Positioned)?ByIdx|PlayMenuSound)\s*\(\s*"
                    + mod.INTEGER_LITERAL
                    + r"\s*,",
                )
            self.assertEqual(cm.exception.code, 1)

    def test_explicit_enum_rejects_missing_value(self):
        import importlib.util

        spec = importlib.util.spec_from_file_location("check_semantic_protocols", SCRIPT)
        mod = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(mod)

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            mod.ROOT = root
            hdr = root / "proto.hpp"
            hdr.write_text("enum Demo { DEMO_A = 0, DEMO_B = 1 };\n", encoding="utf-8")
            names = mod.explicit_enum(hdr, "Demo", "DEMO_", [0, 1])
            self.assertEqual(names, ["DEMO_A", "DEMO_B"])
            with self.assertRaises(SystemExit):
                mod.explicit_enum(hdr, "Demo", "DEMO_", [0, 1, 2])


if __name__ == "__main__":
    unittest.main()
