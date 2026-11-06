# Effect AddedCallback entry dataflow, 2026-11-06

Fresh original inspection of40D1C0..40D1F2 and a current pinned-profile
baseline place all eight linked-byte differences before the first GetAnm
call at+0xE. Both original and candidate keep the callback context in ESI
and the captured ANM manager in EDI. Their registers are not exchanged.

The original loads EDI from4DC550, saves incoming ECX into ESI, pushes8,
then copies EDI into ECX. The candidate saves ECX into ESI, loads ECX from
4DC550, pushes8, then copies ECX into EDI. The global operand begins at+4
in the original and+6 in the candidate. A bounded static instruction transfer
checks both sequences: at the first call, ESI=context, EDI=ECX=the one loaded
ANM pointer, and the pushed saved registers/index are identical. All37 bytes
from+E through+32 are original-equal. The only unlike offsets are
3,4,5,6,7,8,9,D. This is static dataflow evidence, not a runtime gate.

The older external reference's register-swap comment does not describe this
current candidate. Changing nominal resource pointer types or repeating
owner-register permutations is not justified by this comparison. The entry
instruction schedule remains unmatched; neither the original C++ expression
nor unique class/translation-unit ownership is established.

Two natural inline resource-acquisition hypotheses are tested privately:
an AnmManager member and a free function. Each captures the receiver once,
calls the actual GetAnm, checks resident+4, and only calls the actual PreloadAnm
when absent. The existing Effect profile is fixed, with ordinary `inline`
and no forcing attributes. Both retain every51 raw caller byte and the same
four actual symbol/addend/offset identities. COFF section numbers differ as
the hypothetical helper definitions change object organization; every actual
field is independently bound. All19 instructions, two calls and three blocks
remain covered. Both retain eight differences and all43 equal positions.
No helper or game source change is adopted.

Every context protects all15 complete Effect siblings. Two separate native
callee carriers protect five Background-ANM units and16 loader units under
their own existing profiles, including the actual20-byte GetAnm and90-byte
PreloadAnm. A strict canonical replay of the baseline and native carriers
passes36 distinct comparisons5543 physical bytes/206 fields. There are no
private label renames or production manifest changes. Fourteen existing
focused tests pass.

Fresh local Ghidra exports all three original functions after full target
identity and685056-byte mapped-image attestation. The callback's reviewed
physical projection is read back as `int __fastcall (void *context)`, with
ECX input and EAX return, and all mapped bytes are checked again. This does
not establish nominal C++ parameter types or ignored-argument arity. The
callee decompiler integer return labels remain provisional metadata.

Evidence stays in `.analysis/effect-resident-helper-20261106/`; objects stay
below `build/analysis/effect-resident-helper-20261106/`. Source epoch
`a9084cd78d28193536c0b0ecc8f0c8f78b0a4f57`,19 frozen source inputs/Git
blobs, eight compiler/SDK files and six actual objects are sealed by
`control-seal.json` SHA-256
`a81efd28bd9ce66f2061c039284d4010721ca4067292907e5aef8e38668366eb`.
Independent full replay passes. Cleanup prunes54 SHA-identical staged copies;
the changed helpers and frozen inputs remain. A restored complete helper
carrier reproduces all raw bytes/actual fields and15 exact siblings.

There is no new authored exact credit. Totals stay1022 exact/13 nonexact;
Factory acceptance, Windows product/runtime closure and remote publication
are not claimed.
