# Native Bullet, Laser and sprites constructors

The original Japanese TH09 v1.50a `EtamaController` constructor at
`0x415050` calls three distinct constructor bodies using real native
members of the already recovered `BulletManager.hpp` types. Its complete
121-byte/10-field body independently proves the constructor destinations.
The pinned VC7.1 `/O2 /Ob0` compiler generates those bodies from ordinary
empty C++ constructors in `BulletNativeConstructors.cpp`:

| Native source body | Original address | Complete bytes | Fields | Differences |
| --- | --- | ---: | ---: | ---: |
| `BulletTypeSprites::BulletTypeSprites` | 0x414FA0 | 56 | 5 | 0 |
| `Bullet::Bullet` | 0x414FE0 | 58 | 5 | 0 |
| `Laser::Laser` | 0x415020 | 34 | 3 | 0 |
| Implicit `BulletExState` construction alias | 0x41EE20 | 12 | 1 | 0 |

`BulletTypeSprites` constructs five genuine `AnmVm` members; `Bullet`
constructs its native sprite group, two `ZunTimer` fields and an array of
seven transform states; `Laser` constructs its two VMs and a timer. The
implicit `BulletExState` constructor emits the same original 12-byte
physical code as the already authored-exact `PlayerState3031CView`
constructor. It is an alias, **not** 12 new unique bytes or a new accepted
function.

The full original-bound constructor check verifies 160 source-equivalent
bytes and 14 fields. The three separate previously unclassified bodies
contribute 148 bytes of distinct source-equivalent construction but remain
**origin-unclassified**: matching compiler output does not distinguish
historical explicit C++ special members from implicit ones. The existing
canonical authored exact count remains unchanged. A positive check passes;
a deliberately changed first source-code byte in a copy of the compiled
COFF produces a nonzero exit and a difference at offset zero.

Reproduce only this constructor carrier and its exact parent:

```bash
nice -n 12 bash scripts/compile-probe.sh src/BulletNativeConstructors.cpp \
    build/matching/BulletNativeConstructors.obj \
    /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob0 /Oy- /I src
python3 -B scripts/inspect-bullet-native-constructors.py \
    build/matching/BulletNativeConstructors.obj
nice -n 12 python3 -B scripts/build-match-unit.py --unit etama-controller-constructor
python3 -B scripts/compare-coff-function.py --unit etama-controller-constructor --json
```
