# TH09 reconstruction — next-agent handoff

**Start here.** Read [AGENTS.md](../AGENTS.md), the
[bounded workflow](RE_WORKFLOW.md) and [Oracle rules](ORACLES.md).
The [knowledge base](KNOWLEDGE_BASE.md) records target layouts and useful
failed approaches; old trial results are history, not current codegen proof.
Git retains earlier source revisions.

## Verified baseline and boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, base
  `0x00400000`, ignored `resources/th09.exe`, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Never patch or delete it.
- **Last verified 2026-11-11:** **1,025 / 1,037** source-present functions
  byte-exact; **12 nonexact / 28,145 original bytes**. Another 35 candidate
  origins remain reviewed but unresolved; inventory completeness is unproven.
  These are *snapshots*, not a new agent's live proof: regenerate totals with
  `python3 -B scripts/report-reconstruction-status.py`. Canonical authority
  is `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml`, not manually copied status prose.
- **Closed large owners:** `GameplaySetupThread @ 0x0041AF2D` is exact
  (1,689 bytes / 172 relocations). `Title DrawResult` is also exact
  (1,939 bytes); do **not** confuse it with nonexact `OnUpdateResult`.
- **Phase:** source exactness remains open. No complete Windows i386 link
  and runtime closure, dual-Oracle semantic baseline or port is proven.
