# TH09 reconstruction — active handoff

## Operator status — October 30

The operator explicitly resumed reconstruction after maintenance checkpoint
7bc3b956, with the98% goal active. Audit dependencies, shared declarations, ABI
and layouts as well as the large-function bodies; correct tested discrepancies
without assuming earlier notes are conclusive. The98% milestone is unfinished.

The restart game-source baseline is **cc19b4848a6f3401d66b4034eb37dec6a2f74799**
(Controller axis ranges); maintenance checkpoint7bc3b956 adds no game-source
change. The charge, RunEcl and instruction-mask corrections are described below. Use
live Git status and ledgers before dated measurements. Exactness credit and
compiler profiles remain unchanged. New commits use `gpt-6.1-sol:` and the identity/trailer in AGENTS.md;
local commits do not establish remote publication.

Analysis routing is **direct local Ghidra only**, without IDA, REA or Factory
MCP. The original Japanese v1.50a input is ignored `resources/th09.exe`, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
Do not substitute another executable or search Windows mounts.

## Player charge state-effect contract correction

Charge previously declared three implemented state-effect methods with void*
returns; their actual definitions return Effect*, producing different C++ symbols.
Caller and definitions now use one layout-checked PlayerStateEffectView.hpp.
A final source-cold comparison preserves every1197 caller byte,70 fields,58 calls
and all8 nondebug sections/1281 bytes. Only three external method references
change identity. The actual91+91+100-byte callees and existing20-byte Timer body
remain exact. Charge still has1160 overlap differences and13 missing bytes;
there is no new exact credit or product-closure claim.

Ten bounded real-callee/type controls keep actual native bodies checked: effects
and SpawnShots visibility are caller-neutral; resource-body visibility yields
1195 bytes/1162 differences/15 missing and is rejected. Full resource/spawn
declarations alone are neutral. All temporary source trees are removed; records,
recipes and current proof objects remain below
.analysis/gpt-61sol-20261030-charge-native-effects/. verify_current.py replays
that source-bound capture without build caches, or compiles only two affected
carriers with --build. See the KB for the scope and measured limits. Public CI
passes1009 tests/29 skips; ten focused charge tests pass. All10 captured cases
and28 native-body proofs replay before disposable object/debug cleanup; the
packet stays below1MiB with no source snapshots or new source archives.

Enemy's two3-byte callback definitions retain unresolved folded ownership;
matching the PlayerCheckState2 address does not make them that Player method.

## RunEcl native clamp, bullet and Background contracts

RunEcl now calls the existing Enemy clamp and Etama transition members, and the
Background spell method through its shared layout-checked prefix. Three undefined
facades and their diagnostic address aliases are removed. The audit requires the
real canonical members at original call sites409806/40B5B1/40BD13, independently
of candidate offsets. A three-case regression rejects retired names even when
their numeric destinations agree.

Both canonical ECL profiles and three native callee carriers cold-pass all82
existing exact owners:25371 physical bytes/1128 fields. Twenty private reference
names in two units are refreshed only after actual COFF coordinates prove the
same target. Each complete ECL object preserves all203 nondebug sections and
effective relocation identities; only three API and220 private reference strings
change. Complete RunEcl15564/598/30 and Enemy3900/97/13 remain raw-identical.
No definition move, compiler-profile change or new exact credit is adopted.

Eight genuinely connected native-body visibility controls test clamp, bullets,
Background and all three together before/after the caller. Both full callers
remain identical;612 repeated complete exact-body proofs pass. These are bounded
measurements, not proof of original TU ownership. Current source-bound replay is
.analysis/gpt-61sol-20261030-ecl-native-calls/verify_current.py; --build compiles
only five affected carriers. Source staging is temporary; preserve the records. All88
focused ECL tests pass; public CI passes1012 tests/29 skips. Current proof objects
remain after19 disposable object/debug files are removed; no source snapshots or
new code archives remain.

The instruction-mask audit below supersedes the proposed4A7EB0 investigation.
Remaining callback-table and BossLifeMarker data aliases still need independent
storage/definition review; no conclusion follows from address containment alone.

