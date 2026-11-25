# TH09 semantic reconstruction and readability policy

Semantic reconstruction replaces layout-shaped source—raw protocol numbers,
fragmented `*View` overlays, `/ALTERNATENAME` view shims, offset-suffixed field
names, and anonymous storage—with evidence-backed C++ while preserving both
exact VC7.1 code generation and verified Windows i386 position-independent
behavior.

Work is tracked sequentially in [SEMANTIC_PLAN.md](SEMANTIC_PLAN.md). This
policy adapts the cross-title method distilled in TH08's
`SEMANTIC_RECONSTRUCTION.md` and `SEMANTIC_PLAYBOOK.md` to TH09 v1.50a.

## Evidence classes

- **Observed:** TH09 v1.50a access offsets, widths, signedness, bit masks,
  callers/callees, strings, relocations, exact VC7.1 COFF output, and verified
  runtime or replay behavior.
- **Corroborated:** independent TH09 producers and consumers agree, or
  adjacent-title (TH08 / TH095) concepts agree with TH09-local evidence.
  Adjacent-game names remain hypotheses until TH09 evidence confirms them.
- **Inferred:** TH09 dataflow supports a neutral structural or behavioral role
  without an independent string or resource name.
- **Unknown:** only storage width, offset, or alignment is known. Keep it
  explicitly neutral (`unknown...`, `unconsumed...`, `reserved...`).

## Source rules

1. **Preserve VC7.1 i386 ABI and exact codegen.** Field offsets, widths,
   signedness, packing, bitfields, class sizes, virtual/special-member
   signatures, and compiler-sensitive expression shapes must remain exact.
2. **Pin relied-on layout facts.** Retain or add compile-time `sizeof` and
   `offsetof` assertions whenever unifying `*View` types or renaming members.
3. **Name to the strongest evidenced rung.** Prefer verified behavioral roles
   over guesses; keep unconfirmed slots neutral rather than inventing flavor
   names.
4. **Separate namespaces.** Keep ANM file slots, script IDs, sprite IDs, and
   runtime VM indices distinct. Keep ECL opcodes, ECL operand selectors,
   background opcodes, dialogue opcodes, sound IDs, and effect IDs in their own
   explicit enums.
5. **Keep wire formats byte-oriented where true.** Serialized headers,
   variable-length script streams, and Win32/DirectX ABI glue stay faithful to
   their actual memory representation.
6. **Guard closed protocols.** Once an enum or stable ID surface is closed, add
   it to `scripts/check-semantic-protocols.py` so raw magic literals cannot
   silently return.
7. **Do not regress exactness or PI.** Never introduce inline assembly, dummy
   barriers, profile roulette, or behavioral changes under the guise of
   readability.

## Validation matrix

Select verification proportional to the change surface:

| Change surface | Required validation |
| --- | --- |
| Docs / scripts / tests only | `python3 -B scripts/ci.py`, affected unit tests, `git diff --check` |
| Private enum / rename in one TU | `python3 -B scripts/replay-scoped-exact.py --source src/FILE.cpp` |
| Shared header / `.inl` / `*View` merge | `python3 -B scripts/replay-scoped-exact.py --changed-file src/FILE.hpp` (inspect `--dry-run` scope first) |
| Symbol signature or `/ALTERNATENAME` change | Above + rebuild/link trials (`scripts/trial-link.py`) and run `scripts/verify-trial-layouts.py` |
| Behavior / ownership / static data migration | Above + `scripts/probe-trial-replay.py` against retail |
| Closed protocol guard update | `python3 -B scripts/check-semantic-protocols.py` + `python3 -B scripts/ci.py` |

Note on `config/match-units.toml`: if unifying a `*View` type legitimately
changes a C++ mangled symbol name (for example, replacing a view receiver with
the canonical class receiver), verify that the compiled machine bytes and
relocation targets remain 100% identical to the target before updating the
canonical manifest symbol and reducing `/ALTERNATENAME` aliases.
