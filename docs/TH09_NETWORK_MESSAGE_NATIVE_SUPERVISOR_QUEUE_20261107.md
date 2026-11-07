# TH09 network message handler: real Supervisor frame queue calls

Original retail target: Japanese TH09 1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint removes fabricated source receiver casts, not original
compiler instructions or a confirmed historic class spelling.

## Complete message handler and native queue ownership

`SupervisorNetworkMessageView::HandleMessage` at **0x00432B70** owns **768
source function bytes plus 40 bytes of physical switch/table data**. Its
three `InsertReceivedFrame` calls previously converted the global native
`g_Supervisor` into `SupervisorFrameQueueView *`, a zero-storage façade
which now owns no methods. All three source calls now directly use
`g_Supervisor.InsertReceivedFrame`. The genuine Supervisor owns both
frame queues at +0x47C and last-received records at +0x56C; the separate
DirectPlay session object `SupervisorNetworkState` is not the queue owner.
The original receive, handshake and ACK branches and actual 12-byte
packet representations are unchanged.

Four source forms were cold compiled (native object, typed Supervisor
pointer and a local queue alias) in addition to the existing baseline.
**All five produced the same complete 808 machine-code/table bytes** and
46 COFF fields. The adopted code passes the strict original-bound
**768/768 function comparison**, while a separate physical owner audit
proves **808/808 complete original bytes**, including every table field.
Original REL32 `InsertReceivedFrame` calls remain at owner offsets
**+0x89, +0x156 and +0x16F**, all targeting **0x0042E9E0**.

Removing the obsolete include renumbered eight compiler-private `$L`
symbols. Each was checked against its actual same-section COFF definition,
owner-relative destination and the immutable original PE field word before
only its name was updated in the exact match manifest. Every field kind,
addend and source-cold physical byte remains unchanged, with no masked
comparison or guessed original target.

Focused original receipts and source-cold compiler probes are retained in
`.analysis/gpt-web-network-message-native-calls-20261107/`.
No new original-exact source credit is claimed; the broader 1,633-byte
network service and its unchanged 1,627-byte candidate remain nonexact.
No whole-project replay, Windows product link, runtime/semantic acceptance,
Factory receipt or remote push is claimed. Ledger remains
**1,034 source-present / 1,021 exact / 13 nonexact**.
