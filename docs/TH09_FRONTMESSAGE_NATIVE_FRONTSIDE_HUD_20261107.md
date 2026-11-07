# TH09 FrontMessage: real FrontSide HUD owner in full script interpreter

Original: Japanese TH09 v1.50a independently SHA-256 verified as
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a source-facing native C++ layout and object-ownership checkpoint,
not a new complete exact function or proof of uniquely historic nominal
class names or translation-unit partitioning.

## Recover the genuine FrontSide allocation in message script

The complete **2,852-byte `FrontMessageRuntimeView::Update`** interpreter
(0x00416590; **2,968 physical bytes** with its switch/table tail)
previously held a separate `FrontMessageFrontSideView` defining an opaque
+0xA678 prefix, a private 0x558-byte
`FrontMessageFrontSideAuxView`, and a duplicated `LoadScript67` member
name. The original gameplay side actually stores a pointer at +0x18 to
one native `FrontSide` allocation, independently established by
FrontSideInf creation/registration and the already original-exact
`FrontSide::LoadScript67` (21 bytes at **0x0041A210**).

The FrontMessage side now has `FrontSide *frontSide18` rather than a
shadow class, with actual native `FrontSideAuxView` containing the genuine
+0xA678 side-ANM and optional-ANM loaded resource pointers. These are the
same physical members consumed by the main FrontSide functions; no extra
ANM VM array, new constructor, HUD allocation, or altered sprite table is
introduced. The source retains the original side selection, dialogue
script state machine, bytecode processing, portrait interrupts, and
original clock/output behavior. Both `LoadScript67` calls pass the real
FrontSide pointer with their **original REL32 field offsets +0x905 and
+0x910**, each targeting **0x0041A210**.

The previous checkpoint similarly recovered `PlayerLifecycleView *` at
side +0x04, restoring the same 0x403C20 state setter and native +0x303C8
timer. The full message interpreter now has both its real Player and
FrontSide receiver types without two private prefixed object models.
The still-partial message-side and manager projections remain separate
for unproven roles; they are not silently declared complete or exact
original C++ types.

## Original binary and independent leaf evidence

A copy-on-write isolated fixed-profile VC7.1 trial first showed that
replacing the duplicated HUD type preserved **all 2,968 raw physical
bytes and 167 original field offsets/types/addends**. After adoption, the
full code, switch data and every original field were compared against
the immutable retail PE, and the exact canonical comparator returns
**2,852/2,852 source bytes and 2,968/2,968 physical bytes**. The two
changed external C++ call names retain the same source-independent original
`FrontSide::LoadScript67` entry at 0x41A210, itself proven **21/21
bytes** by its independent source owner.

The larger shared header renumbered **30 VC7.1 `$L`/`$run_scripts$`
private label fields** in the whole script. Each was individually verified
against its actual same-section COFF symbol-definition offset and the
immutable original PE jump/table operand before rebinding only its name
in the strict match manifest. No original call address, field addend,
compiler profile, table contents or comparator strictness changed.

Scoped original-bound source-cold replay passes **17 complete already-exact
owners / 7,094 physical bytes / 343 relocation fields** over the message
script and FrontSide files. Focused tests assert the actual HUD member
layout, two native call sites and absence of shadow classes. Full original
probes and natural C++ trial receipts remain locally in
`.analysis/gpt-web-front-message-native-hud-20261107/`.

The 3,900-byte Enemy OnUpdate and 15,564-byte RunEcl remain separate
nonexact bosses with their previously verified 13- and 30-byte residuals.
No whole-project cold replay, Windows game product linking, gameplay
runtime acceptance, Factory receipt or remote push is claimed. Global
tracking remains **1,034 source-present / 1,021 exact / 13 nonexact**.
