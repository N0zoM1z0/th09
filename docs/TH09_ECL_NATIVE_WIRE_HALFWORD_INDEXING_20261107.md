# Native ECL signed-halfword operands in the full 15,564-byte RunEcl boss

Target: SHA-256-verified Japanese TH09 v1.50a executable
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a natural C++ wire-type reconstruction checkpoint. It does not
claim a uniquely recovered historic source spelling or new exactness credit.

## One four-byte argument union, including real signed halfwords

The actual TH09 ECL instruction has a 0x0C-byte header and an original
four-byte `Th09EclWireArgument` union per wire operand. Existing native
readers use `.integer`, `.real` and `.bytes[4]`. The two ECL original
opcode consumers that read signed shorts—**130** (`SET_DEATH_CALLBACK`)
and **150** (`INTERRUPT_SECONDARY_ANM`)—read byte offsets **0** and
**4** of that same union-word sequence.

The union now includes native `short halfWords[2]` with compile-time
assertions of its 4-byte total size and 2-byte element size. The
`Th09EclRunState::RawShort` helper now selects
`arguments[byteOffset / sizeof(Th09EclWireArgument)].halfWords[
(byteOffset % sizeof(Th09EclWireArgument))/sizeof(short)]` instead of
converting the entire argument area to `unsigned char *` and then `short *`.
This retains the original lvalue's signed-width behavior and the two
reviewed aligned byte addresses without inventing a second wire buffer.
The observed offsets are aligned; this is not a promise to preserve a
hypothetical odd-offset unaligned `short &` interpretation. The trailing
zero-length array is the pinned VC7.1 extension, not standard portable C++.

Four natural native indexing variants were compiled with the same pinned
VC7.1 profile. All four preserve the complete original-bound physical
**15,564-byte RunEcl owner**, including **598 relocations, full opcode
and interpolation jump tables, and exactly 30 unchanged ordinary byte
differences** (opcode 4: seven; opcode 155: 23), with zero changed field
or table bytes. The 3,900-byte Enemy OnUpdate companion still has exactly
97 fields, 69 ordered calls and 13 differing ordinary bytes. No newly
byte-exact full boss or auxiliary source is claimed.

Fresh scoped source-cold replay of the EclManager main and Type11
profiles passes **83 existing exact functions / 29,856 physical bytes /
1,389 fields**. The expanded native union caused 20 compiler-private
`$L` spelling changes across the independently exact ECL comparison and
post-update movement owners. Each was first checked using its actual
same-section COFF definition, source-owner-relative offset, immutable
original PE field operand and target VA. Only these verified names were
refreshed in the strict match manifest; comparator strictness, field
addends, original target destinations and original code bytes stayed fixed.

The bounded natural trials and PE/COFF receipts are retained locally in
`.analysis/gpt-web-ecl-native-halfword-20261107/`. This does not
constitute whole-project cold replay, Windows product linking, runtime
semantic or portability acceptance, Factory acceptance or remote push.
The global ledger remains **1,034 source-present / 1,021 exact / 13
nonexact**.
