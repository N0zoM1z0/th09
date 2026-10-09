# TH09 reconstruction handoff

## Active objective

The user resumed on **2026-10-09**: reach **95% exact reviewed-authored bytes**,
work on large functions, use direct IDA Pro MCP and local Bash/compiler Oracles
without Factory MCP, commit locally as `gpt-6.1-sol: ...`, and do not push.
Batch coherent trials before cold replay; reuse unchanged baselines only after
checking their source/backend/object bindings. The goal remains active-incomplete.
The October 7 stop at 7347cb3 is historical.

This checkpoint starts clean at 988b798. Target SHA, direct IDA metadata, entry
and five distributed mapped-byte samples pass. No target patch, IDA write or
delegation. ChargeAttack now uses the existing ZunTimer integer-conversion
declaration instead of its unrelated private getter overlay. One isolated control
and one batch-end canonical cold compile preserve complete machine code and
effective fields; only two call-symbol names change. No ABI, profile, match row
or exactness credit changes. Earlier Options controls and Charge entry
pointer/reference/declaration-order controls need not be repeated.

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
separate. The faithful Windows i386 product/runtime gates remain open;
semantic reconstruction and portability have not started.

## Active frontiers

- RunEcl: 14,792 target code /15,564 physical bytes; current candidate has
  **14,791 code + one alignment + 772 compiler-table bytes**. All 598 fields
  are independently checked, with 2,161 complete differences. Handler frontiers
  remain 4/7/86/155/156/157. Equal pre-table sizes had hidden the code-size gap.
- Enemy OnUpdate: 3,883 authored /3,900 physical bytes, 97 fields, 39 full
  differences; early draw index, descriptor/effect scheduling, trail and homing.
- Title Options: 2,045 target /2,048 candidate bytes, 135 independently decoded
  fields, 610 linked overlap differences plus three excess; all 51 ordered calls
  and 121 complete direct blocks agree. The right-scroll zero schedule remains.
- Player movement: 1,835 authored /1,900 physical bytes, 66 fields, 144 full
  differences. Actual Float3 declaration/body visibility controls are neutral.
- Player charge attack: 1,210 target /1,197 candidate bytes, 70 fields, 1,160
  linked overlap differences plus 13 absent bytes. The integer conversion keeps
  all seven diagnostic regions and 58 ordered calls; byte-mode storage remains open.
- ExAttack type18/type24: 1,436 target bytes; corrected candidate 1,441/44 fields,
  all 31 calls and 23 direct blocks agree; 1,051 overlap differences plus five
  excess bytes. Target stack homes remain open.
- DirectPlay message handler: 768 authored /808 physical bytes, 46 fields,
  125 complete differences, complete 34-block switch graph checked.
- PauseMenu update: 1,734 authored /1,776 physical bytes; 1,492 overlap
  differences plus 44 absent bytes. Use inspect-pause-menu.py.
- Enemy draw: 1,758 bytes, four differences around the second subtraction/Abs.
- Type21 update: 1,074 bytes, 29 fields, 15 ring-preheader differences. Real
  descriptor-member storage is maintained; two record-owned acquisitions are neutral.
- Remaining nonexact owners and maintained source corrections are recorded in
  config/functions.csv, docs/KNOWLEDGE_BASE.md and Git; consult these before trials.

## Current ECL evidence

Current source-bound canonical carrier:
`build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj`, SHA256
`94875d0fa89059d7decded90e9987f79d42388f12b0980cf163cefd154d4d6b9`.
The prior packet binds 43 current source/header/backend inputs and five Oracle
inputs. Its canonical-only audit checks all 67 owners /28,838 physical bytes /
1,215 effective fields, 35 nondebug noncode sections and all 23 configured
exact siblings /9,533 physical bytes /439 fields. No unchanged baseline compile.

Prior code-extent correction and three neutral expression controls are retained
in `.analysis/gpt-6.1-sol-ecl-mutation-expressions-20261009/`. Its retained-only
audit checks captured evidence against the real canonical baseline, without a
new inspection of cleaned trial COFFs. Codegen reporting validates complete
instruction decoding, terminal RET, bounded NOP/self-LEA alignment and branches
inside the code extent. Complete call identity belongs to inspect-ecl-complete.py.

## Latest Charge investigation

Packet: `.analysis/gpt-6.1-sol-charge-timer-conversion-20261009/`.
Source/header/backend bindings agree with the retained October 6 baseline at
`build/gpt-dots-charge-input-owner-20261006/canonical/PlayerChargeAttackUpdate.obj`,
SHA256 `58a7b5ce995113792a748619fb10353a12f4a828712ae5b37eb49afe271a6ff0`.
No unchanged baseline build. Fresh direct IDA confirms the four-byte getter
`mov eax,[ecx+8];ret` at 0x435F00 and both actual timer call sequences.

