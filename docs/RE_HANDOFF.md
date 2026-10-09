# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at d150ad5. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. No target patch, IDA write or
delegation. Two complete RunEcl flat-switch/declaration-scope controls are
compiled once each and rejected as neutral after complete carrier/field replay.
No canonical source, ABI, profile, match row or exactness credit changes.
The previous Charge timer-conversion source correction remains maintained.

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
  remain 4/7/86/155/156/157; source scope controls below are rejected.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 fields, 39 differences;
  early draw index, descriptor/effect scheduling, trail and homing.
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

## Latest RunEcl batch

Packet: `.analysis/gpt-6.1-sol-ecl-flat-local-scope-20261009/`.
Reuse the current canonical carrier after checking all 43 source/header/backend
and five Oracle inputs:
`build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj`, SHA256
`94875d0fa89059d7decded90e9987f79d42388f12b0980cf163cefd154d4d6b9`.
No unchanged baseline or canonical compile; no candidate is adopted.

Each candidate removes the six family wrappers and places all 30 existing
trivial scalar/pointer declarations at function entry or switch entry. Statements,
case-local initializers, labels and macros remain. Twenty-seven bindings have
consumers; three old unused declarations move intact, without invented uses.
This combines two distinct
older controls: the October 3 hoist retained wrappers in a 15,560-byte carrier;
the October 6 flat form left declarations inside fragment positions. The precise
combined current-context route is now measured, and should not be repeated.

Both candidates preserve all 67 owners /28,838 bytes /1,215 effective fields and
35 nondebug noncode sections. Independent raw COFF and repository parsers agree.
Private spellings change 182/162 times at independently unchanged actual local
destinations. Complete opcode-rooted identity, every field/table byte and the full
2,161-byte difference list remain identical. Code/alignment reporting independently
checks 14,791 code + one alignment + 772 table bytes. Batch-end strict checks of
23 existing exact siblings pass 9,533 bytes /439 fields in each carrier. Diagnostic
adapters alter only object paths and eight proved local label spellings; canonical
manifests remain unchanged. All 37 actual unique includes are hash-bound, including
the guarded canonical Control include from EclPostRuntime. These are bounded static
and compiler observations, not original source/TU or native runtime proof.

Lossless complete proof, input hashes, actual include logs, patches, selected direct
IDA windows and source regeneration are retained. Hash-checked cleanup removes
18 owned source/COFF/PDB files, **920,651 bytes**. `prepare.py --restore` reproduces
all 14 source files without compiling. Retained-only audit reconstructs source
hashes from patches and checks captured candidates against the real baseline;
it does not inspect deleted COFFs. Next rotate coverage or use materially different
target evidence; no unsupported forced-register/profile route is justified.

## Other current evidence

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
