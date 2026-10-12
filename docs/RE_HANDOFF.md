# TH09 reconstruction handoff

> **Current-state document, not a session journal.** Refresh the live ledgers
> and object files before new reconstruction work. Historical hypotheses,
> rejected trials and previous snapshots remain in `docs/KNOWLEDGE_BASE.md`
> and Git history (including `git show a02528d:docs/RE_HANDOFF.md`). Do not
> mistake a previous commit's byte counts for the present working tree.

## Operator instructions and identity

Continue original Japanese **TH09 v1.50a** reconstruction, attacking large
boss functions **and** short leaves, with RunEcl, Enemy/Etama, TitleScreen and
ExAttack the main priorities. Use **`gpt-web: ...`** for each new local commit;
commit substantive, verified progress. This is a single-writer worktree:
**inspect and finish dirty/untracked changes**, not avoid or reset them.
Factory's repository shell has no network push transport. Never claim a local
commit is a remote push.

Target: ignored `resources/th09.exe`, PE32 i386, image base `0x00400000`,
entry `0x0047D45F`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The registered Factory provider is `th09-ida`, called as `get_metadata({})`;
its identity attestation is **not** a codegen acceptance. Follow
[`AGENTS.md`](../AGENTS.md), [`RE_WORKFLOW.md`](RE_WORKFLOW.md), and
[`ORACLES.md`](ORACLES.md) for source/profile and phase-gate rules.

## Live ledger snapshot

Checked against `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv` and `config/match-units.toml` by
`python3 scripts/validate-docs.py`; run a fresh status report after any edits.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 932 |
| Source-present non-exact functions | 48 |
| Source-present non-exact bytes | 52,526 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 223,244 |

**223,244 / 275,770 reviewed-authored bytes = 80.9530% exact**.
To reach 95% of this currently classified denominator, another **38,738
bytes** must be exact. The 35 origin-unresolved functions are excluded from
that denominator and could alter it after review. Complete faithful Windows
i386 product/link/runtime closure is **open**; semantic reconstruction and
portability have **not started**. Source presence does not imply exact codegen;
exact codegen does not imply a runnable game.

## Active frontiers

These are **selected** source-present non-exact functions, **not** a complete
48-item inventory or a ranking by difficulty. Size is the target logical
extent unless marked *physical*. Measurements are tied to the maintained
candidate/profile and require fresh compilation after source edits. Consult
`config/functions.csv` and the relevant knowledge-base evidence first.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| RunEcl | 0x004086C0 | **15,564 physical** = 14,792 code + 772 table; 598 fields; **30** ordinary byte differences, zero field/table differences | 146/148 handler shapes; opcode 4 (timer argument, 7 bytes) and opcode 155 (flags, 23 bytes); `scripts/inspect-ecl-complete.py` |
| EnemyManager OnUpdate | 0x00410730 | **3,900 physical**, 97 fields, **39** differences | Four decoded register/copy windows; `scripts/inspect-enemy-manager-update.py --explain` |
| ExAttack Type19/21 initializers | 0x0044A030 / 0x0044B220 | 763 / 727 bytes; 57 / 53 fields; **6 / 9** differences | Vertex/history-cursor scheduling; `scripts/inspect-exattack-trail-init.py` |
| ExAttack Type14/22 Draw | 0x00444130 | 959 target / 974 candidate; 53 fields, 20 direct + 12 indirect calls | Record/Extra lifetime, loop scheduling; `scripts/inspect-exattack-type14-22-draw.py`, **NON-EXACT** |
| ExAttack Type19 Update | 0x0044A330 | 821 bytes, 35 fields, **198** differences | Player-slot and record register residency; `scripts/inspect-exattack-type19.py` |
| ExAttack Type3 Update | 0x00442750 | 780 bytes, 38 fields, **15** differences | `middle.z` store / tail-X call-argument order |
| Etama OnDraw | 0x00413BE0 | 600 target / 594 candidate; 27 candidate fields | Laser body VM base, cap and record cursors |
| Title DrawResult | 0x00423D16 | 1,939 bytes, 82 fields, **22** differences | Score-bank pointer / screen-state read schedule |
| Title DrawCallback | 0x00424898 | 329 target / 328 candidate, 20 fields | Single missing target `DEC EAX` at `+0x101`; `scripts/inspect-title-draw-callback.py` |
| Title Options | 0x004276EB | 2,045 target / 2,048 candidate; 135 fields | Configuration/reference lifetimes |
| Controller input | 0x0042B410 | 808 target / 796 candidate; 32 fields | WinMM X/Y register and read ordering; `scripts/inspect-controller-input.py` |
| Effect AddedCallback | 0x0040D1C0 | 51 bytes; 4 fields; **8** differing bytes | Original direct global-to-EDI load; `scripts/inspect-effect-added.py` |
| Enemy collision-box leaf | 0x00410250 | 82 bytes, 2 direct calls, **31** differing bytes | Float3 return-copy schedule; `scripts/inspect-enemy-collision-box.py` |
| Gameplay setup worker | 0x0041AF2D | 1,689 bytes; 172 target / 171 candidate fields | Reuse-base caching, flags, tail |
| GameManager OnUpdate | 0x0041AA5F | 1,230 bytes, 87 fields, 105 differences | Input snapshot and entry-zero schedule |
| Player movement | 0x0041C170 | 1,835 authored / 1,900 physical; 66 fields | Position clamp and register allocation |
| Supervisor network service | 0x00430AA0 | 1,633 target / 1,632 candidate | Sign test and output workspace |

