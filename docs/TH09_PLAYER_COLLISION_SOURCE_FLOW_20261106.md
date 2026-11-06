# Player bullet collision: loop and native reward calls

The maintained `PlayerLifecycleView::CheckBulletCollision` now scans its active
regions with `for`/`continue` and calls the existing implementations of
`PlayerState4OpsView::AddRespawnResource` and
`GameManagerSideStateView::AddScore`. The two declaration-only cancellation
classes are removed. The complete function remains non-exact: candidate849,
original862,799 unequal overlapping positions and13 missing bytes. The length
gap is not a count of the remaining unequal bytes.

## Original and current evidence

The sole target is the original Japanese1.50a executable, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Fresh direct Ghidra exports cover41DFF0,41BC90 and40F860. Both exports attest
the disk/Program identity,x86:LE:32,image base400000,entry47D45F and all685056
file-backed mapped bytes. Decompiler types remain provisional.

The complete collision owner41DFF0..41E34D has SHA-256
`9dcfe1aca4ac06526072014ee6f956120d9c1d84cc1bba044638556b9b82445c`.
Its26 external fields,12 ordered physical calls,four actual read-only scalar
definitions and three RET12 sites are checked. The target has277 instructions
and33 direct blocks; the current candidate has266 instructions and32 blocks.
Bullet is retained in EBX by the original; the candidate retains position there.
The original loop enters through a jump to the pointer/null test, whereas the
candidate tests the first region before entering its loop.

The actual162-byte resource helper at41BC90 accesses Player+30388 and the
side/front-side chain. The actual33-byte score helper at40F860 follows
side+1C,updates the pointed record+08 and divides the argument by ten. Both
complete bodies and their five combined fields pass the original-bound Oracle.
These facts support using the existing method declarations and thiscall
transport; they do not establish original nominal class names. The score view
describes a prefix of the separately established0x38-byte side record.

The source/interface changes preserve every raw collision byte and all26 field
positions/types/addends. Only the two independently bound callee symbols change.
The public inspector now recognizes those implemented symbols; the two old
declaration-only symbols are rejected. Its original fields,extent,constants,
call order,return sites and byte comparison are unchanged. The existing
unreviewed-alias rejection test covers both removed names.

## Natural-source controls

Sixteen fixed-profile whole-source controls explore query capture,early null
exits,hit-block placement,borrowed position,geometry scopes,shared vectors,
the active-array declaration,indexing,unused-argument spelling,receiver use,
structured continuation and a type switch. All80 repeated exact-sibling
comparisons pass. No register hints,barriers,assembly,padding,extra volatile,
forced-inline additions or profile changes are used.

The pointer `for`/`continue` control reproduces every maintained raw byte and
is adopted. The type switch emits847 bytes; indexed traversal emits875.
Both remain mismatches. Two early-null controls emit830 bytes and two physical
RET12 sites. Their full bytes,26 fields,12 calls and actual literal definitions
are separately decoded; the public original-three-site guard still rejects them.
The borrowed-position model has an explicitly recorded private nominal callee
alias with the same ECX/three-stack-word/RET12 transport and is not adopted.

The original uses EBP-1C/-18 for the Rotate input and later popup position,
and EBP-10/-0C for Rotate output and later effect velocity. A two-vector
workspace recovers these homes,half-size Y at EBP-24 and bounds maximum at
EBP-34/-30,while still emitting849 bytes with799 unequal overlaps. This is a
useful source/storage hypothesis,not evidence that the original explicitly
reused variables or used the tested union. Neither workspace representation is
adopted. Loop/shape/response structure and native vector representation remain
open questions; previous negative probes do not exhaust natural source models.

## Focused validation and reproducibility

Only this source rebuilds,two serial lowered-priority builds for the native-call
and combined structured versions. All five exact siblings remain1220 bytes and
32 fields. Eleven distinct direct callees remain2260 bytes and106 fields using
nine unchanged,source/profile/object-hash-verified carriers. A separate strict
public comparison replays all16 existing units:3480 bytes and138 fields.
No canonical match recipe,match row or exactness count changes.

The ignored packet is `.analysis/player-collision-owner-flow-20261106/`;
objects are in `build/analysis/player-collision-owner-flow-20261106/`.
`inputs.json` freezes the20-input9eea03f6 hypothesis epoch and verified baseline.
`current-proof.json` binds22 current quoted inputs,the two actual callee fields,
all five siblings and eleven callee proofs. `strict-current.json` contains the
independent public comparisons; `controls-review.json` replays all16 complete
controls. The baseline inspector is pinned from Git for historical controls;
current-source verification uses the maintained inspector.

Eighteen focused tests pass. Full public CI passes1386 tests with29 skips.
Target-required tracking,progress,documentation and whitespace checks pass.
Totals remain1034 source-present,1021 exact and13 non-exact. The reconstruction
goal,Windows product closure and later runtime/semantic/port gates remain open.
These are local source/evidence checkpoints,not Factory acceptance or remote
publication claims.
