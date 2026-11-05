# TH09 complete biased ECL movement — 2026-11-05

The maintained `Th09EclRunLate::MoveRandomBiased @00407E30` now reproduces all
516 original bytes and28 fields under the existing EclManager `/Ob0` profile.
Its actual same-object timed-polar callee also reproduces288 bytes/nine fields.
All81 previously canonical EclManager units remain exact. This adds one exact
authored function:1035 source-present /1021 exact /14 nonexact, with259829 of
291883 reviewed-authored bytes exact (89.0182%).

This is a local exact-source/profile result. The main interpreter still has30
unlike bytes and Enemy update13. Original translation-unit ownership, complete
Windows product closure, runtime semantics, portability and Factory acceptance
remain separate open questions.

## Target and boundary evidence

The session recovered clean source checkpoint `bf7b35de` and re-read the live
ledgers. The read-only Japanese TH09 v1.50a `resources/th09.exe` has SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Tracking and target checks pass. Two fresh target-bound Ghidra runs attest the
disk/Program identity, x86 little-endian32 language, image base00400000,
entry0047D45F and every one of685056 mapped file-backed PE bytes. They export
the leaf, main interpreter and timed-polar callee. Ghidra is semantic/boundary
evidence; complete equality is established independently by the pinned
VC7.1 13.10.3077 Oracle.

The original leaf contains145 instructions,14 ordered calls and24 direct
blocks. Its physical input is Enemy inECX and instruction inEAX; the instruction
is retained inESI. The source's internal compiler convention is not a claim
that the public fastcall ABI passes that second pointer inEAX. The actual
`StartTimedPolarDisplacement @00407200` receives Enemy inEDI and instruction
inESI, with the float angle on the stack. Ghidra's provisional prototype omits
those private register inputs; the original instructions and callers establish
them. No register annotations are added to the source.

## Natural source and compiler observations

The earlier Float3 pointer/named-Y access emitted517 bytes and148 unlike
aligned bytes. Direct SDK indexing uses the real inline
`D3DXVECTOR3::operator float *` implementation. Its conversion call followed by
Y indexing produces the original LEA/access under the already-used `/Ob0`
profile. Compile-time assertions check the SDK/native size12 and XYZ offsets.

This change alone leaves ten unlike bytes: the instruction pointer occupies
EBX instead ofESI. Under `/Ob0`, source-only View/Player/ReadInt/ReadFloat and
trigonometry adapters otherwise become real extra function boundaries. The
leaf and its actual polar callee now read the existing native fields and
conditional typed operands directly. The polar callee uses the genuine
`cosf`/`sinf` function-pointer types already present in the former wrappers.
Its original288-byte body and private EDI/ESI transport then emerge naturally;
the complete516-byte caller simultaneously becomes exact. The source adds no
assembly, barriers, register hints, dummy effects, padding or flag search.

The actual same-object SDK getter matches three bytes at shared4343D0, with
zero fields. Unique original nominal ownership and authored/library/compiler
origin remain unknown; this is an uncredited alias. The same-object generated
`@cosf@4` and `@sinf@4` support bodies each match12 bytes/zero fields at401060
and401070. Their existing compiler-generated exclusions remain unchanged.

The two tested profiles were already canonical: main `/Ob1` and the existing
Type11 `/Ob0` carrier. The `/Ob1` copy of the new leaf is510 bytes/27 fields
and is not credited. The `/Ob0` main interpreter also remains nonexact. This
checkpoint does not imply that the whole ECL translation unit has one proven
original compile profile or that its executable link is closed.

## Canonical replay and tooling

Both actual canonical EclManager objects were rebuilt serially at lowered
priority with bounded compiler time. The complete cohort now comprises77 main
units and five Type11-profile units:82 comparisons,27636 physical bytes and
1260 original-bound fields, all exact. Twenty changed private-label field
spellings were refreshed only after proving their actual same-section COFF
definition offsets and immutable original PE destinations. Field offsets,
kinds, addends and target addresses do not change.

The complete15564-byte/598-field main interpreter retains exactly the prior30
difference offsets **and candidate/target values**, with zero field/table
differences. The3900-byte/97-field Enemy update retains13 differences. The
focused RunEcl replay passes its375-call identity census, eight genuine angle
calls, opcode4 timer transport and opcode155 bit behavior, plus the newly exact
leaf and its same-object callees. Unchanged source/profile-bound dependency
proofs are reused; no full cold replay is performed.

The biased-movement inspector now requires the complete same-object SDK,
polar and generated math bodies when binding the SDK symbol. The frontier
selects the accepted Type11 object and rejects incomplete or near-equal
results. Its historical517-byte envelope remains diagnostic-only. Existing
angle-identity checks now include the new leaf's three canonical calls. The
operand-control probe still requires all four reviewed operations and rejects
unrewritten calls, without treating obsolete occurrence counts as acceptance.
Shared Player dependency inventories include all82 ECL units (107 state-scope
or84 damage-scope units overall); original identity/layout checks remain. A
legacy assertion counting five removed adapter calls is dropped because source
spelling is not an acceptance criterion.

```bash
python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/build-match-unit.py --unit ecl-move-random-biased
python3 -B scripts/compare-coff-function.py --unit ecl-move-random-biased --json
python3 -B scripts/inspect-ecl-biased-movement.py --json build/matching/EclManager-Type11.obj
python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings
```

The ignored `.analysis/ecl-sdk-20261105/` retains frozen initial source/header
inputs and manifest, both Ghidra attestations, controls and serial build logs,
complete comparison reports, definition-backed label receipt and focused
replay. `verify-current.py` checks all82 canonical units, complete same-object
callees and unchanged main/Enemy frontiers. `canonical-source-proof.json`
records source/header/manifest/toolchain/SDK/profile/object hashes; the
[source-bound receipts](RE_ACCEPTED_RECEIPTS.md) record its immutable source
binding after the source commit. Trial objects remain under
`build/analysis/ecl-sdk-20261105/`. Only hash-identical per-trial duplicates
are removed, retaining a per-file receipt and all unique inputs/evidence.

Full target-independent CI passes1343 tests with29 skips after updating the
new canonical dependency inventories. Target/tracking, documentation, generated
progress and whitespace checks pass. This local compiler receipt is not a
Factory verdict or a claim about remote publication.