## Shared instruction-mask storage and GameManager global identity

Fresh direct Ghidra and independently decoded original PE accesses confirm
408885 reads DWORD4A7EB0,40FD2A reads its low BYTE, and41AF8A writes DWORD0xFF.
The old ECL and timeline declarations named distinct C++ globals for this same
storage; setup also declared g_GameManager with a competing local-layout type.
All three now use the existing AsciiGameManagerView global root4A7D90 and
layout-checked+120 storage. Setup retains its local view, accessed through local
references. The observed prefix/union does not establish original nominal member
ownership, the complete GameManager class, or whole-product link closure.

Five canonical carrier builds preserve92 existing exact owners:29274 complete
physical bytes/1411 fields, including timeline's complete1048-byte table-bearing
extent and setup's425-byte cleanup. Thirty-nine private names are refreshed only
at proven unchanged COFF coordinates. Ninety-three setup references adopt the
shared native root; timeline's field6C becomes root4A7D90+120, preserving4A7EB0.
All nondebug sections and effective relocation identities remain equal in all
five objects. RunEcl's linked15564 bytes/598 fields/30 differences, Enemy3900/97/13
and Setup1689/171/904 remain unchanged. No new exactness credit is earned.

The ECL audit removes the retired mask alias and requires the native undefined
root, canonical provenance, addend120 and decoded DWORD read. Regression cases
reject equal-address aliases, wrong roots/widths/accesses and missing fields;
the actual prior ECL object is also rejected. Current source/profile/provider-
bound replay is .analysis/gpt-61sol-20261030-ecl-difficulty-data/verify_current.py;
--build compiles only the five affected carriers, serially at lowered priority.
Default replay needs retained proof objects, not build/. Keep documentation,
compiler logs, records and recipes; no old C++ source copies or archives are made.
The unchanged residuals rule out this corrected storage declaration alone as the
explanation for either large owner's remaining bytes.

All92 focused ECL tests and22 focused GameManager/Front tests pass. Public CI
passes1016 tests/29 skips in105.471s. Two obsolete source-spelling assertions
are replaced by original-address/native-relocation checks, rather than making
the new local variable spelling an acceptance rule. Cleanup removes431 ignored
build/debug/bytecode and rejected-getter files,8361802 bytes; all27 research-record
hashes remain unchanged. Cache-free current replay passes after build/ removal.

## Source changes and retained current proofs

All listed improvements remain nonexact where stated. Full code/table/field
comparisons and actual literal definitions govern acceptance; matching fragments
and source-interface cleanup earn no partial exact credit.

| Source checkpoint | Maintained change | Local evidence below `.analysis/` |
| --- | --- | --- |
| 962ddb20 | Replay uses native GameManager mode predicates; 16 affected exact owners retain4,150 bytes/232 fields. | `gpt-61sol-20261029-replay-mode-adoption/verify_current.py` |
| a4a3cd13 | Enemy uses native Front indicator; ECL/Front profiles preserve80 exact owners/25,428 bytes/1,136 fields. | `gpt-61sol-20261029-enemy-periodic-selection/verify_adoption.py` |
| 6bc1377b | Replay uses native RNG; five carriers retain25 exact owners/4,553 bytes/257 fields. | `gpt-61sol-20261029-replay-native-rng/verify_current.py` |
| 015588e0 | Player cancel branches restore five original bytes:849/862 bytes,799 differences and13 missing. Five exact siblings retain1,220 bytes/32 fields. | `gpt-61sol-20261029-collision-workspaces/verify_current.py` |
| cc19b484 | Controller range views restore30 positions without damaging694 prior matches.810/808 bytes,84 differences and2 excess; two helpers retain37+47 exact bytes. | `gpt-61sol-20261029-controller-accumulation/verify_adoption.py` |

