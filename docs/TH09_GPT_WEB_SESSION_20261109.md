# TH09 reconstruction session — 2026-11-09

## EnemyManagerView::OnUpdate: preserve the primary hitbox's third component

The 3,900-byte `EnemyManagerView::OnUpdate` owner was rechecked against the
original-bound IDA function at `0x00410730`. The current source still has
3,900 physical bytes, 97 relocation fields, 69 ordered calls and 170 direct
blocks. Its 13 ordinary differences remain confined to the previously
recorded draw-group index register and special-attack effect/descriptor
register and stack-home scheduling. No field or switch-table identity moved.

The primary hitbox at `EnemyView+0x2DBC` is a contiguous XYZ vector, not just
an XY pair: initialization writes all three components, trail collision
copies and scales the full `Float3`, and the damage call receives the same
three-float storage. Commit `05f3e3cb` records the field model and size guard.

Focused natural source controls for draw-head scope/reference spelling,
direct list insertion, descriptor pointer/member spelling, position copies,
and threshold aliases did not reduce the 13-byte frontier. Rejected
alternatives were restored; no register forcing, assembly, volatile or
literal/relocation guard relaxation was introduced.

## EnemyEclManagerView::RunEcl: opcode 4 and opcode 155

The original-bound `RunEcl` at `0x004086C0` is 14,792 code bytes and 15,564
physical bytes including its owned tables. The current linked carrier has
598 relocation fields and exactly 30 ordinary differences, with zero
field and zero table differences. The residuals are:

- Opcode 4 (`JUMP`): seven bytes in the `ZunTimer::operator=(int)` call-site
  argument/receiver register schedule. The target loads the raw integer into
  ECX and pushes it before loading the active timer receiver; the candidate
  loads the argument into EAX, loads the receiver, then pushes EAX.
- Opcode 155 (`SET_TIMEOUT_SPELL`): 23 bytes in the byte zero-extension and
  masked bit-24 update. The target uses `movzx ecx, byte [instruction+0x0C]`,
  keeps the old flags word in ESI, and computes the masked XOR through ECX
  and EAX. The current source uses a byte local and the same semantic masked
  XOR helper, but VC7.1 chooses a different register schedule.

Natural controls tested during this session included direct wire-member
expressions, int locals, shared switch locals, explicit timer operator calls,
timer pointer/reference aliases, byte and bitfield assignments, direct XOR
expressions, and helper parameter-width changes. Direct JUMP rewrites grew
the owner to 42 ordinary differences; several timeout rewrites changed
literal/field/table placement and grew the difference count above 2,000.
Those candidates were rejected and the maintained source restored.

At the previous committed checkpoint, opcode 155 represented the raw
four-byte argument as a Th09EclWireArgument local and consumed value.bytes[0].
That source form preserved the 15,564-byte physical owner, 598 fields, 30
ordinary differences, and zero field/table differences, but it widened the
source-level read beyond the byte consumed by the target. The follow-up
2026-11-09 probe below supersedes that source spelling with a one-byte copy;
neither form receives exactness credit.

## Focused validation

- `enemy-attached-effect-update`: 151/151 bytes exact.
- ECL main carrier: all 77 exact units passed; ECL Type11 carrier: all 6
  exact units passed.
- Enemy dependencies passed: draw implementation 1,758/1,758; bullet
  descriptor constructor 28/28; bullet pattern 203/203; positioned sound
  153/153; boss marker position 42/42; marker interrupt 32/32; marker state
  20/20.
- `check-boss-frontiers.py --owner enemy --skip-exact-siblings` passed with
  the unchanged 3,900/97/69/170 owner and the exact 82-byte collision leaf.

The broader enemy exact-sibling driver stops at the `anm-manager-execute-script`
comparison's local-label mapping output. This was reproduced with both
`EclRunState.inl` and `EnemyManager.hpp` temporarily restored to their
pre-session committed versions, so it is recorded as a pre-existing
comparison failure rather than attributed to the two source edits. All 77
EclManager exact units and all 6 Type11 units were independently compared
successfully on the current carrier.

No full-project cold replay was run. No partial-owner byte credit was
claimed for either large boss.


## Follow-up: byte-width and register-lifetime probes (2026-11-09)

The opcode-155 input is the byte at instruction +0x0C, not a four-byte
integer. The source now copies only that byte from the shared wire-argument
view before applying AssignTimeoutSpellBit to Enemy primary flags +0x337C:

    unsigned char value;
    memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));
    enemy->primaryFlags337C =
        Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value);

The focused pinned-VC7.1 build and inspect-ecl-complete.py report 15,564 bytes
/ 598 fields / 30 ordinary differences / zero field differences / zero table
differences. The non-crediting audit-ecl-timeout-bit.py independently decodes
both opcode windows: 12 instructions in each, eight instructions in each
flag-update prefix, and equal flag transitions for all 256 wire-byte values
across 12 representative 32-bit flag words (3,072 cases).

IDA's target-attested disassembly at 0x0040B770 shows the original order:
movzx ECX from byte [EBX+0x0C], load old flags into ESI, shift/XOR/AND bit 24,
copy old flags to EAX, XOR the changed bit, and store EAX. The current
candidate reads the correct byte and preserves the complete 30-difference
frontier, but its register/lifetime schedule is not exact: mov CL from the
wire byte, load old flags to EAX, then movzx ECX,CL. The original loads
directly into ECX and keeps old flags in ESI.

Several natural C++ alternatives were rebuilt and rejected by the complete
owner oracle. Direct RawByte locals/expressions, direct wire-byte locals,
pointer aliases, and direct XOR/bitfield assignments moved the owner to 2,048
ordinary differences, 242 field differences, and 26 switch-table differences.
The bitfield assignment has a superficially similar local sequence but a
different register allocation and instruction encoding. Changing helper
parameter order or forcing inlining did not improve it. The original four-byte
union snapshot remains rejected because it widens the wire read.

The remaining large-owner frontiers are unchanged: opcode 4 has seven ordinary
differences around 0x00408938; opcode 155 has 23 around 0x0040B770; and
EnemyManagerView::OnUpdate has 13 ordinary differences in its 3,900-byte
owner. Continue with focused carrier builds and complete diagnostics for only
the owner being changed; no full-project cold replay is needed.
