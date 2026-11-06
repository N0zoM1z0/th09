# ScreenEffect shake: native GameManager side flags (2026-11-06)

The complete `ScreenEffect::CalcShake` owner now reads the native
`g_GameManager.sides[contextIndex].flags & 1u` instead of obtaining the
least-significant flag byte through a `reinterpret_cast<unsigned char *>`.
The real `AsciiGameManagerSideView` uses a `0x38`-byte stride with its flags
word at side offset `+0x34`. A compile-time offset assertion pins both
boundaries. The bit test still operates on precisely the original low bit,
without inventing an extra byte-wide GameManager subobject or new branch.

The fixed VC7.1 target-bound build shows that four natural member-access
forms emit the exact same **316-byte `CalcShake`** owner. The direct native
member form was adopted. Its source is part of the same real translation
unit as the larger **336-byte `CalcShakeEnvelope`**, so a source-only leaf
check would not suffice. Focused `--owner screen` replay was run across all
three actual compiler profiles and **18 existing exact source units**:
**2,562 complete physical bytes, 164 bound fields**, all exact.
The still-open envelope retains **336/336 physical bytes, 20 fields, 8 calls,
26 direct blocks and 26 unlike bytes**; this source change does not claim its
codegen is exact.

VC7.1 changed nine *compiler-local* `$L` symbol names in the separate
`RegisterChain` source-profile object. These were independently observed
from actual COFF section/owner-relative values before updating the canonical
match recipe. All nine point to their previously reviewed original addresses
at the same field offsets and kinds. The entire **480-byte RegisterChain**
linked source reproduces the original PE before and after label refresh.
No external symbol, addend, target destination, code byte or match status was
changed to accommodate the new labels.

Reproduce only affected source profiles:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner screen
python3 -B scripts/validate-tracking.py --require-target
```

Evidence in ignored `.analysis/gpt-web-screen-side-20261106/`,
`.analysis/gpt-web-screen-native-labels.json` and
`.analysis/gpt-web-screen-native-side-focused-accepted.json` is local, not a
Factory or remote acceptance receipt. All affected owners were previously
exact except the unchanged envelope; the authored exact numerator stays 1021.
