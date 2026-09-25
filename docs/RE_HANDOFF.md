# TH09 reconstruction handoff

This is the live restart document. It contains current state and operating rules
only. Historical investigation belongs in docs/KNOWLEDGE_BASE.md and Git
history.

## Authority order

When sources disagree, use this order:

1. config/functions.csv for live authored/excluded/source-present status.
2. config/matches.csv plus config/match-units.toml for canonical exactness.
3. scripts/report-reconstruction-status.py for live totals.
4. This handoff and docs/SMALL_FUNCTION_FRONTIER.md for routing.
5. docs/KNOWLEDGE_BASE.md packets for historical evidence and negative experiments.

Historical packet words such as current, now, remains, candidate sizes,
frontier counts, and exact/non-exact status describe their checkpoint unless a
live ledger row independently confirms them.

## Current state

Supported target: original Japanese TH09 v1.50a.

SHA-256:
10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822

| Measure | Current value |
| --- | ---: |
| Function candidates | 2,191 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 35 |
| Confirmed authored | 979 |
| Classified exclusions | 1,177 |
| Source-present authored mappings | 979 |
| Canonical exact functions | 866 |
| Source-present non-exact functions | 113 |
| Source-present non-exact bytes | 124,280 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 151,389 |

The source-presence frontier is closed. Exact reconstruction is not complete.
The faithful Windows i386 product graph remains open. Semantic reconstruction
and portability have not started.

## Restart checklist

From the repository root:

    git status --short --branch
    git diff --check
    python3 scripts/verify-target.py
    python3 scripts/validate-tracking.py --require-target
    python3 scripts/validate-docs.py
    python3 scripts/progress.py --check
    python3 scripts/report-reconstruction-status.py

If any count in this file differs from the report, the report and ledgers win
and this file must be refreshed before more reconstruction work.

Attest the active semantic-analysis database independently before trusting it.
The expected image base is 0x00400000, entry point 0x0047D45F, image size
0x000E7000, and target hash is the one above. IDA names/types are provisional
semantic evidence; they never grant exactness credit.

## Exactness rules

A function is canonical exact only when it has a target-bound match unit and a
relocation-aware replay accepted by the repository workflow. Source presence,
exact logical size, adjacent-game similarity, IDA naming/decompilation, a
successful compile, or a Git commit do not grant exactness by themselves.

Prefer ordinary source explanations for target code shape. Do not keep register
forcing, volatile used only for codegen steering, artificial padding,
target-byte embedding, arbitrary var_order, fake returns, or profile roulette.
Narrow source-family-backed exceptions already documented and replayed
canonically are separate from this rule.

After a substantive exact promotion, update the ledgers and docs, run the
validators, replay the affected units, then checkpoint with a commit message of
the form:

    gpt-web: short description

## Work routing

The current <=256 authored/source-present/non-exact routing set is maintained in
docs/SMALL_FUNCTION_FRONTIER.md. Start there, but re-read the live
config/functions.csv row before editing any candidate. Size is a routing
heuristic, not a difficulty score or a verified leaf classification.

Do not resume from old roadmap numbers copied into chat or historical packets.
Recompute from the live ledger. Many former plateaus were later closed by
correcting source shape, translation-unit ownership, return contracts, local
lifetimes, or function-local compiler profiles.

Do not turn target size into a global priority rule. Interleave large owners
with smaller leaves: a large owner can unlock ABI, TU, layout, and stack-lifetime
evidence that closes multiple downstream functions, while short functions remain
useful for focused compiler experiments. In particular, do not defer
EclManager::RunEcl merely because it is large.

For a candidate with extensive negative probes, read its live ledger notes and
the latest relevant knowledge packet before trying another spelling. New target
or source-family evidence is a reason to reopen a plateau; repeating previously
rejected codegen steering is not.

### Current RunEcl handoff

At this checkpoint EclManager::RunEcl is complete maintained source but remains
NON-EXACT. A fresh clean-HEAD pinned VC7.1 build is 14,788/14,792 logical bytes
with an exact 0x168 stack frame, 375 immediate direct calls, four indirect
calls, 598 relocations, resolver counts 131/100/17/24, and all 193 compiler-table
entries present. Only four physical handler-length mismatches remain:
opcode 21 INT_SUBTRACT (-3), opcode 23 INT_DIVIDE (+3), opcode 24 INT_MODULO
(-3), and opcode 155 SET_TIMEOUT_SPELL (-1). All other RunEcl physical handler
lengths are target-length.

The 21/23/24 direct arithmetic spelling is supported by the exact adjacent TH08
source family. TH09-local probes using explicit locals, ternaries, out-helpers,
switch-wide macros, typed operand overlays, and declaration reordering do not
close the residuals. Opcode 155 is semantic/CFG-correct; natural bitfield and
shifted-value spellings retain the same one-byte allocator difference. Reopen
these only with new allocator/TU/lifetime evidence; do not add register forcing,
volatile steering, padding, assembly, or target-byte encodings.

## Boundary and origin closure

All 2,191 tracked candidates have boundary/origin review. Current dispositions
are 979 authored, 1,177 excluded, and 35 deliberately unresolved. The
unresolved set is frozen by SHA-256:

126885e1a6a78ac42b0d81852253714cc1c9eb99141066d495b0029a16ca5695

Do not revisit the 35 unknown entries without genuinely new evidence that can
separate explicit authored source, implicit compiler-generated special members,
or folded/shared ownership.

The review scripts remain:

    python3 scripts/review-transition-data.py
    python3 scripts/review-runtime-residuals.py
    python3 scripts/apply-game-origin-review.py --group compiler
    python3 scripts/apply-game-origin-review.py --group authored
    python3 scripts/apply-game-origin-review.py --group ambiguous

Without --apply, they should report the selected dispositions as already
applied.

## Repository and artifact hygiene

Before editing, inspect tracked and untracked work. Never overwrite another
active reconstruction change.

Keep compiler outputs/probes under build/, .analysis/ databases and caches,
Python bytecode, temporary logs, local toolchains, Wine state, and private
analysis artifacts out of Git.

resources/th09.exe is intentionally local/ignored and is the canonical target
input used by verification/replay. Do not delete it as part of ordinary build
cleanup.

build/ and .analysis/ are disposable and may be removed between work sessions.
Canonical outputs must be reproducible from tracked source/config plus the local
verified target and pinned toolchain.

## Documentation discipline

docs/KNOWLEDGE_BASE.md is an investigation history, not a second live ledger.
Preserve useful negative experiments there, but treat later corrections as
superseding earlier conclusions. Do not copy old packet counts into current
routing docs.

docs/PROGRESS.md and resources/progress.svg are generated from the live tracking
state. Update them through repository scripts rather than manual editing.
