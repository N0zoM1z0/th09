# Native source review, 2026-11-05

This review starts at `0a775b76`, on main, with 1019 exact /16 nonexact.
Historical negative results are hypotheses to recheck, not exclusions.
The original Japanese v1.50a target and tracking pass before this work.
Fresh Ghidra exports attest all685056 mapped file bytes before reviewing
SaveReplay, its constructor and both MusicRoom owners, plus ReplayMenu.
The first export's427EE8 selector was not the MusicRoom owner; the corrected
MusicRoom entries426E05 and4244A9 are exported separately. Ghidra names and
bodies remain provisional; complete PE/COFF extents govern codegen checks.

## MusicRoom: native storage and input, no extra exact credit

The two complete functions remain exact in the existing O1/Ob1 profile:
OnUpdateMusicRoom2258 bytes/74 fields and DrawMusicRoom209/6. Both replay from
the same fresh object, including literal payloads, without label rebinding.

Changes use the established shared AnmVm: direct VM pointers, named position,
color, drawEnabled and anchor fields. They remove the padded private VM,
24 conversion casts, a second three-float projection and a raw color-offset
helper. DrawMusicRoom saves/restores the existing Float3 and color members.
The unchanged original instructions also validate the 0x2A4 stride and offsets.

Five source input predicates now use ReplayInputState slot2 word06, replacing
the independent32-bit global alias. Four physical operand sites remain fixed:
+339 and+3DC access4ACF3A; +356 and+394 access4ACF3B. The new native symbol root
is4ACE18 and the actual COFF addends are122/123. Size8E and word offset6 are
compiler-checked. No operand location or effective original address changes.
This is a truthful source-symbol correction, not solving new destinations
from candidate bytes or assigning new data ownership.

Full local public CI passes1330 tests with29 skips. Tracking, documentation,
progress and whitespace checks pass. No function counter increases.

## Fresh retries of remaining owners

All controls compile complete current source units serially with the existing
manifest profiles. Native implementation bodies are included as actual source;
no inert clobbers, artificial returns, barriers or flag search are added.

| Current-source control | Complete result |
| --- | --- |
| SaveReplay baseline | 1238 bytes/53 fields, three unequal bytes |
| Actual record/payload constructors visible | Same1238/53/3 |
| Full GameConfiguration and record construction chain visible | Same1238/53/3 |
| Full construction chain and actual Lzss encoder visible | Same1238/53/3 |
| ReplayMenu baseline | 1280 bytes/57 fields/13 calls/72 blocks,29 unequal bytes |
| Actual SDK vectors and arithmetic | Same1280/57/13/72/29 |
| SDK arithmetic plus one shared formatting buffer | Same1280/57/13/72/29 |
| Direct conditional varargs instead of scalar character/name temporaries | Same1280/57/13/72/29 |
| Function-scope workspace and direct timer/member expressions | Same1280/57/13/72/29 |
| Snapshot selected index once before deriving its record pointer | 1264/1280 bytes,858 unequal overlaps and16 missing; rejected |

All six actual constructor contributions in the tested SaveReplay contexts
are present as18-byte record and15-byte payload bodies with one field each;
merely providing the bodies still does not remove the three caller residuals.
Their unresolved original origin receives no new authored credit. Visibility
of the remaining configuration/codec bodies is not proof those callees are
exact under this caller profile. The unchanged caller comparison grants none.

The ReplayMenu SDK candidates retain exactly the baseline's unequal offsets;
we compare full raw bytes and actual COFF identities before considering any
source simplification. The snapshot counterexample shows why a plausible
high-level rewrite must still pass the complete comparison. Repeated numeric
results here are newly compiled observations at this HEAD, not borrowed old
receipts. A failed experiment does not exhaust other natural source models.

## Reproduction and local evidence

```bash
python3 -B scripts/build-match-unit.py --unit title-screen-music-room
python3 -B scripts/compare-coff-function.py --unit title-screen-music-room --json
python3 -B scripts/compare-coff-function.py --unit title-screen-draw-music-room --json
```

The target-bound diagnostics, actual input-operand proofs, fresh source controls,
compiler logs and Ghidra exports are retained under
`.analysis/native-source-review-20261105/`; objects are under
`build/analysis/native-review/`, with the accepted MusicRoom object under
`build/matching/`. The retained control recipes are `probes.py` and
`menu-followup.py`. Existing unchanged callee source/profile proofs are reused.
The reference inspiration remains [Cirno-Labs/th09](https://github.com/Cirno-Labs/th09)
at d7653152becabb8c86f2f43d49015ecc11bd0652. These tests do not establish a
whole Windows product, runtime closure or unique original C++ declarations.
