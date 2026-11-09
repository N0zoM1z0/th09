# TH09 Front and message native constructors — 2026-11-09

Original target: Japanese TH09 v1.50a, SHA-256
10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822.
The original executable's 143-byte Front constructor and 85-byte embedded
message constructor are independently reconstructed as natural C++ under the
pinned VC7.1 /O2 /Ob0 profile. Both match **all physical bytes and every
relocation identity**, after binding each original PE target operand.

| Original VA | Logical constructor | Bytes | Relocations | Differences |
| --- | --- | ---: | ---: | ---: |
| 0x004187B0 | FrontFactoryConstructView | 143/143 | 12/12 | 0 |
| 0x00418750 | FrontMessageRuntimeView | 85/85 | 7/7 | 0 |
| 0x004343D0 | FrontPanelVertexView *alias* | 3/3 | 0/0 | 0 |

The last target is a **shared three-byte constructor body** already used by
other native logical interfaces. The FrontPanelVertexView specialization
compiles to its complete same-code alias. It does not establish unique
physical ownership or the original class name.

The reconstructed Front allocation is 0x1293C bytes. Its first 88
AnmVm members begin at +0x0C, the ten genuine 0x14-byte plain-data
panel vertices at +0xE86C, and a 0x1D70-byte message runtime at +0xE944.
A scalar animation VM begins at +0x106B4, eight grouped VMs at
+0x10968, and paired animation groups at +0x11EAC and +0x123F4.

The message runtime at +0xE944 constructs eight AnmVm objects at its
own +0x14, two more at +0x1534, one scalar VM at +0x1A7C, and genuine
ZunTimer members at +0x1D40 and +0x1D58. This upgrades a former opaque
0x1D60-byte region to a real nested typed layout. Offset and total-size
assertions in the headers enforce those observed positions.

The previous FrontFactory.cpp had the right constructor *callee identity*
but hid the whole allocation behind a raw storage buffer. Source
definitions are now in FrontFactoryCtor.cpp with a typed header; the
canonical /Ob1 FrontFactory CreateGameSubsystem caller stays byte-perfect.
All three directly affected canonical FrontMessage consumers were rebuilt:
FrontAddedCallback, FrontMessageLoadForSideCount and
FrontMessageReleaseForSideCount remain exact.

Reproduce only affected carriers with:

    nice -n 12 bash scripts/compile-probe.sh src/FrontFactoryCtor.cpp \
      build/matching/FrontFactoryCtor.obj \
      /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
    python3 -B scripts/inspect-front-constructors.py \
      build/matching/FrontSideCtor.obj \
      --factory-object build/matching/FrontFactoryCtor.obj

The original-bound inspector checks complete PE bodies, per-owner original
SHA-256, precise COFF relocation offset/type/symbol/addend, and actual target
destinations. It also covers the two related FrontSide constructors. Four
named origin-unknown rows now point at the independent source-equivalent
constructor carriers in config/functions.csv. There is no new authored
reccmp/matches.csv row: **implicit versus explicit special-member origin
remains unknown**. No canonical authored exact credit or runtime closure
follows from these exact source-equivalent bodies.
