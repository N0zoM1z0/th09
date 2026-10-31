# TH09 enemy/ECL leaf cold replays — 2026-10-31

This inventory records focused source-cold builds and original-bound canonical
comparisons made during the TH09 restart. Every row below returned
`result: exact`, `matched_bytes == compared_size == target size`, and an empty
`first_differences` list. This is local PE/COFF comparison evidence, not an
accepted factory oracle receipt or a claim about original source-file/TU
ownership.

No all-owner cold replay was run. Each listed object was rebuilt only when its
source/profile was needed; sibling functions in that freshly built object were
then compared individually.

| Unit / target function | Target VA | Bytes |
|---|---:|---:|
| `ecl-pop-context` — `Th09EclRunControl::PopContext` | `0x00406680` | 147 |
| `ecl-call-subroutine` — `Th09EclRunControl::CallSubroutine` | `0x00407A80` | 149 |
| `ecl-set-primary-anm-scripts` — `Th09EclRunMovement::SetPrimaryAnmScripts` | `0x00406740` | 80 |
| `ecl-apply-interpolation` — `Th09EclRunControl::ApplyInterpolationOperation` | `0x00407560` | 157 |
| `zun-timer-greater` — `ZunTimer::operator>(int)` | `0x00403DE0` | 20 |
| `float3-constructor` — `Float3::Float3(float,float,float)` | `0x004010B0` | 26 |
| `float3-add` — `Float3::operator+` | `0x00401100` | 54 |
| `float3-subtract` — `Float3::operator-` | `0x00401140` | 54 |
| `float3-divide` — `Float3::operator/` | `0x0040F5A0` | 56 |
| `float3-from-angle-magnitude` — `Float3::FromAngleMagnitude` | `0x00441890` | 32 |
| `effect-polar-set-from-angle-axes` — `EffectPolarScaleView::SetFromAngleAxes` | `0x0040D4D0` | 32 |
| `enemy-ecl-get-subroutine-count` — `EnemyEclManagerView::GetSubroutineCount` | `0x0040F160` | 7 |
| `enemy-ecl-get-subroutine` — `EnemyEclManagerView::GetSubroutine` | `0x0040F170` | 16 |
| `enemy-reset-bullet-rank-influence` — `EnemyView::ResetBulletRankInfluence` | `0x0040F180` | 51 |
| `enemy-manager-find-active-by-side-category` — `EnemyManagerView::FindActiveEnemyBySideCategory` | `0x0040F7D0` | 62 |
| `enemy-attached-effect-update` — `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0` | 151 |
| `enemy-collision-append-box` — `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250` | 82 |
| `enemy-core-release-child-ecl-blocks` — `EnemyCoreReleaseChildEclBlocks` | `0x004100E0` | 48 |
| `enemy-manager-release-core` — `EnemyManagerReleaseCore` | `0x00411F30` | 68 |
| `enemy-manager-reset-for-gameplay-cleanup` — `EnemyManagerView::ResetForGameplayCleanup` | `0x00411FD0` | 177 |
| `enemy-manager-release-subsystem4` — `ReleaseSubsystem4` | `0x00411F80` | 74 |
| `enemy-cleanup-after-deactivation` — `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 |
| `enemy-position-clamp` — `EnemyView::ClampPositionToMovementBounds` | `0x004101D0` | 127 |
| `enemy-integrate-motion` — `EnemyView::IntegrateMotion` | `0x0040F9B0` | 188 |
| `enemy-ecl-release-attached-effects` — `Th09EclRunState::ReleaseAttachedEffects` | `0x0040F560` | 64 |
| `enemy-ecl-manager-release-raw-file` — `EnemyEclManagerReleaseView::ReleaseRawFile` | `0x00406830` | 28 |
| `enemy-manager-card-attack-spawn` — `EnemyManagerCardAttackSpawnView::SpawnFromCardAttack` | `0x00440C80` | 38 |
| `enemy-manager-spawn-opposing-ecl` — `EnemyManagerView::SpawnEnemyUsingOpposingEcl` | `0x004417C0` | 38 |
| `enemy-manager-constructor` — `EnemyManagerView::EnemyManagerView` | `0x004121D0` | 101 |
| `enemy-manager-draw-high-prio` — `EnemyManagerView::OnDrawHighPrio` | `0x00411D50` | 35 |
| `enemy-manager-draw-low-prio` — `EnemyManagerView::OnDrawLowPrio` | `0x00411D80` | 38 |
| `enemy-draw-interpolate-wrapped-angle` — `EnemyDrawInterpolateWrappedAngle` | `0x0040F5E0` | 91 |
| `enemy-ecl-load` — `EnemyEclManagerView::Load` | `0x00405660` | 154 |
| `enemy-ecl-opcode1` — `EnemyEclExInstruction1` | `0x0040C3F0` | 76 |

## Boundaries

- The two large owners remain nonexact: `EclManager::RunEcl` is 15,564 bytes
  with 30 ordinary differences (7 in opcode 4, 23 in opcode 155);
  `EnemyManagerView::OnUpdate` is 3,900 bytes with 13 ordinary differences.
- Exact leaf results do not grant partial byte credit to either boss.
- The next source changes must rebuild their touched carrier and rerun only
  the corresponding full-owner diagnostic plus the leaf comparisons affected
  by that carrier. Do not run the all-owner cold replay for routine iteration.
