# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at c75bc39. Target SHA, direct IDA metadata, entry
and five mapped-byte samples pass. One newly bound replay-save renderer baseline
and five depth-stage controls precede one batch-end cold replay. All controls
remain nonexact; weighted value delays multiplication and raises x87 depth.
Complete fields/CFG/collateral and focused text Oracles pass. No game source,
header, profile, match or exact-credit changes. Prior corrections stay.

## Live ledger snapshot

These totals are checked against the live ledgers by scripts/validate-docs.py.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,192 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 980 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 980 |
| Canonical exact functions | 928 |
| Source-present non-exact functions | 52 |
| Source-present non-exact bytes | 57,269 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 218,501 |

The fixed authored denominator is **275,770 bytes**: **79.2331%** exact.
Another **43,481 bytes** are needed for 95%. Origin-unresolved candidates are
separate. Faithful Windows i386 product/runtime gates remain open;
semantic reconstruction and portability have not started.

## Active frontiers

- RunEcl: 14,792 target code /15,564 physical bytes; candidate has
  **14,791 code + one alignment + 772 compiler-table bytes**. All 598 fields
  are independently checked, with 2,161 complete differences. Handler frontiers
  remain 4/7/86/155/156/157; scope controls recorded in the KB are rejected.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 fields, 39 differences;
  early draw index, descriptor/effect scheduling, trail and homing.
- Gameplay setup worker: 1,689 bytes, 171 fields, 921 full differences; reuse-base
  caching, flags cursor and rate/failure-tail topology remain open.
- GameManager update: 1,230 bytes, 87 fields, 105 differences; input capture and
  entry-zero scheduling, plus six shifted early-return branch displacements.
- Title Options: 2,045 target /2,048 candidate bytes, 135 fields, 610 linked
  overlap differences plus three excess; 51 calls and 121 direct blocks agree.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields, 144 differences.
- Player charge: 1,210 target /1,197 candidate bytes, 70 fields, 1,160 linked
  overlap differences plus 13 absent. Integer conversion preserves seven regions
  and 58 ordered calls; entry register allocation and byte-mode storage are open.
- Enemy draw: 1,758 bytes, four differences around the second subtraction/Abs.
- Type21 update: 1,074 bytes, 29 fields, 15 ring-preheader differences; actual
  descriptor member storage is maintained. Entry-owner-path controls are neutral.
- Remaining owners, including ExAttack18/24, PauseMenu and DirectPlay, are routed
  by config/functions.csv and docs/KNOWLEDGE_BASE.md. Read prior controls first.

## Latest replay-save depth batch

Packet: `.analysis/gpt-6.1-sol-replay-depth-workspace-20261009/`.
The inherited source/COFF-bound candidate remains
`build/gpt-dots-replay-renderer-residuals-20261006/inplace-target-depth.obj`, SHA256
`2c480b9a519f26a2d0dbc7ac4031dc7e89bc9a9a52594d37458ed684f8fa23c1`.
A fresh pinned carrier repeats its complete five owners/834 bytes/41 fields.
Middle/final/both workspace depth and weighted-depth value/reference controls
retain the four calls and full28-block graph but stay794/797/797/796/794 bytes
against800. The797-byte models add a field write; the796-byte value model delays
the first multiplication and raises x87 depth to5 versus target4. One cold
value compile agrees. See KB for full unmasked scores and exact exclusions.
All four collateral bodies/40 bytes/zero fields and nineteen other sections
remain unchanged. Existing text Oracles pass130/130 and55/55 after rebuilding
only their absent canonical cache. Twenty-one terminal files/527644 bytes are
removed; all six source recipes recover after cleanup. Full compact evidence
stays below150 KB. Game source and all exactness claims remain unchanged.
Next rotate to Supervisor::OnUpdate at431110 (844 code +152 table bytes),
reviewing the complete target and prior post-COM sharing negative first.

## Other current evidence

- RunEcl jump/timer controls:
  `.analysis/gpt-6.1-sol-ecl-jump-timer-20261009/`; all five contexts are neutral
  and one cold family replay agrees. All23 exact siblings pass; the folded
  canonical timer-current diagnostic binding and repaired ECL caches stay.
  Six handler frontiers remain open; see KB for exact excluded contexts.
- Enemy draw expression/declaration controls:
  `.analysis/gpt-6.1-sol-enemy-draw-expression-20261009/`; four main differences
  remain. Three controls are neutral; borrowed next-angle reference regresses.
  Exact scope, cold replay and cleanup are recorded in the KB.
- Enemy update object copies: `.analysis/gpt-6.1-sol-enemy-object-copies-20261009/`;
  typed Player +0x3037C Enemy pointer stays maintained. Current canonical cache
  `build/matching/EnemyManagerCore.obj` SHA256
  `60a054387f8a7125ec31eb44184e0bd6ed8ec711a949d61b9c29beae366aa1f9`.
  Seven controls and one canonical cold retain 39 differences; see KB exclusions.
