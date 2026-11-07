# EffectManager native playable side references and third shared record — 2026-11-07

Original-bound target: Japanese TH09 v1.50a (`resources/th09.exe`),
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a source-facing model/ABI checkpoint, not proof of historic original
class names, a whole Windows product, or newly exact authored coverage.

## Creator's two distinct address domains

The original complete 327-byte `EffectManager::Create` at 0x0040D320
computes side addresses from the GameManager base, with the separately
bound GameManager field references at offsets +0x52 (+0x38 addend),
+0x58 and +0x68 (zero addends). The native shared `AsciiGameManagerView`
physically declares **two** 0x38-byte playable side records. Gameplay
setup additionally models a third 0x38-byte physical record at +0x70;
its shared EffectManager pointer is at GameManager +0x7C. The third
record is **not** a third C++ array element of `AsciiGameManagerView::sides`.

`EffectManager::Create` now obtains playable records through
`&g_GameManager.sides[sideIndex]` and
`&g_GameManager.sides[1 - sideIndex]` while the `sideIndex` is within
the array-derived `GameplaySetupNativeSides::PlayableCount`. The shared
record path retains the native byte-address expression rooted at the
GameManager base for index 2. Both the playable and shared branches
publish their original `void *` pointers; no invented third typed
`g_GameManager.sides[2]` reference or alternative manager layout is used.
`Create`'s other chain callbacks, allocations, and the failure return
path retain their existing source chronology.

## Original-bound compiler checks and negative controls

The pinned VC7.1 complete-source-profile replay passes all **15** exact
functions sharing `src/EffectManager.cpp`, preserving the **327-byte
Create** and **475-byte OnUpdate**. Including the independent exact
AnmLoaded resource callee, focused `--owner effect` passes **16 exact
functions / 2,571 complete physical bytes / 101 relocation fields**.
The separate original-bound 51-byte `EffectManager::AddedCallback`
continues to have eight nonmatching entry bytes, four matched actual
fields, two ordered calls and all three CFG blocks; no exact credit
is claimed for it.

The source-owner guard was updated for the actual typed two-side and raw
third-record relationship. Its existing canonical function/callee map,
original field addends, and strict full-physical COFF/PE comparison have
not been relaxed. It explicitly rules out a typed third-side subscript;
new focused tests protect the shared native counts and actual original
`Create` field identities. Reproduction:

```
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner effect
python3 -B -m unittest discover -s tests -p 'test_effect_native_game_sides.py'
python3 -B -m unittest discover -s tests -p 'test_effect_real_anm_loaded.py'
```

Eleven additional natural Effect AddedCallback models were tested under
its unchanged pinned compiler profile. Reusing one captured ANM receiver,
references/temporary owners, conditional publication, and early-return
forms did not improve the original entry mismatch; one publication shape
broke the 51-byte extent. All experiments were restored rather than
adopting regressed source. The local hypothesis reports are retained in
`.analysis/gpt-web-effect-natural-nov7/`. The lack of a positive candidate
does not establish the original C++ source spelling.

Overall source-present/exact/nonexact totals remain 1,034/1,021/13.
No all-owner cold replay, Factory acceptance, remote push, or semantic
runtime equivalence is claimed.
