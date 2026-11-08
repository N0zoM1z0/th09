# TH09 Player Header: genuine three ZunTimers and exact reset/constructor callers

Original executable: Japanese TH09 v1.50a, independently verified
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint reconstructs native C++ field ownership and construction
semantics. It does not assert the original source authored its constructor
explicitly rather than compiler-generated implicit code, or claim a new
accepted byte-exact function.

## Three actual constructed timers in the +0x9C Player header

The maintained `PlayerHeaderStateView` had previously described its first
+0x78 bytes as `unknown00[0x78]`, although the retail executable's
**28-byte constructor at 0x0041EE00** makes three exact
`ZunTimer::ZunTimer @0x00403390` calls on `this`, `this+0x5C`, and
`this+0x68`. The complete target code is `push esi; mov esi,ecx;`
three serial timer constructor calls; `mov eax,esi; pop esi; ret`. Its
callee identity and original 0x28-byte extent are independently known;
the name and whether it was written as an explicit C++ body are not.

`PlayerHeaderStateView` now contains actual constructed `ZunTimer`
objects at **+0x00**, **+0x5C**, and **+0x68**, its flags at +0x74,
`selfLink78` at +0x78 and unchanged state counters and ANM file pointer.
The native header remains precisely **0x9C bytes**. This does not
introduce additional constructors: the same three calls were always part
of the original 243-byte Player constructor at 0x41F070.

The independently exact **29-byte reset at 0x0041BDC0** is now a method
of this actual `PlayerHeaderStateView`, not a duplicate 0x78-byte
`PlayerAddedHeaderResetView` class. Its timer assignment calls still
target the original exact `ZunTimer::operator=(int) @0x401500` twice,
followed by the original low-two-bit clear of `flags74`.
`PlayerLifecycleView::InitializeAddedState` now calls
`this->header24.Reset()` with exactly the original call target VA at
its +0x20E REL32 field. The complete original initializer is **552
source bytes plus 20 bytes of physical jump-table data**.

The PlayerRuntime opponent timing metric at absolute Player +0x80
formerly borrowed `header24.unknown00[0x5C]`; it now borrows the
same genuine `header24.timer5C`, keeping its original metric function
and receiver projection without speculative new method ownership.

## Subsequent canonical exact constructor checkpoint

The standalone original-PE-matching 28-byte constructor has since been
promoted from a private probe into a separately maintained and canonical
`player-header-constructor` unit, with source and all ledgers registered.
See [new 28-byte exact Header constructor](TH09_PLAYER_HEADER_CONSTRUCTOR_EXACT_20261108.md).
The exact ledger now lists **1,035 source-present / 1,022 exact / 13
nonexact**; the original private experiment remains independent evidence.
The details below describe the status *at the preceding structural
checkpoint*, before the separate exactness registration.

## Independent pinned compiler and original-bound whole-owner proof

An isolated pinned VC7.1 constructor translation unit using the real
native header with an empty `PlayerHeaderStateView` special member was
compiled under both `/Ob0` and `/Ob1`. Each naturally emitted
**28/28 bytes**, three original-bound `??0ZunTimer` REL32 calls at
+0x04/+0x0C/+0x14, and zero physical differences from the immutable
retail PE. This is an *independent construction probe*, not an accepted
manifest/mapping: the standalone 0x41EE00 target remains unclassified
until separately source-bound and tracked.

For the adopted code, canonical original PE/COFF comparisons prove
**29/29 reset bytes (two fields), 552/552 initializer code bytes and
572/572 initializer physical bytes (25 fields), and 243/243 Player
constructor bytes**. All original VA fields, including the two
reset and constructor targets, remain unchanged. The native header
visibility renumbered eleven PlayerRuntime, six initializer, six
PlayerReward and two ECL compiler-private `$L` symbols; each was
verified against the actual COFF same-section definition, owner-relative
address and immutable original PE operand before updating **names only**.

Targeted source-cold replay spanning PlayerAddedState, PlayerRuntime,
PlayerOwnerStateReward, FrontResult, EclManager and GameManagerSetup
passed **121 preexisting whole exact functions, 40,996 physical bytes
and 2,068 relocation fields**. Original target, compiler flags,
original field addends and validator strictness were unchanged.

The still-open 3,900-byte Enemy OnUpdate (13 ordinary unlike bytes),
862/849-byte Player CheckBulletCollision (13 missing bytes), and
15,564-byte RunEcl (30 unlike bytes) are not promoted by this source
reconstruction. Receipts and original-PE constructor probes are kept
locally under `.analysis/gpt-web-player-header-three-timers-20261108/`.
No all-project cold replay, Windows product link, gameplay acceptance,
Factory receipt or remote publication is claimed. Global status remains
**1,034 source-present / 1,021 exact / 13 nonexact**.