These paths locate dated source/profile/provider-bound evidence, not mandatory
restart dependencies. Strict verifiers can reject newer source/ledger inputs or
removed experiment copies. Use tracked build/inspection commands below to
replay the current source; historical captures grant no new acceptance.
This maintenance checkpoint passes public CI:1008 tests/29 skips in127.352s.
The obsolete README Factory/IDA requirement is removed from documentation
validation; canonical-target-path checks remain. Controller and Player proofs
passed after source-copy cleanup, before build-cache removal. Controller's saved
object still replays; Player's verifier needs --build now that build/ is absent.
No new game-source cold replay or exactness credit is claimed. Native
product/runtime remain open.

RunEcl and Enemy remain15564/598/30 and3900/97/13. The October30 bounded fresh
main carrier again preserves all75 same-profile exact owners. Direct Ghidra
exports for both are in `ghidra-direct/ecl-enemy-20261030/`; Pause's export is in
`ghidra-direct/pause-update-20261030/`. No new source experiment or adoption
followed before the operator switched to maintenance.

Recent rejected controls remain recorded in the dated KB and packets:
`gpt-61sol-20261029-effect29-witness/`, `...-replay-byte-addresses/`,
`...-queue-representation/`, `...-chain-callback-context/`, and the Controller
packet above. Some unused frozen source copies are deleted. Their negative
results constrain those concrete models, not all natural source forms; an old
replay may require regenerating its recorded source inputs. Actual COFF, findings,
failures and recipes remain as research records.

## Live ledger snapshot


The canonical state is `config/functions.csv`, `config/function-origins.csv`,
`config/matches.csv`, and `config/match-units.toml`, checked by
`python3 scripts/validate-tracking.py --require-target` and
`python3 scripts/validate-docs.py`. The current restart summary is
**1035 source-present / 1003 exact / 32 non-exact**; earlier operator notes
mentioning 979/876/103 are obsolete for this checkout.

| Measure | Count |
| --- | ---: |
| Function candidates | 2,248 |
| Boundary/origin unreviewed | 0 |
| Reviewed but origin-unresolved | 37 |
| Confirmed authored | 1,035 |
| Classified exclusions | 1,176 |
| Source-present authored mappings | 1,035 |
| Canonical exact functions | 1,003 |
| Source-present non-exact functions | 32 |
| Source-present non-exact bytes | 46,966 |
| Authored without maintained source | 0 |
| Canonical exact authored bytes | 244,917 |

**244,917 / 291,883 reviewed-authored bytes = 83.9093% exact**. To reach
98% of the *currently classified* denominator, another **41,129 bytes**
must become complete original-bound exact owners, reaching at least **286,046
exact authored bytes**. The 37 origin-unresolved functions are excluded and could change the denominator. Similarity fractions
and locally matching subsequences never count as accepted exact.

The manifests govern **1003 exact** functions.

The catalog is not proof of complete executable-code inventory. The formerly
uncatalogued **0x0040D677 /0x00444A78** Timer references now belong to the exact
Effect 1/2 and Effect 34 updates. All **203** captured assignment callsites have
catalogued owners; that bounded census proves neither complete executable
coverage nor a return ABI. The Effect 34 **178-byte initializer at 0x00444960** is now exact; its
**946-byte draw at 0x00444AF0** is a maintained, explicitly nonexact frontier.
Do not silently treat uncatalogued code as excluded.

## Reconstruction frontiers

