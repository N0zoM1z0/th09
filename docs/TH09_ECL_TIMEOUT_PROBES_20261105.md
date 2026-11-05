# ECL timeout probes and native-surface controls — 2026-11-05

Started from clean `f1604b1d`. This checkpoint repairs two diagnostic recipes
that had stopped reaching the compiler after the native Enemy migration. It
adds no exact function. Canonical progress remains 1021/1035 functions,
259829/291883 reviewed-authored bytes (89.0182%), with 14 nonexact functions.
Maintained game source, headers, profiles, manifests and both canonical ECL
objects remain unchanged.

## Fresh original evidence

The Japanese v1.50a target passes verification at SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Fresh direct Ghidra exports of RunEcl4086C0 and timer assignment401500 attest
disk/Program identity, i386 language, image base00400000, entry0047D45F and all
685056 mapped file-backed bytes. Ghidra prototypes remain provisional.

Independent PE/COFF disassembly confirms the two remaining source differences.
At RunEcl+277 the original loads the raw time intoECX and pushes it before
loading the timer receiver; the maintained candidate loads EAX and pushes it
after loading the receiver. At+30B0 the original zero-extends the wire byte
intoECX, retains the old flags inESI and forms the new word inEAX. The candidate
first copies CL, reads the old flags intoEAX, then widens CL and forms the
result inECX. These are compiler observations, not recovered original variable
names or evidence for register annotations.

## Repaired recipes

`operand-macros-native-timeout` and `operand-macros-native-plain` still searched
for an assignment through the retired `state` alias. The maintained assignment
now uses `enemy->` and spans two lines. The old assertion therefore stopped
both options before compilation.

The renderer now recognizes the reviewed assignment through either known
storage root, requires the same root on both sides, and replaces only that
assignment with the native timeout bitfield write. The existing single wire
byte copy remains. Missing, mixed, unknown or duplicate roots are rejected.
Target, input-closure, wrapper-semantic, complete-owner and field/table gates
remain unchanged. Six focused tests pass, including both current-source
recipes and invalid storage-root cases.

Both real CLI invocations now compile serially at lowered priority with the
pinned VC7.1 13.10.3077 provider and reach the complete original-bound Oracle:

| Recipe | Physical bytes / fields | Ordinary differences | Field differences | Table differences | Handler shapes |
| --- | --- | ---: | ---: | ---: | --- |
| native timeout | 15564 / 598 | 2019 | 239 | 26 | 144/148 |
| native timeout plus plain timer read | 15564 / 598 | 2031 | 239 | 26 | 143/148 |

Both exit2 with `state=rejected`, preserving the full machine-code/COFF and
whole-owner reports. This is the expected field-position rejection, not a
successful exactness result. The old pre-compilation assertion is gone.

```bash
python3 -B scripts/probe-runecl-operand-context.py --case operand-macros-native-timeout --output .analysis/ecl-state-20261105/public-timeout
python3 -B scripts/probe-runecl-operand-context.py --case operand-macros-native-plain --output .analysis/ecl-state-20261105/public-plain
```

## Bounded source-model controls

Every control uses frozen current inputs, serial bounded compilation and one
of the two existing canonical ECL profiles. None changes maintained game source.

| Control | Complete observation |
| --- | --- |
| Native timeout from typed byte, existing shared integer workspace or state workspace | Each15564/598;1938 ordinary/202 field/26 table differences |
| Native timeout retaining the existing byte memcpy | Same1938/202/26 regression |
| Timeout helper result in the existing shared scalar | 2048 ordinary/242 field/26 table differences |
| Expand source-only accessors with native expressions, existing `/Ob1`, retain genuine math wrapper typing | 15564/598;3693 ordinary/454 field/65 table differences;142/148 handler shapes |
| Expand accessors and genuine math call expressions, existing `/Ob0` | 15560/598; production complete-extent gate rejects |

The native-surface experiment follows the newly exact SDK leaf's profile
question through the complete interpreter. It does not establish a common
original TU/profile. Temporary `do { } while (0)` macro scaffolding produced
extra alignment/control-flow blocks; final controls use expression or compound
statements without that artificial loop. The `/Ob0` experiment with scaffolding
has945 blocks against943 and fails graph pairing; identities must not be
paired by guessing past that failure. Direct math pointer-cast expressions
under `/Ob1` also inline/remove original calls, so the final `/Ob1` control
retains the existing genuine function-pointer wrappers. No variant is adopted.
These finite negatives do not exclude other natural source models.

## Evidence and unchanged baseline

`.analysis/ecl-state-20261105/` retains the fresh Ghidra attestations, frozen
source/header hashes, control recipes and changed inputs, compiler logs, full
reports and the restored public-recipe results. Trial objects remain under
`build/analysis/ecl-state-20261105/`. Only hash-identical copied inputs are
removed with a per-file SHA-256 receipt; frozen and unique inputs remain.

`verify-baseline.py` binds the unchanged game/compiler/profile/object inputs
to the immutable source receipt from `33291074`, checks both repaired controls
reached the full Oracle and repeats the complete main comparison. All82
canonical comparisons are reused from unchanged source/profile/object proofs;
there is no cold rebuild of unaffected owners. The changed diagnostic script
is recorded separately, rather than claiming every old receipt input is still
identical. RunEcl retains the identical30 difference offsets and values,
including zero field/table differences. Enemy remains13 and biased movement
remains canonical516/28 exact. Whole-product, runtime, port and Factory gates
remain open.

The final renderer adds an identifier boundary to reject prefixed/qualified
lookalikes. Its rendered compiler inputs are SHA-identical to both real CLI
replays above, so those compiler proofs are reused. The exact pre-boundary
recipe bytes and digest are retained separately; the reports are not rewritten
to claim they compiled a different producer version.

Final target-independent CI passes1346 tests with29 skips. Target/tracking,
documentation, generated progress and whitespace checks pass. This is local
diagnostic/source-model evidence, not a Factory verdict or remote publication.
