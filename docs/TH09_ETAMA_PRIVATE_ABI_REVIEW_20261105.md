# Etama private call review, 2026-11-05

The session starts at clean `2a8e9d17`, with target/tracking/status checks
passing at1019 exact/16 nonexact. Direct Ghidra verifies disk/Program SHA,
i386, image base, entry and all685056 mapped file bytes before exporting
the original215-byte DrawSingleBullet and600-byte Etama OnDraw entries.
Ghidra's missing formal argument is provisional; the PE instructions and
compiler comparisons below establish the actual transport.

## Native argument and scope constraint

DrawSingleBullet begins at413AF0 and reads Bullet state from `[ebx+DBE]`
at413AF4, without first receiving that pointer in ECX. Its original parent
loads EBX from a draw bucket at413E00, tests it, calls413AF0 at413E06,
then advances EBX through Bullet+DC8 at413E0B. This is a compiler-private
EBX receiver, independently of the source profile's default fastcall spelling.
The calling-convention ledger cell was already blank and remains so; the
new note distinguishes physical transport from the reconstruction's static
C++ declaration. Original nominal linkage, source partition and return
spelling remain unknown.

The original215-byte code SHA is
`2d8ec5a37be5f4c2ec2c3374ed5f0bd2678d683058e406195481f9e063e4a531`.
Its232-byte physical owner, including alignment/four switch entries, has SHA
`7ff2b48cc7018ce7354a5515989ea025a80b4a6d5339e064629be90ed1029d05`.

## Complete split control, rejected

A natural independent-source experiment keeps the actual helper body and
existing profile, but changes its static declaration to ordinary external
fastcall and declares that function in the parent source. The genuine
Float3 conversion body is visible in the reference-shaped parent carrier.
The external helper receives ECX and moves it to a saved register, producing
236 physical bytes/16 fields,216 overlap differences and four extra bytes.
The parent becomes602/600 bytes with68 differences. It no longer equals the
previously observed600-byte static-helper parent. No ABI adapter, register
hint, assembly or per-function profile is introduced to force that split.
Neither source arrangement is adopted.

The original/helper conversion conflict is also rechecked instruction by
instruction. Opaque conversion calls leave the first pointer in `[ebp-4]`;
with the real conversion visible, C2 retains it in EDX across the second
conversion, changing the helper's complete output. A standalone parent match
cannot waive that exact helper's bytes or its private argument transport.

The retained source is freshly rebuilt. Its232-byte/16-field helper and
832-byte/30-field AddedCallback both compare completely exact. The parent
remains600 bytes/27 fields/268 differences. No new exact credit follows.

## Bounded callback inventory

A target-bound scan of the already-reviewed38-row Effect template prefix
at4A0B48 finds55 distinct non-null callback addresses; all55 are existing
authored/source-present ledger entries. This closes only that prefix's
address-to-ledger check. It does not prove the table's complete runtime bound,
all callback tables or the executable inventory.

## Evidence

The ignored `.analysis/etama-pair-20261105/` retains mapped-byte-attested Ghidra,
the original ABI windows/hashes, complete helper disassembly comparisons,
frozen input/header hashes and the two-file compile control. Objects are under
`build/analysis/etama-pair-20261105/`; maintained objects are under
`build/matching/`. `split-probe.py --build` rebuilds both diagnostic sources
serially under the unchanged profile. The rejected candidate uses the pinned
Cirno-Labs parent hypothesis recorded in the integration packet; it is not
accepted source. No original binary or metadata byte is patched.
