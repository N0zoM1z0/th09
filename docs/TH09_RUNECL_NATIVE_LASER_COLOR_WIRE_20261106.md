# TH09 RunEcl laser-color wire member: natural C++ source recovery

The Japanese TH09 v1.50a `EnemyEclManagerView::RunEcl` owner contains the
original inline laser handlers. Its `ReadLaserColor` helper formerly loaded
a 16-bit word by manually adding `0x02` to a byte pointer at the ECL
instruction's trailing `arguments` array, then reinterpreting it as `short`.
The existing original-bound `LaserInstructionArgs` already identifies this
same **short `color02` at payload offset `+0x02`**, following the bullet-type
word. The maintained C++ now reads the actual member through
`reinterpret_cast<const LaserInstructionArgs *>(instruction->arguments)->color02`.
A compile-time `offsetof` assertion pins this member's original wire layout.
No new owner or resource allocation is inferred; the nominal original class
name remains unknown.

Four natural C++ models were actually compiled under the fixed RunEcl profile:
the native laser-argument member, the trailing operand union's byte field,
a record-array field, and a local typed laser-argument pointer. Each preserved
the **entire 15,564-byte physical RunEcl owner with all 598 original-bound
fields**, and the same **30 nonmatching ordinary bytes** (7 at opcode 4 and
23 at opcode 155). No field/table bytes changed, and no new mismatch was
introduced. The direct record-member form was adopted. This makes actual
C++ wire reading more faithful but **does not make RunEcl byte-exact**.

The source change also renumbered seven local compiler symbols in the
992-byte `ecl-post-update-movement` sibling and thirteen in the 892-byte
`ecl-compare-operands` sibling. For all 20 changed `$L` names the candidate
COFF's own section/value and the unchanged original target address match.
Full PE-linked physical bytes and external field identities were checked
before only those local label *names* were refreshed in the manifest.
The focused `--owner runecl` check passed all **99 complete exact related
comparisons (30,461 original bytes, 1,404 fields)** and noncrediting RunEcl
handler, semantic-window, and same-carrier checks. Enemy OnUpdate remains
**3,900 physical bytes, 97 fields and 13 unmatched ordinary bytes**.

Reproduction, limited to the affected owner/dependencies:

```bash
python3 -B scripts/verify-target.py
python3 -B scripts/check-boss-frontiers.py --owner runecl
python3 -B scripts/check-boss-frontiers.py --owner enemy --skip-exact-siblings
```

Target SHA-256:
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Local trial objects, independently bound literal/COFF reports and the label
review remain in ignored `.analysis/gpt-web-runecl-native-laser-color-20261106/`.
The canonical exact tally remains **1,021/1,034**; no full project cold
replay, original game binary patch, remote push, Factory acceptance, or native
product/runtime closure was claimed.
