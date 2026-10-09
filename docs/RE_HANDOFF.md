# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 33f07f6. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. No target patch, IDA write or
delegation. Two complete GameManager update input-array/publication controls
are compiled once each and rejected after full owner/field inspection and
batch-end strict sibling replay. No canonical source, ABI, profile, match row
or exactness credit changes. Prior source corrections remain maintained.

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

## Latest GameManager update batch

Packet: `.analysis/gpt-6.1-sol-setup-input-array-20261009/`.
The current source and all 92 inherited source/header/backend inputs agree with
`build/gpt-dots-setup-continuations-20261006/baseline.obj`, SHA256
`c181cf0b14561687ddf6ba9975ebce492bdfb25d055ea5269183fba608f7e186`.
Reuse that unchanged baseline; no canonical compile or candidate adoption.

OnUpdate is still **1,230 bytes /87 fields /105 complete differences**. Five
initialized input words are captured before either side publication. A const
five-word array with explicit publication is neutral across all twelve carrier
owners /4,590 bytes /406 fields and seven noncode sections. A two-side publication
loop instead emits **1,202 bytes /79 fields /1,106 overlap differences +28 absent**.
It retains a genuine 0x8E cursor loop; eight absolute address fields disappear.
All present fields and all sixteen ordered calls are checked; the eleven other
owners and seven noncode sections are neutral. Neither arrangement is retained.
The target's five word loads and ten absolute stores remain the entry frontier.

Both candidates pass strict replay of eight same-profile exact siblings:
**1,122 bytes /96 fields**, with object-path-only diagnostic adapters. Independent
COFF/PE and repository parsers agree on all owners/fields. Actual logs bind all
86 included headers. Runtime, original source/TU ownership and whole historical
compiler-environment attestation are not inferred.

Complete compressed proof, patches, include logs and direct IDA windows remain.
Hash-checked cleanup removes eight owned source/COFF/PDB/report files,
**772,996 bytes**. `prepare.py --restore` reproduces both source hashes without
compiling; that recipe was checked after cleanup. Retained-only audit rebinds
captured candidate bytes against the real target and checks the actual baseline,
current inputs, source inverses and captured strict reports. Deleted candidate
COFFs are not re-inspected. These precise array/publication models are closed
negatives; next use materially different evidence or rotate owner coverage.

## Other current evidence

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