- **Current source:** native Win32 32-bit address math and word-wrap
  semantics have been recovered across Enemy/ECL, Player/collision, Replay,
  Title, Effect, Sound, ANM, ExAttack, and Supervisor frame queues.
  Replay BeginRecordingStage now also uses native signed-stage strides for
  three frame-pointer rows, the FPS row and inline 0x18-byte buffer links;
  the two Replay focus loops now address their original 0x8E-byte side-state
  records by native DWORD side strides. EffectManager's nine indexed
  template fields now use the real table's native 0x18-byte DWORD stride;
  its 15 exact owners (2,530 bytes / 100 fields) remain cold-verified.
  ScreenEffect's shake side/camera indices now use native 0x38/0xF0-byte
  strides; all 18 same-source exact owners (2,562 bytes / 164 fields) pass,
  with nine verified VC7.1 private-label name rebindings in RegisterChain.
  The 1,323-byte Replay stage initializer and all 12 ReplayInput exact units
  (4,093 bytes / 232 fields) also remain verified.
  Replay capture now reads its original config focus byte through a native
  side offset; Replay Release now addresses real 0x18-byte stage links,
  3x10 frame pointers and FPS pointer slots by unsigned DWORD strides.
  The two affected ReplayInput carriers still protect 12/12 exact units
  (4,093 bytes / 232 fields), with both original-bound open replay guards
  checked and unchanged. Playback's prior-stage
  character marker byte and SaveReplay's compressed checksum stream now
  use genuine Win32 DWORD byte-address reads, while both complete nonexact
  Replay owner guards retain their previous 39 overlapping playback differences and 2 SaveReplay register bytes. The exact 151-byte Enemy
  attached-effect updater now computes the real 24 pointer slots by native
  DWORD addressing; both affected ECL compiler carriers protect 83 exact
  units (29,856 bytes / 1,389 fields) and preserve both large Enemy/ECL
  nonexact frontiers. The already-exact 1,758-byte Enemy draw renderer now
  uses native 0x1C-byte sample addresses and wrapped trail/vertex index
  arithmetic; its four same-source exact owners (1,922 bytes / 61 fields)
  remain verified. Enemy OnUpdate now shares the native ECL secondary VM,
  life-threshold and child-pointer slot lvalues at eight read/write sites;
  the 3,900-byte boss remains at 13 unlike bytes and the same 83 ECL exact
  siblings remain verified. Title OnUpdateResult now also reads the
  selected character-order byte through its native 32-bit table address;
  its 484/485-byte frontier remains open, while all 41 same-source exact
  Title owners (18,972 bytes / 953 fields) remain verified. Seven
  score-name table lvalues in two Title update owners now share real
  0x44C/0xDC/0x2C native bank/row/record strides; those 41 exact Title
  functions still pass their two-carrier scoped cold replay. Title replay
  metadata, path and allocation indices now use the real 0x1EC/0x200/4-byte
  record strides; the exact 1,387-byte replay-menu update and 1,825-byte
  replay-save update remain byte-exact across those same 41 Title owners.
  The standalone Player movement boss now advances its real `0x2F4`-byte option records and
  `0x0C`-byte position-history slots through native DWORD addresses; its
  full 1,900-byte, 66-field owner guard still reports just 122 differences.
  ANM file-loading now computes 33 sprite/texture/script/file-slot addresses
  from the actual 0x44/0x0C/4/0x120-byte record bases and signed indices;
  nine format/pixel-stride table reads also use real native DWORD addresses;
  both affected compiler carriers preserve 17 exact ANM owners (3,258 full
  bytes / 133 fields). The 9,776-byte exact ANM script interpreter also now
  computes its six instruction-stream jumps/advances as native 32-bit byte
  addresses; all five affected exact owners (10,496 full bytes / 421 fields)
  pass two-carrier scoped cold replay with 148 independently verified private
  `$L` reference rebindings. RunEcl's polar/bias movement helpers now form
  16 real four-byte wire-argument accesses through native DWORD addresses;
  the laser-creation opcode also writes its original 32-slot pointer table
  through the native signed selected-slot address. The full 15,564-byte
  RunEcl frontier remains at 30 unlike bytes, while the latter change's
  five involved carriers protect 87/87 exact functions (30,583 full bytes /
  1,409 relocations) with eight fresh independently verified private labels.
  ANM D3D capture now computes 17 surface, descriptor and file-slot accesses
  through authentic 0x04/0x14/0x120-byte native strides; all eight affected
  AnmManagerUtility owners (1,723 bytes / 49 fields) cold-replay exact.
  Etama controller registration and sprite selection now address the real
  0x38-byte GameManager sides and four live sprite-offset DWORD table entries;
  three affected BulletManager carriers retain 18 exact functions (6,711
  bytes / 259 fields), with 16 original-target-verified private labels.
  `GetAnm` and `SpriteHasTexture` now address the same real 0x120-byte ANM
  resource slots by native DWORD stride; 12 exact functions across three
  compiler carriers (603 bytes / 26 fields) remain verified. Etama's six
  draw heads and 23 active 0xD48-byte sprite templates now use native
  index addressing, as do seven live script reads from the real 23x5
  0x14-byte sprite-script rows (column two via an original element const
  reference). The 3 same-source exact draw/added owners (1,664 bytes /
  73 fields) remain exact; 11 further private-label bindings were verified.
  Front's 553-byte exact `InitializeResultBonus` now uses native
  0x38-byte GameManager side addresses for 12 live Player/result reads,
  including final AddScore (553 bytes / 27 relocation fields).
  Background stage camera interpolation now uses real 0x04-byte
  duration/mode and 0x0C-byte ZunTimer slot strides for ten live reads
  and writes; its 2,652-byte stage-script boss and four exact siblings
  remain cold-verified (3,231 complete bytes / 160 relocation fields)
  with 55 original-target-verified compiler-private label references.
  The already-exact 1,436-byte ExAttack Type18/24 boss now uses native
  0x0C-byte position, 4-byte angle and 0x2A4-byte VM lvalue addresses;
  its one-carrier 1,436-byte / 44-relocation exact replay remains verified.
  The separate exact Type4 attack callback now also forms live 0x0C-byte
  trail, 0x2A4-byte VM and 0x38-byte opponent-side addresses, preserving its
  739 complete bytes and 28 original-bound relocations. Type3's matching
  32-record history and live VM/side selections are native too, with its
  780-byte exact body and 38 relocations still verified. The separately
  exact Type14/22 update now forms native 0x294-byte fan and 0x84-byte
  random-row addresses plus five 0x38-byte gameplay-side lvalues; its
  824-byte complete owner and 44 fields pass scoped replay with five
  original-target-verified private labels. FrontSide's exact
  2,381-byte OnUpdate also computes its dynamic 0x2A4-byte HUD VM records
  natively; its create/callback side and auxiliary path indices join a
  one-carrier 15/15 exact replay (4,087 bytes / 176 relocation fields).
  `Background::RenderObjects` now reads actual stage-object pointer slots,
  0x2A4-byte VM entries and 0x0C-byte effect-point records by native
  DWORD index addressing; its full 3,274-byte/113-field exact owner passes.
  The 3,094-byte exact `FrontSide::OnDraw` now advances eleven real VM
  position cursors by native 0x2A4-byte address words, and both viewport
  side slots use the Supervisor 0xF0-byte stride (209 bound fields).
  The 357-byte exact FrontMessage Setup leaf now also reads four text
  and shadow color values through real native DWORD table addresses,
  retaining all 10 relocations. The separate 1,491-byte exact FrontCalc
  callback now uses original 0x64-byte panel rows/0x14-byte vertices,
  DWORD timer/color/result slots and 0x38-byte opponent-side addresses;
  all 107 original relocations remain exact. The full 3,238-byte exact Player pattern
  selector now uses native word addresses for 38 flag/grid/ReplayInput/
  config/history accesses; two impacted compiler carriers protect both
  exact functions (3,650 full bytes / 154 fields), including 18 independently
  verified private-label rebindings in ResolvePatternOffset.
  The 209-byte DrawMusicRoom leaf now reads/writes the selected actual
  0x2A4-byte song VM through native DWORD addresses; the same-source
  2,258-byte OnUpdateMusicRoom remains exact, with 2/2 owners and
  2,467 full bytes / 80 fields scoped cold-verified. The 2,045-byte exact
  Title options updater now uses native help VM, selected menu VM and
  global help-text pointer addresses; both its source owners remain exact
  (2,079 complete bytes / 138 original relocation fields).
  The distinct exact 2,803-byte Title key-config updater now also uses
  native help-text/VM byte bases and eight real 0x2A4-byte paired digit
  advances; its two same-source owners remain byte exact (2,909 bytes /
  118 original-bound relocation fields).
  The 2,968-byte exact FrontMessage opcode interpreter now reads 20 packed
  halfwords, two live wire bytes and five third-input-state references
  through native DWORD addresses; two affected compiler carriers preserve
  both exact source functions (3,007 bytes / 167 fields), with 30
  PE-target-proven private/internal label rebindings.
  The already-exact 1,939-byte Title DrawResult owner now uses native
  0x44C-byte score banks, 0xDC-byte difficulty rows, 0x2C-byte rank
  records and genuine label/alphabet indices; all 82 relocation fields
  remain exact under its single-carrier replay.
  These patches preserved exact siblings but **did not** promote any of the twelve
  open functions. Use `git log --oneline` for the actual commits; do not
  restore scratch files as if they were accepted changes.

