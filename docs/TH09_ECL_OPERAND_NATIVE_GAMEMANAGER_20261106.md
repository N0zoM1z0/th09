# ECL operand GameManager storage: native C++ cleanup (2026-11-06)

The four exact ECL operand resolvers no longer manufacture an `int&` from
`reinterpret_cast<unsigned char *>(&g_GameManager) + 0x00B4`. The three
read/write sites for ECL wire operand `0x2739` now use the shared native
`AsciiGameManagerView::valueB4` directly, with an explicit i386 `offsetof`
assertion. All other operand cases and their original source-level expressions
are unchanged. This is not a claim about the original field's gameplay name.

## Fixed-profile complete original-bound replay

| Affected owner | Physical bytes | Actual COFF fields | Private label refresh |
| --- | ---: | ---: | ---: |
| `ecl-resolve-int` | 1,980 | 165 | 103 |
| `ecl-resolve-int-lvalue` | 726 | 39 | 37 |
| `ecl-resolve-float-lvalue` | 968 | 49 | 48 |
| `ecl-resolve-float` | 2,044 | 121 | 103 |
| **Total** | **5,718** | **374** | **291** |

All four independently compiled VC7.1 `/O2 /Ob1` owners reproduce the **whole
physical original-bound extent**, including adjacent switch data, and all
374 address fields. There are zero unlike bytes and no missing/excess data.
The original PE was SHA-256 verified before these comparisons. The already
exact function count does not increase.

Replacing the old reinterpret-cast expression changed compiler-private `$L`
labels across the four large switches. Only `symbol` spellings were refreshed
in `config/match-units.toml`: for each of the 291 changed labels, its actual
COFF section/value and owning function entry independently reproduce the
pre-existing original target address, with the same relocation kind and field
offset. The remaining fields retain their actual symbol identities. The
original target values, source ABI, compiler flags, physical sizes, and
function addresses were **not** modified. The manifest was then checked by
the unmodified strict canonical comparator for all four complete functions.

Focused reproduction:

```bash
python3 -B scripts/verify-target.py
for unit in ecl-resolve-int ecl-resolve-int-lvalue \
  ecl-resolve-float-lvalue ecl-resolve-float; do
  python3 -B scripts/build-match-unit.py --unit "$unit"
  python3 -B scripts/compare-coff-function.py --unit "$unit"
done
python3 -B scripts/validate-tracking.py --require-target
```

The ignored local source/COFF evidence and per-label original-location audit
are in `.analysis/gpt-web-ecl-typed-refreshed.json`. Neither renamed compiler
labels nor a local Git commit establish new authored exactness or Factory
acceptance. No target, library or compiler binary was patched.
