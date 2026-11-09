# TH09 reconstruction session — 2026-11-09

## RunEcl opcode 155: native byte-width correction

EclManager::RunEcl remains a 15,564-byte large owner with 598 relocation
fields. The current source-bound checkpoint is still 30 ordinary byte
differences: seven in opcode 4 and 23 in opcode 155. There is no field or
switch-table drift, and no exactness credit is claimed.

The opcode-155 input is the byte at instruction +0x0C, not a four-byte
integer. The source now copies only that byte from the shared wire-argument
view before applying AssignTimeoutSpellBit to Enemy primary flags +0x337C:

    unsigned char value;
    memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));
    enemy->primaryFlags337C =
        Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value);

This preserves the complete large-owner binding frontier while making the
wire-width contract explicit. The focused pinned-VC7.1 build and
inspect-ecl-complete.py report 15,564 bytes / 598 fields / 30 ordinary
differences / zero field differences / zero table differences.

The non-crediting audit-ecl-timeout-bit.py independently decodes both the
original and candidate opcode windows. It confirms 12 instructions in each,
eight instructions in each flag-update prefix, and equal flag transitions
for all 256 wire-byte values across 12 representative 32-bit flag words
(3,072 cases). This is semantic evidence only, not codegen exactness.

## Target-bound opcode evidence and rejected alternatives

IDA's target-attested disassembly at 0x0040B770 shows the original order:

1. movzx ecx, byte ptr [ebx+0x0C]
2. mov esi, [edi+0x337C]
3. shift/XOR/AND bit 24
4. copy old flags to EAX, XOR the changed bit, and store EAX.

The current candidate reads the correct byte and preserves the full-owner
30-difference frontier, but its register/lifetime schedule is not exact:
mov cl,[ebx+0x0C], load old flags to EAX, then movzx ecx,cl. The original
loads directly into ECX and keeps old flags in ESI.

Several natural C++ alternatives were rebuilt and rejected by the complete
owner oracle. Direct RawByte locals/expressions, direct wire-byte locals,
pointer aliases, and direct XOR/bitfield assignments moved the owner to
2,048 ordinary differences, 242 field differences, and 26 switch-table
differences. The bitfield assignment has a superficially similar local
sequence but a different register allocation and instruction encoding.
Changing the helper's parameter order or forcing inlining did not improve it.
The original four-byte union snapshot remains rejected because it widens the
wire read.

## Remaining large-owner frontiers

- Opcode 4: seven ordinary differences remain around 0x00408938; the target
  uses a raw dword argument and a specific timer-assignment receiver schedule.
- Opcode 155: 23 ordinary differences remain around 0x0040B770; the source
  byte width is now explicit, but old-flags register lifetime and result
  placement still differ.
- EnemyManagerView::OnUpdate: 13 ordinary differences remain in its
  3,900-byte owner. Earlier pointer/index spelling probes did not improve the
  complete owner.

Continue with focused carrier builds and the complete diagnostic for only the
owner being changed. Do not run a full-project cold replay for these probes.
