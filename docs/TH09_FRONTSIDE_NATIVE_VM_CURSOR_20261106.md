# FrontSide draw native ANM position cursor (2026-11-06)

The original TH09 1.50a `FrontSide::OnDraw` consists of a complete 3,094-byte
owner that iterates several arrays of animation VMs for HUD positions. In the
maintained source, the loop cursor points inside each `AnmVm` at position
`+0x208`, then reads a second position relative to that cursor at `+0x80`.
The loops advance by `0x2A4`, the size of the native VM. This unusual cursor
is a compiler-relevant source form, not a separately allocated VM type.

The new source constructs the cursor from the actual native member
`&vm->pos`, and converts the cursor back through
`offsetof(AnmVm, pos)`. Compile-time checks tie the whole `0x2A4` stride,
`AnmVm::pos +0x208`, `AnmVm::pos2 +0x288`, and cursor `pos80 +0x80` to the
same independently verified storage. The draw calls, target selection and
memory traversal are unchanged. No register hint, artificial padding,
volatile qualifier, assembly or byte patch is introduced.

A separate natural direct-`AnmVm*` cursor model was compiled. Although it
uses fewer projection types, VC7.1 emits **3,127** bytes instead of the
original **3,094**. It therefore fails the full-owner extent check and was
not adopted. The accepted member-anchored cursor instead reproduces all
3,094 original bytes under the existing pinned VC7.1 profile, with the
canonical relocation identities and physical function extent unchanged.

Local focused reproduction against the SHA-256 verified Japanese original:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/build-match-unit.py --unit front-side-draw
python3 -B scripts/compare-coff-function.py --unit front-side-draw
```

Both trial sources and results remain under the ignored
`.analysis/gpt-web-frontside-native-cursor-20261106/` for source-shape
review. The function was already exact, so this changes source quality and
unifies VM storage evidence but adds no new authored byte-exact credit. Native
product/runtime closure and original nominal translation unit remain open.