- Type14/22 draw: `.analysis/gpt-6.1-sol-draw1422-callee-context-20261009/`;
  canonical math declaration and timer conversion stay maintained. Current cache
  `build/matching/ExAttackDrawType14Type22.obj` SHA256
  `cf6d50537f2768a58a4b480072929db81feaa806bdb760addb9f8d02e5fc724f`.
  Twelve controls and one canonical cold reject normalization-call loss; see KB.

- Player collision batch: `.analysis/gpt-6.1-sol-collision-member-visibility-20261009/`;
  actual difficulty +0x11C and coordinate +0x358 connections remain maintained.
  Eight controls precede one canonical cold replay; six existing siblings pass.
  The 849/862-byte owner has 32/33 blocks, so its frontier includes loop layout.
  Current object SHA remains
  `c9ea2f6617302b613b16c66ec53f512df3a0a9ae07c8a0679fd83cdc886240ee`.

- PauseMenu batch: `.analysis/gpt-6.1-sol-pause-partial-visibility-20261009/`;
  five actual +0x13C member connections remain maintained. Thirteen controls and
  the single canonical cold replay are documented in the KB; eleven exact
  siblings pass. Current object hash remains
  `1145e31c07bebac38e69639a835721fbdcb155761ba0251b492acdc03135f788`.

- Enemy lifetime batch: `.analysis/gpt-6.1-sol-enemy-scratch-lifetime-20261009/`;
  four controls are neutral, hoisting all four vectors regresses. One batch-end
  cold compile agrees; precise scopes are excluded by the KB and retained proof.
- Gameplay helper visibility: `.analysis/gpt-6.1-sol-gameplay-helper-visibility-20261009/`;
  all four complete workers remain 1,689 bytes /171 fields /921 differences.
  Actual declaration/implementation context and omitted helpers are rejected;
  all present exact siblings pass. See KB for exact controls and retained recipes.
- GameManager update arrays: `.analysis/gpt-6.1-sol-setup-input-array-20261009/`;
  explicit publication is neutral, two-side loop regresses; see KB.
- Latest flat ECL scope controls: `.analysis/gpt-6.1-sol-ecl-flat-local-scope-20261009/`;
  all 67 owners and 23 exact siblings are neutral. See KB for precise exclusions.
- Prior ECL mutation controls and code-extent correction:
  `.analysis/gpt-6.1-sol-ecl-mutation-expressions-20261009/`. Its precise direct
  timeout/comma-jump/opcode-snapshot controls are neutral. Decoder tests remain.
- Type21 canonical-only proof checks all 23 ECL exact siblings and the actual
  67-owner carrier. No need to rebuild unchanged historical cohorts.
- Charge conversion: `.analysis/gpt-6.1-sol-charge-timer-conversion-20261009/`;
  current carrier `build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj`,
  SHA256 `4744bb7c52cce4a50b55aa7162578ce79f9cf613b56e9e454f0cc3bb1df4971d`.
  Existing timer conversion replaces the private getter overlay; complete code
  and effective fields remain unchanged. Original folded spelling is unknown.
- Options: `.analysis/gpt-6.1-sol-options-index-dispatch-20261009/`; unsigned
  selectors, index reuse and real right-input switch are neutral. Current cache
  `build/matching/TitleScreenOptions.obj` remains hash-bound by its retained audit.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 -B .analysis/gpt-6.1-sol-replay-depth-workspace-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-ecl-jump-timer-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-enemy-draw-expression-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-draw1422-callee-context-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-pause-partial-visibility-20261009/audit.py --retained-only
python3 scripts/inspect-pause-menu.py
python3 -B .analysis/gpt-6.1-sol-enemy-object-copies-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-enemy-object-copies-20261009/replay.py --retained-only
python3 -B .analysis/gpt-6.1-sol-gameplay-helper-visibility-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-setup-input-array-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-ecl-flat-local-scope-20261009/audit.py --retained-only
python3 scripts/report-ecl-codegen.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 scripts/inspect-ecl-complete.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 -B .analysis/gpt-6.1-sol-type21-preheader-20261009/audit.py --canonical-only
python3 -B .analysis/gpt-6.1-sol-charge-timer-conversion-20261009/audit.py --retained-only
python3 scripts/inspect-player-charge-text.py build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation. Complete
RunEcl inspection returns 1 for expected differences. Full candidate replay requires
rebuilt objects and a fresh trial manifest; retained-only replay verifies captured
proof. Earlier compact evidence and source corrections remain in the KB and Git.

Validation: complete carrier/target proof, strict affected siblings, tracking,
progress, isolated CI and whitespace pass. CI runs 79 tests: 68 pass and eleven
optional Capstone checks skip. Local checkpoints are not pushed. The 95% objective
and native product/runtime/later phase gates remain open.
