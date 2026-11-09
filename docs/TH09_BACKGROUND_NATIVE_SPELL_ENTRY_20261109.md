# Native Background spell entry in the RunEcl boss

TH09 v1.50a's 21-byte `BeginSpellBackground` at `0x4067B0` changes
`Background::spellBackgroundState` at +0xCCC to 1 and
`spellBackgroundFrame` at +0xCD0 to 0. Both fields belong to the full
0x65C4-byte constructed Background, not a separate 0xCD4-byte object.
The method now belongs to `Background`, and RunEcl opcode 186 invokes it
on the real side Background pointer without a cast. The redundant
`BackgroundSpellStateView.hpp` was deleted.

The original PE RunEcl REL32 operand at +0x3654 still resolves to
`0x4067B0`. Pinned VC7.1 gives an original-exact 21-byte leaf, and
RunEcl retains **15,564 bytes, 598 bound references and the previous
30 ordinary differences at opcodes 4 and 155**. Its complete callsite
audit recognizes the new member identity. The nine exact Background
owners (2,828 bytes, 171 references) and 77 ECL exact siblings (24,368
bytes, 1,096 references) remain exact. Twenty private ECL switch labels
were rebound only after the actual COFF definitions matched original PE
jump targets. No full cold replay was performed.
