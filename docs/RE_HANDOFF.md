# TH09 agent handoff

This page is the **current** restart guide. Read [AGENTS.md](../AGENTS.md),
[workflow](RE_WORKFLOW.md), and [Oracle policy](ORACLES.md) before editing.
The [knowledge base](KNOWLEDGE_BASE.md) retains older experiments as **history**;
its intermediate code sizes and match counts must not override this page or
the canonical ledgers. Git holds prior source revisions.

## Verified state

- Target: original **Japanese TH09 v1.50a**, ignored `resources/th09.exe`;
  SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  PE: Windows i386, image base `0x00400000`. Do not modify the target.
- At this handoff, **1,025 of 1,037 source-present authored functions are
  byte-exact**; **12 functions / 28,145 original bytes** remain nonexact.
  These totals are generated from `config/functions.csv`, `config/matches.csv`
  and `config/match-units.toml`. Refresh them with
  `python3 -B scripts/report-reconstruction-status.py` and see
  [generated progress](PROGRESS.md).
- **GameplaySetupThread @ `0x0041AF2D` is now exact, not an open boss.**
  The canonical unit `game-manager-gameplay-setup-thread` reproduces all
  **1,689/1,689 bytes**, 172 actual original-bound relocation fields,
  35 ordered calls, 413 instructions, 78 control-flow blocks, and the
  original literal. Natural C++ switch case order `4,1,3,2,0` with mode 1's
  fixed-rate branch first resolved the old 3-byte near-jump mismatch.
  `4339d22e` records the source/match promotion; `a4dc6af5` adds strict
  byte/field/graph regression checks. Both GameManagerSetup compiler profiles
  cold-pass **12 exact units / 4,680 bytes / 415 fields**.
- **RunEcl now preserves native 32-bit overflow in nine integer opcodes.**
  The original-bound 15,564-byte interpreter still has only its existing
  30 differences (opcodes 4/155), with all 598 fields/tables intact. Six
  affected VC7.1 compiler carriers cold-pass **91 exact functions / 36,301
  bytes / 1,783 relocations**; eight changed internal compiler-label names
  were independently verified against original PE/actual COFF before rebinding.
  This is source-semantic progress, not a new exact function. See the
  2026-11-11 note in [knowledge](KNOWLEDGE_BASE.md).
- **Enemy death-reward leaf arithmetic also uses native 32-bit words.**
  All four reward interval/value/doubling paths now preserve original i386
  word-wrap behavior even for extreme negative levels. Its dedicated source
  carrier recompiled and canonical replay passed **1,142/1,142 exact bytes
  and 39/39 relocations**. No exact-count change; see the 2026-11-11
  [knowledge note](KNOWLEDGE_BASE.md).
- **Opcode 148 native GameManager counter semantics are restored.**
  RunEcl's 1,800-frame addition now explicitly wraps at 32 bits, matching
  the original direct DWORD `ADD` at `0x40B2F1`. Its complete owner still
  has only the 30 existing differences. Scoped replay passed 87 exact
  functions / 30,583 bytes / 1,409 relocations in five carriers; eight
  private labels were independently COFF/PE-attested before rebinding.
- **Replay LoadReplayData 32-bit wire arithmetic is now defined.**
  Its complete 591-byte exact body and all 14 relocations are unchanged.
  Both ReplayInput compiler carriers passed 12 exact owners / 4,093
  bytes / 232 fields. The related SaveReplay and BeginPlaybackStage
  frontiers remain at two differing bytes and two missing bytes,
  respectively; see the 2026-11-11 knowledge note.
- **EffectManager's native effect counters are preserved in source.**
  Eight operations across spawning, initialization and update now
  explicitly perform modulo-32 arithmetic. One affected compiler
  carrier cold-passed all 15 exact functions (2,530 bytes / 100 fields);
  the 51-byte AddedCallback still has only its prior eight register
  differences. See the 2026-11-11 knowledge note.
- **SaveReplay mode/user-info lengths now honor native DWORD overflow.**
  The three retail `ADD` operations at mode selection and user-info size
  construction use explicit unsigned 32-bit source arithmetic. The
  1,238-byte source candidate still differs only at `+0xB7/+0xBB`; both
  ReplayInput carriers pass 12 exact siblings (4,093 bytes, 232 fields),
  while BeginPlaybackStage keeps its old nonexact frontier.