Other unresolved owners (including PauseMenu, Replay, network and remaining
Title helpers) live in `config/functions.csv`. The exact short-function subset
is maintained separately in [`SMALL_FUNCTION_FRONTIER.md`](SMALL_FUNCTION_FRONTIER.md).

## Closed codegen owners and accepted receipts

- **`EnemyManagerDrawImpl @ 0x00411670`**: **1,758 / 1,758 complete bytes,
  53 actual relocations**; pinned match unit `enemy-manager-draw-impl`.
  Its two draw wrappers and interpolation leaf also remain exact. A real
  `float` interpolation result, `long double` delta, and explicit float
  argument conversion resolved the final x87 FSUBR/PUSH scheduling. Factory
  **pass/accepted** receipt `receipt:44cacda296eb6f3a51916daa49d7a31f9fba8d6c1b8400983bfa3c93d1fead8f`
  is bound to source commit `480f2028f82b790a080be6f09994736286d0bc95`.
- **`EtamaController::OnUpdate`**, **`TitleScreenView::OnUpdateMusicRoom`**,
  **`TitleScreenView::TitleSetupThread`** and **`ExAttackUpdateCallbackType21`**
  are *already* canonical exact. Four accepted cold replays of these existing
  owners belong to source commit `f6aa0e960900e90b314b0fb0d77aa3d820b5ffe5`;
  they are not four new matches.
- **`EffectManager::OnUpdate`**: **475 authored bytes exact**, with a separate
  Factory pass/accepted receipt
  `receipt:9b01a390ab05d73f20cdde28aeed7c67123343bafefd730b004e74db52eb34dd`
  bound to source commit `5dcd7f2`. The adjacent **Effect AddedCallback is
  still non-exact**; the receipt does not apply to it.

Factory receipts are **source-commit scoped**. An old accepted job cannot
prove exactness for a changed compiler-input tree, a different owner or the
whole product. The canonical exact ledger is local and must always be
replayable independently.

## Restart and verification

Read the AGENT rules, active ledgers and dirty/untracked files **before**
mutations. Through Factory/Web call `th09-ida.get_metadata` with exactly `{}`;
its passed target attestation includes mapped-byte sampling. The host-only
`scripts/check-ida-mcp.py` is for **local direct IDA**, not Factory shell.

```bash
git status --short --branch
git diff && git diff --cached
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/build-match-unit.py --unit enemy-manager-draw-impl
python3 scripts/compare-coff-function.py --unit enemy-manager-draw-impl --json
python3 scripts/build-match-unit.py --unit exattack-type21-update
python3 scripts/compare-coff-function.py --unit exattack-type21-update --json
python3 scripts/inspect-ecl-complete.py build/matching/EclManager.obj
python3 scripts/report-ecl-handler-shapes.py --explain all build/matching/EclManager.obj
python3 scripts/validate-docs.py
python3 scripts/progress.py --check
python3 scripts/ci.py
git diff --check
```

The **RunEcl** complete inspector returns **1 for known differences**; always
read the full result and ensure identity/relocation/CFG checks passed. Other
near-match diagnostics also return nonzero until truly exact. Build an object
from the **maintained source and the unit's actual VC7.1 flags** before using
any cached `.obj`; ignored `build/` or `.analysis/` outputs are not proof.
Do not run the whole historical cohort unnecessarily. Preserve codegen exact
units when cleaning, and never alter source just to manipulate comparisons.

## Evidence and cleanup boundaries

`docs/KNOWLEDGE_BASE.md` is the historical, source-context-bound evidence
index. Its old **931/49** and **Enemy draw 1,758/4-difference** statements
are historical, *not* the current state. Only the live tracking CSVs and
fresh cold comparisons determine current status. `docs/RE_HANDOFF.md` is
**replace-in-place**, not an append-only dated journal. The old extended
handoff (including dated `gpt-web` sessions) remains recoverable with
`git show a02528d:docs/RE_HANDOFF.md`.

Scratch in `.analysis/` and object caches in `build/` are ignored and
non-authoritative. Keep compact results and trial recipes for open frontiers.
This handoff reviewed **19 named `gpt-web-*` trial directories** and deleted
**491 regenerable `.obj` plus 508 `.pdb` files (90,504,679 bytes)** from their
top level only. The audited path list, classifications and surviving recipes
are in ignored `.analysis/gpt-web-handoff-clean-20261012/cleanup.json`.
**Retained:** all candidate `.cpp`, probe `.py`, result JSON/TXT and copied
`src/` trees, all other `.analysis/` directories and all `build/` files.
To reproduce a deleted candidate, rerun that directory's recorded `probe.py`
with the pinned compiler and compare against the verified original; ignored
object outputs are not canonical evidence. Remaining scratch, private games,
provider databases, Wine prefixes and toolchains were **not** bulk-deleted.