Remove the unrelated PlayerChargeTimerCurrentView overlay and use the existing
ZunTimer::operator int declaration directly in both SpawnShots arguments.
This is a target-facing type/ABI model, not recovered original method spelling
or folded-source ownership. Both complete emitted owners /1,217 bytes /70 fields
and six nondebug noncode sections remain equal after independently binding the
two explicit symbol changes. The complete main stays 1,197/1,210 bytes, 342/350
instructions, 58 ordered calls and 1,160 differences plus 13 absent. All 70 target
fields are decoded; internal branch targets and final RET are checked. The sole
20-byte comparator retains existing exact bytes under this carrier profile,
which differs from its standalone unit; no new credit follows.

Current canonical carrier:
`build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj`, SHA256
`4744bb7c52cce4a50b55aa7162578ce79f9cf613b56e9e454f0cc3bb1df4971d`.
The maintained seven-region diagnostic passes. The batch ends with this one
canonical cold compile and full proof; no unchanged cohort replay. Complete
raw/field/noncode proof, actual include logs, inverse patch, input hashes and
selected direct IDA windows are retained. Hash-checked cleanup removes the
candidate COFF and both PDBs, 148,160 bytes. Retained-only verification checks the
captured candidate proof and real canonical object without recompiling or
claiming a new inspection of deleted candidate COFF. Source regeneration uses
prepare.py --restore. Next investigation needs different target-supported
entry/data-flow context; do not repeat the existing local-order controls.

## Prior Options investigation

New packet: `.analysis/gpt-6.1-sol-options-index-dispatch-20261009/`.
The frozen current source and only actual include, pinned stddef.h, agree with
the inherited source/profile/backend/object-bound baseline at
`build/gpt-dots-options-menu-20261003/baseline.obj`, SHA256
`9abfd67374215cfecc57856d7272d6cf28c3e03babedda467d6852b78bfc0cf4`.
Current `build/matching/TitleScreenOptions.obj` is also checked at SHA256
`1ddf1bd8088e4c2daa68450db99787c6b021c2b7f51429743389747cb60e6f34`;
its complete owner bytes/fields/data agree. No unchanged baseline build.

Three controls convert all seven freshly read selectors to unsigned menu
indices, reuse the existing consumed index for those selectors, or switch on
the real right-scroll return with zero/default arms. Selected/default cases,
fresh reads around calls and continuation effects remain; there is no zero
surrogate. All two owners /2,082 bytes /138 fields and two nondebug noncode
sections remain raw/field/data neutral. Target and candidate full decoding cover
all 135 actual address fields independently, including indexed globals. Each
full main-owner replay has 610 differences plus three excess bytes, all 51 calls
and 121 direct blocks. Batch-end strict PlayMenuSound passes 34/34 in all trials.
None is integrated; do not repeat these precise models unchanged.

Full proof is losslessly retained as audit.json.gz with patches, actual compiler
logs, selected direct IDA windows, independent parser and source-restoration
recipe. Each actual include closure is checked against preobserved hashes; the
historical loaded environment remains unproved. All three compiler calls are
terminal. Verified source restoration and compression precede removing ten
owned copies/COFFs/PDBs/redundant reports, **1,198,498 bytes**. Both baseline
objects and inherited evidence stay. Retained-only verification checks captured
trial proof against both actual baseline objects; it does not inspect deleted
trial COFFs or compile. Next use different target-supported data-flow or caller
context, rather than repeating selector types, index reuse or input-switch shape.

## Restart commands

```bash
git status --short --branch
git diff
python3 scripts/verify-target.py
python3 scripts/check-ida-mcp.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 -B .analysis/gpt-6.1-sol-charge-timer-conversion-20261009/audit.py --retained-only
python3 scripts/inspect-player-charge-text.py build/gpt-6.1-sol-charge-timer-conversion-20261009/canonical.obj
python3 -B .analysis/gpt-6.1-sol-options-index-dispatch-20261009/audit.py --retained-only
python3 scripts/inspect-title-options.py build/matching/TitleScreenOptions.obj
python3 scripts/compare-coff-function.py --unit title-screen-play-menu-sound --json
python3 -B .analysis/gpt-6.1-sol-type21-preheader-20261009/audit.py --canonical-only
python3 scripts/report-ecl-codegen.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
python3 scripts/inspect-ecl-complete.py build/gpt-6.1-sol-type21-preheader-20261009/canonical.obj
```

Call direct IDA get_metadata with exactly {} during entry attestation. Complete
RunEcl inspection returns 1 for its expected differences. The prior Options packet's
prepare.py --restore reproduces all three sources without compiling; original
trial COFFs were intentionally cleaned. Full audit requires rebuilt trial objects;
retained-only audit verifies the captured proof. Earlier movement proof remains in
`.analysis/gpt-6.1-sol-movement-vector-context-20261009/`; restore its sources
before its full audit. Earlier retained corrections are documented in the KB.

Validation: focused decoder tests, complete canonical/retained proof, existing
exact sibling Oracles, tracking, progress, isolated CI and whitespace pass.
CI runs 79 tests: 68 pass, eleven optional Capstone checks skipped. Local
checkpoints are not pushed. Native product/runtime and later phase gates remain open.
