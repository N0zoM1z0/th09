# Cirno-Labs TH09 reference audit — 2026-11-05

The operator requested a clone and an independent check using this repository's
Oracle. The ignored checkout is `_reference/Cirno-Labs-th09`, pinned to
[`d7653152becabb8c86f2f43d49015ecc11bd0652`](https://github.com/Cirno-Labs/th09/tree/d7653152becabb8c86f2f43d49015ecc11bd0652).
Its `tools` submodule is pinned to
`edf50486ef7f1777f8c097155f7e79b16d1b8592`. Neither checkout is vendored into
this repository. Upstream identifies its source license as GPL-3.0-only;
this checkpoint imports no upstream implementation into our MIT source tree.

## What the upstream percentages mean

At the pinned revision, `docs/progress.svg` reports **1099/1114 game
functions (98.65%)**, but **88.11% of game code bytes**. The older README
still says 1086/1114 and 84%. The image separately reports library code,
rebuilt executable code, data and file similarity; none is an independently
proved whole-game exactness percentage under our rules. Our authored
denominator also differs, so comparing 98.65% directly with our 84.8275%
reviewed-authored-byte coverage is invalid.

In particular, the image's **96.97% executable-file** row is a separate
self-reported measure. `tools/render_progress.py::summary_rows` and
`exe_stats` obtain it from equal bytes at the same file offsets (including
library code and other executable contents). It is not the game-function
match rate or our reviewed authored-byte measure. No rebuilt upstream EXE
was independently produced or compared in this session.

The upstream `orig/th09_150a.sha1` gives
`89c8fdef84d25b2ee8bbd2026f8a343d7ef57c03` for its executable, equal to our
canonical original's SHA-1. All experiments here use **our**
`resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
No second executable was downloaded or substituted.

Upstream documents VC7.1 SP1 **13.10.6030**, whereas our compiler/backend
are the pinned RTM Oracle. Its function comparator resolves named references
through `symbols.csv` and actual owner-local definitions. Other relocation
words are masked; referenced initialized data is checked separately where
available, with pointer fields inside that data also masked. Its function
extraction ends at a detected switch table. These are useful checks, but they
do not establish our complete physical-owner and relocation-identity claim.
For example, upstream records Supervisor update as **844 code bytes**;
our independently reviewed complete extent is **996 physical bytes**.

## Independently reproduced positive sample

Only three external translation units were cold-built, serially at lowered
priority, with `scripts/compile-probe.sh` and its component-hash checks.
The original upstream C++/header bytes were mirrored below `.analysis/` as
CP932 with its documented trail-backslash encoding rule. The source-specific
upstream profiles were preserved: main menu `/Oy- /Gr /GX /O1 /Ob1`, app
and unsorted `/O2 /Ob0 /Oy- /Gr /GX`. No profile search was performed.

| TH09 entry | Upstream name | Our maintained owner | Full physical bytes | Fields | Result |
| --- | --- | --- | ---: | ---: | --- |
| `0x00423D16` | `MainMenu::DrawState13` | Title `DrawResult` | 1939 | 82 | zero differences |
| `0x004276EB` | `MainMenu::UpdateState9` | Title `OnUpdateOptions` | 2045 | 135 | zero differences |
| `0x00424EDD` | `MainMenu::SelectEntryStep` | Title active character cursor | 211 | 6 | zero differences |
| `0x00424FB0` | `MainMenu::SelectEntry2` | Title reverse character cursor | 175 | 2 | zero differences |
| `0x0042505F` | `MainMenu::SelectEntry3` | Title inactive character cursor | 175 | 2 | zero differences |
| `0x0042E9E0` | `App::AddRecord` | Supervisor frame insertion | 213 | 0 | zero differences |
| `0x00431110` | `App::OnUpdate` | Supervisor update, including tables/tail | 996 | 92 | zero differences |
| `0x0042B410` | `ReadPad` | Controller input | 808 | 32 | zero differences |

**Eight external functions, 6562/6562 physical bytes, 351/351 relocation
fields, zero unresolved fields and zero byte differences.** All eight
correspond to currently nonexact maintained owners. This is strong evidence
that the reference can help our remaining work; it is not a random sample or
a measurement of the entire foreign repository.

The audit resolves named upstream COFF symbols through its immutable mapping;
each complete replay must reproduce the actual original operand words. Local
labels use their actual COFF section-relative definitions, including table
entries. Literal addresses come from our pre-existing, independently recorded
owner dictionaries, and **actual object definitions** are checked against
original PE payloads with our literal validator. Result drawing checks all
38 literal references covering 15 distinct definitions. Options and Supervisor
each check one additional string definition. No original operand is used to
solve an otherwise unknown reference, and no relocation or ordinary byte is
masked for a positive result. Only up to 15 proven trailing alignment bytes
may be excluded after the independently reviewed physical extent; non-padding
tails and tables stay in the comparison.

Direct Ghidra was also reattested against disk and Program SHA-256, Windows
i386 language, base `0x00400000`, entry `0x0047D45F`, and **685056 file-backed
mapped bytes**, including PE headers and all five sections. Bounded exports
include result drawing, the three cursor functions and frame insertion.
This supplies independent semantic/ABI evidence, not compiler exactness.

## Complete ECL interpreter counterexample

The additional `ecl` group cold-builds only upstream `src/ecl.cpp` using its
documented `/O2 /Ob0 /Oy- /Gr /GX` profile and our pinned compiler. At
`0x004086C0`, upstream records **14792 code bytes**; our complete reviewed
owner is **15564 physical bytes**, including its 772-byte tail/tables.

The candidate has the full **15564 bytes and 598 COFF fields**, but **3695
non-relocation bytes differ** at identical physical offsets. This alone
falsifies exactness, independently of any unresolved pointer identity.
The original physical extent SHA-256 is
`e9433504219119eba40ed4d11537782178135effeb6c95d74b6274e0e7f5e7ea`.
The current audit additionally leaves **12 relocation fields unresolved**:
the emitted `@fmodf@8` / `@atan2f@8` helpers and selected literal references
lack independently supplied bindings in this group. It deliberately does
not infer those addresses from original operand words. Its reported 4164
partially relocated overlap differences therefore are **not** a complete
relocation-bound residual metric; the 3695 ordinary differences are the
decisive measured counterexample.

Upstream's progress image and `hooks.allow_diff` already mark
`EclManager::RunEnemy` as nonmatching. This does not contradict its stated
function total. It shows why a promising reference does not establish exact
reconstruction of every large owner. Our maintained RunEcl's 30-byte
residual in the November 4 handoff was not rebuilt here; this audit grants
no new claim about its present compiler output or possible source improvements.

```bash
python3 -B scripts/audit-cirno-reference.py --group ecl --build
```

This writes `.analysis/cirno-labs-audit-20261105/ecl-report.json` and its
source/profile/object-bound build receipt. A second read reuses that receipt;
it does not rebuild the previously verified title/system cohort.

## Reproduce the bounded check

```bash
git clone https://github.com/Cirno-Labs/th09.git _reference/Cirno-Labs-th09
git -C _reference/Cirno-Labs-th09 checkout d7653152becabb8c86f2f43d49015ecc11bd0652
git -C _reference/Cirno-Labs-th09 submodule update --init tools
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/audit-cirno-reference.py --group title --group system --build
python3 -B -S -m unittest discover -s tests -p test_cirno_reference_audit.py
```

On an existing clone, inspect it first and do not overwrite dirty work.
Omit `--build` to reuse only a source/profile/object-hash-bound receipt.
The script rejects a different or dirty upstream revision, wrong target,
stale source/profile/object proofs, incomplete/overlapping relocations,
incorrect literal initializers and unreviewed reference bindings. An unresolved
reference yields a nonexact diagnostic, never positive exactness credit.

Detailed JSON, build receipts, CP932 mirrors and attested Ghidra exports are
under ignored `.analysis/cirno-labs-audit-20261105/`; objects are under
`build/reference-audit/`. The title/system receipt is
`title-system-report.json`. These can be regenerated and do not accompany a
normal clone. The audit stores foreign source hashes, profiles, object hashes,
complete original-extent hashes, relocations/addends, actual literal checks
and mismatch offsets.

## What this does and does not advance

This checkpoint adds a reproducible external-source investigation. Our own
gameplay source, `config/functions.csv`, `config/matches.csv` and canonical
match units are unchanged: **1005 locally exact authored functions** remain.
There is no Factory acceptance, Windows-product build, runtime parity or
remote-publication claim. Upstream's parity workflow is separate evidence;
it was read, not executed or independently accepted here.

Natural source hypotheses worth investigating next are the indexed native VM
loops in the three cursors, the full menu translation-unit context around
result/options, the typed frame insertion loop, Supervisor's full switch
tables and the controller's input-state expression order. Reconstruct changes
in our native owners against fresh TH09 evidence and replay affected canonical
cohorts before any exact promotion. Foreign nominal names and layouts remain
hypotheses. Upstream also contains inline assembly and a volatile cast in other
owners; a foreign match alone does not waive our natural-source rules.

Validation at this checkpoint: the eight positive replays above, the bounded
nonexact ECL replay, 11 focused target-independent audit tests, tracking,
generated-progress and documentation checks, and `git diff --check` pass.
An audit command succeeds when it produces a valid diagnostic, including a
nonexact sample; inspect each sample's `result` rather than interpreting exit
status zero as an exactness receipt.
