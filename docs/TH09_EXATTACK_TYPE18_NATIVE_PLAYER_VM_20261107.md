# Type18/24 ExAttack uses native Player, ANM VM, and angle receiver

Target: Japanese TH09 1.50a, verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a fixed-profile C++ reconstruction checkpoint, not an original
nominal type/TU identity claim, full Windows product link, or runtime proof.

## A 1,436-byte complete owner, not only an inline leaf

The original exact `ExAttackUpdateCallbackType18_24` at 0x004491E0 handles
interpolation, four VM instances, trail history, collision, rotation,
attack timing, and resource switching. It previously used synthetic
`ExAttackType1824PlayerView`, side/GameManager, VM and VM-position classes,
all overlaying the same physical allocations as the existing native Player,
GameManager, and animation views. The source now uses the genuine two-side
`AsciiGameManagerView` and `PlayerLifecycleView` pointer, whose embedded
`header24.anmFile98` lies at native Player +0xBC and collision query at
+0x36C. Four VM objects are now native `PlayerAnmVmView` instances of size
0x2A4, with the real VM position field at +0x288.

The established original-exact `PlayerAngleView::AngleToPoint` at 0x0041BB70
still has its own source-facing float-vector operand type. Its caller now
casts the genuine Player receiver to that existing *verified projection*
only at this method boundary and passes a matching position projection. A
new compile-time check also proves that both Player receiver types locate
the world position at +0x1B88, with identically sized and positioned X/Y
components in the 12-byte argument view. This uses normal source-level
layout checks, not a register hint, barrier, or method thunk.
The remaining Player fields use their actual native owner. No new
`PlayerLifecycleView` method was invented or assigned a fabricated target.

Four original VM-rotation calls bind the independently exact 20-byte
`PlayerAnmVmView::SetZRotation` at 0x0040F840; four interrupt calls bind
the 18-byte exact `AnmVm::SetInterrupt` at 0x00406790. The other recovered
original methods are Player collision at 0x0041DFF0 (still **nonexact**),
angle at 0x0041BB70 (exact), and the shared 95-byte circle append at
0x00440D30 (exact). The original order of frame updates, comma-sequenced
VM position/rotation publication, attack flags, and return branches is
unchanged.

## Full physical original-bound validation

Two natural C++ variants were compiled with the same pinned VC7.1 profile:
first native Player/GameManager plus the verified angle projection, then
that source with genuine ANM VM members. Both reproduced every byte of
the prior complete exact body and preserved each of its 44 relocation
sites and addends. The adopted model was then independently verified
against the immutable retail PE: **1,436/1,436 physical bytes and all 44
actual fields**, with no differing code/table bytes or inferred target
adjustments. The 17 changed symbol names are: seven GameManager +4
references, one Player collision call, one genuine angle call, four real
VM rotation calls and four real VM interrupt calls. The previously
recorded original destination and actual field addend were checked for
every changed symbol before updating only the manifest spelling.

Fresh scope-cold canonical replay passes the complete Type18/24 owner.
Separately, source-cold replay of its native providers passes **nine
existing exact functions / 628 bytes / 29 fields** across the actual
angle, rotation, interrupt and ANM-loaded source carriers. Combined,
that is **ten distinct exact functions, 2,064 bytes and 73 fields**.
These are existing matches protected during a source-type refactor;
no new authored exact credit is claimed. Target-bound private evidence:
`.analysis/gpt-web-exattack-type18-native-player-20261107/`, including
`full-original-audit.json`, `source-exact.json`, `providers-exact.json`,
and both independently compiled source variants.

Focused source/ABI and ExAttack regression tests pass. The global tally
remains 1,034 source-present / 1,021 canonical exact / 13 nonexact.
No full-all cold replay, semantic/portability gate, Factory acceptance,
whole-game link, or remote push is claimed.