- **RunEcl late Spell VM index arithmetic now matches native wraparound.**
  The three signed original fields are combined as unsigned 32-bit words,
  preserving the original signed ANM script argument. Only the two
  affected EclManager profiles were cold-rebuilt: 83 exact owners,
  29,856 bytes and 1,389 fields. The complete 15,564-byte interpreter
  still differs at only the existing opcode-4/155 bytes; twelve new
  opcode-155 variants were rejected for whole-owner regressions.
- **Network and reward side-index arithmetic now honors native i386 wrap.**
  Both network service side inversions and the reward leaf's opposing-side
  and effect-index arguments use explicit DWORD arithmetic. The network
  service remains nonexact at 1,627/1,633 candidate/target bytes, 106
  fields and 1,381 unlike overlaps; its current COFF body/call inventory
  is unchanged. The reward leaf cold-replayed 1,142/1,142 exact bytes,
  all 39 fields, with one actual compiler carrier. See the 2026-11-11
  knowledge note; no new exact function is credited.
- **Player bullet-cancel reward id uses native side-index arithmetic.**
  The still-open 862-byte collision owner retains its existing 849-byte
  candidate, all 26 fields, twelve calls and 33/32 target/candidate CFG
  blocks. Three affected VC7.1 profiles cold-pass five exact siblings
  (1,220 bytes / 32 fields); the 13 missing bytes remain unresolved.
- **EffectManager Create now uses original native side arithmetic.**
  The exact 327-byte factory retains all 27 relocation fields while
  reconstructing GameManager side/other-side addresses as original 0x38
  stride DWORD expressions and both chain priority adds as modulo-32
  words. One carrier cold-passes 15 exact siblings (2,530 bytes, 100
  fields). The source ABI guard still checks authentic GameManager
  addresses and rejects false relocated targets. The 51-byte callback
  remains nonexact by eight bytes; see the 2026-11-11 knowledge note.
- **ScreenEffect arcade pulse and alpha packing now use native DWORDs.**
  The repetition decrement and six alpha-color shifts retain the original
  physical Win32 results. Three affected VC7.1 profiles cold-pass all 18
  exact functions (2,562 bytes / 164 fields); nine private `$L` names
  were independently COFF/PE-proven before rebinding. The 336-byte shake
  envelope remains nonexact with its original 26 differences.
- **Title OnUpdateResult three frame counters now use native DWORD increments.**
  The nonexact 484/485-byte owner still has 34 fields, nine calls, 218
  unlike overlap bytes and one missing byte. Both affected compiler
  profiles passed 41 exact siblings (18,972 bytes, 953 fields); nine
  compiler-private labels were independently PE/COFF-proven before
  manifest rebinding. See the 2026-11-11 knowledge note.
- **EffectNativePoolCapacity now preserves original dynamic DWORD sums.**
  Four shared helper functions now wrap primary/secondary/sentinel count
  arithmetic at 32 bits. One actual carrier cold-passes 15 exact owners
  (2,530 bytes / 100 fields); AddedCallback retains eight differences.
- **RunEcl primary/alternate ANM sequence offsets now wrap natively.**
  Ten signed-wire `movementInt+1..5` arguments now use unsigned DWORD
  additions before passing signed script indices. RunEcl remains 15,564
  bytes/598 fields with only the same 30 opcode-4/155 differences.
  Affected-only cold replay passed 87 exact functions / 30,583 bytes /
  1,409 fields in five compiler carriers; eight private labels were
  independently verified against full PE/COFF evidence before rebinding.
- **Effect pool pointer addresses now use native 0xD8-stride DWORD math.**
  FailureSentinel, both spawners and the fixed-slot allocator form byte
  addresses from the original effect array base and wrapped count/index
  words, not out-of-bounds C++ pointer arithmetic. The one affected VC7.1
  carrier cold-passed 15 exact owners / 2,530 bytes / 100 fields;
  AddedCallback stays at its eight original differences.
- **Three Title character-cursor helpers now preserve native DWORD wrap.**
  Nine direction/bounds updates use explicit modulo-32 arithmetic with
  unchanged signed comparisons. Two affected compiler carriers pass 41
  exact siblings / 18,972 bytes / 953 fields; nine renamed internal labels
  were independently PE/COFF-proven before ledger rebinding. Nonexact
  Title OnUpdateResult retains its 484/485-byte residual.
- **The whole native Windows i386 product is not closed.** Original compiler
  project partition/linkage, some origins/data ownership and runtime behavior
  remain unproven. Exact source is not product or semantic/port completion.

