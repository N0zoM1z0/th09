# TH09 focused Factory replay receipts — 2026-10-31

Forty-four individually submitted, forced-recompile function-exact replay
jobs completed. The first fifteen were bound to clean commit
`9048c2c0fd2084b1dfd6345d30f6dfe150a54036`; the next five were bound to clean
commit `3d91c2dd9b54f27d4c5cbc4f49eba6c2e9ec18a4`; the next five were bound to
clean commit `66fbdb6610e8fbb2a2fef8aaf9b48f121c8466d2`; the next nine were
bound to clean commit `4d720d24042d2006b5754ac4971cff3406b9e5fe`; the final
ten were bound to clean commit `a809cf68230bad4bb36148b96c8315c497d06df9`.
Only documentation files changed between these bindings; no source files
changed. Every job returned `receipt_verdict=pass` and
`acceptance_decision=accepted`; every retained native report returned
`result=exact`, matched the full compared size, and had an empty
`first_differences` list.

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

| `EnemyEclManagerView::Load` | `0x00405660` | 154 | `e540ef77539e2350266440f9eb2d3cf31b4c61d5e9f7eba35bffe2cad7c9dfad` |
| `EnemyManagerView::OnDrawHighPrio` | `0x00411D50` | 35 | `77ca5d767a294e0048c787790e2eb8bcd39d8a9aad8d4a81d5b257cb25a71b` |
| `EnemyManagerReleaseCore` | `0x00411F30` | 68 | `64a6726daf7458db275322a5ca34b69710a042232e7f482d9c9dad9602541bab` |
| `ReleaseSubsystem4` | `0x00411F80` | 74 | `87a6db2576edd83a127e77897217e12a2b78a603221324c2c05488b20a451c97` |
| `Th09EclRunMovement::SetPrimaryAnmScripts` | `0x00406740` | 80 | `e5cc112485003e90338aeebc3fa772e85f04f3591b0803d5680645d0b893cdd7` |

### Five further receipts

| Function | Target VA | Bytes | Receipt |
|---|---:|---:|---|
| `EnemyManagerView::OnDrawLowPrio` | `0x00411D80` | 38 | `1ad5d2ad5921bebab12f242349b23a623a9e12d1e16245c2e394398a593aa795` |
| `EnemyManagerView::SpawnEnemyUsingOpposingEcl` | `0x004417C0` | 38 | `fc59668ddf878ba0d053762aa2436d3ba9a75f97e31a9ac84740ad162847f97b` |
| `EnemyManagerCardAttackSpawnView::SpawnFromCardAttack` | `0x00440C80` | 38 | `05039e2882ac575152559710b10515cb745479a4e809e9351b8cf943db322cb3` |
| `Float3::operator/` | `0x0040F5A0` | 56 | `002595b8749e37b5af7039c1c6fafa66d4109596ab80ce611ed62bc03a956777` |
| `Float3::FromAngleMagnitude` | `0x00441890` | 32 | `40811bf5f9c93508ffb3e786e008b3662378a2ff1b1cd8b512dc30a84a0852e7` |

| `Float3::operator+` | `0x00401100` | 54 | `ec9435aa993a45042da4fefa5af8bc2faa9028cabe99be7b481d9f3574202497` |
| `Float3::operator-` | `0x00401140` | 54 | `b37708e238bbb90b258ce509cbad738bb3f436ec17cc90653ce732e776431bcc` |
| `ZunTimer::operator>` | `0x00403DE0` | 20 | `c5b50148d8639a9c0cb26f5c8d1ee8102b2345cac5f37c175e2d62b972e22b75` |
| `EnemyEclManagerView::ReleaseRawFile` | `0x00406830` | 28 | `131ddaae42d157c5e3487960b49c2034b2b3be6ca0a1dfcd5f1ebc09c2f5dc50` |
| `EnemyEclOpcode1` | `0x0040C3F0` | 76 | `96cecb6bc7118df1add92f23ba65904146ee41c9b73d1208fdbea8b7bf4ed2f6` |
| `EffectPolarView::SetFromAngleAxes` | `0x0040D4D0` | 32 | `8e2835a345e7d90a0c3594a0069651525ee9e4ff5517839f02d5bf5e96bfe242` |
| `EnemyEclManagerView::ReleaseAttachedEffects` | `0x0040F560` | 64 | `77b7de521534f8ff577a0647705f418011ba649c133f3e5449aae97a9db9fe3b` |
| `EnemyView::DrawInterpolateWrappedAngle` | `0x0040F5E0` | 91 | `584ab53bfe9d2b14e0195a8809253dfdcd339b69be276baefe15c48939daaa85` |
| `EnemyCoreReleaseChildEclBlocks` | `0x004100E0` | 48 | `0f5e861e85a020d8862e019a73eb6ddb936467848325d35cc966296f767e60b6` |

| `EnemyManagerView::DrawImpl` | `0x00411670` | 1,758 | `50195a68273e92fb0ed1026aaf5b558589714e08f64d8b89fadf461272137567` |
| `EclResolveFloat` | `0x004068A0` | 1,633 | `891884069a4002b649337421e38e89f33abade97a2793e3ea22022f38b96f3f7` |
| `EclResolveInt` | `0x00405760` | 1,572 | `6102ced8582565eeaac3c97c753e90dc000332681808107649c4410d1fb7c7fc` |
| `EnemyHandleDeathRewards` | `0x004102B0` | 1,142 | `ed8b323eea0ffa447d10a6f0e2a7603702977037b09f9b5377a52ac220ad8ef2` |
| `EnemyScheduleRuntimeRun` | `0x0040FCC0` | 974 | `6f15721406d0c5b49b6235c357101ee202033b3ca37564e18c213e76c5561098` |
| `EclPostUpdateMovement` | `0x00408180` | 967 | `ad22885ee620e15b9ee23a90edb39fd53053304d1e2d11a1ec242032cf8dead2` |
| `EtamaControllerView::Update` | `0x004146F0` | 2,195 | `fc78f5ca0b231706013a4ca7e9099e0f97f206132fc849d81b20821f8b445c5c` |
| `EtamaControllerView::SpawnSingleBullet` | `0x00412960` | 1,893 | `a365fc18c2b3b753d9767df1ddfb0ca33d3bf767b7d25be380cdee6e59532d07` |
| `EnemyManagerView::Initialize` | `0x0040FA70` | 554 | `65aed519f9b67c89f43447031c9ad5573bb821fb6ff187da6d572a68f3098284` |
| `EnemyManagerView::SpawnEnemy` | `0x0040F340` | 373 | `e6df96ae3829c9519fd586c4bffa7997331c703d3eef6b502c49902ec543904c` |

**Total: 15,766 bytes across 44 exact function reports.**

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
