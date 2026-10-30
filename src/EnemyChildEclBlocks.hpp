#pragma once

struct EnemyView;

// Shared target-bound free-function interface at 0x004100E0.
// The original nominal owner is unknown; do not redeclare its private layout
// as the unrelated complete EnemyCoreView used by the update reconstruction.
void __fastcall EnemyCoreReleaseChildEclBlocks(EnemyView *enemy);
