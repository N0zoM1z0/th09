# Native VM alpha and sprite-index ownership — TH09 (2026-11-06)

The physical TH09 v1.50a animation VM has a 0x2A4-byte maintained native
layout. The 811-byte `AnmManager::Draw3D` owner previously checked VM+0x1F3
through an opaque byte pointer. The original VM's +0x1F0 dword is the shared
color1 ARGB payload (already consumed by native animation scripting). The
reconstruction now reads `vm->color1Script.a`, explicitly checked by
`offsetof(AnmVmBase, color1Script) + offsetof(AnmVmScriptColor, a) == 0x1F3`.
The alpha==0 early exit and all subsequent rendering calls remain unchanged.

The exact `AnmVm::AnmVm` leaf previously wrote -1 to a `short` at the opaque
VM+0x214 address after clearing the VM. The already maintained base layout
identifies this as `activeSpriteIndex`. The leaf now sets
`activeSpriteIndex = -1`, guarded by the actual field offset. It does not
add a new secondary allocation, constructor call, byte patch or register hint.

With pinned original Japanese 1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
focused VC7.1 original-bound comparisons were rebuilt and verified:

| Unit | Complete original physical bytes | Result |
| --- | ---: | --- |
| `anm-manager-draw-3d` | 811 | Exact |
| `anm-vm-constructor` | 45 | Exact |
| `anm-vm-base-constructor` | 62 | Exact |

Reproduce with `scripts/build-match-unit.py --unit UNIT` followed by
`scripts/compare-coff-function.py --unit UNIT` for each named unit; the full
product has not been built or runtime-tested. These functions were already
counted as exact. The checkpoint improves game source's real ABI/data model,
not its authored exact-match numerator or original nominal class certainty.
