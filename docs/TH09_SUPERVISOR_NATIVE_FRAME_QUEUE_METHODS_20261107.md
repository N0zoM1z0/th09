# Native Supervisor frame-queue member methods across network and Title

Original target: Japanese TH09 v1.50a, independently verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores natural C++ receiver ownership, not historic
class spelling, whole Windows linking, or newly accepted exact coverage.

## The real receiver is Supervisor, not a new queue allocation

The native `Supervisor` has one 0x7B0-byte allocation, with **twenty
0x0C-byte frame queue records at +0x47C**, grouped as two ten-frame sides,
and **two last-received records at +0x56C**. The four previously exact
queue members were named on an empty
`SupervisorFrameQueueView : Supervisor` projection. This inherited façade
has no data or vtable of its own; actual callsites pass the same
Supervisor pointer. The native `Supervisor` now directly declares and
implements `InsertReceivedFrame`, `InsertPredictedFrame`, `PopFrame` and
`AreFrameQueuesSynchronized`. The temporary zero-storage view remains only
for source callers that are not yet migrated; it no longer declares any
methods and is not an independent resource lifetime or second ABI.

These four original entries and complete source owners are:

| Native method | Original VA | Complete bytes |
| --- | --- | ---: |
| InsertReceivedFrame | 0x0042E9E0 | 213 |
| InsertPredictedFrame | 0x0042EAC0 | 191 |
| PopFrame | 0x0042EB80 | 119 |
| AreFrameQueuesSynchronized | 0x0042ECF0 | 49 |

The independently exact **177-byte ResetTitleMode4Supervisor** and
**768-byte network message handler** continue to call the original
`InsertReceivedFrame` entry. Their five actual REL32 callsites now name
the native method `Supervisor::InsertReceivedFrame` rather than an
unsupported derived-class owner, with all offsets/addends and destinations
at **0x0042E9E0** preserved. The message handler also has a separately
original-bound **40-byte physical switch/table tail** (808 bytes total).

## Complete original PE/COFF and negative controls

A copy-on-write natural C++ experiment compiled the four methods and both
exact callers before adoption. All **six complete physical owners** retained
exactly the same compiler bytes and field offsets/addends when the receiver
was renamed. The adopted code was then independently compared with the
unchanged retail executable using the previously committed manifest as
the baseline: **six exact physical owners / 1,557 bytes / 59 original fields**.
The network message's eight changed compiler-local `$L` symbol spellings
were verified against their actual same-section COFF definitions and
unchanged original PE VAs before rebinding. No original call target,
source-profile flags, comparator strictness, function extent or table data
was adjusted. The 213-byte InsertReceivedFrame is already **complete
byte-exact**; old test documentation claiming that it remains unclassified
was explicitly corrected.

Canonical original-bound source-cold checks and focused network/Title tests
are reproduced in the ignored local review directory
`.analysis/gpt-web-network-native-queue-20261107/`. The complete
1,633-byte `SupervisorServiceUpdate` remains a separate nonexact boss
with an existing 1,627-byte candidate and must be independently audited
when its temporary caller façade is removed. This stage does not credit
that nonexact boss or any of the four already exact methods as new matches.

Overall ledger remains **1,034 source-present / 1,021 exact / 13 nonexact**.
No full cold replay, runtime game/network acceptance, Factory receipt,
whole Windows executable, or remote publication is claimed.