These are routing measurements, not exactness credit. The ledgers list all32
nonexact functions. The opcode4 setter/assignment visibility and whole Enemy
storage controls are already measured; consult their actual frozen inputs before
repeating them. Historical assumptions remain hypotheses.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | **15,564 physical** (14,792 code + 772 table); 598 bound fields; **30** differing non-field bytes | Opcode4 timer transport and opcode155 flag update. Keep all598 fields/tables and complete15564 bytes; actual Timer ABI and RNG ownership are already corrected. `--owner runecl`. |
| **EnemyManager::OnUpdate** | 0x00410730 | **3,900 physical**,97 fields, 69 calls,170 blocks; **13 differing bytes** (29->13) | Draw index2, side/threshold4, descriptor-position copy7. Keep real20-byte Timer greater co-visible, all97 fields and3887 matching physical bytes. |
| **Effect Type29 draw** | 0x00448750 | **2,220 /2,205 bytes**,129 original referents,72 ordered calls,41/41 physical blocks; **1170 overlap differences +15 missing** | Main/satellite fan chronology, polar receiver reloads and ordinal graph. Bounded emulator fixtures are not whole-runtime or exactness proof. `--owner effect29`. |
| **Effect 34 draw** | 0x00444AF0 | **946/946 physical**,53 original referents,20 direct +12 COM calls,286 instructions,16 blocks; **78 linked differences** | Entry65, store order9 and radius operands4 remain. Preserve868 matching bytes, all53 fields, actual float data and full physical extent. `--owner effect34`. |
| **EtamaController::OnDraw** | 0x00413BE0 | **600/600 physical**, 27 fields, 13 calls, 17 blocks; **268** differences | Laser body-VM position cursor. Preserve332 matching bytes and the genuine final DrawBulletLayerEffects member identity. `--owner etama`. |
| **PauseMenu::OnUpdate** | 0x00434740 | **1,776 original physical / 1,732 candidate**; 46 direct calls plus 2/3 original/candidate timestamp calls; 10-way switch; **1,453** overlapping differences + 44 missing | Share timestamp tail for states3/8/9 and recover branch/VM scheduling. Keep real UnPause and existing exact siblings. `--owner pause`. |
| **Player movement boss** | 0x0041C170 | **1,900/1,900 physical**, 66 fields, 23 calls, **129** differing bytes | Lower-bound EAX/EDX transport and encoding cascade. Native framerate/coordinate owners are corrected; preserve1771 matching bytes and both tables. `--owner movement`. |
| **Player charge update** | 0x0041F810 | **1,210 target /1,197 candidate bytes**, 70 fields, 58 calls, 350/342 instructions; **1,160 overlap differences +13 missing** | Recover source chronology and missing13 bytes; preserve repaired SpawnShots transport and actual call/field identities. |
| **Front weighted message RNG** | 0x004162D0 | **693 target / 681 candidate**, 15 fields, 6 calls, **569** overlap + 12 missing | Primary/opposing table selection and remaining control flow; exact FrontCalc sibling remains protected. `--owner front`. |
| **Front added initialization** | 0x00418480 | **706/706**, 37 original fields, 21 calls, 19 equal blocks; **222 differences** (236→222) | Receiver and panel-store scheduling; preserve484 matching bytes and all native GameManager/ANM identities. `--owner frontadded`. |
| **TitleScreen::OnUpdateResult** | 0x004266B5 | **485 / 484**, 147/147 instructions, 34 fields, 9 calls; **218** overlap + 1 missing | Character snapshot and remaining control-flow/byte deficit. Keep the exact Title setup siblings. |
| **TitleScreen::DrawReplayMenu** | 0x004234F6 | **1,280/1,280 bytes**,376/376 instructions,57 original-bound fields,13 ordered calls,72 matching direct blocks; **29 differences** | Menu bank/record chronology. Preserve all57 fields,13 calls,72 blocks and the full original-bound residual guard. `--owner replaymenu`. |
| **TitleScreen::DrawReplaySave** | 0x004239F6 | **800/800 physical**, 245 instructions, 41 fields, 4 calls, 28/28 matching blocks; **96 linked differences** | VM/bank transport and scheduling. Keep245 instructions,41 fields and28 physical blocks. |
| **TitleScreen::DrawResult** | 0x00423D16 | **1,939/1,939**, 82 fields, 16 calls, 92 blocks; **22** differences | Record/bank/currentScreen lifetimes. Existing source/visibility models are bounded negatives; all1917 matching bytes remain protected. `--owner title`. |
| **TitleScreen::OnUpdateOptions** | 0x004276EB | **2,045 / 2,048**, 135 fields, 51 calls, 121 blocks | Options-menu loop and missing bytes; keep complete original-bound field/call/graph checks. `--owner title`. |
| **Title character cursors** | 0x00424EDD / 0x00424FB0 / 0x0042505F | Active target/candidate **211/215**; Reverse/Inactive **175/175** | Active cursor size and cursor state/transport; all three remain nonexact despite Reverse/Inactive having equal physical lengths. `--owner title`. |
| **ExAttack Type3 update** | 0x00442750 | **780/780**, 38 fields, 24 calls, 17 blocks; **15 maintained differences** | Middle-Z chronology; preserve765 matching bytes. Actual Timer visibility and native vector interfaces do not resolve15. `--owner type3`. |
| **ExAttack Type14/22 draw** | 0x00444130 | **959/959**,53 original referents,20 direct +12 COM calls,290/291 instructions; **525 linked differences** | Frame20 versus1C, polar/store scheduling and physical jumps9/10. Preserve434 matching bytes,53 fields and actual scalar/helper payloads. `--owner exattack`. |
| **GameManager gameplay setup worker** | 0x0041AF2D | **1,689/1,689**, 172/171 fields, 35 calls, 78/77 blocks; **904 differing bytes** (911→904, no damaged matches) | Side registration and shared limit/rate publication. Keep native creator calls and164 aligned identities; B4 multiplicity reflects branch sharing. |
| **Player CheckBulletCollision** | 0x0041DFF0 | **862 / 849**, 26 fields, 12 calls; **799** overlap + 13 missing | Region producer, activeB100 pool and missing physical bytes. Preserve five same-source exact helpers and separate CalcItemBox/FindCollision proofs. `--owner player`. |
| **Replay SaveReplay** | 0x00420C60 | **1,238/1,238 bytes**, 380 instructions, 53 fields, 23 call operands, 49 matching blocks; **3 differences** (152→3) | Frame-destination register at+B7/+BB and checksum SIB at+3F6. Preserve1235 matching bytes and all12 exact same-source siblings. `--owner replaysave`. |
| **Replay BeginPlaybackStage** | 0x00420840 | **540 / 538**, 152/151 instructions, **32/32 ordered original PE/COFF fields**, 5 calls, 12/12 matching CFG blocks; **39** overlapping unlike bytes + 2 missing (49→39) | Seed/tail chronology and two missing bytes. All32 ordered referents and first30 physical field positions are already restored. `--owner replay`. |
| **ScreenEffect shake envelope** | 0x004227A0 | **336/336**, 132 instructions, 20 fields, 8 calls, 26 blocks; **26** differences (28→26) | EDI/ESI owner allocation. Preserve310 matching bytes; timer-float visibility regresses physical size and x87 lifetime. |
| **Controller input** | 0x0042B410 | **808 / 810**, 32 fields; **84** overlap differences /2 excess bytes | Both midpoint evaluations per axis are restored; register scheduling and two excess bytes remain. Preserve all694 old matches plus30 restored positions,32 fields and native globals. |
| **Supervisor network HandleMessage** | 0x00432B70 | **768 authored /808 physical bytes**, 46 fields, 17 direct +2 virtual calls, 34 matching blocks; **105 differences** (125→111→105) | Packed-input/frame scheduling. Keep all six switch targets, map bytes and703 matching physical bytes. `--owner netmessage`. |
| **Supervisor network service** | 0x00430AA0 | **1,633 / 1,632**, 108 source fields | Queue ownership and1395 overlapping differences. Keep original referents and exact queue siblings independent. `--owner network`. |
| **Supervisor::OnUpdate** | 0x00431110 | **844 authored / 996 target physical**, 952 candidate physical | Whole-owner physical deficit, startup chronology and remaining GameManager registration alias debt. Keep real Title/Ending symbols and version data. `--owner supervisor`. |
| **ECL MoveRandomBiased leaf** | 0x00407E30 | **516 / 517**, 28 fields, 14 calls, 24 blocks; 148 overlap + 1 extra | Y-pointer LEA versus MOV alias after actual Float3 conversion. Keep complete owner, fields, branches and exact ECL cohort. `--owner runecl`. |
| **Effect AddedCallback leaf** | 0x0040D1C0 | **51/51**, 4 fields, 2 calls; **8** differences | Remaining receiver/allocation scheduling. Preserve43 matching bytes and genuine AnmLoaded/Chain interfaces. `--owner effect`. |
| **Supervisor received-frame leaf** | 0x0042E9E0 | **213/213**, 67 instructions, 9 blocks; **35** differences | Packed input/seed16-bit store order. Preserve178 matching physical bytes and both exact queue members. `--owner queue`. |

