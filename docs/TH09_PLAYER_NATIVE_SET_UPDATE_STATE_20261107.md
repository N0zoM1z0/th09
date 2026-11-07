# TH09 native Player SetUpdateState across exact selector and Enemy/ECL bosses

Original target: Japanese TH09 v1.50a, independently SHA-256 verified
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint recovers ordinary C++ method ownership and direct physical
receiver relationships, not uniquely identified original class names or
any new byte-exact source-present owner.

## Twelve-byte setter is a method of the real Player allocation

The original **12-byte `SetUpdateState` at 0x00403C20** writes the first
four bytes of the real Player allocation; it had previously been declared
on an artificial four-byte `PlayerState4OpsView` facade. The source now
declares and defines `PlayerLifecycleView::SetUpdateState`, retaining
identical argument, thiscall and original target address. The still-needed
`PlayerState4OpsView` only declares separately reviewed transition
methods and adds no physical Player storage.

The natural-C++ migration covers the independently exact **3,238-byte
Player gameplay selector** and **412-byte physical resolver**,
**400-byte Player CheckState4**, three **91/91/100-byte State3 effects**,
Player Runtime stage/state paths, and the complete open **3,900-byte
Enemy OnUpdate**. Those callers now use the actual `PlayerLifecycleView`
pointer, not casts to a duplicate four-byte source object. The original
Enemy code must reload the receiver from `manager->sideState320->player04`
at the call site to preserve VC7.1's original register and literal-field
chronology. Simply calling through a previously held local Player pointer
changes real literal field positions, and is **not adopted**.

The native Enemy checker was updated only to expect the new source C++
symbol at its **original +0xC06 REL32 field to 0x00403C20**, without
altering its original PE operand, 97-field coverage or byte comparisons.
The independently bound ECL interpreter still uses the exact original
`RunEcl` owner and full opcode jump tables; it is not retargeted.

## Complete original PE/COFF and negative controls

Several independent fixed VC7.1 models were compiled for the exact setter,
State3 leaf, 400-byte CheckState4 and 3,238-byte selector before adoption.
All complete bytes and every COFF relocation position/type/addend remained
identical, with the true setter symbol at its existing original call VA.

After adopting the full call chain, targeted source-cold replay passes
**110 existing exact complete functions / 39,191 physical bytes /
1,846 original-bound relocation fields**. A native-source visibility
change renumbered 17 PlayerRuntime, two ECL and 18 selector-resolver
compiler-private `$L` symbols. The 18 selector local symbols specifically
belong to its 412-byte physical owner containing the original jump table.
Before updating *only the names* in the strict manifest, each actual COFF
same-section definition, owner-relative symbol offset, immutable original
PE operand and exact destination was audited. All 37 changes are
name-only: no original target, field addend, jump-table byte, compiler
profile or comparator strictness was modified.

The complete **3,900-byte Enemy OnUpdate** retains 97 original fields,
69 ordered calls, its original graph and exactly **13 ordinary differing
bytes**. The complete **15,564-byte RunEcl** retains 598 original-bound
fields, zero table/field differences and 30 ordinary differences
(opcode 4: seven; opcode 155: 23). Both are still **nonexact**. Original
Source compiler/tracking tests and focused Player/Enemy tests guard this
checkpoint. Private bounded trial reports and audited original receipts:
`.analysis/gpt-web-player-native-update-state-20261107/`.

No full-project cold replay, game product link, runtime semantics acceptance,
Factory acceptance or remote publication is claimed. The global ledger
remains **1,034 source-present / 1,021 exact / 13 nonexact**.
