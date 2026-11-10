# Title draw callback now uses the genuine TitleScreen and ANM managers

The original TH09 v1.50a `TitleDrawCallback` at `0x424898` is a complete
329-byte fastcall callback. It previously constructed an unrelated
`TitleDrawView` prefix solely to reach VM storage and four draw methods.
The new source uses the complete, independently construction-bounded
**0x1B388-byte `TitleScreenView`**. Both VM pointers (+0x11B98/+0x11B9C),
VM count (+0x1B224), current screen (+0x1B228), chain state (+0x1B238)
and all four render methods are now members of the real object.

The callback also accesses the existing native `AnmManager` texture cache
at +0x12880 instead of casting that manager to a second cache view.
Only the already-original-bound field's C++ access visibility changed;
its address, storage, and original render-state call order did not.

Pinned VC7.1 `/O1 /Ob1` recompilation verifies **329/329 original bytes,
20/20 relocation fields**. Four method REL32 operands at +0x107, +0x121,
+0x12A and +0x133 were decoded directly from the pinned original PE and
still resolve to `DrawReplayMenu` 0x4234F6, `DrawReplaySave` 0x4239F6,
`DrawResult` 0x423D16 and `DrawMusicRoom` 0x4244A9.

Scoped replays of **63 already-exact Title/ANM functions, 27,377 bytes and
1,367 relocation fields** all pass, including the 1,592-byte Title start
menu, 1,939-byte result draw, 800-byte replay-save draw, and 341-byte
ANM constructor. The added native method declaration renumbered nine
compiler-private Title start-menu `$L` labels. All nine actual COFF
same-section definitions were checked against their original absolute PE
jump destinations before their manifest names were rebound. The start-menu
manifest also had an omitted `+0x388` member addend for the Supervisor
reference at +0x47; this was independently corrected only after checking
the immutable original absolute dword `0x4B3488`.

The separate nonexact 1,280-byte replay-menu renderer retains **57 bound
fields and 29 unlike bytes**. This checkpoint corrects original object
ownership and one previously under-specified relocation, but does not
increase the canonical exact count, prove runtime equivalence, or use a
whole-project cold replay.
