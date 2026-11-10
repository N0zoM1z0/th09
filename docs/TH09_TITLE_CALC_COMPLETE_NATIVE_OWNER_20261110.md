# TH09 TitleCalc now dispatches on the complete constructed TitleScreen

The original Japanese TH09 v1.50a `TitleCalcCallback` at **0x42A9CB**
comprises 264 bytes of executable code followed by two switch tables,
for **328 original physical bytes**. Previously it declared its own
partial `TitleScreenView` prefix for the draw/calculation receiver despite
the complete **0x1B388-byte** native TitleScreen already used by the
main Title state machine, draw callback, Replay menu and result renderer.

The callback now includes the shared native `TitleScreenReplayView.hpp`
and uses its actual VM pointer +0x11B98, help-text VM +0x11B9C,
VM count +0x1B224, screen code +0x1B228 and chain state +0x1B238.
The existing two exact method owners `OnUpdateKeyConfig` (0x427EE8)
and `OnUpdateMusicRoom` (0x426E05) are explicitly declared on this
same real receiver. All branches and the original AnmManager script-array
and help-VM calls remain unchanged.

The complete pinned VC7.1 original-bound comparison reproduces
**328/328 bytes and 36/36 relocation fields**. Seventeen compiler-private
switch labels changed names because the source now includes the full
Title header. Each true COFF same-section definition was checked against
the immutable original PE destination before the local symbol spelling
was rebound. A separate nine-label rebind on the 1,592-byte Title
OnUpdateStartMenu owner was proved in the same way.

Scoped recompilation of **47 Title exact functions, 24,447 bytes and
1,270 references**, including the full StartMenu, Options, replay-save
draw, result-draw and native TitleDraw, remains strict-original-exact.
The still-nonexact 1,280-byte replay-menu draw stays at **57 bound
references and 29 differing bytes**. Local Title/music-room/Replay tests
pass. No new canonical exact credit or full cold replay is claimed.
