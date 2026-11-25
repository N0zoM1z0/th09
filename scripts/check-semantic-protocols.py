#!/usr/bin/env python3
"""Fail when closed TH09 semantic protocol surfaces regain raw magic literals."""

from __future__ import annotations

from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
SOURCE_SUFFIXES = {".cpp", ".hpp", ".inl"}
INTEGER_LITERAL = r"[+-]?(?:0[xX][0-9A-Fa-f]+|[0-9]+)(?:[uUlL]+)?"


def strip_comments(text: str) -> str:
    def blank(match: re.Match[str]) -> str:
        return "".join("\n" if ch == "\n" else " " for ch in match.group(0))
    return re.sub(r"//[^\n]*|/\*.*?\*/", blank, text, flags=re.DOTALL)


def fail(message: str) -> None:
    print(f"error: {message}", file=sys.stderr)
    raise SystemExit(1)


def source_files() -> list[Path]:
    return sorted(
        path
        for path in SRC.rglob("*")
        if path.is_file() and path.suffix.lower() in SOURCE_SUFFIXES
    )


def reject_pattern(paths: list[Path], label: str, pattern: str) -> None:
    matcher = re.compile(pattern, re.MULTILINE | re.DOTALL)
    hits: list[str] = []
    for path in paths:
        code = strip_comments(path.read_text(encoding="utf-8", errors="replace"))
        for match in matcher.finditer(code):
            line = code.count("\n", 0, match.start()) + 1
            relative = path.relative_to(ROOT).as_posix()
            spelling = " ".join(match.group(0).split())
            hits.append(f"{relative}:{line}: {spelling[:120]}")
    if hits:
        fail(f"{label}:\n  " + "\n  ".join(hits))


def explicit_enum(
    path: Path, enum_name: str, prefix: str, expected_values: list[int]
) -> list[str]:
    text = strip_comments(path.read_text(encoding="utf-8"))
    match = re.search(rf"enum\s+{enum_name}\s*\{{(.*?)\}};", text, re.DOTALL)
    if match is None:
        fail(f"could not find enum {enum_name} in {path.relative_to(ROOT)}")

    entries = re.findall(
        rf"\b({prefix}[A-Z0-9_]+)\s*=\s*([+-]?(?:0[xX][0-9A-Fa-f]+|\d+))",
        match.group(1),
    )
    names = [name for name, _ in entries]
    values = [int(value, 0) for _, value in entries]
    if sorted(values) != sorted(expected_values) or len(entries) != len(expected_values):
        expected = ", ".join(str(v) for v in sorted(expected_values))
        fail(f"{enum_name} must contain one explicit, unique entry for: {expected}")
    if len(set(names)) != len(names):
        fail(f"{enum_name} contains a duplicate name")
    return names


def check_ecl_opcode_protocol() -> None:
    explicit_enum(
        SRC / "EclOpcodes.hpp",
        "Th09EclOpcode",
        "TH09_ECL_OPCODE_",
        list(range(1, 188)),
    )


