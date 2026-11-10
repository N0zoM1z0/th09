# Bounded native text position view in the nonexact Title replay-menu boss

The original TH09 v1.50a `TitleScreenView::DrawReplayMenu` at **0x4234F6**
uses the native `AsciiManager::AddFormatText(Float3*, ...)` text renderer.
Its maintained source still needs the original SDK `D3DXVECTOR3` math
operators to preserve the VC7.1 x87 instruction scheduling in the
selected-replay interpolation branch. Both types occupy the same verified
12-byte x/y/z layout, but historically the source repeated eight casts
from the SDK vector to `Float3*` at individual text-draw calls.

The source now makes **one checked, borrowed `Float3 &textPosition` view**
of the same `D3DXVECTOR3 position` allocation and passes `&textPosition`
to all eight native text calls. It also names the SDK displacement vector
used in the interpolation formula, without changing the arithmetic order.
Neither the rendering position nor the serialized ReplayData records are
copied or separately allocated.

Two scoped experiments were completed: 16 original record/selection
expression shapes and 12 new SDK/native vector and interpolation shapes.
The pure-native `Float3` expression introduces unsupported relocation
identities, and early ReplayData/mode-index snapshots change hundreds of
code bytes. The selected borrowed-view and displacement combination
preserves the complete **1,280-byte candidate, all 57 original-bound
relocations, 13 ordered calls, 72 CFG blocks and 16 verified literal
symbols**, with the same **29 unlike bytes**. It is still **NONEXACT**;
no new canonical exact credit is claimed. The source and strict
PE/COFF diagnostic were checked together, with no full cold replay.
