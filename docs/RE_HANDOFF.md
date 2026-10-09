# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 91817fb. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. Thirteen complete PauseMenu
partial-visibility/member/body-order trials precede one canonical-path cold compile.
Five existing GameManager +0x13C byte-view expressions become the asserted member;
eight State4 private manifest spellings refresh with full coordinate/byte proof.
All eleven same-TU exact siblings pass. No new exact credit, ABI/profile change,
target patch, IDA write or delegation. Prior source corrections stay maintained.

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

## Latest PauseMenu batch

Packet: `.analysis/gpt-6.1-sol-pause-partial-visibility-20261009/`.
Current cache: `build/matching/AsciiManagerMenu.obj`, SHA256
`1145e31c07bebac38e69639a835721fbdcb155761ba0251b492acdc03135f788`.
The old cache was verified before reuse and then replaced by the single canonical
cold compile. All 87 source/header/backend/Oracle inputs are pinned.

Four target stores and the real AsciiManager consumer address 0x4A7ECC, the already
asserted GameManager byte member +0x13C. Exactly five source expressions now use
`inGameMenu`. The whole cold carrier equals the isolated connected-member trial:
13 owners, 4,186 physical bytes, 266 fields and three nondebug sections. Pause stays
1,692 code /1,732 physical bytes, 107 fields, 1,492 overlap differences and 44 missing.
No newly exact function or byte is credited. Original larger ownership/runtime open.

Partial IsVisible opacity gives 1,772/107 fields/1,113 overlap differences plus
four missing. Partial SetInvisible opacity gives 1,748/107/1,509 plus 28 missing.
HasFlagBit0 opacity is neutral alone and with either context. Actual-member controls
preserve those results. Both VM bodies opaque gives 1,772/107/1,131 plus four missing.
Moving actual helper definitions after Pause is code-neutral and reverses the two
four-byte constant sections; it does not reproduce genuine body separation.
None of these diagnostic contexts is adopted.

All present strict siblings pass in all thirteen candidates; canonical's eleven
units cover 2,420 authored /2,448 physical bytes and 158 fields. Eight State4 names
are the only manifest changes, after full 1,172-byte and actual local-base proof.
Each trial has 78 actual headers /117 include events. Source/profile/ABI unchanged
apart from the five documented member expressions; no historical cohort rebuild.

Complete compressed captures, patches, recipes, logs and proof hashes remain.
39 owned source/COFF/PDB files, 2,014,782 bytes, are removed. Captured JSON compresses
from 3,865,560 to 272,998 bytes, checked losslessly. `reconstruct.py --restore`
reproduces all thirteen copies without compiling, checked after cleanup. Retained
audit checks actual current canonical state and captured complete candidates;
deleted trial COFFs are not re-inspected. Rotate to another large owner or obtain
new target-supported control flow; do not repeat these precise visibility contexts.

## Other current evidence

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
python3 -B .analysis/gpt-6.1-sol-pause-partial-visibility-20261009/audit.py --retained-only
python3 scripts/inspect-pause-menu.py
python3 -B .analysis/gpt-6.1-sol-enemy-scratch-lifetime-20261009/audit.py --retained-only
python3 -B .analysis/gpt-6.1-sol-enemy-scratch-lifetime-20261009/replay.py --retained-only
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