## Remaining source-present nonexact owners

These are complete original PE extents, **not partial exactness credit**.
Recheck the chosen owner with its actual compiler and target-bound inspector.
Do not blindly replace a source with the adjacent-game reference version.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 bytes, 598 fields; 30 unlike | Opcode 4 timer register schedule (7), opcode 155 flag write (23). Includes original owned tables. `--owner runecl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 bytes, 97 fields, 69 calls; 13 unlike | Draw-index and spawn-descriptor register lifetime. `--owner enemy`. |
| **Player movement** | 0x0041C170 | 1,900/1,900 bytes, 66 fields, 23 calls; 122 unlike | Option/index scheduling and Float3 transport. `--owner movement`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210 bytes, 70 fields, 58 calls; 235 unlike | Mode byte and input receiver register lifetime; same 53-block CFG. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 484/485 bytes, 34 fields, 9 calls; 218 unlike overlaps, 1 missing | Character snapshot and result selection. `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280 bytes, 57 fields, 13 calls; 29 unlike | Selected record EDX/ECX scheduling. `--owner replaymenu`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 849/862 bytes, 26 fields, 12 calls; 799 unlike overlaps, 13 missing | Original has 33 CFG blocks versus 32 candidate; branch/layout and bullet receiver. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238 bytes, 53 fields, 23 calls; 2 unlike | First frame-header copy: destination EDI versus ECX at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538/540 bytes, 32 fields, 5 calls; 39 unlike overlaps, 2 missing | Seed load and input-delay tail. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336 bytes, 20 fields, 8 calls; 26 unlike | ESI/EDI timer/owner lifetime. `--owner screen`. |
| **Supervisor network service** | 0x00430AA0 | 1,627/1,633 bytes, 106 fields; 1,381 unlike overlaps, 6 missing | Waiting-block scheduling and queued frame packet. Use **`scripts/inspect-supervisor-service.py`** with its separately compiled `SupervisorNetwork.cpp` (the `--owner network` route tests other network/queue owners). |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51 bytes, 4 fields, 2 calls; 8 unlike | ANM manager/owner EDI/ESI scheduling; keep exact Create caller. `--owner effect`. |

## Resume with a bounded build

```bash
git status --short --branch       # inspect and resolve all dirty/untracked work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Complete nonexact owner inspection (builds only the relevant carrier):
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# For an already exact unit, rebuild its real compiler carrier and compare:
nice -n 12 python3 -B scripts/build-match-unit.py --unit game-manager-gameplay-setup-thread
python3 -B scripts/compare-coff-function.py --unit game-manager-gameplay-setup-thread --json

# After editing a source or included .inl/.hpp, replay only affected exact owners:
python3 -B scripts/replay-scoped-exact.py --changed-file src/GameManagerSetup.cpp --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/GameManagerSetup.cpp

# Fast housekeeping/ledger/docs checks; not a substitute for source cold replay:
python3 -B scripts/ci.py
```

`build/matching/` is a small generated object cache. It can be rebuilt with
`build-match-unit.py`. Historical `build/gpt-web/` trial objects, PDBs and
copied candidate sources were **deleted to save storage**; the maintained
source, canonical manifests, tests and key conclusions in the knowledge base
replace those outputs. New experiments may recreate their own scratch builds.
Do not use `--owner all`, the all-unit cold replay or `ci.py --full` for
routine work. Do not change VC7.1 flags to manufacture a match.

## Next-agent priorities and protected inputs

Start with **large bosses and small leaves together**. A high-leverage path is
RunEcl opcode 4/155 or Enemy's 13 register bytes, alternating with
SaveReplay's two bytes or Effect AddedCallback's eight. The collision owner
needs an actual 33rd CFG block, not a padding or `goto` size trick.
Previous semantic DWORD/byte-cursor fixes are committed; further equivalent
arithmetic spellings often preserve the same residual. Historical failed
forms and exact x86 evidence are in the relevant knowledge-base sections.

Preserve ignored `resources/th09.exe`, the pinned VC7.1/game-specific Wine
prefix under `.tools/wine/`, analyzer state in `.analysis/ghidra-direct/`,
shared tools and `_reference/` hypothesis source. `.analysis/gpt-web/`
contains compact private probe **scripts** that may assist further exploration;
old generated diagnostics were cleared. No push is implied by local commits;
recheck branch state rather than trusting the previous session's ahead count.
Use `gpt-web: <description>` for every substantive Git checkpoint.
