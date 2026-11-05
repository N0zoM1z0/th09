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

All four reproduce every original byte and relocation under the existing
pinned local compiler profiles: **774 physical bytes, ten fields**. There
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

The ledger changes from **1005 to 1009 exact functions** and from
247597 to **248371 exact authored bytes**; the 291883-byte reviewed denominator
is unchanged. Whole-product/native-runtime/Factory acceptance is separate.

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

## Remaining reference inventory

Address comparison between our 30 initial nonexact owners and the immutable
upstream progress image found sixteen upstream-positive candidates. The four
above are adopted. The other candidates are result/options/replay-save draws,
controller input, Supervisor update and network handler, biased ECL movement,
Etama draw, random message setup, Front added callback, ExAttack type3 update
and the shared ExAttack type14/22 draw. Each needs native source integration
and its own complete original-bound replay; the upstream image is routing
evidence only. Upstream's nonexact ECL interpreter is not a replacement for
our stronger maintained source. No assembly or volatile matching tactic will
be imported from the reference.
