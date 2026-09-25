# TH09 short-function frontier

This is a live routing snapshot, not an exactness ledger.

Authoritative state lives in config/functions.csv; canonical exact credit
requires config/matches.csv plus config/match-units.toml. If this file ever
disagrees with those ledgers, re-filter the ledgers and update this file before
starting work.

The filter is:

- confirmed authored;
- maintained source present;
- not canonical exact;
- target logical size <=256 bytes.

At the current checkpoint it yields **11 functions**, of which **2 are at most
128 bytes**. The repository-wide canonical total is **872 exact functions**.

Size is only a routing heuristic. This is not a verified leaf-function set, a
difficulty ranking, or evidence about whole-product closure.

| Target address | Bytes | Module | Current non-exact source function |
| --- | ---: | --- | --- |
| 0x0040D1C0 | 51 | EffectManager | EffectManager::AddedCallback |
| 0x00410250 | 82 | EnemyManager | EnemyAppendCollisionView::AppendPlayerCollisionBox |
| 0x00424FB0 | 175 | TitleScreen | TitleScreenView::SetCharacterCursorReverse |
| 0x0042505F | 175 | TitleScreen | TitleScreenView::SetCharacterCursorInactive |
| 0x00442220 | 204 | Player | PlayerShotDrawCallbackType1 |
| 0x00424EDD | 211 | TitleScreen | TitleScreenView::SetCharacterCursorActive |
| 0x0042E9E0 | 213 | Supervisor | SupervisorFrameQueueView::InsertReceivedFrame |
| 0x0042C290 | 220 | FileSystem | FileSystem::TryDecryptFromTable |
| 0x00432240 | 243 | SupervisorNetwork | ParseNetworkConfigValue |
| 0x00443B10 | 249 | Player | PlayerPositionCallback30404Type4 |
| 0x00447620 | 249 | Player | PlayerPositionCallback30404Type8 |

## How to use this list

Before changing any row:

1. Read its current config/functions.csv evidence and notes.
2. Recompile the maintained source under the recorded local profile.
3. Check the latest relevant docs/KNOWLEDGE_BASE.md packet for bounded negative
   experiments and later supersession notes.
4. Reopen an old plateau only when fresh target evidence, source-family evidence,
   TU ownership, ABI/return-contract evidence, or a newly exact dependency gives
   a reason to do so.

Do not copy historical candidate sizes or packet-era frontier counts into this
file. Once a function becomes canonical exact, remove it from the table rather
than retaining a historical note here.
