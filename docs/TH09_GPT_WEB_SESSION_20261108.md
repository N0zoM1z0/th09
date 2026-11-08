# TH09 reconstruction session — 2026-11-08

This note records fresh live-worktree and target-attested observations from the 2026-11-08 session. It is not an acceptance claim by itself; Factory replay receipts are listed separately below.

## Reproducible baseline

- Repository: `th09`, branch `main`, source HEAD `85fb775d42c727d7c9c28cfd56f0958720ab2111`.
- Original target: TH09 1.50a Japanese PE, SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
- Native IDA provider `th09-ida` re-attested the target successfully.
- The live worktree was clean before and after the focused experiments. No pre-existing dirty or untracked user changes needed resolution.
- Builds were limited to the relevant EclManager translation unit and targeted function replays. No full cold replay was submitted.

## Large owner: `EnemyManagerView::OnUpdate`

Fresh build: `scripts/build-match-unit.py --unit enemy-attached-effect-update` followed by `scripts/inspect-enemy-manager-update.py build/matching/EclManager.obj --json`.

- Target and candidate extent: 3,900 bytes each; 3,883 code bytes.
- 97 relocation fields agree; 69 ordered direct calls agree; 170 reviewed direct control-flow blocks agree; switch destinations agree.
- 13 byte differences remain at owner-relative offsets:
  `0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294, 0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD`.
- The inspector is diagnostic-only and grants no exactness credit.

New local source-shape probes were compiled and then discarded because none reduced the 13-byte frontier: draw-group pointer alias, unsigned and byte index locals, byte-offset pointer arithmetic, side-state pointer alias, direct side-state accesses, memberwise spawn-position copy, and a local `Float3` copy. No speculative variant was retained.

The current source still calls the separately reconstructed 15,564-byte `EnemyEclManagerView::RunEcl` owner. Its 30-byte frontier and the shared callback origin remain independent issues; the caller's structural agreement does not promote those callees.

## Large owner: `EnemyEclManagerView::RunEcl`

Fresh focused build: `scripts/build-match-unit.py --unit enemy-attached-effect-update`; comparison: `scripts/inspect-ecl-complete.py`.

- Physical owner: 15,564 bytes / 598 relocation fields.
- 30 ordinary byte differences remain, at offsets `0x278, 0x27A-0x27E, 0x280` and `0x30B0-0x30C2, 0x30C4-0x30C6, 0x30C8, 0x30CA`.
- Target-attested IDA disassembly independently confirms two concentrated regions:
  - ECL opcode 4 jump/timer assignment at `0x408937`: target loads the raw dword into ECX, pushes it, then loads the active timer receiver and calls `ZunTimer::operator=(int)`. The direct-union-member expression was tested; it increased the owner mismatch count from 30 to 42, so the original wire-argument snapshot was restored.
  - ECL opcode `0x9B` at `0x40B770`: target uses the byte at instruction `+0x0C`, reads the primary flag word at Enemy `+0x337C`, and performs the masked XOR update of bit 24. The bitfield-assignment and local-copy variants shifted the owner substantially (2,140+ or 2,290 differences); those variants were discarded. The original raw-byte snapshot plus `AssignTimeoutSpellBit` source remains the reproducible 30-difference baseline.
- The opcode-4 and opcode-`0x9B` target instruction sequences are captured in the session's target-attested analysis artifact; they are provisional semantic evidence, not exactness receipts.

## Accepted small Enemy leaves

Each claim below was submitted as an individual forced-recompile Factory replay against the clean source binding at HEAD `85fb775d42c727d7c9c28cfd56f0958720ab211`. Every job completed with `receipt_verdict=pass` and `acceptance_decision=accepted`:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0` | 151 | `receipt:00dcd2786f7d1e832f5f2dae9f2d6dd095c0adcc4d761f8dfe580cccb5b934a7` |
| `EnemyCoreReleaseChildEclBlocks` | `0x004100E0` | 48 | `receipt:fee8d5701ced8bd6585fa68ae67aabe081f9a6aeb733c755199a272e39b0ac49` |
| `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 | `receipt:07d301aeccaa5f2bb886579284c057808a4b9f748700d7333fa613064d9e7da5` |
| `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250` | 82 | `receipt:6b6fda568ea5ac576e4d78a7ba34807cab85498a5347757a3f95d970274b5b3b` |

These are independent accepted leaves. They do not imply that `OnUpdate` or `RunEcl` is exact.

## Next work

1. Continue attacking `EnemyManagerView::OnUpdate` as a large owner; use the exact 13 offsets above as the current frontier, and seek a source-level explanation for the register and descriptor-store order rather than register steering.
2. Continue attacking `RunEcl` as a large owner; keep opcode 4 and opcode `0x9B` isolated so one experiment cannot shift the other 598 relocation fields.
3. Continue with additional small leaves and replay each exact claim independently.
4. Revalidate every adopted source variant against the live original target; imported notes and historical trials are leads, not proof.
