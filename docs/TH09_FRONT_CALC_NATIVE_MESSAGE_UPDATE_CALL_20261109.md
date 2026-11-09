# Native Front calc calls the real message Update owner

The 1,491-byte complete `FrontCalcCallback` at 0x00417630 used to call the
actual message Update function (0x00416590) through a source-only facade
`FrontCalcMessageRuntimeView::Update`. That facade had no corresponding
native C++ definition even though the real `FrontMessageRuntimeView::Update`
was reconstructed and proved original-exact in its own translation unit.

`FrontCalcCallback.cpp` now invokes the **actual** nested
`front->messageRuntimeE944.Update()` member on the native Front allocation.
The abandoned facade retains only its still-separate `Setup` declaration;
its Update declaration was removed. The original +0xE944 receiver offset,
thiscall ABI, all other game actions and source chronology remain unchanged.

After rebuilding only the affected `/O2 /Ob1` Front calc carrier with the
pinned VC7.1 toolchain, the strict whole-owner original PE/COFF Oracle proves:

- `FrontCalcCallback`: **1,491/1,491** original bytes and **107/107**
  reviewed relocation fields, zero differences.
- Its actual REL32 reference at original-relative **+0x187** now uses
  `?Update@FrontMessageRuntimeView@@QAEHXZ` at unchanged **0x00416590**;
  the old fake `?Update@FrontCalcMessageRuntimeView@@QAEHXZ` is retired.
- The independently replayed callee remains **2,968/2,968** physical bytes
  and **167/167** original fields, including all 116 switch-table bytes.

Only the source symbol identity was refreshed in the real canonical caller
manifest after both true original-bound endpoints were compared. A numeric
address alias alone was not used to assert C++ ownership. No new unique
codegen-exact authored function credit, full cold replay or runtime closure
is claimed.
