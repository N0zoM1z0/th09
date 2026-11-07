# One native Player circle collision method across seven ExAttack callback owners

Original target: Japanese TH09 v1.50a executable, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a reviewed natural C++ source/ABI reconstruction checkpoint,
not proof of historic nominal type spelling or product/runtime acceptance.

## Actual receiver and call graph

Eleven original ExAttack-to-Player call instructions target the **same**
`0x00440D30` circle collision append method. Its `this` receiver is the
native Player collision query at Player +0x36C: one Player pointer, 128
0x30-byte native records, and the counter at query +0x1804. The existing
205-byte AppendBoxRecord and 245-byte AppendLaserRecord methods already
belong to `PlayerCollisionQueryStateView`; the 95-byte original-exact
circle variant previously used an independently named copy of that physical
layout. Seven callers also declared their own separate circle-only method
views despite invoking exactly the same original function.

The native `PlayerLifecycleView.hpp` now declares **all three** methods on
`PlayerCollisionQueryStateView`, and `PlayerCollisionAppendCircle.cpp`
defines that genuine method without the duplicate 0x1808-byte class.
Seven original ExAttack callbacks use one native query type at their
Player +0x36C member, with compile-time checks that the embedded query
location and size match the shared Player allocation. This removes seven
source-local fake member-method declarations and their duplicate query
record arrays without altering how the callbacks perform their attacks,
collision radii or branch decisions. The real unmodified nested
`ExAttackType*PlayerView` projections retain their other source-observed
fields; their full historic class identities remain unproven.

All **11 actual source COFF call fields** now name the single genuine
`?AppendCircleRecord@PlayerCollisionQueryStateView@@...` symbol. Two
natural Type17 C++ branches compile to **one** target call site: counting
source call expressions is not the same as counting machine-code calls.
The independent original owner remains 95 bytes and retains the same
normal thiscall return transport and field writes. Catalog/implemented and
reccmp/manifest declarations now refer to the same genuine member symbol.

## Complete immutable-target proof and label preservation

Eight fixed-profile VC7.1 source-cold compiler carriers were built from
the changed source. All **eight complete original-bound functions** passed:
**4,907 physical bytes** and **203 actual relocation fields**. This scope
includes the 1,436-byte **ExAttack Type18/24** and the 824-byte physical
Type14/22 function including its independently checked switch/table tail.
No body/tail bytes, original call addresses, addends or control-flow
positions were changed. Current strict per-unit `compare-coff-function.py`
and `replay-scoped-exact.py` replays pass.

Comparison against the *pre-change committed manifest*, not the edited
manifest, identifies only 11 C++ circle-method symbol renames plus five
compiler-private local-label names in the Type14/22 owner. All five private
labels were checked using their real same-section COFF symbol offsets and
the unchanged source owner base, against the already reviewed original
PE destinations. The 11 call fields stay at their original offsets and
all resolve to 0x00440D30 with no addend. No synthetic new target was
invented or accepted.

An initial private audit incorrectly required a zero addend whenever the
old TOML omitted `addend`. Seven real `g_GameManager` fields in the Type18/24
object have an **actual COFF addend of 4** despite that omitted optional
manifest field. The original PE itself confirms the +4 encoding.
Re-reading the real source object/immutable PE showed that the supposed
21-byte difference was entirely a checker artifact; the strict canonical
comparator also passes Type18/24 over all 1,436 bytes. This correction
changes no game code, no target field, and no external ABI.

Ignored bounded evidence:
`.analysis/gpt-web-native-circle-20261107/head-to-new-proof.json`,
`final-source-cold.json`, the eight individual strict comparisons, and
`audit-head-to-new.py`. The newer source-level tests enforce one native
method and all seven source callers. Existing numerical pool-capacity tests
were updated not to demand the eliminated private view.

Counts remain **1,034 maintained source functions, 1,021 canonical exact,
13 nonexact**. No all-owner cold replay, full Windows product link, runtime
Oracle, Factory acceptance, or remote publication is asserted.
