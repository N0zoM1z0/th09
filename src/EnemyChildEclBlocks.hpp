#pragma once

struct EnemyView;

// Shared target-bound free-function interface at 0x004100E0.
// The original nominal owner is unknown. Use the native Enemy allocation;
// the opaque EnemyCoreView tag at other helper boundaries has no layout here.
void __fastcall EnemyCoreReleaseChildEclBlocks(EnemyView *enemy);
