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
