# Etama exact sprite-template table and 23-of-32 initialization (2026-11-07)

The target is the original Japanese TH09 1.50a executable with verified
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This work advances the maintained native C++ source of the **already exact**
Etama `AddedCallback` boss, not a short leaf-only wrapper.

The original Etama controller holds **32** native `BulletTypeSprites` records,
while its resource initializer walks the **23** authored sprite-script rows.
Each row supplies five independent script indices (normal bullet, three
spawn modes, despawn). Treating 32 as the loop limit would overrun the source
script table; the distinction is physical, not a hypothetical extra game mode.
The source loop now derives its upper bound from the true
`kBulletSpriteScripts[23][5]` array instead of a separate literal 23.

Four natural VC7.1 source contexts were cold-compiled and compared over the
**full physical extents**, including local relocation tables:

| Loop source model | Exact siblings | Complete bytes | Fields | Table data |
| --- | ---: | ---: | ---: | --- |
| Maintained `i < 23` | 4 | 1,667 | 73 | 460 bytes exact |
| `i < sizeof(table)/sizeof(table[0])` **adopted** | 4 | 1,667 | 73 | 460 bytes exact |
| Named enum with table-derived count | 4 | 1,667 | 73 | 460 bytes exact |
| Named local constant with table-derived count | 4 | 1,667 | 73 | 460 bytes exact |

The chosen direct `sizeof` form preserves **all original COFF label names**,
relocation offsets and actual object bytes, unlike the latter two variants
which require eleven separately original-bound compiler-local label renames.
The complete 832-byte physical `EtamaController::AddedCallback` remains
exact, along with 232-byte `DrawSingleBullet`, 600-byte `OnDraw` and the
three-byte pointer conversion leaf.

## Independent initialized table proof

The original `_kBulletSpriteScripts` definition at **0x0048E628** consists
of exactly **23 × 5 × 4 = 460 bytes**. SHA-256:
`0879b881ea6bbb3f36369729f6ddc99e83aa56eb95df941c0673ea31024df577`.
The pinned compiled object defines the same 460 byte sequence as initialized,
read-only COFF data. `scripts/inspect-etama-sprite-script-table.py` verifies
that entire payload and all **seven** actual `DIR32` source references,
including their reviewed offset/addend sequence:

`(+0x52, 0), (+0x6B, 4), (+0x84, 8), (+0x97, 12), (+0xB0, 16),
(+0x170, 0), (+0x1DC, 0)`.

Every source reference resolves to its independently decoded original PE
pointer at the same operand address, not merely to a similarly named table.
Target-independent negative tests reject changed data bytes, missing/altered
references and incorrect table shape. The original-target run separately
confirms all 460 bytes and seven referenced original addresses.

Private source-cold experiment reports and the final table audit live in
`.analysis/gptweb-etama-native-template-count-20261107/`. The original
executable, generated objects and private diagnostic packets are not tracked
by Git. All four functions were *already exact*: this is an independently
verified source/data ownership improvement, with **no new exact-function
credit**. The global tally remains 1,034 source-present / 1,021 exact /
13 nonexact. Product closure, runtime, porting, Factory acceptance and
remote publication remain separate and unproved.
