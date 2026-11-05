# Cirno-Labs integration — 2026-11-05

The operator authorized integrating useful reference implementations, crediting
them in commits, and pushing progress on `reconstruction/cirno-labs-integration`.
The immutable source reference is [Cirno-Labs/th09 at d7653152](https://github.com/Cirno-Labs/th09/tree/d7653152becabb8c86f2f43d49015ecc11bd0652).
Adaptations retain GPL-3.0-only terms and the original MIT grants as described
in [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md).

## Adopted complete owners

| Original entry | Maintained native owner | Physical bytes | Fields | Canonical unit |
| --- | --- | ---: | ---: | --- |
| `0x00424EDD` | Title active cursor | 211 | 6 | `title-character-cursor-active` |
| `0x00424FB0` | Title reverse cursor | 175 | 2 | `title-character-cursor-reverse` |
| `0x0042505F` | Title inactive cursor | 175 | 2 | `title-character-cursor-inactive` |
| `0x0042E9E0` | Supervisor received-frame insertion | 213 | 0 | `supervisor-frame-queue-insert-received` |
| `0x00423D16` | Title result drawing | 1939 | 82 | `title-screen-draw-result` |
| `0x004276EB` | Title options update | 2045 | 135 | `title-screen-options-update` |
| `0x0042B410` | Controller input | 808 | 32 | `controller-get-controller-input` |
| `0x00431110` | Supervisor update | 996 | 92 | `supervisor-on-update` |

| `0x004162D0` | Front weighted random message setup | 693 | 15 | `front-message-random-setup` |
| `0x00418480` | Front added callback | 706 | 37 | `front-added-callback` |
| `0x004239F6` | Title replay-save drawing | 800 | 41 | `title-screen-draw-replay-save` |
| `0x00442750` | ExAttack Type3 update | 780 | 38 | `ex-attack-update-type3` |
| `0x00432B70` | Supervisor network-message handler | 808 | 46 | `supervisor-network-message-handler` |
| `0x00444130` | ExAttack Type14/22 drawing | 959 | 53 | `ex-attack-draw-type14-22` |

All fourteen reproduce every original byte and relocation under the existing
pinned local compiler profiles: **11308 physical bytes, 581 fields**. There
are no assembly blocks, register hints, dummy effects, added defensive
branches or fabricated return values. The cursor loops use the existing
native VM elements and methods; the queue scan and shift use the existing
Supervisor-owned record array. Original Ghidra/PE evidence independently fixes
their physical fields, branch behavior, ret16 ABI and ignored EAX result.

The complete Title source cohort is replayed by `check-boss-frontiers.py
--owner title`, including both existing compiler profiles and the three new
canonical units: **49 canonical comparisons pass**. Nine compiler-private labels changed spelling; each was
checked against its **actual current COFF section-relative definition** and
the unchanged original target VA. No original target, field kind or offset
was adjusted. The evidence packet retains `labels-TitleScreen.json`.
The real SetSprite callee also cold-replays exactly (189 bytes/four fields).
The complete ExecuteScript callee replays all **9776 bytes/368 fields** plus
its four exact helpers. That fresh build exposed 107 stale private-label
spellings in its old manifest; their actual unchanged COFF destinations were
proved and only the spellings refreshed. The source and original targets
remain unchanged; see `labels-AnmManagerExecuteScriptExact.json`.
The four variable helpers (184/132/148/256 bytes) likewise replay exactly;
their separate carrier required 41 proven label-spelling refreshes, retained
in `labels-AnmManagerExecute.json`.
Queue replay includes the two pre-existing exact queue consumers and the
new received-frame unit; its frontier now requires zero differences.
All three queue comparisons pass. Full target-independent CI passes **1329
tests (29 skipped)**, tracking, documentation, progress and whitespace checks.

The first four-function checkpoint is `f4b4c902`, independently verified on
the remote integration branch. The second batch adds result/options, controller
and Supervisor update. Result drawing removes the old compiler-only barrier;
the natural ranking/name-keyboard source now reproduces all 1939 original bytes.
Options includes our real scrolling helper body, visible just as it is in the
reference: its emitted 42-byte body independently equals the original, and
the options caller becomes exact without a new ABI or compiler profile.
Controller preserves the native 0x8E input stride and signed configuration
threshold reads. Supervisor includes all four switch tables, not only its
844 authored bytes. Focused controller and Supervisor replays pass three and
eleven canonical comparisons respectively.
The eight-owner checkpoint passes full target-independent CI: **1329 tests,
29 skipped**, plus tracking, documentation, progress and whitespace checks.
The retained historical nonexact diagnostic envelopes do not grant credit;
the newly registered canonical units require complete zero-difference replay.

The ledger changes from **1005 to 1019 exact functions** and from
247597 to **258713 exact authored bytes (88.6359%)**; the 291883-byte reviewed denominator
is unchanged. Whole-product/native-runtime/Factory acceptance is separate.

The complete fourteen-owner integration passes full target-independent CI:
**1329 tests, 29 skipped**, plus target/tracking, documentation, generated
progress and whitespace checks. The fourteen new canonical units separately
replay all **11308 physical bytes /581 fields** with zero differences. Restored
Etama source retains both earlier exact siblings and its 268-byte diagnostic.
This is a local exact-source checkpoint, not Windows product/runtime or Factory
acceptance. The first two source checkpoints are `f4b4c902` and `944870a9`;
Git records the following six-owner checkpoint without a self-referential hash.

## Reproduction and evidence

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/check-boss-frontiers.py --owner title
python3 -B scripts/check-boss-frontiers.py --owner queue
python3 -B scripts/ci.py
```

Only the affected owners/dependencies are compiled. The focused complete-byte
inspectors remain diagnostics; exact credit comes from the new canonical
match units. Full target-independent CI is required here because a shared
frontier cohort and zero-difference guard changed. Original-bound JSON,
compiler logs, label proofs and newly attested Ghidra exports are retained
below `.analysis/cirno-integration-20261105/`; objects stay below `build/`.
The target is the pinned Japanese v1.50a executable, never an upstream build.

The third batch's focused Front, Front-added and Type3 runs protect five,
five and eight canonical units respectively. Title replay again passes 49
listed canonical siblings and separately requires the new 800-byte save draw.
Historical word-order/native-context controls now read an explicitly frozen
pre-Cirno Type3 root from `scripts/fixtures/type3-pre-cirno.cpp.txt`, with its
immutable source commit and SHA documented beside it. They still hash live
headers and clearly report historical input provenance. Current Type3 acceptance
uses its new canonical unit, not those obsolete source hypotheses.

## Third native-source checkpoint and rejected candidates

The latest six owners add 4746 physical bytes and 230 fields. Front weighted
selection uses real table indices and fallback scans. Front initialization
uses indexed native vertices and `AnmLoaded::rawData04`, deleting an unused
private eight-byte resource view. Replay-save drawing uses native replay data
and SDK vector operations. Type3 keeps three real collision positions in its
ordinary function scope; the original middle-Y / middle-Z / tail-X chronology
is recovered, with all 780 bytes and 38 fields equal.

The network handler comparison includes 768 code bytes, all six switch targets
and the 16-byte switch map: 808 physical bytes in total. Its received-frame
argument is unsigned 16-bit, as corroborated by the original caller's
zero-extension of the packet input and the exact callee's low-word stores.
The seed remains signed short; four physical argument slots and `ret 16`
are unchanged. Canonical callers retain fixed original target addresses;
only the target-consistent C++ decorated parameter name changes. Mode4 reset
(177 bytes) and synchronized-frame query (49 bytes) also replay exact.
The still-nonexact network service body is unchanged, but the shared narrow
parameter changes its compiled result from 1626 bytes/1376 unequal overlaps/
seven missing to 1627/1381/six missing. Eighteen previously equal positions
change and fourteen unequal positions become equal; all 106 field identities
and 41 direct-call destination multiplicities remain independently bound.
This reviewed consequence is not reported as an improvement or exact credit.
The before/after closure and full COFF comparison are retained in the packet.
The old two volatile
load hints in the handler are removed. No original nominal class is claimed.

Type14/22 drawing remains in the real ECL compiler carrier with the existing
32-byte native polar helper, not the reference's assembly implementation.
All 959 bytes, 53 operands, 20 direct and 12 COM calls, constant payloads and
physical jumps are checked. The changed ECL carrier also requires replay of
all 81 canonical source units across both existing profiles, including this
new draw owner, plus sixteen affected exact dependencies: **97 canonical
comparisons pass**. The 15564-byte main
interpreter remains nonexact; the upstream interpreter is not substituted.

All sixteen upstream-positive routing candidates have now been investigated.
Fourteen are integrated as canonical exact. Two remain open:

- `MoveRandomBiased @0x00407E30`: both retained native source and the
  reference-native trial emit 517 candidate bytes against 516 original bytes,
  with 148 unequal overlapping bytes, 28 fields and 14 calls. No promotion.
- `Etama OnDraw @0x00413BE0`: the reference-native candidate can reproduce
  all 600 bytes/27 fields when the real three-byte Float3 conversion body is
  visible. That same visibility changes instructions in the previously exact
  232-byte physical `DrawSingleBullet` sibling. Moving the definition later
  does not solve it; extracting the static helper changes its actual calling
  context and also fails. A candidate without the visible body fails the
  independently required field inventory. These attempts are rejected as a
  complete integration. Original Etama source, vector-body locations and
  canonical boundaries and targets are retained. Restored-source replay proves
  both old exact siblings; fifteen stale private label spellings are refreshed
  only after checking actual unchanged COFF destinations. No context trick, false ABI or helper
  alias is introduced. Its original 268-byte frontier remains unchanged.

These are bounded negatives, not proof that natural exact source is impossible.
The retained local packet includes `rejected-etama-context.patch`, the candidate
logs, the no-visible-body rejection and both biased-movement comparisons.
The upstream progress image is routing evidence only. Broader subsystem names,
layouts and still-unmatched code remain hypotheses requiring TH09-local proof.
