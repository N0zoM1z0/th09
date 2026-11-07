# TH09 FrontMessage exact 2,968-byte owner uses true native Player pointer

Original Japanese TH09 v1.50a binary, independently verified SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint is a normal C++ receiver and physical-layout reconstruction,
not unique historic nominal class-name proof or new canonical exact credit.

## Front message script and one Player allocation

The complete exact FrontMessage update handler at original code entry
0x00416590 (2,852 source code bytes + 116 physical switch/data tail,
**2,968 bytes total**) previously declared a private
`FrontMessagePlayerView` with its own +0x303C8 timer, +0x303F0/+0x303FC
state words and local `SetUpdateState`. The side's `player04` at +0x04
is in fact the same 0x30F70-byte `PlayerLifecycleView` allocation used by
Enemy, Player runtime, and the original 12-byte state setter.

The script now directly declares `PlayerLifecycleView *player04`, with
compile-time checks of the full native Player size, +0x00 update state,
+0x303C8 real `ZunTimer`, and +0x303F0/+0x303F8/+0x303FC state words.
It calls the real native Player's `SetUpdateState(1)` at the unchanged
original **0x00403C20** target, then resets the same native Player timer.
No duplicate Player object is allocated, and the original front-side
portrait animation, ECL input state, message string decoding and script
switch/choreography are unchanged.

## Complete immutable PE and COFF proof

After the ordinary C++ source change, the maintained `FrontMessageUpdate.cpp`
was compiled under its fixed VC7.1 original profile. An independent
original-PE-field audit verified all **2,968 physical bytes and 167 COFF
relocation fields**, including the full original jump/switch table tail.
The source change renumbered **30 compiler-private `$L`/`$run_scripts$`
local labels**, and one external call symbol changed from the old partial
Player class to the true Player at **unchanged REL32 offset +0xA6 / target
0x00403C20**. Each local symbol was required to have an actual
same-section COFF definition at the exact original target VA and correct
original PE field operand; only those 30 proven private names were
rebound in the strict original match manifest.

The strict canonical comparator still returns exact for the full
**2,852-byte function and 2,968-byte physical owner / 167 fields**.
The native Player setter provider remains independently exact (12 bytes).
The affected source-only replay validates both complete FrontMessage
functions and the setter leaf, rather than running a whole-project cold
replay. The original-bound audit receipts and trial files are under
`.analysis/gpt-web-front-message-native-player-20261107/`.

No new canonical exactness is credited, and the still nonexact 3,900-byte
Enemy OnUpdate and 15,564-byte RunEcl bosses are not changed. No Windows
product link, runtime/semantic acceptance, Factory receipt or remote push
is asserted. Global ledger remains **1,034 source-present / 1,021 exact /
13 nonexact**.
