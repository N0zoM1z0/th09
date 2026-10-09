# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 0215dcf. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. Eight complete Player collision
member/helper/loop-entry trials precede one canonical-path cold compile. Difficulty
+0x11C and coordinate +0x358 now use the existing GameManager receiver; complete
code and effective field destinations stay unchanged. All six same-TU exact
siblings pass. No new exact credit, ABI/profile change, target patch, IDA write or
delegation. Prior source corrections stay maintained.

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

## Latest Player collision batch

Packet: `.analysis/gpt-6.1-sol-collision-member-visibility-20261009/`.
Current cache: `build/matching/PlayerCollisionHelpers.obj`, SHA256
`c9ea2f6617302b613b16c66ec53f512df3a0a9ae07c8a0679fd83cdc886240ee`.
Historical baseline source, all actual project headers and retained object hashes
were checked before reuse; current backend hashes do not attest historical loading.

CheckBulletCollision remains 849 candidate /862 target bytes, 26 fields, 804
complete overlap differences and 13 missing, 266/277 instructions, 32/33 direct
blocks and 12 equal ordered calls. The residual includes loop topology as well as
preserved registers; do not describe it as allocation alone. Target +0x11C is the
already reviewed difficulty; target +0x358 is the coordinate value initialized to
288.0f. Both now use asserted members on the existing GameManager receiver.

Actual AddRespawnResource body visibility, before/after the owner and combined
with the members, is code-neutral; its emitted 162-byte helper/5 fields is exact.
Actual Rotate body visibility before/after gives 865 bytes, 834 overlap differences
and 3 excess, and homes Bullet in EBX but retains 32 blocks. Two natural loop-entry
controls give 846 bytes, 818 differences and 16 missing despite restoring 33 blocks.
Rotate emits a 42-byte body under this caller profile; its canonical 62-byte body
uses /Ob0. No helper or profile migration is retained, and no credit follows.

One batch-end canonical compile reproduces all seven owner bodies, 2,190 bytes,
58 fields and seven nondebug data/directive sections of the isolated member trial.
COFF debug tag/line pointers differ, with equal function lengths and symbol
coordinates; full auxiliary records are retained. Six strict existing siblings
cover 1,341 authored bytes and 32 fields. Exact ledgers and unit manifests stay
unchanged. Native ownership, data closure, runtime and semantic gates stay open.

24 owned source/COFF/PDB files, 851,200 bytes, are removed; all eight source recipes
reconstruct to their raw hashes after cleanup. Three full reports compress
losslessly from 479,503 to 52,565 bytes. `audit.py --retained-only` checks actual
baseline/canonical objects and rebinds captured candidate bytes/fields against the
verified target; it does not re-inspect deleted COFFs. `reconstruct.py --restore`
restores isolated sources without compiling. Do not repeat these precise controls
without a new TH09-local model.

## Other current evidence

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
