"""TH09 original physical 0x1EC replay schema shared by four source owners.

These are target-independent source/schema and canonical manifest guards.
The Japanese PE/COFF byte test and compiler-private relocations are separately
replayed from the committed worktree under authentic VC7.1 profiles.
"""

import csv
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "src/ReplayDataOriginalView.hpp"


def manifest_units():
    with (ROOT / "config/match-units.toml").open("rb") as source:
        return tomllib.load(source)["units"]


class SharedReplayDataPhysicalLayout(TestCase):
    def test_all_four_translation_units_use_one_reviewed_record(self):
        consumers = {
            "src/ReplayInput.cpp": "ReplayDataView",
            "src/TitleScreen.cpp": "ReplayDataView replays[50]",
            "src/TitleScreenReplaySaveDraw.cpp": "ReplayDataView replays[50]",
            "src/TitleScreenReplayMenuDraw.cpp": "ReplayDataView replays[50]",
        }
        for filename, expected in consumers.items():
            with self.subTest(filename=filename):
                body = (ROOT / filename).read_text()
                self.assertIn('#include "ReplayDataOriginalView.hpp"', body)
                self.assertIn(expected, body)
                self.assertNotIn("struct ReplayDataView", body)
                self.assertNotIn("struct ReplayFrameDataStartView", body)
                self.assertNotIn("struct ReplaySaveReplayDataView", body)
                self.assertNotIn("struct ReplayMenuReplayView", body)
                self.assertNotIn("struct ReplayMenuFrameView", body)

    def test_real_replay_frame_and_record_have_exact_original_offsets(self):
        text = HEADER.read_text()
        self.assertIn("struct ReplayFrameDataStartView", text)
        self.assertIn("struct ReplayDataView", text)
        self.assertIn("ReplayDataView();", text)
        self.assertIn("ReplayFrameDataStartView *frameStart[3][10];", text)
        self.assertIn("char playTime[10];", text)
        self.assertIn("char playerName[8];", text)
        offsets = {
            "ReplayFrameDataStartSizeIs20": "0x20",
            "ReplayDataSizeIs1EC": "0x1EC",
            "ReplayDataFrame0At20": "0x20",
            "ReplayDataFrame1At48": "0x48",
            "ReplayDataFrame2At70": "0x70",
            "ReplayDataFpsAt98": "0x98",
            "ReplayDataConfigAtDC": "0xDC",
            "ReplayDataVersionAt1DC": "0x1DC",
            "ReplayDataPlayTimeAtC4": "0xC4",
            "ReplayDataPlayerNameAtCE": "0xCE",
            "ReplayDataDifficultyAtD7": "0xD7",
            "ReplayDataReplayModeAt1E4": "0x1E4",
            "ReplayDataVersusModeAt1E5": "0x1E5",
        }
        for name, address in offsets.items():
            with self.subTest(field=name):
                self.assertIn("typedef char " + name, text)
                self.assertIn("== " + address + ")", text[text.index("typedef char " + name):][:205])
        self.assertIn("ReplayDataFrameCharacterAt06", text)
        self.assertIn("offsetof(ReplayFrameDataStartView, value06) == 6", text)

    def test_two_nonexact_title_bosses_access_identical_playtime_and_frame_schema(self):
        save = (ROOT / "src/TitleScreenReplaySaveDraw.cpp").read_text()
        menu = (ROOT / "src/TitleScreenReplayMenuDraw.cpp").read_text()
        self.assertIn("ReplayDataView *replay = replays;", save)
        self.assertIn("replay->magic != 0;", save)
        self.assertIn("valid ? replay->playTime", save)
        self.assertIn("valid ? replay->playerName", save)
        self.assertIn("ReplayDataView *replay = &replays[i];", menu)
        self.assertIn("ReplayDataView *replay = &replays[selected];", menu)
        self.assertIn("replay->frameStart[0][i]->value06", menu)
        self.assertIn("replay->frameStart[1][i]->value06", menu)
        self.assertIn("replay->value0D7", menu)
        self.assertIn("replays[selected].value1E4", menu)
        self.assertIn("replay->value1E5", menu)
        for false_owner in ("ReplayMenuReplayView", "ReplaySaveReplayDataView",
                            "ReplayMenuFrameView"):
            self.assertNotIn(false_owner, save + menu)

    def test_all_existing_50_exact_replay_and_title_units_stay_tracked(self):
        units = manifest_units()
        items = {name: entry for name, entry in units.items()
                 if entry.get("source") in
                 ("src/ReplayInput.cpp", "src/TitleScreen.cpp")}
        self.assertEqual(len(items), 50)
        self.assertEqual(sum(x.get("compare_size", x["size"]) for x in items.values()), 22504)
        self.assertEqual(sum(len(x["relocations"]) for x in items.values()), 1175)
        self.assertEqual(items["title-screen-setup-thread"]["size"], 534)
        self.assertEqual(items["replay-manager-begin-recording-stage"]["size"], 1323)
        self.assertEqual(items["title-screen-start-menu"]["size"], 1560)

    def test_compiler_private_labels_cannot_redirect_start_menu(self):
        units = manifest_units()
        owner = units["title-screen-start-menu"]
        self.assertEqual(owner["target_address"], 0x00429E23)
        expected = {
            962: 0x0042A43B,
            1560: 0x0042A1E9,
            1564: 0x0042A208,
            1568: 0x0042A23C,
            1572: 0x0042A286,
            1576: 0x0042A2D2,
            1580: 0x0042A293,
            1584: 0x0042A302,
            1588: 0x0042A380,
        }
        observed = {
            row["offset"]: row
            for row in owner["relocations"]
            if row["symbol"].startswith("$L")
        }
        self.assertEqual(set(observed), set(expected))
        for offset, address in expected.items():
            with self.subTest(offset=hex(offset)):
                self.assertEqual(observed[offset]["target"], address)
                self.assertEqual(observed[offset]["type"], "DIR32")

    def test_nonexact_renderer_boundaries_are_not_promoted_by_type_unification(self):
        with (ROOT / "config/functions.csv").open(newline="") as stream:
            entries = {int(row["address"], 16): row for row in csv.DictReader(stream)}
        for address, size, owner in (
                (0x004234F6, 1280, "TitleScreenView::DrawReplayMenu"),
                (0x004239F6, 800, "TitleScreenView::DrawReplaySave")):
            with self.subTest(owner=owner):
                self.assertEqual(entries[address]["proposed_name"], owner)
                self.assertEqual(int(entries[address]["size"]), size)
                self.assertNotEqual(entries[address]["status"], "matching")
        exact = ROOT / "scripts/check-boss-frontiers.py"
        self.assertIn("guard_title_replay_save(replay)", exact.read_text())
