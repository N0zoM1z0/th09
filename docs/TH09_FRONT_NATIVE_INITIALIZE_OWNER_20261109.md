# Front message initializer belongs to the full native factory object

The original TH09 v1.50a `InitializeMessageRuntime` at 0x004181E0
operates on the actual 0x1293C-byte Front object, not a separate truncated
0x11EAC-byte `FrontMessageOwnerView` allocation. The exact factory constructs
`FrontMessageRuntimeView` at +0xE944, and both of the initializer's callers
have a pointer to that full Front allocation.

The source method now belongs to `FrontFactoryConstructView`, and both
`FrontAddedCallback` and `GameManagerSetupLayout::CleanupGameplayState` call
that real member. The redundant Front owner layout has been retired;
`FrontMessageOwnerView.hpp` retains only the distinct input-source record.
The original source-facing method identity and the two caller REL32 symbols
are rebound to the real C++ class, with **unchanged PE destinations**.

Fresh pinned VC7.1 compilation verifies the complete original bodies:

- Initializer: **133/133 bytes**, nine relocated fields, target 0x4181E0.
- Front added callback: **706/706 bytes**, 37 fields, caller operand +0x2B8.
- Gameplay cleanup: **425/425 bytes**, 43 fields, caller operand +0x1C.
- The ten other exact GameManager owners and the exact Front factory,
  1,491-byte Front calc, 1,474-byte Front draw and five constructor-source
  equivalents remain original-equal under their existing profiles.
- The **nonexact 1,689-byte GameplaySetupThread** still emits 1,682 bytes
  with 172 bound fields, 694 overlap differences and seven missing bytes;
  the modified native type does not create a regression or exact credit.

Both direct whole-owner inspectors and the focused `--owner gamemgr` and
`--owner frontadded` guards passed, as did 31 Front-message local tests and
six original GameManager cleanup tests. The tests and inspectors now require
the *real* native method symbol and full allocation offsets. No broad cold
replay or native-product/runtime claim was made.