## Restart after a new reconstruction request

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md), affected ledger rows and the relevant dated KB entry.
Recover staged, unstaged and untracked work first. Prioritize complete RunEcl
and Enemy, including whole-function dataflow, shared types and real TU boundaries;
small leaves and the other large owners remain in scope.

```bash
export PYTHONDONTWRITEBYTECODE=1
git status --short --branch
git diff
git diff --cached
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
# Analysis: cached direct Ghidra; every mapped file byte is checked.
python3 -B scripts/ghidra-analysis.py --function 0x004086c0 --function 0x00410730
# After relevant source changes, choose the affected owner only:
nice -n 10 python3 -B scripts/check-boss-frontiers.py --owner runecl
# Documentation/status checkpoint checks:
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/progress.py --check
git diff --check
```

Ghidra12.1.4 is reached through `.tools/ghidra`. The runner imports once and
reuses `.analysis/ghidra-direct/243bffea8dcbe99a/th09.gpr` with
`-noanalysis -readOnly`. It checks disk/Program identity, language, entry and all
685056 file-backed mapped bytes before each export. Metadata hashes alone are
insufficient. Types and boundaries remain provisional; preserve reviewed physical
owners, including alignment and tables, for the pinned VC7.1 Oracle.

Build only affected canonical units/profiles and protect complete exact siblings
and callees; reuse unchanged source/profile-bound proofs. Missing `build/` objects
are expected after cleanup: run the configured build before comparing. Full CI
is needed for shared validator/CI changes, not ordinary docs or exact checkpoints.
No generic runtime hardening or product build is required for each exact edit.

