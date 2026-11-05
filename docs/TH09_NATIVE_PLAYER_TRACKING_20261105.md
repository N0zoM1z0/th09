# Native Player tracking consumers, 2026-11-05

The bounded continuation starts at clean `f62998d3`. The original target,
tracking and reconstruction status checks pass. Fresh local Ghidra exports
cover Enemy update, deactivation cleanup and SHT callbacks Update1, Update3
and Spawn7. Disk/Program SHA, i386, base/entry and all 685056 file-backed
mapped bytes are independently attested. Ghidra remains semantic evidence;
the pinned compiler and complete original PE/COFF comparisons decide equality.

## Four exact leaves

The three SHT callbacks now take the existing `PlayerLifecycleView *`, removing
three callback-only Player layouts. Cleanup also reads that native Player.
Their shared tracking component projects the existing opaque region at
`+0x30364`; the homing pointer is at `+0x3037C`. The new shared header checks
the native member offset, available storage and component pointer offset.
The opaque region is retained: introducing a constructed `Float3` member
would also change the native Player's construction obligations.

The actual SHT table cells independently retain Spawn7 at `0x0044AB90`,
Update3 at `0x00443150` and Update1 at `0x004415E0`. The fastcall registers,
stack arguments, complete bodies and actual external referents are unchanged.
Only the three function symbols' nominal Player parameter types change in
the canonical recipes; no target destination or relocation row changes.
Original nominal class names and complete Player ownership remain provisional.

Fresh old/new builds preserve all raw bytes and actual COFF identities.
The maintained source then rebuilds under each existing profile and passes:

| Owner | Complete bytes | Fields |
| --- | ---: | ---: |
| Enemy cleanup | 184 | 2 |
| SHT Update1 | 471 | 19 |
| SHT Update3 | 248 | 9 |
| SHT Spawn7 | 139 | 5 |
| Total | 1042 | 35 |

Twenty-two configured callee/sibling units are freshly rebuilt: ZunTimer,
HasTicked, angle helpers, Player forward angle and shot initialization.
Three unchanged source/profile/object-bound callee proofs are reused and
fully compared: attached-effect release, child-block release and the SDK
vector pointer alias. Their source inputs and objects still match the
sealed `c8a0bc15` receipt. Fixed CRT/trigonometric call destinations remain
covered by the complete leaf relocation comparisons.

Enemy tests pass91 with three skips; Player tests pass114 with two skips.
Two old source-spelling assertions requiring the retired cleanup Player
projection are removed; existing physical owner/field and byte guards remain.
No new runtime matrix or source-format acceptance test is added.

Reproduce the four leaves with their existing unit names:
`enemy-cleanup-after-deactivation`, `player-shot-update-callback-type1`,
`player-shot-update-callback-type3`, `player-shot-spawn-callback-type7`.
Build with `scripts/build-match-unit.py --unit NAME`, then use
`scripts/compare-coff-function.py --unit NAME --json`.

The ignored packet `.analysis/runecl-current-handlers-20261105/` retains the
frozen91-file old/native contexts, actual callback-table operands, Ghidra
attestation, complete private closure comparison, compiler commands/logs,
canonical leaf/callee reports and focused test logs. Build objects remain
under `build/`. These are four preserved existing exact functions, not four
new matches. Totals remain1035 source-present/1022 exact/13 nonexact;
Windows product, runtime and later phases remain open.
