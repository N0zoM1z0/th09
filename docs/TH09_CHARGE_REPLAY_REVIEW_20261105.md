# Charge and replay source review, 2026-11-05

Started from clean main `b9f119b1`. Original target, tracking and status checks
pass: 1019 exact / 16 nonexact. Direct Ghidra verifies disk/Program SHA, i386,
image base, entry and all 685056 mapped file bytes before the successful charge
and replay exports. The combined charge/Effect export failed to decompile the
Effect entry; it is not a successful two-function analysis receipt.

## Structured charge update

The independent complete Cirno-Labs `player.cpp` build reproduces our current
charge function's linked body: 1197 candidate bytes against the 1210-byte
original, 70 resolved fields, no unresolved references and 1160 linked overlap
differences. All five actual scalar initializers are checked. The raw reference
COFF uses GameManager+E8 where our source uses the separate configuration alias;
their addends differ, but independently established effective targets agree.
This is a compiler observation, not an exact replacement or proof of original
source spelling.

The maintained source now follows the structured pressed/held/released flow
and accesses native timers directly. It removes the timer pointer aliases and
the separate jump into the shooting block. Two remaining labels express release
and the shared shot phase. Native Player, input, effects, CardAttack, SHT and
timer interfaces remain. Attribution is in source and THIRD_PARTY_NOTICES.md.

Fresh before/adopted builds use the same recorded /O2 /Ob1 /EHsc profile.
Every raw byte, linked byte and actual COFF field record is identical between
the two maintained models. Both have 342 instructions, 70 fields, 58 ordered
original calls, 53 blocks, five verified scalar definitions and 13 missing
original bytes. The visible 20-byte Timer >= definition independently matches
its complete original body in both objects. The original has three EBP-1 byte
accesses; neither source generates them. Original and candidate direct graphs
still differ. No match-ledger promotion follows.

The old source-text test required five pointer-alias spellings even though both
native expressions produce identical machine code. Those assertions are
removed; native type/layout and complete target/COFF checks remain.

The source checkpoint is local commit `bccbc5e4`. A new focused entry is now
available without historical experiment scripts:

```bash
python3 -B scripts/check-boss-frontiers.py --owner charge
```

It verifies the original target, cold-compiles only this source with the
existing Player /O2 /Ob1 /EHsc profile, checks the complete 1210-byte original
and all 70 actual fields/58 calls/five literal definitions, then compares the
visible Timer body in the same object. Every one of the 37 already-matching
physical positions is protected individually; same-count substitutions fail.
Partial reductions remain explicitly noncrediting. Nonexact compiler runs
now inherit lowered priority and a 180-second timeout with a kill deadline.
The fresh focused receipt is `.analysis/charge-20261105/focused.json`.

Full target-independent CI passes 1338 tests (29 skipped), including the new
wrong-target/census/extent, Timer-byte/field and all 37 byte-substitution
negative cases. Tracking, documentation, generated progress and whitespace
checks pass. This public CI is separate from the focused compiler replay.

## Fresh rejected replay and Effect controls

Replay BeginPlaybackStage is freshly reproduced at 538/540 bytes, 32 fields,
five calls, 12 equal direct blocks, 39 overlap differences and two missing bytes.
Visible actual seed getter/setter/reset bodies, setter placement and typed
GameConfiguration assignment preserve this frontier. Removing the existing
single volatile pointer access produces 49 differences and reorders a field.
Native header fields plus direct side pointers have the same regression.
None is adopted; the volatile access is historical source, not a recommended
codegen technique or recovered original qualifier.

The 51-byte Effect AddedCallback baseline still has eight differences.
Making the real GetAnm body visible, before or after the callback, changes its
physical/instruction extent and is rejected by the complete-owner inspector.
No register forcing or profile search is used.

## Evidence

`.analysis/charge-20261105/` retains Ghidra, the independent reference-build
receipt, before/adopted complete comparisons and source recipes. `verify.py`
compiles both maintained models serially at lowered priority and compares the
entire owner and Timer sibling. Objects are below
`build/analysis/charge-20261105/`; the recorded charge profile is unchanged.

Replay controls and complete reports are in
`.analysis/replay-stage-20261105/` (`probe.py`, `config-probe.py`, `native-probe.py`);
the focused baseline is `.analysis/replay-stage-baseline-20261105.json`.
Effect controls are in `.analysis/effect-getter-20261105/probe.py`.
Objects stay below `build/analysis/`. No historical evidence is deleted.

Reference revision is `d7653152becabb8c86f2f43d49015ecc11bd0652`.
No new exact credit, Factory acceptance, remote publication or native-product
runtime claim is made.
