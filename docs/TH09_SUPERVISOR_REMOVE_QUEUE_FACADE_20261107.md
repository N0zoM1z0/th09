# TH09 Supervisor: eliminate the last zero-storage frame-queue façade

Original original-bound retail target: Japanese TH09 v1.50a, verified
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint completes native C++ receiver consolidation, not original
class-name archaeology, full-product linking or new canonical exact credit.

## Original queue allocation and real Title receiver

One **0x7B0-byte `Supervisor` allocation** owns both frame queues at
+0x47C (`entries47C[20]`, 0x0C bytes per record) and their two last-frame
records at +0x56C. Its four original-exact methods have already been
recovered under the real Supervisor C++ type. The complete nonexact
1,633-byte network-service boss and the original-exact 808-byte network
message handler now directly call the same native queue methods.

The remaining `TitleSupervisorView` was a zero-storage derivation of the
now-empty `SupervisorFrameQueueView`. The exact **177-byte
`ResetTitleMode4Supervisor`** now declares `TitleSupervisorView : Supervisor`
and inserts both -9999 sentinel frames directly through the genuine
`g_TitleSupervisor.InsertReceivedFrame` method. Its source-facing function
parameter and decorated symbol remain `TitleSupervisorView *`: this does
not invent a new original ABI or turn it into a separate queue allocation.
The original two distinct `memset` operations for the 0xF0-byte queue and
0x18-byte last-received interval, the RNG seed reads, and the network state
reset retain their original sequence and field offsets.

All remaining source includes of `SupervisorFrameQueueView.hpp` in the
queue producers/consumers were removed. The header itself was deleted;
no live C++ source refers to `SupervisorFrameQueueView` now. Existing
layout checks and original-bound tests were redirected to the real
`Supervisor` fields and the source-level `TitleSupervisorView : Supervisor`
relationship. This explicitly supersedes the older assertion that the
empty helper façade was a required structural source owner.

## Source-cold and original PE/COFF evidence

Five independent natural-C++ fixed-profile VC7.1 trials demonstrated
byte-identical **177/177 Mode4 code bytes and twelve field identities**
with a native Supervisor pointer, typed Title pointer, const pointer,
reference and direct-global call forms. The direct-global form was
adopted, and the now-irrelevant queue header was removed.

The final affected-source replay compiled and compared **seven complete
existing exact owners, 2,553 physical bytes and 151 relocation fields**:
177-byte Mode4, four queue leaves of 213/191/119/49 bytes, the full
**808-byte physical network-message owner** (768 code + 40 table bytes),
and the **996-byte physical `Supervisor::OnUpdate` caller** (844 code +
152 table/data bytes). Original relocation names, call targets, function
extents, all actual compiler-built fields and bytes remain unchanged.

The independent 1,633-byte `SupervisorServiceUpdate` was also recompiled
under its fixed original compiler profile, and compared against the
previous full source-cold object: **all 1,627 candidate bytes and 106
COFF fields remained identical**, with the same **1,381 unlike overlapping
byte positions plus six missing bytes** relative to the retail PE.
No whole-project cold replay was performed. The private bounded receipts
are stored in `.analysis/gpt-web-supervisor-mode4-native-20261107/`.

The original compiler and data-layout proof does not establish a working
product link, runtime/semantic acceptance, Factory exactness receipt or
remote push. The overall ledger is unchanged: **1,034 source-present /
1,021 complete exact / 13 nonexact**.
