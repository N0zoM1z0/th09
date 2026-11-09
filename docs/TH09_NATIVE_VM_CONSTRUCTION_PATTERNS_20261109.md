# Three additional TH09 native VM construction patterns (2026-11-09)

The SHA-256-verified original Japanese TH09 v1.50a has three previously
origin-unclassified, complete member-construction bodies at `0x435D40`,
`0x435D70` and `0x435DA0`. Fresh original instructions establish ordinary
VC7.1 EH array construction with authentic `AnmVm` instances. They do not
identify the enclosing gameplay object or whether constructors were explicit.

| Original VA | Constructed layout | Physical bytes | Relocations | Unlike bytes |
| --- | --- | ---: | ---: | ---: |
| `0x435D40` | seven `AnmVm` at +8, one at +0x1284 | 39 | 3 | 0 |
| `0x435D70` | five `AnmVm` at +8, one at +0x0D3C | 39 | 3 | 0 |
| `0x435DA0` | three `AnmVm` at +8 | 28 | 2 | 0 |

Ordinary empty C++ constructors in `NativeVmGroupConstructors.cpp`, compiled
under the pinned original VC7.1 `/O2 /Ob0` profile, reproduce all **106 / 106**
physical bytes and all eight actual COFF fields. Every absolute/relative
operand targets either the original `AnmVm` constructor `0x403A10` or original
vector constructor helper `0x401470`. The independent comparison checks complete
raw COFF code, exact relocations, the original target body hash and linked bytes.
No manual calls to the EH constructor helper, assembly or padding are used.

Reproduction (only this constructor carrier):

```bash
nice -n 12 bash scripts/compile-probe.sh src/NativeVmGroupConstructors.cpp \
  build/matching/NativeVmGroupConstructors.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
python3 -B scripts/inspect-native-vm-group-constructors.py \
  build/matching/NativeVmGroupConstructors.obj
```

The three rows in `config/functions.csv` now indicate maintained candidate
source, but remain **unclassified**. These constructor bodies must not enter
the canonical authored exact ledger before independent authorship review.
They do not change the thirteen still-nonexact authored bosses.
