# TH09 focused reconstruction — continuation, 2026-10-31

This live checkpoint uses Japanese TH09 1.50a, PE SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
It distinguishes observed complete original/COFF comparisons from canonical
exact matches and whole-product/runtime closure. None of the losing C++ probes
below is adopted. No full cold replay was run.

## Front weighted picker — strict current-source regression evidence

`FrontMessageRandomView::SetupRandomForSide` at `0x004162D0`, complete
693-byte target and candidate, is currently **351 different bytes**, 205/206
decoded instructions, 15 independently identified original referents, six
ordered calls and 41/40 direct CFG blocks. It is **non-exact**. Dated 681-byte
or 569-difference descriptions are not the current compiler source. The
current SHA-pinned PE and complete pinned VC7.1 source COFF independently
reproduced these values; the baseline object and JSON are retained under
`.analysis/gpt-web-front-current-20261031.json` and
`.analysis/gpt-web-front-current-literals-20261031.json`.

A previously unguarded proof gap was identified: two source relocations at
`+0x274` and `+0x29C` had the *correct* reviewed C-string symbol and target
address (`0x0048EE7C`) but the diagnostic did not check the actual two-byte
COFF **definition**. It now checks the real source string `" "` plus NUL,
its initialized read-only section and owner, original PE payload, and both
source field references. A target-independent adversarial test mutates the
actual string bytes/terminator without altering the relocation metadata,
proving the new guard rejects this input. The old guard would have counted
exactly the same 351 original-linked code-byte differences.

The Front boss gate also protects the **342 previously equal physical byte
positions**, through an independently recorded 693-bit byte-residual mask.
New or swapped unequal bytes fail even if the difference count is unchanged.
The complete 693-byte bound, no missing bytes and the literal definition
inventory are required; original identities and call sequence are unchanged.

`python3 scripts/check-boss-frontiers.py --owner front` freshly recompiles
the Front owner and protects four previously exact Front siblings (2,386
summed physical bytes, 144 relocation fields), all passing. There is **no new
canonical exactness credit** or unsupported acceptance claim from this guard.
The earlier `--owner front --skip-exact-siblings` verified the same physical
owner. Actual original/COFF proof was obtained from the compiled source,
not a preexisting dated hypothesis.

### Bounded natural C++ counterchecks

The first weighted-window loop was tested as pointer/index `for` forms,
indexing by actual entry fields, a counted loop and a bounded continue loop.
Rearranging the saved table field restores and splitting the true opponent
index multiplication into a factor-five plus factor-two computation was also
compiled. The best complete owners remain 693/351 and 41 candidate blocks;
other forms become 695–727 bytes with 593–613 overlapping differences. The
serial pinned VC7.1 source candidates and independent whole-owner reports are
under `.analysis/gpt-web-front-reordering-20261031/`. Equal-result candidates
were not promoted merely because their C++ spelling differed.

## Enemy OnUpdate boss: independent source-literal proof

A fresh pinned VC7.1 compile and complete original PE audit still finds the
3,900-byte EnemyManager::OnUpdate owner **nonexact, with 13 differing bytes**.
All 97 original PE fields, 69 ordered calls, 1,026 decoded instructions, 170
CFG blocks and four switch entries remain unchanged. Separate complete-owner
`RunEcl` remains 15,564 bytes/598 fields/30 different bytes.

The previous Enemy inspector checked floating-point address referents and
original target payloads but **not the COFF-defined literal payloads**. It now
independently validates five actual four-byte read-only scalar definitions at
owner offsets `+0x2A0`, `+0x2AF`, `+0x8CB`, `+0xA55`, `+0xD61`, including
source symbol/section/value, literal bits, original independently reviewed PE
address, initialized read-only data section, and exactly one reviewed original
code-field reference each. Target-independent adversarial tests mutate every
source scalar, the original values, section flags, and relocation metadata;
they fail closed where the previous code-operand-only inspector did not.

Fresh focused `--owner enemy --skip-exact-siblings` checks both the complete
Enemy update and separate exact Enemy collision wrapper without touching
unrelated carriers. It does **not** credit the 3,900-byte nonexact owner.
Experiment logs under `.analysis/gpt-web-enemy-probes-20261031/` also show
natural GameManager/side pointer, descriptor position and index-local source
models: all survivors at best remain 3,900/97/13; a reordered side-state
producer breaks original fields/extent. These are negative bounded evidence,
not proof that the original source had one unique variable spelling.

