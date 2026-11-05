# Native Controller input owner, 2026-11-05

The continuation starts at clean `265f8b9c`. Target/tracking/status checks pass.
Fresh Ghidra attests disk/Program SHA, i386, base/entry and all685056 mapped
file bytes before exporting430AA0 service and42BE40 input.

Controller::GetInput now uses the shared native Supervisor's keyboard and
configuration fields. Its padded private Supervisor projection and conflicting
global declaration are removed. Size7B0 and offsets10/388 are compiler-checked.
Five actual global references retain their independently known4B3100 root and
unchanged COFF addends through the native symbol. No original operand moves.

Fresh canonical replay reproduces all819 input bytes and nine fields. The
actual GetControllerInput callee and both same-source setter siblings also
replay exact. Eleven focused tests pass, along with tracking, generated
progress, documentation and whitespace checks. No function counter increases.

The service baseline remains1627/1633 bytes,106 fields,41 direct calls,
1381 unequal overlapping bytes and six absent bytes. Exposing the complete
real input body in that TU retains every input byte/field and leaves the
service frontier unchanged; no body-visibility split is adopted.

An input-contract investigation remains separate at this checkpoint: the
maintained input body never reads its formal index. A complete physical E8
scan of original.text finds only service calls430EC8/431046, and a full-file
DWORD scan finds no absolute42BE40 reference. This is a bounded direct/address
census, not exclusion of computed indirect calls or unique original C++ arity.
No-argument controls preserve the full819-byte helper but emit a still-nonexact
1616-byte service with17 absent bytes. Shared-workspace and member-owner
controls do not close the service. The index interface is retained here pending
the separately reviewed caller correction.

Ignored `.analysis/service-flow-20261105/` retains frozen input/header hashes,
source controls, serial compiler logs, complete target-linked diagnostics,
native input/callee receipts and target-attested exports. `probe.py` reproduces
the controls; the canonical input entrypoint remains `build-match-unit.py
--unit controller-get-input` followed by its strict canonical comparison.
Experiment objects are below `build/analysis/service-flow-20261105/`.
Totals remain1019 exact/16 nonexact. Product/runtime, Factory acceptance and
remote publication remain independent open claims.
