# Direct setup reset fields, 2026-11-05

This source continuation follows `ecd136eb`, which recovers the three setup
records and the shared EffectManager slot. Its fresh target-attested Ghidra
export includes the complete ResetGameManager entry41A7C3. Original target
and tracking checks remain valid.

ResetGameManager now clears the actual setup side flags and existing rank,
wait, playtime, replay-pause and menu fields directly. The duplicate private
GameManagerResetRecord/GameManagerResetView definitions and pointer conversion
are removed. Two original four-byte stores at+BC and+110 receive typed setup
members with checked offsets and widths; their higher-level roles stay
unknown. The separate replay-mode word at+114 is preserved. The existing
shared native ECL word at+380 retains its source interface.

The complete100-byte reset remains original-exact, with no relocations.
Three actual source/profile builds retain all12 affected canonical units:
3987 physical bytes/335 fields. Each profile is checked against its own fresh
object. Forty Supervisor local field spellings are again verified from actual
same-section definitions and original operands before manifest name updates;
all full code and physical tables/tails remain exact.

The complete GameplaySetupThread guard remains1689/171/904, with35 ordered
calls and its unchanged original-only/source-only referent multisets. This
source consolidation does not resolve its control-flow/codegen frontier or
promote any function. Twenty-four focused tests pass. Tracking, progress,
documentation and whitespace checks pass; totals stay1019 exact/16 nonexact.
Whole-product linking/runtime and Factory acceptance remain separate.

The scoped reproduction remains the three source/profile builds described in
TH09_SETUP_THIRD_RECORD_20261105.md. Ignored
`.analysis/setup-reset-20261105/verify-source.py` cold-builds and strictly
compares those same12 units; its native-proof.json, native-worker.json and
label-proof.json retain the full receipts. Frozen pre-change inputs and logs
are preserved there. Objects stay below
`build/analysis/setup-reset-20261105/`. The earlier packet retains the large
worker source controls and independent full reference build.

Final receipt recovery separates the two checkpoints' generated objects. The
first source checkpoint is independently recompiled from immutable `ecd136eb`
inputs into its own setup-flow paths; its earlier receipt is retained as
native-proof-before-replay.json. Current reset objects are copied into the
setup-reset paths with identical SHA-256 and corrected receipt routes. This
repairs local artifact provenance without changing target equality or source.
