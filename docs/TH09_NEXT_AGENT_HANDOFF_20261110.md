# TH09 next-agent handoff — 2026-11-10

**This is the current restart guide.** Start in the registered live `th09`
worktree; do not use the November 9/8 handoffs or archived experiment notes as
snapshots of the current checkout. Read [AGENTS.md](../AGENTS.md), the
[live status/frontier](RE_HANDOFF.md), [workflow](RE_WORKFLOW.md), and
[original-bound Oracles](ORACLES.md). Check Git before editing: the latest
source checkpoint when this guide was prepared is **`746b0e6d`**; subsequent
handoff/cleanup commits can move `HEAD` without changing source.

## Original target, compiler and ledger

- Original Japanese **TH09 v1.50a**, ignored read-only
  `resources/th09.exe`, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Never substitute a localized/modified executable or patch target bytes.
- Reconstruct **natural C/C++ with pinned VC7.1** and the original unit's
  compiler profile. The ignored `.tools/wine/` runtime is needed to compile.
- Canonical tracked totals: **2,248 candidates; 1,037 source-present
  authored functions; 1,024 complete exact; 13 nonexact covering 29,834
  original bytes; 35 reviewed origin-unresolved cases.** Authored-byte
  exactness is **262,097 / 291,931 = 89.7805%**. These numbers describe
  the tracked inventory, not a proven complete executable inventory.
- Native Windows i386 product linking, runtime semantics and portability are
  **not** closed. Exact codegen and C++ source-equivalent but historically
  unclassified constructors are separate facts.

Reconfirm with live commands rather than copying totals from a dated note:

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
```

## Most recent source work: the actual shared Title owner

These source commits supersede the November 9 handoff's old `e7964962`
source-baseline claim. Each kept already-exact owners exact; **no new 1,024
exact-ledger credit** resulted from the native source-type improvements.

| Code commit | Changed C++ ownership | Whole original-bound result |
| --- | --- | --- |
| `f32ca36b` | `TitleDrawCallback` uses full `TitleScreenView`, four real draw methods and `AnmManager`'s +0x12880 texture cache | 329/329, 20 fields; 63 impacted Title/ANM owners exact |
| `096077ac` | `TitleCalcCallback` uses that same complete 0x1B388-byte Title, including 16-entry jump table | 328/328 physical bytes, 36 fields; 47 same-owner consumers exact |
| `6bba2cc7` | Nonexact Replay-menu draw shares one checked `Float3` text view and names the SDK displacement | **Still 1,280/1,280, 57 fields, 29 unlike bytes** |
| `746b0e6d` | Full Title music-room shares the constructed Title, 32 x 0x292 POD tracks and native ANM fields | 2,258/2,258 Update, 209/209 Draw; 49 impacted owners, 26,914 bytes/1,350 fields exact |

The original music-room 11 indexed global references **do require real COFF
addends**: seven warning pointers at +4 through +0x1C and four side-2
ReplayInput bytes at +0x122/+0x123. They were checked against actual
original PE DWORDs and are mutation-guarded. The main Title StartMenu,
TitleCalc and FrontMessage switch tables may acquire different VC7.1 `$L`
**names** when a shared header changes; only rebind after proving the same
COFF section-relative destination equals the immutable original PE target.
Do not treat such names as function identity or accept mismatching code.

Other recently recovered native groups: Front/FrontSide and Supervisor
rendering, Player death/reward/CardAttack/GameManager value +0xCC,
Player collision creators/allocator and shot/damage, Enemy append-collision
and attached-effects, RunEcl background spell entry, and AsciiManager boss
markers. Consult their dated `docs/TH09_*_20261109.md` or
`docs/TH09_*_20261110.md` notes for specific owner/field proofs.

## Unfinished large bosses and suggested next probes

The [live 13-owner frontier table](RE_HANDOFF.md#reconstruction-frontiers)
is the place to start; re-run the *chosen* owner before trusting a historical
residual count. Prioritize work that can actually change a complete function:

| Owner | Current verified boundary | Best remaining question |
| --- | --- | --- |
| `runecl` | 15,564 bytes / 598 fields / 30 unlike | Opcode 4 timer register transport and 155 flag write (7 + 23 bytes) |
| `enemy` | 3,900 / 97 / 13 unlike | Draw group EAX/ECX register and bullet-descriptor position scheduling |
| `movement` | 1,900 / 66 / 122 unlike | Position-history and direction/option lifetime; native focus Effect no longer needs a cast |
| `replaymenu` | 1,280 / 57 / 29 unlike | Selected ReplayData register lifetime; early index snapshots and pure Float3 operator shape regressed |
| `replaysave` | 1,238 / 53 / 2 unlike | Destination register EDI vs ECX at +0xB7/+0xBB |
| `effect` | 51 / 4 / 8 unlike | Original ANM manager receiver held in EDI; natural source expression selection unresolved |

Additional nonexact owners: ChargeAttack (1,210 bytes/235 unlike),
Title OnUpdateResult (485 original, 484 candidate), GameplaySetupThread
(1,689/1,682), Player CheckBulletCollision (862/849), Replay
BeginPlaybackStage (540/538), ScreenEffect shake (336/336, 26 unlike),
and Supervisor network update (1,633/1,627). **None is exact.** Do not
promote a near-match or local equal subrange into canonical exact credit.

The useful negative experiments are documented in dated source notes and
ignored `.analysis/` proof folders. Earlier notes sometimes record candidate
sizes and hypotheses **superseded** by later work (e.g. ChargeAttack
1,195/1,210 versus present 1,210/1,210); verify against current source.

## Fast, recoverable loop

```bash
# An exact source owner (recreate its .obj when build/ has been cleaned):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-music-room
python3 -B scripts/compare-coff-function.py --unit title-screen-music-room --json

# Still-nonexact whole-owner diagnostics (choose ONE, never --owner all):
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
# Other focused owners: runecl, movement, replaymenu, replaysave, effect
```

Compare the **entire original owned physical extent**, not only decoded
instructions: PE bytes, tails/tables, all COFF relocation offset/type,
root/addend/actual original target and external calls. Recheck impacted
exact siblings when changing a shared header. Experiment in copied source,
restore rejected candidates, and commit useful changes as
`gpt-web: <English subject>` after verification. **Do not run full cold
replay.** Inspect and recover any dirty/untracked work before starting.

## Local storage, cleanup and recoverability

`resources/th09.exe`, `.tools/wine/`, `.git/` and saved Ghidra/IDA databases
are protected inputs. `build/` and analysis compiler objects are **generated
caches**, not canonical proof; a handoff cleanup may remove them. Rebuild
only the selected source unit. Archive old trial **source/data** losslessly
with path, byte size and SHA-256 before removing loose copies. Never delete
`.analysis/` indiscriminately. The ignored archives and cleanup manifests
are local-only and **do not travel with a Git clone**. Exact deleted paths,
reclaimed storage and extraction instructions are recorded in
[artifact cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md).

Earlier source notes may refer to old `.analysis/` paths that are now inside
an archive. Check the cleanup history before concluding that their evidence
was lost. Keep accepted original-bound ledgers, scripts, tracked source and
tests untouched by storage cleanup.
