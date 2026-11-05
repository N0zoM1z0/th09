# Complete Effect Type29 draw — 2026-11-05

`EffectDrawCallbackType29 @00448750` now reproduces the complete original
2220-byte owner, all 129 fields, 675 instructions, 72 ordered calls and 41
physical blocks. The original and fully linked candidate both hash to
`8ecf5d55f2ef0de67316f09b3a6d9422b4475bf9a7fdf1ea9dd10919247a3455`.
The new canonical unit is `effect-draw-type29`, using the existing EclManager
Type11 `/O2 /Ob0` profile. The main `/Ob1` copy remains uncredited.

This adds one authored exact function and 2220 reviewed-authored exact bytes:
1022/1035 functions, 262049/291883 bytes (89.7788%), with 13 source-present
nonexact functions. Product/runtime/semantic/port gates remain open.

## Re-investigation and natural source

The continuation starts from clean `e7511353`. The previous immutable
[source-bound receipt](RE_ACCEPTED_RECEIPTS.md) still matches all 138 recorded
inputs and actual canonical objects. The maintained Type29 source has 92
ordinary differences; its complete extent, original-position fields and physical
graph already agree. The previous [polar recovery](TH09_EFFECT29_POLAR_RECOVERY_20261105.md)
remains the source of the native two-dimensional polar API and SDK callee proofs.

Fresh direct Ghidra work reattests the Japanese v1.50a disk and Program SHA-256,
x86 little-endian 32-bit language, image base00400000, entry0047D45F and all
685056 file-backed mapped bytes. Its provisional decompilation and the decoded
original independently expose the initial angle/fan-start stores. Ghidra is not
used as an exactness Oracle.

The selected source gives the main fan and each of the six satellite fans its
own angle variable, initialized for that fan. This models seven independent
angle accumulations rather than sharing one mutable value through every scope.
It recovers the original angle/pointer stack homes and floating-add scheduling,
reducing 92 ordinary differences to ten. The ten positions are one exchanged
pair of initial stores, not ten independent missing operations:

- Original +51: initialize the main angle to negative pi; +58: capture the fan start.
- Intermediate +51: capture the fan start; +54: initialize the main angle.

The final source initializes the main angle before capturing its real fan start,
just as each satellite prepares its own angle before recording its start.
Declaration order is retained. The existing pointer assignment is separated
from its declaration; no unused variables, extra reads/writes, compiler
variable-order directives, register annotations, barriers, assembly, padding or
profile changes are introduced. Full original-bound comparison then reaches
zero differences. This establishes a natural source model, not unique original
variable names or translation-unit ownership.

Two independent controls are rejected: initializing one angle outside the
visibility branch emits 2220 bytes with 154 differences and loses original field
positions/physical graph; loop-local counters retain the prior 92-difference raw
body. The ten-difference lifetime-only intermediate is not promoted. The final
exact source preserves every formerly equal position.

## Complete owner, aliases and affected closure

Original Effect prefix row29 points to00448750 at004A0E14. Direct control flow
covers 672 reachable instructions; the physical owner contains 675 instructions
and ends with its return at00448FFB. Four following CC bytes remain unowned.
The canonical comparison includes every physical byte through that return.
All 129 actual relocation offsets, kinds, addends, literal payloads and referents
are independently checked. The diagnostic also retains six local-center ECX
reloads and zero vertex reloads.

The same actual `/Ob0` object contains original-equal, relocation-free callees:

| Definition | Original address | Complete bytes |
| --- | --- | ---: |
| Existing SetPolar2D primitive | 00441890 | 32 |
| Genuine SDK D3DXVECTOR3 constructor | 004010B0 | 26 |
| Genuine SDK D3DXVECTOR3 accumulator | 00405730 | 36 |

These support logical call aliases and add no authored/origin credit. The native
20-byte vertex and SDK 12-byte XYZ center retain their checked layouts. The
existing polar primitive is unchanged in this checkpoint.

Two serial lowered-priority canonical builds cover both affected ECL profiles.
All 83 canonical units pass 29856 complete physical bytes and 1389 fields.
Two private label names are refreshed only after their actual same-section
COFF definitions and immutable original operand destinations agree. No offset,
kind, addend, referent, boundary or profile is changed to obtain equality.
The previous 48 other polar-cohort units retain their source/profile/object
proofs and are reused without new compiler runs. This is not a full cold replay.

The complete RunEcl owner retains identical raw/linked hashes and all 30 prior
difference offsets and values at15564 bytes/598 fields; field/table differences
remain zero. Enemy update retains the identical 13 offsets at3900 bytes/97
fields. The focused driver passes both large owners, Type34 draw, Type26 update,
Type19 update, biased movement with actual callees, and the complete Type29
inspection. The guard now protects all 2220 Type29 bytes and requires its
complete original hash, field/call/graph and same-object callee evidence. The
new owner adds five original-bound angle calls and 28 native GameManager XY
calls to the accepted inventory (49/154 total). Shared Player state/damage
replay inventories now cover108/85 units, including the new exact owner; these
counts do not cause a wider cold replay for this source-only change.

## Reproduction and retained evidence

```bash
python3 -B scripts/build-match-unit.py --unit effect-draw-type29
python3 -B scripts/compare-coff-function.py --unit effect-draw-type29 --json
python3 -B scripts/inspect-effect-type29.py build/matching/EclManager-Type11.obj --json
python3 -B scripts/check-boss-frontiers.py --owner effect29
```

Ignored `.analysis/effect29-complete-20261105/` retains frozen quoted-source
closure, the fresh Ghidra attestation, bounded source controls, actual canonical
build logs, complete reports and recipes. `probe.py` compiles one frozen
source-lifetime hypothesis; `verify.py` checks every actual affected canonical
owner and the original-bound private-label destinations. The source-bound
receipt records both compiler profiles and objects, all input and SDK hashes,
complete comparison reports and the immutable source commit separately from
Factory acceptance. No native-product or runtime acceptance is inferred.

Full public CI passes1351 tests with29 skips. Target/tracking, documentation,
generated progress and whitespace checks pass. After its documentation update,
the18 focused Type29 tests also pass. The first CI run identified old accepted-
inventory counts; those inventories now include the exact owner and preserve
all identity/profile/physical-byte checks.

Source checkpoint `b6a9792c2013e67c375799abe9c49c6ab74f3685` and the immutable hashed local proof are
recorded in [source-bound receipts](RE_ACCEPTED_RECEIPTS.md). Later recipe runs
write separate replays rather than overwrite that source-bound receipt.
