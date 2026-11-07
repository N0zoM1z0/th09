# ScreenEffect native shake-envelope phases and scoped dependent compiler replays

Original: Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a genuine **shared C++ type and large-owner source edit**. Original
historical class/member spelling and full runtime equivalence are not claimed.

## Recovering three native phase dwords

`ScreenEffect` is the original-backed 0x38-byte callback payload. The contiguous
parameter words at +0x1C, +0x20, and +0x24 are shared by several ScreenEffect
types. `CalcShakeEnvelope` uses them as **fade-in**, **hold**, **fade-out**
durations. A native union inside `ScreenEffect` now gives the envelope one
three-int named `shakeEnvelope` subrecord while preserving the original
`rawParameter0/1/2` words across the 11 C++ source users of the shared
header (including ScreenEffect itself). Compile-time checks enforce three mapped offsets, a 12-byte envelope
and the unchanged 0x38-byte owner. The envelope's `ZunTimer` is now borrowed
through one ordinary reference, not a second timer or a fabricated subclass.

The fade-out path still obtains independent `const volatile int &` values in
the **original two-load order**, preserving the signed integer conversions and
the ordinary VC7.1 compiler's x87/conditional behavior. More direct C++ phase
arithmetic, typed local snapshots, earlier timer captures and direct reference
conversions were cold-compiled but rejected if the full original extent moved.
The separate probe packet under
`.analysis/gptweb-screen-envelope-20261107/` and
`.analysis/gptweb-screen-envelope-phases-20261107/` retains all independent
compiler inputs/objects and original-bound diagnostics.

## Full affected-source and exact-original validation

The **336-byte `ScreenEffect::CalcShakeEnvelope` remains explicitly nonexact**:
336/336 bytes, 132 instructions, 20 original-bound relocations, eight ordered
calls, 26 original CFG blocks, **26 different bytes**. An apparently shorter
334-byte source is not progress; it changes previously matching original
instructions. No exactness or source-language uniqueness credit is awarded.

Because the native header is shared, we enumerated the transitive C++ include
closure instead of validating only the ScreenEffect TU. **All 21 affected
compiler-profile objects from 11 source TUs were freshly built**, covering
**99 already-exact functions**, **38,327 complete physical bytes and 2,315
relocations**. Each code owner was independently linked against the original
PE and required to match *every physical code/table byte*. A second strict
canonical replay against an isolated edited manifest also passed all 99.
That set includes large Ending and Title bosses, D3D initialization, and
small Supervisor, GameManager and ScreenEffect leaves.

The altered C++ header shifts certain old VC7.1-generated internal label
numbers in `Ending`, `FrontMessageUpdate`, `ScreenEffectRegisterChain`, and
`TitleScreen`. **79 local-only symbol names** were rebound after validating
each actual same-COFF-section definition, offset, relocation kind/addend and
previously reviewed absolute original PE address; target VAs, source byte
layout, and external referents did not change. The private reader under
`.analysis/gptweb-screen-envelope-phases-20261107/replay-consumers.py` checks
all physical owner bytes, all 99 unit manifests and the complete source/object
SHA before publishing the narrowly reviewed label spellings. This is not a
relaxed matcher.

The ordinary `--owner screen` complete source-cold replay independently
reverified the 18 exact ScreenEffect source owners across its three pinned
profiles (**2,562 bytes / 164 relocation records**) and retained the open
336-byte shake-envelope frontier. The verification was scoped to affected
sources and compiler profiles, **not** the project-wide full cold replay.

No canonical count change is claimed: 1,034 source-present / 1,021 exact /
13 nonexact. Full product linking, runtime oracle, semantic recovery, and
Factory acceptance remain open independently of this source checkpoint.
