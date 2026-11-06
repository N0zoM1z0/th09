# RunEcl reference-parameter interrupted comparison, 2026-11-06

The interrupted `borrowed-enemy` experiment is **now fully diagnosed**, not
accepted. Its compile had completed on the previous source epoch, but the
private postcompile audit threw `KeyError`: it looked for the original
pointer-parameter symbol in `inspect-enemy-manager-update.symbol_aliases`.
That map never contained the symbol; the relevant reviewed authored call
identity is in `semantic_aliases`. The old failure said nothing about the
candidate's correctness. Preserve the old source and recovery inventory.

A SHA-verified restore of the actual i386 COFF (rather than a recompiled
imitation) gives the following original-bound observations:

| Check | Restored result |
| --- | --- |
| Candidate | `?RunEcl@EnemyEclManagerView@@QAEHAAUEnemyView@@@Z` (`EnemyView&`) |
| Object SHA-256 | `98f93ee0922b4cf13187c9b215acebc6ad336bacf8b436a9506c037585556f09` |
| RunEcl owner | 15,564 physical bytes, 598 fields; 30 ordinary differing bytes |
| RunEcl fields and tables | Zero differences, complete original-bound mapping |
| Actual EnemyManager::OnUpdate caller | 3,900 bytes, 97 fields; 13 differing bytes |
| Same-object exact units | 77 checked, zero failures |

The source's compiled reference parameter still receives the enemy address
on the stack, with the original ECX receiver and RET4 contract. That ABI
transport alone does **not** prove the source parameter type was a reference.
The same 30/13 unresolved owner differences remain. No source change, matching
ledger change or exactness credit follows from this recovered result.

## Reproducibility and non-crediting checks

The two public RunEcl identity/byte checkers now optionally accept only the
reviewed pointer and reference symbol projections. The default pointer
projection is unchanged. Both perform the original 15,564-byte/598-field
comparison; the optional reference projection can never relax table, call,
literal or relocation validation. A third arbitrary mangled symbol fails
before reading its object. The program's original target is never patched.

The archived object is local ignored evidence. Restore it when available:

```bash
python3 -B scripts/restore-analysis-archive.py \
  --bundle .analysis/handoff-archive-20261106 \
  --path build/analysis/runecl-timeout-storage-20261106/borrowed-enemy.obj --apply
python3 -B scripts/audit-ecl-callsite-identities.py \
  build/analysis/runecl-timeout-storage-20261106/borrowed-enemy.obj \
  --reference-owner
python3 -B scripts/inspect-ecl-complete.py \
  build/analysis/runecl-timeout-storage-20261106/borrowed-enemy.obj \
  --reference-owner --output .analysis/gpt-web-borrowed-canonical.json
```

The last command exits 1 deliberately because the **complete owner is still
nonexact**; it nevertheless writes the complete diagnostic report. The
companion Enemy caller proof and all 77 same-object checks are preserved at
`.analysis/runecl-timeout-storage-20261106/gpt-web-borrowed-recovered.json`.
The reusable private recovery driver is
`.analysis/gpt-web-runecl-borrowed-audit.py`. These ignored paths are **not**
portable proof artifacts and no Factory acceptance is claimed. The retained
reference case is a falsifiable source-shape hypothesis, not adopted gameplay
code or proof that other natural C++ owner models are exhausted.
