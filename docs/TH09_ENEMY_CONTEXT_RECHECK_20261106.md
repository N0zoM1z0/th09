# Complete Enemy context recheck, 2026-11-06

The continuation starts at clean `ff0ba04d`. Original target, tracking and
reconstruction status checks pass. The latest source receipt's112 inputs
and14 objects remain unchanged. No production source model is adopted here.

Fresh local Ghidra exports cover Enemy update, the descriptor constructor,
Effect spawn, Timer greater and Timer periodic query. Disk/Program SHA,
i386, image base/entry and all685056 file-backed mapped PE bytes agree with
the original Japanese1.50a executable. These exports establish current
semantic observations; only the complete compiler/PE comparisons measure codegen.

All experiments use the existing main ECL compiler profile. Compilation is
serial, low priority and resource bounded. The frozen91-file input closure
and two additional unchanged callee source files are retained. Complete
RunEcl remains15564 bytes/598 fields/30 differences in every context.

| Complete source model | Enemy observation |
| --- | --- |
| Descriptor constructor view derives from its native payload | 3900 bytes/97 fields/13 differences |
| Descriptor coordinate copy uses memcpy | 3900/97/13 |
| Front tests use native flag fields without the word snapshot | 3900/97/13 |
| Both native Player locals become one shared workspace | 3900/97/13 |
| Both callback-threshold loops share their index | 3900/97/13 |
| Shared Player, draw-head and callback workspace | 3900/97/13 |
| Actual Effect SpawnEffect body co-visible | 3900/97/13 |
| Actual Timer HasTickedEvery body co-visible | 3900/97/45 |
| Both actual periodic and Effect bodies co-visible | 3900/97/45 |
| Periodic body plus derived descriptor view | 3900/97/45 |
| Periodic body plus named force-death position and native byte copy | 3900/97; literal-field position guard rejects |

The constructor-view control independently preserves the complete28-byte
constructor. Co-visible HasTickedEvery remains exactly33 bytes; co-visible
SpawnEffect remains exactly184 bytes, including when both are present.
Matching callees do not make the caller exact. The last model's rejected
field positions are not masked or assigned a partial-equality percentage.

The measured13-byte baseline still consists of two draw-index bytes and
eleven side/threshold/descriptor-copy transport bytes. The periodic context
changes the residual pattern; it is not an improvement. The derived view,
shared workspaces and co-location are source hypotheses, not evidence of
original inheritance, variable spelling or translation-unit identity.
TH08 and the pinned Cirno-Labs reference supplied hypotheses only; every
TH09 size, operand and comparison above comes from the local original.

The ignored packet `.analysis/enemy-current-frontier-20261105/` retains
`inputs.json`, frozen sources, changed model sources, `case-index.json`,
compiler commands/logs, full reports and the attested Ghidra export.
`experiment.py` reproduces a selected existing case with the fixed profile.
Objects remain below `build/analysis/enemy-current-frontier-20261105/`.
These finite negative results do not exhaust natural source models.

No canonical source, profile, target, matching row or exact counter changes.
The existing83 ECL proofs remain source/profile/object-bound and are reused;
there is no full cold replay. Totals remain1035 source-present/1022 exact/
13 nonexact, with product/runtime and later phase gates still open.
