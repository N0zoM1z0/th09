# Player movement source review, 2026-11-05

This bounded continuation starts at `d5bce054`. The original Japanese 1.50a
target and tracking pass. Fresh direct Ghidra attests disk/Program SHA, i386,
base/entry and all 685056 mapped file bytes before exporting the movement owner.

The maintained movement source now consumes the vector's actual float-pointer
conversion directly for XY integration and clamping. It removes the subsequent
reinterpretation back to a PlayerPositionView. Main VM calls use the genuine
zero-offset AnmVm base conversion; option calls use their actual vm00 member.
Four main-VM casts and one option-record cast are removed. The existing input
offset and its independent read remain unchanged.

Fresh full-source before/adopted probes use the same pinned /O2 /Ob1 /EHsc
profile. Every raw byte of the entire 1900-byte physical owner is identical,
including the two switch tables. All 66 actual COFF fields preserve kind,
offset, addend and effective destination. Only two compiler-private table label
names change; their actual same-section offsets remain0x72C and0x74C.
The complete original mismatch set remains exactly122 positions, with no
substitution into the other1778 already-matching bytes. All23 original calls
remain in order. This is a native source-access improvement, not new exactness.

The separate direct input-pointer control is rejected by the existing complete
decoded-field/actual-COFF coverage check. No lowered byte count is accepted
from that rejected input and no field guard is relaxed. Combining it with the
float-pointer source has the same rejection. The initial private comparison
also rejected the adopted source's renamed local labels; the subsequent
definition comparison proves both unchanged destinations rather than ignoring
their identities.

The maintained source independently cold-replays through:

```bash
python3 -B scripts/check-boss-frontiers.py --owner movement
```

It retains1835 code/1900 physical bytes,66 fields,23 calls and122 differences.
The separate complete 522-byte Player OnUpdate/40-field caller and genuine
18-byte AnmVm SetInterrupt callee replay exactly. All27 movement-focused tests
pass, together with tracking, documentation, progress and whitespace checks.
The ledger remains1019 exact/16 nonexact. No full cold replay, product/runtime,
Factory acceptance or remote-publication claim follows.

The ignored `.analysis/player-clamp-20261105/` retains frozen input/header
hashes, source controls, Ghidra, complete comparisons, actual local-label proof
and the maintained canonical receipt. `probe.py` pins the starting movement
source to `d5bce054`; `native-vms.py` retains the native source transformation.
Objects are below `build/analysis/player-clamp-20261105/` and `build/matching/`.
