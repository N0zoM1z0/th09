# GameManager compiler profiles retain independent objects

The existing `/Os` GameManager profile and the existing no-`/Os` Supervisor
loading-helper profile now have separate generated object paths. Their source,
compiler flags, target bindings and original byte acceptance are unchanged.
This prevents one valid focused build from invalidating the other's cache.

The collision was reproduced before changing the manifest. Building
`supervisor-setup-loading-vms-and-capture` overwrote `GameManagerSetup.obj`.
The helper remained exact at 214 bytes, but the primary OnUpdate comparison
then rejected a `0x575`-byte function against its `0x4CE`-byte original extent.
This was a wrong-profile object, not a game-source regression. Both actual
objects were retained, and restoring the verified `/Os` object restored the
complete 1,230-byte primary match.

The primary ten units retain `build/matching/GameManagerSetup.obj`; the helper
now uses `build/matching/GameManagerSetupNoOs.obj`. The focused frontier runner
already reads each unit's configured object, so no runner exception or profile
change is needed. The regression test now requires the shared source and
different output objects instead of asserting the old path collision.

After both builds, all eleven existing units compare exact simultaneously:
2,991 physical bytes and 243 relocation fields. The primary gameplay worker
retains its identical complete 1,689-byte / 171-field diagnostic and 904 unlike
bytes. All 27 GameManager tests pass; target/tracking, progress, documentation
and whitespace checks pass. Totals remain 1,022 exact / 13 nonexact.

The ignored `.analysis/replay-frame-destination-20261105/` packet retains the
before/after comparisons, original manifest, both actual original-profile
objects, compiler logs and `cache-isolation-verified.json`. The verification
records the 40 quoted source inputs, both actual profiles and object hashes.
This is compiler-cache and exact-source evidence. It does not establish an
original TU partition or native product/link/runtime closure. The replay-name
field investigation continues separately with its candidate preserved.

The source/configuration checkpoint is
`eae1a5a418c8ea633bf57d9f2ddc957dc7bd6d19`. Its immutable ignored
`cache-source-proof.json` is SHA-256
`7a812f69f196528727b3f3e0d665e84738391be99e7c3126c1205e76bde12227`.
It binds the 40 source inputs and four metadata inputs to normalized committed
Git blobs, both actual profiles, both retained historical objects, pinned
compiler/SDK hashes and all eleven complete comparisons. It is a local
source-scoped compiler receipt, with no Factory or remote-publication claim.
