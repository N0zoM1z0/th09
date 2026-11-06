# Effect receiver models and actual literal definitions, 2026-11-06

The Effect AddedCallback diagnostic now verifies the actual read-only COFF
definition of `etama.anm`, including its terminator, against the original PE.
Previously it verified the original string and the source relocation's name,
but did not read that source symbol's definition. A deliberately malformed
OBJ with `Xtama.anm` under the unchanged symbol produced the identical old
51-byte/eight-difference report. The same fixture is now rejected before a
complete comparison can succeed. No game input or runtime behavior is changed.

The four actual relocation identities, their ordering/addends, both calls,
all51 original bytes,19 instructions and three blocks remain required. The
checker now obtains actual symbol section/value coordinates from the COFF
reader and uses the existing literal-definition validator. Undefined, writable,
truncated or counterfeit literal definitions fail; a missing required literal
also fails. The public result format remains unchanged.

## Fresh original and compiler observations

Direct Ghidra rechecks AddedCallback40D1C0 and Create40D320 after disk/Program
SHA-256, x86 little-endian32, base400000, entry47D45F and all685056 mapped bytes
are attested. The creator stores its owner at ChainElem+1C and its callback
address at+8; the actual callback operand starts at Create+C9. The complete
327-byte/27-field creator remains protected in every source model.

At the first GetAnm call, ESI is the owner and EDI is the captured ANM manager.
The resource returned from GetAnm/PreloadAnm is in EAX. The old C++ comment
calling EDI the ANM resource is corrected. All37 bytes from callback+E to+32
already match; the maintained eight differences are still+3..+9 and+D.
The RET at+32 ends the owner; thirteen followingCC bytes remain unowned.
These observations distinguish service identity from resource identity; they
do not recover unique original C++ types, ignored-argument arity or TU ownership.

Four coherent natural models use the existing Effect profile:

| Model | Callback bytes/fields | Original unlike bytes |
| --- | --- | --- |
| Borrowed owner reference, original global lookup |51/4|8|
| Borrowed owner reference, captured receiver lookup |51/4|11|
| Static void-pointer adapter over ordinary inline instance initialization |51/4|8|
| Local object references and one conditional publication |51/4|11|

The reference-parameter models change the actual declaration, definition and
Create registration together. Their real callback symbol is bound only to the
independently observed Create+C9/40D1C0 role; no address or operand is changed.
They preserve the physical ECX input, EAX result and plain RET transport. Full
creator equality plus a nonexact callback does not establish original nominal
type uniqueness. The maintained void-pointer interface stays unchanged.

The instance model preserves the actual registered static symbol and its full
51-byte body. Its additional emitted thiscall instance body also retains the
same eight differences. That unregistered compiler projection receives no
original-owner or exactness credit. Ordinary inline is not forced. All models
retain two calls and the complete three-block graph. Each protects15 exact
same-source siblings:60 repeated comparisons10120 physical bytes/400 fields.
No source model, new qualifier, parameter or helper is adopted. No assembly,
barriers, register/variable-order hints, dummy clobbers or profile search is used.

## Current-source and validation scope

The only game-source edit corrects the manager/resource comment. A fresh fixed-
profile Effect build preserves all17 emitted regular bodies and every actual
field identity/private definition. The callback stays51/4/eight, without new
exact credit. Unchanged source/profile/object proofs reuse the real GetAnm and
PreloadAnm carriers; all five Background units and16 loader units replay in
their existing profiles. The loader's separate constructor profile is unaffected.
Strict public replay passes36 complete existing units5543 bytes/206 fields.
No canonical recipe, destination, addend, extent or matching ledger changes.

Four new target-independent tests cover actual bytes/terminator, read-only
source definition provenance, missing definitions and independent original
literal identity. All18 focused Effect tests pass with two optional decoder
skips under Python-S. Full public CI passes1381 tests/29 skips. The malformed
object fixture is an Oracle regression input, not a source candidate, target
patch or game-input/runtime hardening test. The original executable is untouched.

The ignored packet is `.analysis/effect-native-receiver-20261106/`. It retains
all four complete models and objects, both before/after malformed-object
reports, the fixture, unchanged original-bound baseline, current build and
mapped-byte Ghidra evidence. An initial descriptor-key failure happened before
compilation. A later driver correctly stopped on its mistaken single-profile
assumption for the loader; the completed Effect object was hash-recovered,
not rebuilt or discarded. Those setup/review logs remain separate from compiler
counterexamples. The final scope selects each actual callee's existing profile.

Totals remain1034 source-present/1021 exact/13 nonexact. The exact-phase goal
and Windows product/runtime, semantic and portability gates stay open. This
checkpoint does not establish Factory acceptance or remote publication.

## Immutable source and regression binding

Source commit `d833f9bd8886b87a739aead5b96b4b95bd10b989` binds26 tracked
inputs/Git blobs and13 selected compiler/SDK files. The sealed object set has
three current objects, four source-model objects, the original baseline and
the explicitly malformed Oracle fixture. The source/control proof SHA-256 is:

`4b24dce75d5014ef9319d5ee9dd0ddb451ad38720e241cf162e8cb64d365fe50`

`python3 -B .analysis/effect-native-receiver-20261106/verify.py` passes an
independent read-only replay of36 current exact comparisons,17 preserved
Effect bodies, all four complete models/60 exact siblings, the emitted instance
projection and all mapped-byte Ghidra ranges. It restores the unmodified legacy
checker from its immutable Git blob, sets only its repository-root path for
private replay, reproduces the old malformed-literal acceptance and verifies
that the new checker rejects that same hashed fixture. No target byte changes.

Cleanup removes57 SHA-identical staged inputs and five disposable PDBs:
62 files/632437 bytes. All five new compiler objects and the malformed fixture
remain, along with frozen/changed sources, original baseline, reused native
objects and setup/recovery evidence. Explicit path/hash receipts resolve each
removed duplicate to its retained frozen input. This is local source-scoped
Oracle evidence, not Factory acceptance or remote publication.
