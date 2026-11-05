# Charge input lifetimes and flag representation — 2026-11-05

Fresh current-source work reproduces the existing nonexact charge-update owner:
1210 original bytes versus1197 candidate bytes,70 independently bound fields,
58 ordered calls,350/342 instructions and1160 unequal overlaps plus13 missing
bytes. No game source or exactness ledger changes in this checkpoint.

## Original-bound observations

The session starts from clean source/doc checkpoint2e706d07. Target, tracking
and progress checks pass for the original Japanese TH09 v1.50a executable.
Fresh direct Ghidra attests disk/Program SHA-256, x86 little-endian32-bit language,
base00400000, entry0047D45F and all685056 mapped file bytes. Ghidra is provisional
semantic evidence; the pinned compiler and complete PE/COFF comparison remain
the exactness Oracle.

The original owner41F810 stores the mode byte at EBP-1 (+55), compares it again
at+1C7, and reloads it at+1DB. Its input receiver is EBX and side index EDI;
Player remains ESI. The current candidate keeps mode inBL, input inEDI and the
initial side inEBX. It has no corresponding stack-byte accesses or frame.
At the held-input join, it specializes the already-known mode branch, dropping
an original comparison/conditional pair and adding a direct jump. Both graphs
have53 blocks, but their complete direct edges do not agree. Equal block counts
and the58-call trace do not establish complete graph equality.

The gap therefore includes frame/mode transport and conditional specialization,
not13 arbitrary absent instructions. Both original and candidate retain the
same real Timer >=20-byte body in the actual object. The comparison validates
all70 referents in order and five actual scalar definitions, rather than
rebasing fields or hiding shifted operands.

## Frozen, unchanged-profile controls

All trials use the existing Player-on-update `/O2 /Ob1 /EHsc /Oy-` profile and
31 frozen quoted inputs. Compiles run serially at lowered priority. Each trial
checks the complete owner and actual20-byte Timer sibling.

| Hypothesis | Candidate extent | Unequal overlaps | Missing bytes | Observation |
| --- | ---: | ---: | ---: | --- |
| Charge-level snapshot scoped to its real branch | 1197 | 1160 | 13 | Complete raw body, linked body and all actual COFF rows unchanged |
| Input state scoped before the shared shot phase | 1197 | 1160 | 13 | Complete raw body, linked body and all actual COFF rows unchanged |
| Native unsigned-long charge bitfield at bit2 | 1197 | 1160 | 13 | Complete raw body, linked body and all actual COFF rows unchanged |
| Native input-record reference | 1195 | 1153 | 15 | Extent regresses; no mode stack home; rejected |

The bitfield hypothesis is suggested by the pinned Cirno-Labs player header,
while the TH09 target independently fixes the charge bit0x4. The natural
bitfield source and current explicit masks are compiler-indistinguishable in
this bounded context. This proves neither a unique original C++ field type nor
a reason to migrate the shared Player header. No register/variable-order hints,
volatile reads, barriers, assembly, padding, ABI substitutions or profile search
are added. None of these private hypotheses is adopted.

The read-only TH08 shooting/bomb-state source and TH095 camera charge view
supply no equivalent PvP input-mode/storage conclusion. Their provisional
semantics, addresses and layouts are not TH09 evidence. One historical input-
carrier result is restored through the SHA-verified analysis-archive helper;
it is comparison context, not a current-source proof or a chosen implementation.

## Evidence and continuation

Ignored `.analysis/charge-complete-20261105/` retains frozen inputs, fresh Ghidra
attestation, actual objects/build logs, full original/candidate disassembly,
complete reports and reproducible `probe.py`/`proof.py` recipes. `source-proof.json`
binds the unchanged game inputs, pinned compiler/SDK hashes, baseline object,
four controls and original-bound reports. No existing canonical source or object
is overwritten. Generated duplicate trial headers are removed only after
per-file SHA agreement with the retained frozen inputs.

The input-mode storage/liveness and actual source context remain open. Three
natural lifetime/representation controls leave the complete1197-byte body
unchanged; the shorter reference control is not an improvement. This bounded
negative does not exhaust natural source possibilities. Exact totals remain
1022/1035 and262049 reviewed-authored bytes (89.7788%);13 owners remain nonexact.
Product/runtime, dual-Oracle semantics, portability and Factory gates remain
separate.

The retained diagnostic source proof is bound to game-source commit
`2e706d07ecf9c51f4ef41d860bf2dbc6af7c6365`; its SHA-256 is
`79191dd7b6cd730fd34da3d63f22044fc774185abac1dcd0201fdd3b012d89ac`. This records noncrediting investigation evidence, not an accepted exact owner.

For later checks, `.analysis/replay-recorded-investigation.py --packet NAME`
writes `source-replay.json` and verifies that the recorded proof stays unchanged;
the original recipe is retained for provenance.
