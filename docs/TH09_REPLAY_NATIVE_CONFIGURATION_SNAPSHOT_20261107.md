# TH09 bidirectional native GameConfiguration snapshot — 2026-11-07

The original target is Japanese TH09 1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a source-level reconstruction of the **genuine 0xCC-byte native
configuration object**, not a change to the format, allocated memory, or
replay behavior. Original historical C++ spelling is not established.

## Two directions, one owned type

`ReplayDataView` physically includes a `GameConfiguration configSnapshot` at
`+0xDC`; `AsciiGameManagerView` physically holds the active
`GameConfiguration *configurationE8` at `+0xE8`. The exact `GameConfiguration`
compiler layout is **0xCC bytes**, including the 0x6C-byte controller binding
prefix. The exact 1,323-byte `BeginRecordingStage` and open 540-byte
`BeginPlaybackStage` had two independent raw memory-copy expressions:

| Event | Recovered actual native C++ operation |
| --- | --- |
| BeginRecordingStage | `replayData->configSnapshot = *g_GameManager.configurationE8` |
| BeginPlaybackStage | `*g_GameManager.configurationE8 = replayManager->replayData->configSnapshot` |

These now copy the actual same type in opposite directions using ordinary
C++ struct value assignment instead of describing one direction with a
magic `0xCC` byte count. A compile-time structural assertion binds the
serialized size, the live pointer target type, and the original `+0xDC`
wire location. The configuration fields, replay-data pointers and original
setup/seed call order are unchanged. The compiler emits the same 51-DWORD
copy code for these genuinely owned structs.

## Deliberately exhaustive 540-byte tail experiments

The independent original PE/COFF comparison shows exactly why
`BeginPlaybackStage` is **not** exact. Retail TH09 zero-extends the `stageSeed`
through `xor edx, edx; mov dx, [eax+4]`; the current VC7.1 code uses
`movzx eax, word ptr [eax+4]`, with two fewer bytes and shifted later
restore/store scheduling. These are compiler/operand observations, not proof
of an original variable declaration. Twenty-one source-cold models of seed
locals, integer widths, references, API receivers and chronology retained
the 538-byte nonexact baseline or broke previously matching code. Eleven
more natural typed `GameConfiguration` copies and aliases were compiled and
compared over the entire owner: all preserved **538/540 bytes, 32 field
identities, five ordered calls, 12 graph blocks, 39 unlike overlaps and two
missing target bytes**. No register-forcing hint or length padding was
introduced to imitate the original instructions.

Four isolated paired configurations (baseline, recording-only assignment,
playback-only assignment, both assignments) compile with the original pinned
no-EH and EH VC7.1 profiles. In each case, the **12 exact ReplayInput source
units** remain completely original-equal. The selected version was adopted
only after a fresh `--owner replay` scoped replay independently verified its
15 exact native dependencies (**4,141 bytes / 234 relocations**) and the
complete open playback owner. The separate `--owner replaysave` replay checked
all 12 exact siblings (**4,093 bytes / 232 relocations**) and the full
1,238-byte open SaveReplay, which still has only `+0xB7/+0xBB` unlike bytes.

All lexical source/headers, compiler commands, object hashes, full original
field positions and failed experimental variants are retained as **ignored
private evidence** under `.analysis/gptweb-playback-seed-source-20261107/`
and `.analysis/gptweb-replay-config-snapshot-20261107/`. The PE and canonical
original target remain unchanged. No new exact function credit is claimed;
tracking stays 1,034 source-present / 1,021 exact / 13 nonexact. No full cold
replay, whole Windows build, runtime/port Oracle, Factory acceptance or Git
push is claimed.
