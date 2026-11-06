# ANM reader native slot path, 2026-11-06

The complete **390-byte** AnmManager::ReadAnmEntries source now addresses
its filename via the actual AnmManagerReadSlotView::path member, instead of
casting AnmLoaded* to a char* and adding a magic 0x20.

One manager has native 0x120-byte slots containing AnmLoaded (0x1C),
a releasePending word (+0x1C), and path (+0x20). The source now obtains one
AnmManagerReadSlotView pointer from the existing manager array; both the
AnmLoaded pointer and filename buffer derive from the same slot.
The existing compile-time native layout assertions already pin these fields.
This clarifies ownership without assigning path to the smaller resource.

Six natural C++ variants were compiled with the pinned VC7.1 profile.
Three native slot-member variants were whole-owner exact; direct repeated
indexing or an independent destination alias instead increased the function
from 390 to 399 bytes and were rejected.

The selected source was compiled once and checked against **all 16 exact
functions in the /O2 /Ob1 ANM loader source object**: every full owner
retained exact original bytes and its reviewed relocation fields. The
separately compiled /O2 /Ob0 constructor also remained full-original exact
(341 bytes, 29 fields). No new authored-byte credit, target modification,
padding or artificial register forcing was needed.

Focused reproducibility:

~~~bash
python3 -B scripts/build-match-unit.py --unit anm-manager-read-anm-entries
python3 -B scripts/compare-coff-function.py --unit anm-manager-read-anm-entries
python3 -B scripts/build-match-unit.py --unit anm-manager-constructor
python3 -B scripts/compare-coff-function.py --unit anm-manager-constructor
python3 -B -m unittest discover -s tests -p 'test_anm_read_slot_native_path.py'
~~~

These source/COFF comparisons are not fresh Factory receipt acceptance,
native runtime validation or remote Git publication.
