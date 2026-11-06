# RunEcl wire-argument storage and full-owner replay — 2026-11-06

The maintained nonexact 15,564-byte `EclManager::RunEcl` now accesses the
native variable-length instruction's `arguments` field instead of computing
`instruction + 0x0C` in three places:

- `Th09EclRunState::RawByte`: byte-offset access through the actual wire tail;
- `Th09EclRunState::RawShort`: short access through the same wire tail;
- opcode `SET_TRAIL`: its typed `TrailInstructionArgs` reads from that tail.

`EclOpcodes.hpp` already asserts `sizeof(Th09EclWireArgument)==4` and
`offsetof(Th09EclRawInstructionHeaderView, arguments)==0x0C`. The two
integer-width readers preserve their existing little-endian storage behavior;
the trail's 16-byte typed view and opcode control flow are unchanged. This is
natural C++ source consolidation, not a new ABI, helper entry or invented
owner. Nominal original type/TU ownership remains unresolved.

## Original-bound compiler evidence

Four separate fixed-profile source trials changed only the byte reader,
short reader, trail reader, or all three. Each compiled the whole real ECL
translation unit, checked 15,564 physical original target bytes, all 598
actual COFF fields and complete tables, and retained exactly 30 unlike
ordinary bytes (opcode 4: seven, opcode 155: 23). No field/table difference,
new missing byte or target substitution occurred. The combined typed model
is adopted; the remaining opcode mismatches grant no exactness credit.

The adopted source requires only eight compiler-private owner-local label
renames: seven fields of the independently complete
`ecl-post-update-movement` (992 physical bytes/41 fields) and one of
`ecl-compare-operands` (892 bytes/37 fields). Every field was mapped from its
**actual compiled symbol section and offset** to the pre-existing original
VA before changing the canonical manifest; all other relocation identities
remain as before. Both whole owners replay exactly.

The full `--owner runecl` focused gate was then rebuilt/replayed, not a
whole-game cold replay. It passes **99 complete canonical comparisons**
(30,461 physical bytes / 1,404 fields), the RunEcl two-window 30-difference
guard, 146/148 matching opcode-handler shapes, the original timer-jump and
timeout-bit behavioral transfer checks, and the separate exact 516-byte ECL
movement helper. The real `EnemyManagerView::OnUpdate` in the same main
object still contains 3,900 physical bytes/97 fields/13 differences at the
previously protected offsets. No already-matching byte was sacrificed.

Five additional ordinary value/reference/timer-receiver models of opcode 4
were compiled and completely compared, but all **regressed** from 30 to 42
RunEcl differences (zero field/table differences). None is adopted; see the
ignored `.analysis/gpt-web-runecl-jump-20261106/results.json`. These are
bounded negative results, not an impossibility proof or an authorization for
register hints, `volatile`, assembly, barriers or compiler profile changes.

Focused reproduction:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner runecl
python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/inspect-enemy-manager-update.py build/matching/EclManager.obj --json
python3 -B scripts/validate-tracking.py --require-target
```

The adopted trial objects/reports are ignored evidence at
`.analysis/gpt-web-runecl-wire-typed-20261106/`; the 8-field original-bound
label audit is `.analysis/gpt-web-runecl-wire-labels.json`. Local Git
checkpoint, source fidelity, full-owner byte exactness, Factory acceptance,
native Windows product closure and runtime semantics remain distinct.