def check_ecl_operand_protocol() -> None:
    expected = [*range(0x2710, 0x2770), 0x2775]
    names = explicit_enum(
        SRC / "EclOperandIds.hpp",
        "Th09EclOperandId",
        "TH09_ECL_OPERAND_",
        expected,
    )
    path = SRC / "EclOperands.cpp"
    reject_pattern(
        [path],
        "numeric ECL operand dispatch label",
        r"\bcase\s+(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )
    code = strip_comments(path.read_text(encoding="utf-8"))
    dispatched = set(re.findall(r"\bcase\s+(TH09_ECL_OPERAND_[A-Z0-9_]+)\s*:", code))
    if dispatched != set(names):
        missing = sorted(set(names) - dispatched)
        extra = sorted(dispatched - set(names))
        fail(f"EclOperands.cpp selector mismatch (missing={missing}, extra={extra})")


def check_background_protocol() -> None:
    opcode_names = explicit_enum(
        SRC / "Background.hpp",
        "BackgroundStageOpcode",
        "BACKGROUND_OPCODE_",
        list(range(35)),
    )
    explicit_enum(
        SRC / "Background.hpp",
        "BackgroundInterpolationMode",
        "BACKGROUND_INTERPOLATION_",
        list(range(8)),
    )
    explicit_enum(
        SRC / "Background.hpp",
        "BackgroundCameraMotionMode",
        "BACKGROUND_CAMERA_MOTION_",
        list(range(5)),
    )
    path = SRC / "BackgroundStageScript.cpp"
    reject_pattern(
        [path],
        "numeric BackgroundStageScript dispatch label",
        r"\bcase\s+(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )
    code = strip_comments(path.read_text(encoding="utf-8"))
    dispatched = re.findall(r"\bcase\s+(BACKGROUND_OPCODE_[A-Z0-9_]+)\s*:", code)
    if len(dispatched) != 35 or set(dispatched) != set(opcode_names):
        fail("BackgroundStageScript.cpp must dispatch every named opcode once")


def check_front_message_protocol() -> None:
    expected = [
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x17, 0x18, 0x19, 0x1A, 0x1C,
    ]
    names = explicit_enum(
        SRC / "FrontMessageTableView.hpp",
        "FrontMessageOpcode",
        "FRONT_MESSAGE_OPCODE_",
        expected,
    )
    path = SRC / "FrontMessageUpdate.cpp"
    reject_pattern(
        [path],
        "numeric FrontMessage dispatch label",
        r"\bcase\s+(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )
    code = strip_comments(path.read_text(encoding="utf-8"))
    dispatched = re.findall(r"\bcase\s+(FRONT_MESSAGE_OPCODE_[A-Z0-9_]+)\s*:", code)
    if len(dispatched) != len(expected) or set(dispatched) != set(names):
        fail("FrontMessageUpdate.cpp must dispatch every named opcode once")


def check_enemy_timeline_protocol() -> None:
    expected = [0, 1, 2, 3, 4, 5, 8, 10, 11, 12, 13, 14, 15, 16, 17]
    path = SRC / "EnemyScheduleRuntimeAdvance.cpp"
    names = explicit_enum(path, "EnemyTimelineOpcode", "ENEMY_TIMELINE_OPCODE_", expected)
    reject_pattern(
        [path],
        "numeric EnemyTimeline dispatch label",
        r"\bcase\s+(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )
    code = strip_comments(path.read_text(encoding="utf-8"))
    dispatched = re.findall(r"\bcase\s+(ENEMY_TIMELINE_OPCODE_[A-Z0-9_]+)\s*:", code)
    if len(dispatched) != len(expected) or set(dispatched) != set(names):
        fail("EnemyScheduleRuntimeAdvance.cpp must dispatch every named opcode once")


def check_supervisor_and_title_protocols() -> None:
    explicit_enum(
        SRC / "SupervisorStates.hpp",
        "SupervisorState",
        "SUPERVISOR_STATE_",
        [-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13],
    )
    reject_pattern(
        [SRC / "SupervisorUpdate.cpp"],
        "numeric SupervisorUpdate dispatch label",
        r"\bcase\s+[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleScreenId",
        "TITLE_SCREEN_",
        list(range(1, 17)),
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleMatchSubmode",
        "TITLE_MATCH_MODE_",
        list(range(5)),
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleStartMenuItem",
        "TITLE_START_MENU_",
        list(range(8)),
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleOptionsScreenState",
        "TITLE_OPTIONS_STATE_",
        [0, 1],
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleOptionsMenuItem",
        "TITLE_OPTIONS_ITEM_",
        list(range(9)),
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleKeyConfigScreenState",
        "TITLE_KEYCONFIG_STATE_",
        [0, 1],
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleKeyConfigMenuItem",
        "TITLE_KEYCONFIG_ITEM_",
        list(range(10)),
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleRegistrationContext",
        "TITLE_ENTRY_",
        [0, 1],
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleSubscreenState",
        "TITLE_SUBSCREEN_STATE_",
        [0, 1, 2, 3],
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleReplayMenuState",
        "TITLE_REPLAY_MENU_",
        [0, 1, 2],
    )
    explicit_enum(
        SRC / "TitleScreenStates.hpp",
        "TitleReplaySaveState",
        "TITLE_REPLAY_SAVE_",
        list(range(5)),
    )
    reject_pattern(
        [
            SRC / "TitleCalcCallback.cpp",
            SRC / "TitleDrawCallback.cpp",
            SRC / "TitleScreen.cpp",
            SRC / "TitleScreenOptions.cpp",
            SRC / "TitleScreenKeyConfig.cpp",
        ],
        "numeric Title callback/menu dispatch label",
        r"\bcase\s+[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )


def check_gameplay_state_protocols() -> None:
    explicit_enum(
        SRC / "GameDifficulty.hpp",
        "GameDifficulty",
        "DIFFICULTY_",
        list(range(5)),
    )
    explicit_enum(
        SRC / "PlayerDirection.hpp",
        "PlayerMoveDirection",
        "PLAYER_DIR_",
        list(range(9)),
    )
    reject_pattern(
        [
            SRC / "GameManagerSetup.cpp",
            SRC / "PlayerGameplayState.cpp",
            SRC / "PlayerMovement.cpp",
            SRC / "PlayerRuntime.cpp",
            SRC / "PlayerAddedState.cpp",
            SRC / "PlayerOwnerStateReward.cpp",
            SRC / "EclPostRuntime.inl",
            SRC / "AsciiManagerMenu.cpp",
            SRC / "AsciiMenuState5Update.cpp",
            SRC / "SampleNetworkInput.cpp",
            SRC / "AnmManagerRuntime.cpp",
        ],
        "numeric gameplay/menu state-machine dispatch label",
        r"\bcase\s+[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\s*:",
    )


def check_sound_protocol(paths: list[Path]) -> None:
    explicit_enum(
        SRC / "SoundIds.hpp",
        "SoundIdx",
        "SOUND_",
        list(range(54)),
    )
    reject_pattern(
        paths,
        "numeric fixed sound ID",
        r"\b(?:PlaySound(?:Positioned)?ByIdx|PlayMenuSound)\s*\(\s*"
        + INTEGER_LITERAL
        + r"\s*,",
    )


def check_effect_protocol(paths: list[Path]) -> None:
    explicit_enum(
        SRC / "EffectIds.hpp",
        "Th09EffectId",
        "TH09_EFFECT_",
        [5, 7, 9, 13, 14, 15, 19, 34, 35],
    )
    reject_pattern(
        paths,
        "numeric fixed effect ID",
        r"\bSpawnEffect(?:WithVelocity|InFixedSlot)?\s*\(\s*"
        + INTEGER_LITERAL
        + r"\s*,",
    )


def main() -> int:
    paths = source_files()
    check_ecl_opcode_protocol()
    check_ecl_operand_protocol()
    check_background_protocol()
    check_front_message_protocol()
    check_enemy_timeline_protocol()
    check_supervisor_and_title_protocols()
    check_gameplay_state_protocols()
    check_sound_protocol(paths)
    check_effect_protocol(paths)

    print("TH09 semantic protocol checks passed")
    print("  ECL opcodes: 187/187 explicit")
    print("  ECL operand selectors: 97/97 named (0 numeric cases)")
    print("  Background opcodes/interpolation/camera modes: 35/8/5 named")
    print("  FrontMessage opcodes: 24/24 named")
    print("  Enemy timeline opcodes: 15/15 named")
    print("  Supervisor, Title & gameplay state-machine dispatches: named (0 numeric cases)")
    print("  SoundIdx (0..53) & fixed PlaySound*/PlayMenuSound calls: 0 numeric literals")
    print("  Th09EffectId & fixed SpawnEffect* calls: 0 numeric literals")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
