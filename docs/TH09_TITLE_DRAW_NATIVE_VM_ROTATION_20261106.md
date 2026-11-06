# TH09 title draw callback: native animation rotation (2026-11-06)

The original Japanese TH09 v1.50a `TitleDrawCallback` at `0x00424898` reads
one floating-point word of each displayed animation VM while choosing the
rendering path. The maintained source previously reconstructed this as a
manual `AnmVm* + 0x08` byte-address cast. It now uses the native
`AnmVmBase::rotation.z` member. The real VM has its rotation at offset zero
and the Z float at +0x08; a compile-time `offsetof` assertion checks this
boundary against the already shared native VM layout. No fake VM type,
byte patch, new allocation, register forcing or compiler profile change is
needed. The original nominal C++ names remain unproven.

The pinned VC7.1 match-unit build and complete original-bound comparison for
`title-draw-callback` reproduce **329 of 329 physical bytes** and **20 of 20
original relocation fields**. The draw-loop rendering calls and branch
chronology are unchanged. This was already an exact function; the change
improves the source's storage semantics and adds no new exactness numerator.

Focused reproduction (not a whole-game cold replay):

```bash
python3 -B scripts/build-match-unit.py --unit title-draw-callback
python3 -B scripts/compare-coff-function.py --unit title-draw-callback
python3 -B -m unittest discover -s tests -p test_title_draw_native_rotation.py
```

Original target identity is SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
A local Git commit is a source checkpoint, not Factory or runtime acceptance.
