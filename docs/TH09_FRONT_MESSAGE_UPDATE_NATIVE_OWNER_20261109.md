# Front message interpreter now shares the constructed native runtime

The full original-bound `FrontMessageRuntimeView::Update` at 0x00416590
is now compiled against **the same** 0x1D70-byte `FrontMessageRuntimeView`
class as the exact Front factory constructor, Draw, Load and Release. The old
`FrontMessageUpdate.cpp` contained its own duplicate full runtime definition
and a separate 0x10-byte instruction header, even though the actual native
wire header has a variable-length 0x08-byte prefix.

The independent full-runtime source class was removed. Opcodes with wider
arguments use the narrow `FrontMessageLongInstructionView` derived from the
shared 8-byte wire prefix, explicitly laying out trailing +8 and +0xC
32-bit operands. The four groups of portrait VMs are **typed projections**
over the genuine eight-member `AnmVm` array that the native factory
constructs. Each pair is 0x548 bytes and comprises two actual 0x2A4-byte VMs.
This avoids introducing a second constructor-owned array or a fabricated
call boundary. Compile-time assertions check these original physical offsets.

The message interpreter's switch, timer, side-state, graphics, text and input
behaviors have **not** been rewritten to fit a smaller partial excerpt:

| Real source owner | Original bound physical bytes | Relocations | Difference count |
| --- | ---: | ---: | ---: |
| `FrontMessageRuntimeView::Update` | **2,968/2,968** (2,852 code + 116 table) | **167/167** | **0** |
| Same TU `DecodeFrontMessageString` leaf | **39/39** | **0/0** | **0** |

The pinned VC7.1 `/O2 /Ob1 /Oy-` carrier was freshly compiled. All 30
renamed compiler-private owner-local `$L` table labels were checked
*individually* against their actual same-section COFF definitions and the
original PE destinations before changing only their spellings in the exact
manifest. No relocation offset, symbol target, byte extent, switch destination,
call ABI, original literal or historical exactness category was relaxed.

A separate natural C++ control replaced the 38 typed portrait accesses with
one local pointer snapshot. It emitted **2,972 bytes** instead of the original
2,968, so the control was rejected and the original-equal typed accesses
retained. This demonstrates that source alias lifetimes can affect VC7.1
without implying a semantic difference. The repeat casts are a bounded
source-facing view of the original constructed VM array, not a separate
allocation or a register-forcing hint.

Reproduce just the affected carrier and its decoder profile:

```bash
nice -n 12 python3 -B scripts/build-match-unit.py --unit front-message-update
python3 -B scripts/compare-coff-function.py --unit front-message-update --json
nice -n 12 python3 -B scripts/build-match-unit.py --unit front-message-decode-string
python3 -B scripts/compare-coff-function.py --unit front-message-decode-string --json
```

The prior `9e0efd2f` native state checkpoint independently protects the other
nine Front consumers and constructor chains. This code reuse is meaningful
C++ source recovery but **not** a new authored exact-function credit: these
function extents were already original-exact. No full cold replay, native
Windows product link or runtime-semantic gate was performed.