The complete ASCII pause-menu and popup source now also uses true
0x2A4-byte VM and 0x38-byte score-popup/side native byte addresses.
Its one-carrier scoped replay verifies both large 1,776/1,172-byte
menu updates plus eight exact siblings: **10/10 owners, 4,206 bytes,
264 relocations**, with 19 COFF-proven compiler-private label rebindings.

The exact SoundPlayer queue processor now indexes all original audio
queue/pan/buffer/volume and BGM row/format storage with native unsigned
DWORD words. The full 1,456-byte `ProcessQueues` physical owner and
all its 58 fields matched; eight affected compiler carriers preserve
**20/20 exact SoundPlayer functions (5,400 bytes / 184 relocation fields)**,
with eight PE-target-verified private-label rebindings.

The `Ending::RunEndingScript` interpreter also now advances its live
0x2A4-byte ANM VM records and 68-byte text buffer by native DWORD
addresses. Its **1,727-byte complete original PE owner / 88 fields**
remains exact, as do all nine affected exact siblings across two
compiler carriers (**10/10 functions, 3,015 bytes / 159 fields**), with
31 COFF/PE-target-proven private label rebindings.

## Remaining source-present nonexact owners

Each row describes the **complete original PE owner**, not partial exact
credit. These are the last reviewed source frontiers; recompile the chosen
owner after editing it. `--owner` routes refer to
`scripts/check-boss-frontiers.py` unless another inspector is specified.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 bytes, 598 fields; 30 unlike | Opcode 4 timer register scheduling (7), opcode 155 flag write (23). Original owned tables intact. `--owner runecl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900 bytes, 97 fields, 69 calls; 13 unlike | Draw-index and bullet-descriptor register lifetime. `--owner enemy`. |
| **Player movement** | 0x0041C170 | 1,900/1,900 bytes, 66 fields, 23 calls; 122 unlike | Option/index scheduling and Float3/history transport. `--owner movement`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210 bytes, 70 fields, 58 calls; 235 unlike | Mode byte and input receiver; 53-block CFG retained. `--owner charge`. |
| **Title OnUpdateResult** | 0x004266B5 | 484/485 bytes, 34 fields, 9 calls; 218 unlike overlaps, 1 missing | Character byte load/sign extension. **Use `inspect-title-update-result.py`**, not `--owner resultdraw` (which checks the already-exact DrawResult). |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280 bytes, 57 fields, 13 calls; 29 unlike | Selected record EDX/ECX lifetime; 19 list/path/stage-frame reads now use native 32-bit strides. `--owner replaymenu`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 849/862 bytes, 26 fields, 12 calls; 799 unlike overlaps, 13 missing | Original 33 CFG blocks vs candidate 32; needs a real branch. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238 bytes, 53 fields, 23 calls; 2 unlike | First frame-header copy uses EDI vs ECX at +0xB7/+0xBB. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538/540 bytes, 32 fields, 5 calls; 39 unlike overlaps, 2 missing | Seed-load/input-delay tail. `--owner replay`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336 bytes, 20 fields, 8 calls; 26 unlike | ESI/EDI timer/owner lifetime. `--owner screen`. |
| **Supervisor network service** | 0x00430AA0 | 1,627/1,633 bytes, 106 fields; 1,381 unlike overlaps, 6 missing | Waiting/prediction block layout. Compile `SupervisorNetwork.cpp` and use `inspect-supervisor-service.py`; `--owner network` tests other owners. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51 bytes, 4 fields, 2 calls; 8 unlike | ANM manager/owner EDI/ESI schedule. Keep exact Create caller. `--owner effect`. |

## Resume with only affected compiler carriers

```bash
git status --short --branch         # recover all staged/dirty/untracked work
git log -6 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# One original-bound nonexact owner (not a whole-game replay):
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# Only exact siblings affected by an edited .cpp/.inl/.hpp:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl

# One canonical exact owner, only when relevant:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# Correct Title OnUpdateResult path (resultdraw is DIFFERENT):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# Network service: standalone source, no exact siblings in that .cpp:
# The generated build/matching directory may be absent after cleanup.
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

# Documentation/ledger checks (no game compiler required):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
git diff --check
```

The scoped exact replay **does not** accept a nonexact boss; run its
complete original-bound inspector separately. Some standalone nonexact
sources have no canonical exact unit and correctly cannot run scoped
exact replay. Exact acceptance covers **complete physical code, tails,
owned switch tables and all original-bound relocations**, not a partial
code slice or a similarity score. Do not use `--owner all`, an all-source
cold replay or `ci.py --full` for routine iteration. Do not change the
pinned VC7.1 profile or add inert register hints to force a match.

## Best next questions; avoid repeating failed experiments

Work on large bosses **and** small leaves. Previous changes to natural
32-bit addresses and signed wrap are already included in maintained source.

- **RunEcl:** opcode 4 has seven register bytes; opcode 155's byte-to-bit-24
  XOR/write has 23. Fields/tables are already right. Pointer aliases,
  bitfields, union operands, timer receivers and source-order variations
  generally preserved 30 differences or broke table targets. See the
  [opcode notes](KNOWLEDGE_BASE.md); don't replay cosmetic rewrites.
- **Enemy:** 13 specific register bytes remain in draw-group and
  spawn-descriptor construction. Avoid improving a local register at
  the expense of original literal relocations.
- **SaveReplay:** only +0xB7/+0xBB differ; frame-header constness,
  natural dest arithmetic, typed copies and moving link initialization
  have not solved the EDI/ECX choice. Preserve the 23 calls.
- **Player collision:** one genuine original CFG block and 13 bytes
  are missing; manufactured `goto`, `ret` or padding is not recovery.
- **Effect callback:** eight receiver/manager register bytes remain;
  pointer/ref/inline wrappers seldom change them. Its Create caller
  remains exact.
- **Supervisor network service:** the retail owner has 1,633 bytes versus
  the 1,627-byte candidate, with 106 relocations and 41 direct calls but a
  different wait/success block schedule. Reversing branches or sharing an
  exit did not restore the original layout; do not mistake fewer overlapping
  differences in a longer candidate for a strict improvement.
- **Title OnUpdateResult / DrawReplayMenu:** distinguish them from the exact
  DrawResult leaf. The result updater needs its actual signed character-byte
  read and one missing byte; replay menu has 29 register-lifetime differences.
  Pure pointer casts and cached source-index snapshots have not fixed either.

The pinned neighboring `_reference/Cirno-Labs-th09`, TH08 and TH095
code are hypothesis sources, **not** original TH09 evidence.

## Protected inputs and scratch lifecycle

The obsolete ignored `build/` cache was removed for this handoff,
including `build/gpt-web/` trial copies/PDBs/diagnostics and
`build/matching/` compiled objects. **The absence of an object is expected**:
use `scripts/build-match-unit.py --unit <unit-id>` for a selected exact
owner or `scripts/replay-scoped-exact.py --changed-file <source>` for its
*affected* exact cohorts. For a standalone nonexact probe, recreate
`build/matching/` and use `scripts/compile-probe.sh` with the pinned profile
above. Do not run project-wide `--owner all` or cold replay to rebuild caches.
Removing generated objects does not revoke existing ledger matches, nor does
an old receipt prove the current source without recompilation.

**Preserve** ignored `resources/th09.exe`, `.tools/wine/` (the game-specific
VC7.1/Wine runtime and prefix), `.analysis/ghidra-direct/` (saved Ghidra
projects), `.analysis/factory-runtime/`, shared Factory tools and
`_reference/`. The six small ignored
`.analysis/gpt-web/20261111-prove-*.py` scripts are retained *only in this
live worktree* as optional private-label same-section PE/COFF proof helpers;
they are not canonical evidence and require fresh selected objects first.
Do not delete these protected inputs to save space, or commit originals,
SDKs, executable assets or analysis databases. No remote push was requested.
See [tool routing](TOOLS.md) and [shared runtimes](SHARED_TOOLS.md).

Commit real improvements as `gpt-web: ...`. Always inspect and recover
dirty/untracked work instead of deleting it. Check live Git ahead/behind
status; do not carry forward an old count.
