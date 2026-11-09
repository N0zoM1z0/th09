# TH09 native VM composite construction (2026-11-09)

An original-PE inspection identified the complete **227-byte** constructor
at 0x00435DC0. The original call order, operand widths, and member offsets
establish a much richer native C++ allocation than the previous unresolved
special-member ledger row. The mapped original is Japanese TH09 v1.50a;
its SHA-256 was checked before every byte comparison. IDA did not supply
a function body at this address, so the physical target instructions
and terminal RET at 0x435EA2 were decoded directly from the original PE.

`NativeVmCompositeOwnerView` now constructs nine **individual** `AnmVm`
members at 0x0000 through 0x1520 (they are not a nine-element array),
then four array members at +0x17C4. It contains 256 0x60-byte records
at +0x2264, then the exact seven+one, five+one and three-member VM groups
at +0x82A0, +0x97C8 and +0xA7A8. A scalar `AnmVm` is at +0xAF9C,
200 records of size 0x38 at +0xB240, and a final `AnmVm` at +0xDE08.
Each 0x38-byte record constructs one real `ZunTimer` at its +0x1C offset.
The whole C++ allocation is 0xE0AC bytes. Unassigned intervals stay
opaque rather than receiving speculative gameplay fields.

Ordinary default C++ constructors under the pinned VC7.1 `/O2 /Ob0`
profile reproduce all these code bodies, including authentic EH vector
constructor calls. Nothing directly calls the compiler helper in source.

| Original constructor | Physical bytes | Relocations | Unlike bytes |
| --- | ---: | ---: | ---: |
| Composite owner 0x435DC0 | 227 | 20 | 0 |
| Timer-bearing record 0x435AF0 | 15 | 1 | 0 |
| Seven+one VM 0x435D40 | 39 | 3 | 0 |
| Five+one VM 0x435D70 | 39 | 3 | 0 |
| Three VM 0x435DA0 | 28 | 2 | 0 |
| Empty 0x60 record alias 0x4343D0 | 3 | 0 | 0 |

**351/351 physical bytes and 29/29 relocation identities agree** with
the original target. The shared three-byte 0x4343D0 body is only a
source-equivalent alias, not unique new game code. The script checks every
body's original SHA-256, actual COFF symbol/offset/type/addend and linked
original-PE operand; mismatched fields cannot earn an exact result.

Run only this affected constructor compilation and comparison:

```bash
nice -n 12 bash scripts/compile-probe.sh src/NativeVmGroupConstructors.cpp \
  build/matching/NativeVmGroupConstructors.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
python3 -B scripts/inspect-native-vm-group-constructors.py \
  build/matching/NativeVmGroupConstructors.obj
```

These native special-member source equivalents remain origin-**unclassified**.
The target cannot distinguish an explicitly written empty constructor from
an implicitly generated one. No authored exact ledger row is added, and no
Windows product/runtime closure is claimed.
