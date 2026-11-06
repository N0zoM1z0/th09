# GameplaySetupThread: native side-score buffer extents (2026-11-06)

The original Japanese TH09 v1.50a gameplay initialization thread allocates
and clears a real per-side `SetupStateBuffer`, the same observed 0xA0-byte
score/phase state used by the exact GameManager setup and cleanup owners.
The maintained source previously repeated a literal `0xA0` in the allocation
and two independent `memset` paths even though the **actual structured
`SetupStateBuffer` has `sizeof(...) == 0xA0`** under the pinned i386 ABI.
The current C++ uses `sizeof(SetupStateBuffer)` at all three allocation/clear
sites. The object lifetime, allocation operator, two-side loop, conditional
second-side reset, constructor absence and original calls remain unchanged.

Eight natural C++ context variants were **actually compiled** under the
existing VC7.1 `/O2 /Os` profile, including all/individual native `sizeof`
expressions, computed pointer sizes, an explicit native side reference in the
registration loop, and manager flags through a borrowed global reference.
All `sizeof` variants reproduced the exact baseline physical code and 172
actual operand fields. The side-reference loop changed the original length;
it was not adopted. This is source-layout fidelity, **not** a new exact owner.

## Complete game-thread frontier and exact siblings

The original thread is 1,689 bytes. Its current source remains a **1,682-byte
candidate / 172 fields / 35 ordered calls**, with seven missing bytes and
694 unlike overlapping positions. The `sizeof` source change introduces no
new unlike offset; the two unmatched dispatch field spans, 78 blocks and
reviewed opponent configuration calls retain their original observations.

The focused `scripts/check-boss-frontiers.py --owner gamemgr` replay passes
all **16 related exact source-unit comparisons** (4,623 complete physical
bytes, 334 fields) and the separately checked 425-byte exact cleanup owner.
Neither the ledger count nor any other byte- or branch-exactness assertion
is increased. The original target remains Japanese 1.50a SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

The ignored local trial packet is
`.analysis/gpt-web-setup-native-buffer-20261106/`. Reproduction requires only:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner gamemgr
python3 -B scripts/validate-tracking.py --require-target
```

No whole-project cold replay, original executable patch, Factory acceptance,
new credited authored bytes, native product closure or runtime claim is made.
