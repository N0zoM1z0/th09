# TH09 boss re-investigation — 2026-10-31

This checkpoint records independent native IDA decompilation and rejected
source-shape experiments. The purpose is to keep the current hypotheses
falsifiable; none of these diagnostics grants byte exactness.

## RunEcl: independently rechecked opcodes 4 and 155

Native IDA decompilation of `0x004086C0` independently agrees with the
Ghidra-bound interpretation:

- Opcode 4 calls `ZunTimer::operator=(int)` (`0x00401500`) with the signed
  dword at instruction `+0x0C`, then advances by the dword at `+0x10`.
- Opcode 155 updates only bit 24 of the enemy primary flags using the masked
  XOR transition:

  `old ^ ((old ^ (wireByte << 24)) & 0x01000000)`

The original native expression is not evidence that a direct C++ spelling
will reproduce VC7.1's global register allocation. The current maintained
`memcpy`/helper spelling preserves the complete owner except for the seven
JUMP bytes and 23 timeout-opcode bytes. The behavior guards remain
non-crediting and cover 90 JUMP combinations and 3,072 timeout-bit cases.

### Isolated source-shape trials (all rejected and restored)

The opcode 4 target sequence is `mov ecx,[ebx+0x0C]; push ecx; mov
ec x,[edi+0x2CE0]; add ecx,8; call 0x00401500`. The maintained C++ form
emits the same call and timer semantics, but allocates the argument in EAX and
orders the push after loading the timer object. Testing `register int value`
did not change the 30-byte residual. Replacing the `memcpy` load with a direct
operand expression, or initializing from `RawInt`, increased the residual to
42 ordinary bytes; neither was retained.

For opcode 155, the target loads the byte with `movzx ecx,[ebx+0x0C]`, keeps
the old flags in ESI, performs the masked XOR, and stores the result through
EDI. The following experiments did not preserve the owner:

- A direct `RawByte` expression with explicit shifted-value/old-flags locals
  produced 2,048 ordinary differences, 242 relocation-field differences, and
  26 switch-table differences.
- A combined IDA-shaped direct-operand trial produced 2,060 ordinary, 242
  relocation-field, and 26 switch-table differences.
- Inline assembly and `_emit` raw bytes both produced 1,901 ordinary, 201
  relocation-field, and 25 switch-table differences. The opcode-specific
  semantic audit correctly refused to run because the original full RunEcl
  owner layout was no longer present.
- A pre-shifted helper signature and explicit new/old flag locals retained the
  baseline 30-byte residual without reducing it.

All such experiments were restored. The owner inspector and semantic guards
were not weakened. These results indicate that the remaining mismatch is
coupled to VC7.1's whole-switch register allocation and local ownership, not
just the arithmetic expression in either case.

## EnemyManager::OnUpdate: independently rechecked special attack branch

Native IDA decompilation of `0x00410730` confirms the special-attack flow:

1. Test the enemy secondary flag `0x1000`.
2. Call the timer's `HasTickedEvery(2)`; if true, spawn effect 9 at the enemy
   position through the side's EffectManager.
3. Compare the special-attack timer with manager threshold `+0x2AC44C`.
4. If elapsed, construct the bullet descriptor, copy enemy position, derive
   count from side character index `+0x20`, compute speed from the global
   value at `0x004A7E44`, set the remaining descriptor fields, and call the
   Etama primary spawn routine.
5. Deactivate and clean up the enemy.

The decompiler exposes the same observed data: enemy position at `+0x2D74`,
side state at manager `+0x320`, character index at side `+0x20`, and the
special threshold at manager `+0x2AC44C`. This corroborates behavior, not
original C++ names or translation-unit ownership.

A focused source trial replacing the descriptor's aggregate position copy
with three explicit scalar locals moved the native RunEcl call relocation in
the shared object from candidate offset `0x397` to `0x399`; the owner inspector
correctly rejected that layout. Replacing the duplicate manager view with the
public `EnemyManagerView` type, and testing a separate TU, did not reduce the
13-byte residual. Removing the local descriptor alias and writing directly
through `storage.descriptor` produced an OnUpdate object byte-identical to the
baseline (3,900/3,900 bytes), so it was not a codegen improvement. Those
experiments were restored.

## Restored live baseline

After all rejected trials, the live worktree is clean and only the two
boss-frontier diagnostics were rerun:

- `EclManager::RunEcl`: 15,564 bytes; 30 ordinary differences; zero relocation
  field and table differences. The only residual groups remain opcode 4 (7)
  and opcode 155 (23).
- `EnemyManagerView::OnUpdate`: 3,900 bytes; 13 ordinary differences; all 97
  relocation fields agree; 69 calls agree; reviewed control flow and all
  switch destinations agree.

Do not interpret a source spelling that changes the owner's physical extent,
relocation positions, or switch table as a local improvement. Rebuild only the
shared ECL/Enemy carrier and rerun these two owner inspectors after relevant
source changes; no all-owner cold replay is required.
