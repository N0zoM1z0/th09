# FrontDraw uses native constructed panel vertices and Float3

The TH09 1.50a Front draw callback at 0x417C10 renders the two panel strips
from `FrontFactoryConstructView::panelVerticesE86C`, two sets of five real
`FrontPanelVertexView` elements of 0x14 bytes. The old `FrontLineVertex`
struct was an independent duplicate of that exact five-field storage.
Both the 318-byte line-strip renderer and its 1,474-byte caller now use the
real vertex type. The score text's three-float position is likewise a
`Float3` rather than a duplicate anonymous layout and reinterpret cast.

Pinned VC7.1 recompilation verifies both *complete* original PE bodies:
FrontDrawCallback **1,474/1,474 bytes and 127/127 relocations**;
FrontDrawLineStrip **318/318 bytes and 26/26 relocations**. The two caller
REL32 fields at +0xC5 and +0xD5 were independently checked against the
original PE target 0x422330 before changing the callee's mangled symbol.
Relevant native Supervisor/Front-message tests pass. These were already
counted exact; no new exact credit or full cold replay is implied.