## Evidence, archives and cleanup

The October24 stop/Factory session, H01–H66 narratives and older CI counts have
moved out of this active handoff to
[the checkpoint archive](RE_HANDOFF_CHECKPOINT_ARCHIVE_20261018.md#october-30-archival-of-the-cc19b484-handoff).
[The October24 hypothesis checklist](TH09_BOSS_HYPOTHESES_20261024.md) is historical;
its old source SHA and queued language do not authorize work.
The records in [the accepted-receipts archive](RE_ACCEPTED_RECEIPTS.md) remain immutable source-commit-bound
history, not current-source credit. [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains
TH09 facts and measured models. For short owners use
[SMALL_FUNCTION_FRONTIER.md](SMALL_FUNCTION_FRONTIER.md).

October30 cleanup deletes unused experimental C++ copies, compiler debug/bytecode
caches and source-only archives:67,225 files/758,909,554 logical bytes, plus7,349
empty directories. `.analysis/` falls from about2GiB to1.2GiB. Documentation,
JSON/configuration records, logs, patches, COFF evidence and the working Ghidra project remain. No new old-code
archive is retained. `build/` is cleared after validation and can be rebuilt.

Use [cleanup history](RE_ARTIFACT_CLEANUP_HISTORY.md) for exact retained/deleted
scopes and measured storage savings. Keep needed ignored research records with
this worktree when transferring local evidence. Deleted code snapshots are not
restart prerequisites; rebuild current source with the pinned configuration.

Original executable/game data, toolchains, SDK/Wine/provider state, Ghidra
projects, unique objects and current source-bound proofs are retained. The generic
cleaner previews a broader scope, including retained `.analysis/` objects:
review its paths and do not run `--apply` wholesale or `git clean -X`.

Native Windows i386 compile/link/runtime closure is open. Semantic reconstruction
with both original and native-product runtime Oracles, then portability, comes
later. Exact functions and this cleanup do not establish a runnable product.
