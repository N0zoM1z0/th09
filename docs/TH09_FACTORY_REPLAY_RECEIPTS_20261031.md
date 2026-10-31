# TH09 focused Factory replay receipts — 2026-10-31

Fifteen individually submitted, forced-recompile function-exact replay jobs
completed against the clean live source binding at commit
`9048c2c0fd2084b1dfd6345d30f6dfe150a54036`. Each job returned
`receipt_verdict=pass` and `acceptance_decision=accepted`; its retained native
report returned `result=exact`, matched the full compared size, and had an
empty `first_differences` list.

These are targeted function replays, not a whole-project cold replay.

| Function | Target VA | Bytes | Receipt |
|---|---:|---:|---|
| `Float3::Float3` | `0x004010B0` | 26 | `4af15c316b212641cf6ad7995f8fd3d9f3dc9859aebc4ddcaa17ee7d2398b17c` |
| `Th09EclRunControl::PopContext` | `0x00406680` | 147 | `67402c36f309d600fef1a64ec6589a7ce82c141e546937d1f454894e6259c85b` |
| `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0` | 151 | `6afce47df0c14ecdacc26614262c845b1546ce61d2ee17b615a1d58ee5b6a3bb` |
| `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250` | 82 | `0ec92bc17416671ca8c84123382aa73e453bfbdc9937b02fcdbd53b69fb175ac` |
| `EnemyEclManagerView::GetSubroutineCount` | `0x0040F160` | 7 | `1a4b0f3ccb9fb6372861ac0971fc205d4f44e94a43e63fdbee14182fe37228f5` |
| `EnemyEclManagerView::GetSubroutine` | `0x0040F170` | 16 | `7bc3347e2563ab5acfc77655459a8f0485480b928e5c2b333ca3e31e2404007a` |
| `EnemyView::ResetBulletRankInfluence` | `0x0040F180` | 51 | `522ad3b3261d6e4488fa7347514aa782756d56b8babbac4d151aaab72d716817` |
| `EnemyManagerView::FindActiveEnemyBySideCategory` | `0x0040F7D0` | 62 | `dfa5ac7abdb13ca3f6eb3f63258fe61e546403dee0554bb140692406759749c9` |
| `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 | `6f67e225c04894f56b75a12b5775c151f7e7fbdae889f863cb6e1c1a1792a714` |
| `EnemyView::IntegrateMotion` | `0x0040F9B0` | 188 | `398283e48797e50385cfcb5c545d2d371af6a0b2f480c62deab8e0a40f5ea198` |
| `EnemyView::ClampPositionToMovementBounds` | `0x004101D0` | 127 | `8f2eb653b547b067e205c6931a781850d4ae01ae9528cdf21a18e5f619f7a498` |
| `Th09EclRunControl::CallSubroutine` | `0x00407A80` | 149 | `06e6f073c0563938e75fa9b81968832a25229a706294eb007723d4504e73250e` |
| `Th09EclRunControl::ApplyInterpolationOperation` | `0x00407560` | 157 | `799114b50176e51b2718b3c5df2f2f21e136fc18daa2162ca495a1a7082fa10b` |
| `EnemyManagerView::ResetForGameplayCleanup` | `0x00411FD0` | 177 | `27578cc36f52fbb37a0946e2da878669f0dbf1121d4a26903be3d2ece39aae20` |
| `EnemyManagerView::EnemyManagerView` | `0x004121D0` | 101 | `1fdef9aebbf265619b655e8191aaad70da77f8fc1e45c6fe110b3cba69efb1c6` |

**Total: 1,625 bytes across 15 exact function reports.**

## Scope and limitations

- The two boss owners are still not exact: `EclManager::RunEcl` retains 30
  ordinary differences, and `EnemyManagerView::OnUpdate` retains 13.
- These function receipts do not prove source-file ownership, original
  translation-unit partition, whole-object ownership, whole-image closure, or
  runtime behavior.
- A registry consistency discrepancy remains open. The individual job records
  report pass/accepted, but the aggregate accepted-facts query still returns
  zero OracleResults and the acceptance-registry summary reports zero accepted
  candidates. Do not infer aggregate registry promotion from these job records;
  retain the individual receipt IDs as the direct evidence and investigate the
  registry synchronization separately.
