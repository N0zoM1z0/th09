# Weighted Front message selection uses the constructed runtime

`SetupRandomForSide` at original TH09 v1.50a address 0x4162D0
now belongs to `FrontMessageRuntimeView`. The old
`FrontMessageRandomView` contained a complete independent 0x1D70-byte
facade even though the factory constructor and exact Setup/Update/Draw/
Reset/Load/Release functions use one physical runtime. Its duplicate
header has been deleted.

The 693-byte weighted-selection function now uses the original shared
primary/secondary message tables, native mode +0x10, actual text VMs at
+0x1534 and +0x17D8, shared color fields, font-size fields and the real
message flags. `FrontCalcCallback` invokes this real member directly,
instead of casting its +0xE944 subobject to a separately declared type.

The original-bound VC7.1 `/O2 /Ob1` comparison passes every one of
**693 code bytes and 15 relocation fields**. The actual FrontCalc caller
retains **1,491/1,491 bytes, 107/107 fields**, including its unchanged
REL32 destination at original-relative +0x4FE -> 0x4162D0. In total,
**13 affected exact functions / 9,775 bytes / 599 fields** and five
constructor-source equivalents pass full original-byte comparison.
The 2,968-byte message interpreter's 30 compiler-private label spellings
were rebound only after their same-section definitions proved unchanged
original switch targets.

The independent full-owner random-message inspector also verifies all
693 bytes, literal definitions, 15 original field identities, ordered
calls and control flow. The local 31 Front-message tests and six random
Oracle corruption tests pass. No new authored exact credit, runtime
scenario or full cold replay is claimed.
