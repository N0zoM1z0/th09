# TH09 ANM shared file-slot view (2026-11-06)

The exact ANM manager file accessor `GetAnm` previously computed a raw
`this + 0x24 + index * 0x120` address. Meanwhile `ReadAnmEntries` had a
target-reviewed typed view of exactly that allocation, with an `AnmLoaded`
resource followed by `releasePending` and a filename buffer. The new
`src/AnmManagerFileSlotView.hpp` shares that view between the two real
source translation units, rather than introducing an independent copy of the
layout or changing the allocation/ABI.

- Manager's first file slot begins at `+0x24`; slot stride `0x120`.
- `AnmLoaded` occupies the slot's first `0x1C` bytes.
- `releasePending` is at slot `+0x1C`, `path` at slot `+0x20`.
- `GetAnm(index)` returns `&slots[index].loaded`, preserving the exact
  pointer expression and the unmodified original handling of index values.

`ReadAnmEntries` still uses its original member order, file pointer and
filename-copy sequence. The original class name and complete manager/TU
ownership are unknown; the shared view asserts only observed native storage.

## Focused original-bound compiler proof

From the pinned Japanese TH09 v1.50a original, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
VC7.1 was run under the existing canonical profiles. The 5 exact ANM
Background units and 16 exact ANM Load units are independently compared with
the SHA-attested PE after source recompilation. The additional single ANM
constructor under its `/Ob0` profile is independently rebuilt/verified.
All **22** complete owners remain exact with unmodified addresses, bytes,
relocation names, destinations and total extents. Specifically, GetAnm is
20/20 original bytes, ReadAnmEntries is 390/390, and the full constructor is
341/341. This changes no authored exact-credit total.

The alternative loader slot view (`AnmManagerLoadSlotView`) remains separate at
this checkpoint; it has a different base-relative perspective despite
mapping some of the same physical words. Do not assume type identity solely
from equivalent numeric addresses. A distinct compiler trial is required
before removing or replacing that view.

Only the affected units are replayed. No whole-program cold replay, product
closure, runtime semantic acceptance or target-byte modification is implied.
