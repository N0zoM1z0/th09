# Replay menu record reads, 2026-11-05

The bounded source continuation follows the Etama ABI review at `26cbc2ed`.
Original target/tracking checks pass. Fresh Ghidra verifies disk/Program SHA,
i386, base/entry and all685056 mapped file bytes before exporting4234F6.

## Maintained source

The selected replay record is now read through a const pointer. Name and date
format arguments use their direct original field/empty-slot expressions,
removing two redundant inline wrappers. The existing SDK interpolation,
selected-index acquisition order, field layout, variadic text ABI and compiler
profile are unchanged. Existing Cirno-Labs attribution covers the adapted
renderer; this observation does not identify unique original C++ spelling.

Fresh full-TU controls test const pointer, const reference, direct arguments,
their combination and a staged record cursor. All produce the same1280 raw
bytes and the same57 actual COFF offset/kind/symbol/addend records as the
current baseline. The maintained const/direct combination is independently
cold-rebuilt through the focused ReplayMenu gate and retains the same payload.

All29 original-linked mismatch offsets remain identical; every one of the
other1251 physical positions remains equal. The complete renderer has376
instructions,57 fields,13 ordered calls and72 equal direct blocks. All16
actual read-only literal definitions and their30 references are verified.
There is no new exactness credit or relaxed original-byte guard.

## Real owner contexts

The maintained body is also cold-compiled in each of the existing three
contexts: standalone, real TitleScreen declarations, and complete native
TitleScreen implementation. Every context emits the same full raw body and
actual COFF fields and retains1280/57/29. Only this renderer is scored in the
native-context experiment; no other emitted Title function receives credit.

```bash
python3 -B scripts/check-boss-frontiers.py --owner replaymenu
python3 -B scripts/probe-title-replay-menu-context.py \
  --output .analysis/replay-menu-20261105/contexts
```

Nine focused tests pass, along with tracking, documentation, generated progress
and whitespace checks. Totals remain1019 exact/16 nonexact. Whole-product,
runtime, Factory acceptance and remote publication remain separate.

The ignored `.analysis/replay-menu-20261105/` retains frozen input/header hashes,
source controls, compiler logs, target-bound diagnostics, raw/COFF identity
proof and the three-context receipt. `probe.py` pins the pre-change source
commit. Probe objects are below `build/analysis/replay-menu-20261105/`; the
maintained object is below `build/matching/`. Native-context temporary compiler
objects are removed by the existing probe after its verified receipt is written.
