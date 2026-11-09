# Front result bonus uses the constructed Front and real GameManager pointer

The original TH09 v1.50a `InitializeResultBonus` at `0x415E50` works on
the same **0x1293C-byte Front allocation** as the factory, message runtime
and draw/calc callbacks. It now belongs to `FrontFactoryConstructView`.
Its five result animation VMs are `resultVms10968[3..7]` (original +0x11154),
its bonus values are `resultValues11E88[0..6]`, and its mode/frame/side
fields are the actual constructed members at +0x1095C/+0x10960/+0x10964.
The prior duplicate `FrontResultBonusView` has been removed.

The original Front message interpreter at +0x88F reads a dword at
**0x4A7E38**, equal to `g_GameManager` at `0x4A7D90` plus **0xA8**.
The shared native GameManager now exposes its real Front pointer at +0xA8;
opcode 9 calls `g_GameManager.frontFactoryA8->InitializeResultBonus()`.
The former source-only `g_FrontMessageOwner` global and its proxy method
are gone. The original PE operand was separately checked before binding
the new true GameManager-root relocation and its +0xA8 addend.

The pinned VC7.1 full original-bound replays establish:

- `FrontFactoryConstructView::InitializeResultBonus`: **553/553 bytes,
  27/27 relocation fields**, exact, unchanged original target `0x415E50`.
- `FrontMessageRuntimeView::Update`: **2,968/2,968 physical bytes,
  167/167 relocation fields**, exact, including the whole switch table and
  unchanged original bonus call at +0x894.
- The Front calc/draw and GameManager source consumers remain exact. The
  `Supervisor::OnUpdate` 996-byte/92-field carrier also remains exact after
  40 private `$L` names were checked against actual COFF definition offsets
  and the immutable original PE targets.
- The still-nonexact 1,210-byte `PlayerChargeAttack` keeps its previous
  235 difference bytes, 70 fields and original 53-block control-flow graph.

All 11 directly compared GameManager sibling functions remain exact. The
focused FrontResult, FrontMessage and charge tests pass. No full cold replay
or increase of canonical authored exact credit is claimed.
