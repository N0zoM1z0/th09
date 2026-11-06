# TH09 bullet/laser native Player collision receiver (2026-11-06)

The original Japanese TH09 1.50a Etama update is a complete **2,216-byte**
physical owner (2,195 authored code bytes plus its original compiler tail).
In the existing reconstruction it appended bullet box/laser hit records via
four copies of a raw `Player* + 0x36C` cast. This is not a separate game
allocation: the exact Player collision and movement source already defines
`PlayerLifecycleView::collisionQuery36C`, a 0x1808-byte subobject at that
exact offset, with the two original AppendBox/AppendLaser methods.

`src/BulletManager.cpp` now includes that actual native Player header, removes
the duplicated minimal PlayerCollisionQueryStateView declaration and invokes
`player04->collisionQuery36C.AppendBoxRecord/AppendLaserRecord` directly.
Compile-time assertions protect both the native +0x36C offset and the
collision query's 0x1808-byte extent. No new callee, surrogate object, hidden
copy, profile hint, assembly or executable patch is introduced. Original
nominal class/TU identity remains unproved by the matching source names.

## Complete original-bound VC7.1 verification

All **18** exact same-source BulletManager units, in their three actual
canonical compiler carriers, were compiled and compared to the pinned PE.
The 15-owner main carrier has 6,144/6,144 complete physical bytes and
244 actual relocation fields matching. The other carriers have one 161-byte
sprite selector and two 203-byte bullet pattern owners, all complete and
exact. The two largest original-bound main owners are:

| Unit | Complete compared bytes | All field identities |
| --- | ---: | ---: |
| `etama-controller-update` | 2,216 | 88 |
| `etama-spawn-single-bullet` | 1,932 | 47 |

The real header makes VC7.1 renumber **six** compiler-local switch labels in
OnUpdate and **ten** in SpawnSingleBullet. The manifest records their actual
new `$L` symbol names at the **same** 16 original field offsets/types/target
destinations. Every replacement was computed from the candidate COFF's own
section and owner-relative symbol values, independently checked against the
unchanged manifest original addresses, then strict complete `compare_unit`
replay succeeded. No source-call or external field symbol was relabeled.

`python3 -B scripts/check-boss-frontiers.py --owner bullet` passed the bounded
Bullet source/dependency cohort: 30 complete original-bound comparisons,
21,091 physical bytes / 764 bound fields. It is **not** a whole-game cold
replay, 30 newly distinct functions, or remote Factory acceptance.

The ignored reproducibility packet is under
`.analysis/gpt-web-bullet-collision-local-labels-20261106.json`, with the
focused replay summary in `.analysis/gpt-web-bullet-native-collision-focused.json`.
All changed source/member paths and canonical units remain ordinary VC7.1 C++.
The authored exact numerator is unchanged because these owners already matched.