## EnemyManager 3,900-byte original-bound scalar definitions

The maintained `EnemyManagerView::OnUpdate` still has **13 physical byte
mismatches** in its complete original 3,900-byte owner (3,883 code bytes,
one alignment byte and four switch-table pointers). It retains all 97
source-to-original field identities, 1,026 decoded instructions, 69 ordered
calls, 170 control-flow blocks and four exact switch destinations. This is
**not** a new exact function.

A fresh complete original/COFF check exposed a proof gap distinct from the
remaining register differences: the original field binder checked `__real@`
name bits and the original PE value, but not that the **source COFF actually
contained** the corresponding float constant in initialized read-only data.
The new scalar audit now binds each literal's independent physical offset,
actual source section/value, and original PE value. The five source/target
pairs are:

| Owner offset | Scalar symbol | Original destination |
| --- | --- | --- |
| `0x2A0` | `__real@3dcccccd` | `0x0048E4B0` |
| `0x2AF` | `__real@3f800000` | `0x0048E2A4` |
| `0x8CB` | `__real@00000000` | `0x0048E314` |
| `0xA55` | `__real@42800000` | `0x0048E4A8` |
| `0xD61` | `__real@42000000` | `0x0048E5E0` |

The target-independent tests in `test_enemy_manager_update_bindings.py`
change each scalar's actual COFF bytes, source-section identity and read-only
flags without changing the linker symbol. All fail the new audit; the current
pinned VC7.1 owner passes. This is stricter evidence about the **full big
boss**, not an instruction-only similarity metric and not exactness credit.
No gameplay C++ source or canonical ledger is changed.

Focused reproduction:

```sh
python3 scripts/build-match-unit.py --unit ecl-pop-context
python3 scripts/inspect-enemy-manager-update.py build/matching/EclManager.obj --json
python3 scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

## EnemyManager::OnUpdate — verified actual scalar definitions

The non-exact 3,900-byte `OnUpdate @ 0x00410730` remains 13 different bytes,
97 independently bound source/original address fields, 1,026 decoded x86
instructions, 69 ordered direct calls, 170 original/source CFG blocks and
four matching switch-table destinations. The five scalar references at
`+0x2A0`, `+0x2AF`, `+0x8CB`, `+0xA55`, and `+0xD61` were formerly accepted
on source COFF *names* and matching PE contents without checking that the
compiler object really defined the expected scalar bytes. This source/object
proof was incomplete despite its correct per-field PE operand binding.

The inspector now validates all five real read-only VC7.1 COFF definitions,
including their initialized section flags, source section/value boundaries,
original PE scalar payloads, 32-bit literal names, and unique references.
They are 0.1f, 1.0f, 0.0f, 64.0f, and 32.0f with actual PE destinations
`0x48E4B0`, `0x48E2A4`, `0x48E314`, `0x48E4A8`, and `0x48E5E0` respectively.
The boss guard independently enforces this five-entry inventory; the test
suite mutates every COFF scalar payload while leaving all relocation metadata
unchanged and rejects missing, reordered, mislocated or writable definitions.
No additional original gameplay behavior or compiler target boundary was
inferred, and **no new exactness credit** is assigned. Reproducer:
`python3 scripts/check-boss-frontiers.py --owner enemy` (scoped carrier and
actual native callee/sibling proof only), with the receipt under
`.analysis/gpt-web-enemy-scalars-20261031.json`.

Independent whole-`EclManager.cpp` original-bound probes tried genuine draw
index widths, pointers and references, descriptor/position copies, side-state
receiver lifetimes, and real `SpawnEffect` owner/argument locals. Byte-neutral
valid forms retained all 13 residuals; moving the side-state reception before
the constructor altered owner size or field identities, and was rejected.
The unreplaced current RunEcl 15,564-byte owner remains 30 differences.
All candidate source and COFF outputs are retained under ignored
`.analysis/gpt-web-enemy-probes-20261031/` for falsifiable review.

## Scoped changed-header exact replay smoke after include-graph checkpoint

The current HEAD `19d82edb` was inspected after it advanced from the prior
Front checkpoint. Its new include resolver canonicalizes the repository root,
walks reverse literal quoted include edges transitively, and refuses symlinked
changed paths outside `src/`. The new root-fallback/nested-subdirectory and
outside-symlink tests are target-independent; all **11** scoped-replay tests
pass.

A live `--changed-file src/EffectType34Family.inl` run selected only the actual
`src/EclManager.cpp` include consumer, then compiled its **two** real VC7.1
profiles and compared all **80** canonical exact siblings: 4 Type11 carrier
units and 76 main ECL carrier units. All passed: **26,161 complete physical
bytes and 1,179 original-bound relocation fields**. This is existing exact
sibling regression evidence, not a new exact match, and does not certify the
nonexact 15,564-byte RunEcl interpreter. The `src/EclRunState.inl` five-carrier
plan remains dry-run only; no full cold replay was launched.

## Late focused source-cold bosses and leaf trials — 2026-10-31

The current source tree was clean before this batch. All experiments below
were compiled serially with the configured VC7.1 source profile into ignored
`.analysis/` objects and checked against complete original-PE owner extents.
The source/profile manifests, game source and target executable were never
modified to accommodate a losing candidate. The original Japanese 1.50a SHA
and canonical tracking were revalidated. No all-source cold replay was run.

### ReplayManager::SaveReplay, 1,238-byte big owner

Seventeen new real read-only pointer, byte-reference, local checksum sample,
addition/parenthesization, C++ aggregate-copy, destination-cast and source
pointer variants all compile to **1,238 physical bytes, 380 instructions, 53
original-bound relocation fields, 23 same call operands and 49 matching CFG
blocks**. Every variant retains exactly three mismatched original bytes:
`+0xB7`, `+0xBB` (copy destination register) and `+0x3F6` (checksum SIB).
Thus these compile-backed rewrites prove **no** improvement, not that a matching
original source is impossible. Evidence:
`.analysis/gpt-web-save-readalias-20261031/` (17 compiled reports, source and
object SHA-256s). A new maintained-source focused
`check-boss-frontiers.py --owner replaysave` independently replays the full
SaveReplay owner and **12** actual canonical same-source exact siblings:
**4,093 physical bytes and 232 verified fields**. Receipt:
`.analysis/gpt-web-save-final-20261031.json`. No exact credit was added.

### EffectManager::AddedCallback, 51-byte leaf

Seventeen natural pointer constness, receiver declaration/lifetime, cached ANM
lookup, resource view and conditional-expression models were compiled with the
real EffectManager carrier. **Twelve** preserve the original **8** residual
byte positions; **five** variants routing the first GetAnm call through a
cached receiver produce **11** differences. The only maintained shape remains **51/51 physical, 19 machine
instructions, four original-bound fields and two ordered calls**, with eight
unlike bytes at `+0x03..+0x09` and `+0x0D`. The current source was independently
rebuilt by `build-match-unit.py --unit effect-manager-clear`, followed by
`inspect-effect-added.py --json`. This diagnostic exits nonzero when a genuine
non-exact owner is found; its complete JSON is preserved under
`.analysis/gpt-web-effect-added-final-20261031.json`. The bounded trial
recipes and linked-object reports remain in
`.analysis/gpt-web-added-lifetime-20261031/`.

### ECL MoveRandomBiased, 516-byte target leaf, and true RunEcl boss

Twelve real Float3/Y-cursor acquisition variants were compiled against the
whole actual `EclManager.cpp` carrier; eight pass the complete original-bound
28-field oracle and stay **517-byte candidate vs 516-byte original, with 148
overlapping differences and 24 matching CFG blocks**. Two other compilable
forms change the physical extent and are rejected by the oracle; two do not
compile under pinned VC7.1. No apparent local register resemblance was allowed
to waive physical extent, field or call requirements. Trial evidence:
`.analysis/gpt-web-biased-y-view-20261031/`.

The same current-source big-owner `--owner runecl --skip-exact-siblings`
recompilation verifies **15,564 physical bytes, 598 source/original referents,
zero field or switch-table differences and 30 ordinary differences**, localized
to original wire opcode 4 (**7**) and opcode 155 (**23**). The two bounded
non-crediting semantic probes cover 90 timer/jump and 3,072 timeout-bit
cases; these do not constitute whole-game runtime proof. Receipt:
`.analysis/gpt-web-runecl-final-20261031.json`.

**Outcome:** no additional exact function or authored byte is claimed in this
negative-source-candidate batch. The 1,005 exact/1,035 source-present ledger
and 247,597/291,883 reviewed-authored exact bytes remain unchanged. The source
code, original bytes, canonical match units and existing exact owners were
preserved, with trial source/COFF and JSON retained for further investigation.
