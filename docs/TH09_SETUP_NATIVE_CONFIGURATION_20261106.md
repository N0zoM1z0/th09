# Setup configuration cell, 2026-11-06

The setup worker now allocates, copies and reads its configuration through
`GameManagerSetupLayout::configurationE8`. Its deleted callback releases the
same typed member. The former Setup-local `g_GameConfiguration` declaration
and untyped `heapE8` member are removed. Replay already consumes this physical
configuration cell through the shared native GameManager view. Checked offsets
and pointer width preserve +0xE8 and the 0x390-byte allocation.

Fresh local Ghidra analysis covers the entire original 1689-byte worker at
41AF2D..41B5C5. Disk and Program SHA-256, i386 language, base400000,
entry47D45F and all685056 file-backed mapped bytes are attested. Ghidra's
cached no-argument/stdcall label remains provisional; the maintained physical
fastcall declaration and ignored context parameter are not inferred from it.
The original allocation/copy/later reads address4A7E78, precisely
GameManager4A7D90+E8. Original nominal class and translation-unit ownership
remain unknown.

Five actual worker COFF fields at+DF,+F0,+11C,+17A,+2DF change from the
separate configuration symbol to the native GameManager root plusE8.
Every effective destination remains4A7E78. Complete relocation binding makes
the linked body identical to the preceding source candidate. The deleted
callback retains all422 original bytes and41 fields; this is a source storage
consolidation, not new exact-function credit.

Three isolated builds use the existing two GameManagerSetup profiles and the
affected SupervisorUpdate profile. All12 canonical comparisons pass3987
physical bytes and335 fields, including the Supervisor844-byte code and its
152-byte physical tail. Forty changed private Supervisor label spellings are
verified against actual same-owner definitions and original operands before
manifest updates. No target or relocation destination changes. The alternate
Setup profile's complete physical carrier also preserves its previous linked
body and effective fields; that comparison is not original-exact credit for
the alternate worker.

The main complete worker stays1689 bytes/171 fields/35 ordered calls and904
unlike bytes. The original has172 fields and78 blocks; the candidate has77
blocks. Every previously equal position and both referent multisets remain
protected by the maintained full frontier guard. Configuration aliases do not
explain its missing root reloads, selector-versus-flags loop anchor, or shared
rank-store tails.

Five fixed-profile complete contexts are retained: baseline, configuration
member alias, configuration member without the untyped alias, native side-bit
field, and signed score-threshold arithmetic. Each retains904 differences and
all ten exact same-object siblings. The latter two are negative hypotheses
and are not adopted. Neither the side-bit field nor the original arithmetic
signedness is established by their unchanged code generation.

The bounded evidence packet is `.analysis/setup-side-loop-20261106/`;
generated objects remain below `build/analysis/setup-side-loop-20261106/`.
`probe.py` freezes40 source inputs and the recorded provider recipes.
`current-source-build.json`, `current-units.toml` and
`configuration-field-bindings.json` retain all actual objects, complete
comparisons and the five alias changes. Thirty-four existing focused tests
pass. Tracking remains1035 source-present/1022 exact/13 nonexact. Product,
runtime, Factory acceptance and remote publication remain separate.
