# ECL and Front indicator review, 2026-11-05

Started from clean `2483e40d`. Original Japanese 1.50a target, tracking and
status checks pass. Fresh direct Ghidra exports attest disk/Program SHA,
i386, base/entry and all 685056 mapped file bytes for RunEcl, Enemy update,
the Front indicator setter and Front AddedCallback.

## Adopted native indicator storage

Front's indicator setter now uses the shared 0x2A4-byte AnmVm, removing its
separate padded VM and four-byte color classes. Original writes identify
alpha at VM+0x1F3 and position at VM+0x208. The two VMs are at Front+0x123F4
and +0x12698, the same objects initialized by AddedCallback and consumed by
DrawCallback. Compiler assertions retain those field offsets and stride.
AddedCallback now clears the native alpha fields directly, removing two raw
byte-offset casts.

Fresh canonical comparisons retain both complete owners: indicator setter
213 bytes/10 fields and AddedCallback 706 bytes/37 fields, all exact. Both
actual ECL source profiles are rebuilt; all 81 canonical units retain 27120
physical bytes/1232 fields. The complete Enemy owner remains3900/97/13;
RunEcl remains15564/598/30, with seven ordinary differences in opcode4 and
23 in opcode155 and no field/table differences. No exact counter increases.

Two private labels change names only. Their actual same-section COFF
definitions and immutable original operand encodings prove the destinations:
movement +139 still targets408548; comparison +14 still targets407A4C.
Every canonical owner is checked before the manifest spelling is updated.

## Fresh complete-owner controls

All controls use frozen current source inputs and the existing /O2 /Ob1
profile, serial lowered-priority bounded compiles. No register hints, new
assembly, volatile forcing or compiler/profile search is introduced.

| Control | Complete observation |
| --- | --- |
| Current baseline | RunEcl15564/598/30; Enemy3900/97/13 |
| Native unsigned timeout bitfield | RunEcl1938 ordinary/202 field/26 table differences |
| DWORD bitfield type; signed timeout bit | Same regression as native unsigned |
| Promoted wire, direct byte/XOR, explicit old/result words, byte reference | RunEcl2048 ordinary/242 field/26 table differences |
| Explicit timer operator call, shared scalar, typed jump arguments | RunEcl42 ordinary differences; fields/tables unchanged |
| SDK or D3DVECTOR descriptor assignment | Enemy remains13; RunEcl remains30 |
| Actual Timer assignment body visible | RunEcl remains30; Enemy remains13 |
| Actual assignment and SetCurrent bodies visible | RunEcl15560 bytes/598 fields; complete-extent guard rejects |
| Actual bullet descriptor constructor visible | RunEcl30; Enemy13 |
| Constructor plus real periodic timer body visible | RunEcl30; Enemy45 |
| Typed union wire arguments, with or without direct native assignments | RunEcl15640 bytes/606 fields; complete coverage guard rejects |

None of these controls is adopted. Rejection is bounded compiler evidence,
not proof that other natural source hypotheses cannot work. The real compiler
pin is build13.10.3077 in tools.lock.toml; the reference repository's SP1
description does not describe our installed Oracle.

## Reproduction and evidence

Build only the affected carriers:

```bash
python3 -B scripts/build-match-unit.py --unit front-enemy-indicator
python3 -B scripts/compare-coff-function.py --unit front-enemy-indicator --json
python3 -B scripts/build-match-unit.py --unit front-added-callback
python3 -B scripts/compare-coff-function.py --unit front-added-callback --json
python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/build-match-unit.py --unit player-position-callback-type11
```

The ignored `.analysis/ecl-frontier-20261105/` retains frozen inputs/hashes,
changed controls, recipes, Ghidra, complete reports and definition-backed
label proof. `verify-native.py` compares all81 ECL units using the two fresh
objects, plus the full Enemy/RunEcl diagnostics and indicator setter.
The separate AddedCallback receipt is `front-added-native.json`.
Generated objects are below `build/analysis/ecl-frontier-20261105/` and
`build/matching/`. Only SHA-identical per-case source/header duplicates are
removed; the shared frozen inputs and every changed source remain, with the
cleanup manifest. Historical evidence and game/tool inputs are untouched.

All16 Enemy binding tests and seven Front Added tests pass. Tracking,
documentation, progress and whitespace checks pass. This source/layout
checkpoint makes no new Factory, remote, native-product or runtime claim.
