# PlayerRuntime: shared native Player movement and ANM VM members (2026-11-06)

This checkpoint replaces multiple repeated raw layout expressions in the real
Player runtime source with **already independently reviewed native C++
subobjects**. It is not a change of compiler profile, a fake class, injected
code, a binary patch, register forcing or an added authored function.

The recovered source now uses:

- `PlayerLifecycleView::movementFields1CCC` to initialize four speed
  multipliers (absolute offsets `+0x1CDC`, `+0x1CE0`, `+0x1CE4`, `+0x1CE8`)
  during respawn and reset, and as the actual source of velocity during the
  400-byte state-4 collision routine.
- The native `PlayerAnmVmView` fields `scale.x`, `scale.y`, and `color1` during
  transition and flashing. Those are the same animation VM words at +0x18,
  +0x1C and +0x1F0 independently used by ANM rendering and title drawing.
- `&player->header24` when the 522-byte OnUpdate invokes the exact Player
  selector, instead of reconstructing its already-known +0x24 subobject with
  byte addition.
- `&tailStates30454[0].storage[4]` to identify the draw record inside the
  actual four-element, 0x2C4-stride tail-state array, instead of an absolute
  `this + 0x30458` cast. The inner tail-state draw interpretation remains
  provisional rather than claiming a new original class name.

Compile-time assertions in this source verify each native owner offset, VM
word and tail stride. The regular C++ transformations remove two dozen
unnecessary offset/cast operations while preserving the original call
chronology and scalar behavior. A 11-model local source study (individual
and combined cases) checked *complete* original PE-linked byte bodies; each
maintained all previously matching bytes for all 17 main-profile owners.

## Focused fixed-profile proof

The original target is Japanese v1.50a with SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
All **21 current exact functions** from the source were built in their five
existing VC7.1 `/O2` profiles (one main `/Ob1` carrier and four independently
named alternate carriers), then compared to the complete original owners:
**5,391 matching physical bytes and 288 independently bound fields**.

The main 17-function carrier contributes 4,368 bytes / 243 fields. Among
those, `PlayerLifecycleView::OnUpdate` remains exact at 522 bytes and the
852-byte physical `UpdateBeforeState` (814 code bytes plus table) remains
exact. Its C++ source lexical changes caused **11 compiler-private `$L`
label renamings** at existing original data fields; each new name was checked
using its actual i386 COFF section/value, plus the same independently retained
original target coordinate. Every byte of the linked physical owner was
compared before rewriting only private label spellings in the manifest. No
field offset, relocation type/addend, external identity, physical extent or
original destination changed. All 21 canonical strict comparisons pass.

The other four carrier owners—constructor (243 bytes), SHT file loader (172),
collision-region updater (273), and bomb-state updater (335)—also remain
exact. No full cold game replay, native product link or runtime scenario was
run. All functions had been previously counted as exact: the authored
numerator remains **1,021** and no new exactness credit is claimed.

Reproduce with one build of `player-on-update`, and one build of each of the
four other named units, followed by canonical original-byte comparisons for
the 21 `src/PlayerRuntime.cpp` entries in `config/match-units.toml`.
The ignored proof and tested individual models are under
`.analysis/gpt-web-player-runtime-natural-20261106/` and
`.analysis/gpt-web-player-runtime-native-labels.json`.
