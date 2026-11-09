# TH09 FrontSide constructor recovery — 2026-11-09

The original Japanese TH09 1.50a executable has two adjacent constructor
bodies which are reproduced **completely** by ordinary, typed C++ member
construction under the original VC7.1 **/O2 /Ob0** compilation profile.

- At 0x0041A470, **FrontSideAuxView** has **28 / 28** bytes and **2 / 2**
  original relocation fields with no byte differences.
- At 0x0041A490, **FrontSide** has **307 / 307** bytes and **26 / 26**
  original relocation fields with no byte differences. This includes
  independently sized AnmVm member arrays, individual AnmVm members,
  the auxiliary view and two real ZunTimer members.
- The independently exact FrontSide::Create (0x0041A5D0) calls the latter
  constructor at its verified original address. The latter calls the
  auxiliary constructor at its original address.

The members and array bounds were already visible in FrontSide.hpp. The
newly recovered compiler-context fact is their different **/Ob0** carrier.
Compiling both definitions inside FrontSide.cpp with its canonical /Ob1
profile emits 375/38 bytes and loops through AnmVm constructors.
Moving only those two ordinary definitions to src/FrontSideCtor.cpp
under /Ob0 reproduces the exact original 307/28 byte bodies.
No padding, inline assembly, fabricated helper calls or codegen hints.

The original-bound comparison in scripts/inspect-front-constructors.py
checks target hashes, physical sizes, all COFF relocation symbols, types,
offsets and addends, original PE address operands, and every linked byte.
To reproduce:

    nice -n 12 bash scripts/compile-probe.sh src/FrontSideCtor.cpp \
      build/matching/FrontSideCtor.obj \
      /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
    python3 -B scripts/inspect-front-constructors.py \
      build/matching/FrontSideCtor.obj
    nice -n 12 python3 -B scripts/build-match-unit.py --unit front-side-create

The full set of **15/15 existing exact functions** in the FrontSide.obj
carrier were recompiled and replayed with all physical bytes/relocations
after the split. Caller ABI and target operands remain unchanged.

**Origin classification remains open:** these could be implicitly
compiler-generated special members, so do not add them to the authored/exact
manifest. This is source-equivalent original byte proof, not a linked-product
or native-runtime closure claim.
