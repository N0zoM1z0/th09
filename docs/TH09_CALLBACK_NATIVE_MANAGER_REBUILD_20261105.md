# Six exact callbacks rebuild with the native GameManager

The CardAttack category query and ExAttack update callbacks 2, 15, 20, 25 and
26 now declare the actual shared GameManager type. Their private GameManager
and side projections conflicted with the native object already included by
their headers. All six committed old sources reproduce that compiler failure
with the committed old header, so this is an existing reconstruction-source
problem rather than a regression inferred from pending header work.

## Source recovery

The callbacks use the existing 0x38-byte native sides and their EnemyManager,
Etama and Player pointers. Three callbacks' Player ANM reads now use the
native `header24.anmFile98` path: offsets 0x24 and 0x98 select the same original
Player+0xBC word. The small fake Player/side/GameManager structs are removed.
No animation, timing, spawn, RNG, descriptor, return or failure branch changes.
Existing descriptor carriers and original call identities are preserved.

The original GameManager root remains `0x4A7D90`. Ten actual COFF references
now use its native identity, retaining independently verified original operand
destinations and actual side-member addends. The manifest changes only these
symbol identities. No original target, field kind, offset, profile or extent
is adjusted to accept a different function.

| Complete owner | Bytes | Relocation fields |
| --- | ---: | ---: |
| CardAttackUpdateNoCategory3 | 24 | 2 |
| ExAttack update 2 | 137 | 5 |
| ExAttack update 15 | 481 | 17 |
| ExAttack update 20 | 405 | 11 |
| ExAttack update 25 | 475 | 17 |
| ExAttack update 26 | 446 | 14 |
| Total | 1968 | 66 |

All six complete functions match with their existing pinned `/O2 /Ob1`
profiles. The fixed source also builds using the old header. Full raw bodies
and actual COFF rows are identical across the old and pending header contexts;
both contexts pass all six original-byte comparisons. The independent old
closure contains 28 tracked source inputs. Thus this checkpoint stands on its
own, without relying on the pending shared-header replay or historical caches.

## Evidence scope

Direct Ghidra attests disk/Program hashes, i386, base, entry and all 685056
file-backed mapped bytes. The five ExAttack entries export completely. The
initial six-entry request reports no database function at the 24-byte CardAttack
entry `0x440CC0`; it is not a successful six-function export. The complete
PE/COFF Oracle independently verifies that leaf's original instructions, data
operand and real EnemyManager call. No database boundary is manufactured.

The ignored `.analysis/replay-frame-copy-20261105/` packet retains the old
source/header closure and all six actual conflict logs, fixed sources in both
header contexts, compiler logs, actual objects and complete original-bound
comparisons. `old-callbacks.json`, `native-callbacks-verified.json` and
`fixed-callbacks-old-header-verified.json` distinguish the observations.

This restores current source rebuild evidence for six already tracked exact
functions. It adds no function-exact, authored-origin, product/runtime or
Factory acceptance credit. Totals remain 1035 source-present / 1022 exact /
13 nonexact. The larger affected-header replay continues independently.

The immutable source checkpoint is
`533bf0267c1e3ac646265fb51688e2d0825eb23d`. The ignored packet's
`callbacks-source-proof.json` is SHA-256
`a2825a29ef2b7053ada10f885282bcd02eebff6543ba43826908b552952a30d9`.
It binds 36 tracked inputs and normalized Git blobs, six actual old-header
objects/profiles, eight pinned compiler/SDK inputs, both matching contexts,
original failure logs, attested exports and all six complete results. Six
Card tests pass; the focused ExAttack suite with the repository scripts on
its import path passes 63 tests with four skips. The later
[native-state review](TH09_REPLAY_NATIVE_STATE_20261105.md) completes the shared
header replay and describes the independent verifier for both receipts.
