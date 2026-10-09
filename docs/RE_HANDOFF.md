# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 5319533. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. Seven Enemy update object/copy
controls precede one canonical-path cold compile. The private Player homing
field now holds the observed Enemy pointer directly; the complete code/fields
remain neutral. Early tracked-position capture and direct memmove are rejected.
No new exact credit, profile change, target patch, IDA write or delegation.
Prior corrections stay.

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

## Enemy update object-copy batch

Packet: `.analysis/gpt-6.1-sol-enemy-object-copies-20261009/`.
Current cache: `build/matching/EnemyManagerCore.obj`, SHA256
`60a054387f8a7125ec31eb44184e0bd6ed8ec711a949d61b9c29beae366aa1f9`.
The actual former cache and unchanged compiler/header inputs were checked before
reuse. Its production path now contains the cold build. The baseline reference
capture is explicitly a separately compiled neutral control, corroborated against
the prior independent complete raw capture; it is not the former whole COFF.

Player +0x3037C is compared with the current Enemy, cleared on deactivation,
read as an Enemy at +0x2D78, and assigned the current Enemy by the target.
The private view now uses EnemyCoreView*, removing the repeated void* cast.
The unrelated opaque focus-getter return ABI stays unchanged. Original class
spelling, larger layout ownership and native runtime behavior remain unknown.

Side/destination references and direct temporary-address memcpy are neutral.
Capturing trackedPosition before the vector calls gives 3,888 physical bytes,
3,289 overlap differences plus twelve missing, and is rejected. Direct memmove
adds a seventieth call and a 98th field absent from target; its external target
destination remains unknown and no full resolved-byte score is claimed.
Typed homing and canonical cold preserve all five owners, 99 fields and seven
nondebug noncode sections. Main remains 3,900 physical /3,883 code bytes with
97 bound fields and 39 differences. Nine strict reports preserve the existing
151-byte/two-field sibling. Cleanup removes 22 source/COFF/PDB files, 961,875
bytes; all seven complete recipes recover after cleanup. Compressed complete
captures, target disassembly, proof and source inversion remain below 200 KB.
Use the new packet's retained audit and replay; older Enemy packets retain
historical input bindings and are not current-source replay commands.

## Latest Type14/22 draw batch

Packet: `.analysis/gpt-6.1-sol-draw1422-callee-context-20261009/`.
Current cache: `build/matching/ExAttackDrawType14Type22.obj`, SHA256
`cf6d50537f2768a58a4b480072929db81feaa806bdb760addb9f8d02e5fc724f`.
Historical raw source, all declared/actual project headers and retained object
hashes were verified before reuse; new hashes do not attest historical DLL loading.

Target remains 959 bytes, 290 instructions, frame 0x20. Maintained callback is
974 bytes, 297 instructions, frame 0x24, with 53 fully bound fields, 905 linked
overlap differences and 15 excess. All 32 ordered calls (20 direct/12 D3D indirect)
and 17-block direct graph agree. Removing the private timer view and using the
existing integer conversion preserves raw bytes and the getter's 0x435F00 target.
The angle declaration agrees with the existing math definition under /Gr; both
float arguments still use the stack, return ST0 and RET8. Original source spelling
of that convention and folded getter remains unknown.

Actual 111-byte AddNormalizeAngle body visible before/after the callback, alone,
with the target-observed 4-byte timer leaf, or with the conversion, gives 958 bytes,
52 fields and 31 calls. Each drops precisely the required 0x42AED0 call at target
0x444158. Near target size is rejected; no body migration or fake result consumer
is retained. Timer body visibility before/after, real extra/center references
alone/together and canonical interface changes are neutral. Added helpers exactly
match their known physical leaves and receive no duplicated authorship credit.

One batch-end canonical compile matches the isolated interface trial on every
raw owner byte, all fields, symbol coordinates and all five nondebug data/directive
sections. There is one callback and no accepted sibling in this TU. Focused
canonical normalization/getter physical Oracles pass 111/111 and 4/4. All thirteen
actual traces contain 81 distinct headers plus source and 121 include events.

36 terminal owned source/COFF/PDB files, 1,796,717 bytes, are removed. Every one of
the twelve raw CRLF source recipes was restored and checked after cleanup, then
removed. Logs compress losslessly from 208,852 to 10,629 bytes; complete raw
nondebug captures and all bound/decoded proof remain below 0.5 MB. Run
`audit.py --retained-only` to reparse actual baseline/canonical objects and rebind
captured deleted candidates, or `reconstruct.py --restore` for source-only
reconstruction. Do not repeat these precise callee/reference contexts unchanged.

## Other current evidence

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
