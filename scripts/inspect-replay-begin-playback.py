#!/usr/bin/env python3
"""Original-bound 540-byte TH09 BeginPlaybackStage diagnostic (never exact credit).

All absolute address operands come from a fixed, SHA-256-pinned original PE
manifest. The separate source relocation values are resolved through reviewed
canonical units and three independently identified TH09-original call/data
fields. In particular, a source relocation is never moved to fit target bytes.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00420840
SIZE = 540
SYMBOL = '?BeginPlaybackStage@ReplayManagerView@@SIHPAU1@@Z'
TARGET_SHA256 = 'af0dd0d85e36de147b67c07e8790bd200adb04f2862496dac928b3731dff5f64'
CALL_ORDER = (0x0042C970, 0x004205E0, 0x0040E470, 0x00404970, 0x00420160)

# The complete PE disassembly gives these 32 *original physical* operands.
# Fields at 0x14D..0x15D are deliberately not reordered to match the COFF.
TARGET_FIELDS = (
    (0x017, 'DIR32', 0x004A7EC4), (0x02D, 'REL32', 0x0042C970),
    (0x03A, 'REL32', 0x004205E0), (0x05D, 'DIR32', 0x004A7E8C),
    (0x076, 'DIR32', 0x004A7D90), (0x07B, 'REL32', 0x0040E470),
    (0x084, 'DIR32', 0x004A7D90), (0x089, 'REL32', 0x00404970),
    (0x0B0, 'DIR32', 0x004A7E68), (0x0D7, 'DIR32', 0x004A7DB0),
    (0x0E1, 'DIR32', 0x004A7DE8), (0x0EB, 'DIR32', 0x004A7DB8),
    (0x0F5, 'DIR32', 0x004A7DF0), (0x0FF, 'DIR32', 0x004A7DBC),
    (0x109, 'DIR32', 0x004A7DF4), (0x113, 'DIR32', 0x004A7E84),
    (0x11D, 'DIR32', 0x004A7EC8), (0x12D, 'DIR32', 0x004A7EAC),
    (0x13D, 'DIR32', 0x004A8108), (0x14D, 'DIR32', 0x004A810C),
    (0x15D, 'DIR32', 0x004A7DAC), (0x163, 'DIR32', 0x004A7DC0),
    (0x173, 'DIR32', 0x004A7DF8), (0x180, 'DIR32', 0x004A7DE4),
    (0x18B, 'DIR32', 0x004A7DAC), (0x196, 'DIR32', 0x004A7DE4),
    (0x1A6, 'DIR32', 0x004A7DAC), (0x1BE, 'DIR32', 0x004A7DE4),
    (0x1CC, 'DIR32', 0x004A7DAC), (0x1EA, 'DIR32', 0x004A7E78),
    (0x202, 'DIR32', 0x004ACE0C), (0x208, 'REL32', 0x00420160),
)
# Independently decoded PE referents: cross-check the native mode units and
# supply the stage-seen global absent from the exact-unit registry.
TARGET_ONLY_BINDINGS = {
    '?IsGameMode0@AsciiGameManagerView@@QAEHXZ': 0x0040E470,
    '?IsGameMode1@AsciiGameManagerView@@QAEHXZ': 0x00404970,
    '?g_ReplayStageSeen@@3PAEA': 0x004A7E68,
}


SOURCE_FIELDS = (
    ('?g_GameManager@@3UAsciiGameManagerView@@A', 'DIR32', 0x004A7D90, 0x134),
    ('?OpenFile@FileSystem@@YIPAEPBDPAHH@Z', 'REL32', 0x0042C970, 0),
    ('?LoadReplayData@ReplayManagerView@@SIPAUReplayDataView@@PAU2@H@Z', 'REL32', 0x004205E0, 0),
    ('?g_ReplayStageIndex@@3HA', 'DIR32', 0x004A7E8C, 0),
    ('?g_GameManager@@3UAsciiGameManagerView@@A', 'DIR32', 0x004A7D90, 0),
    ('?IsGameMode0@AsciiGameManagerView@@QAEHXZ', 'REL32', 0x0040E470, 0),
    ('?g_GameManager@@3UAsciiGameManagerView@@A', 'DIR32', 0x004A7D90, 0),
    ('?IsGameMode1@AsciiGameManagerView@@QAEHXZ', 'REL32', 0x00404970, 0),
    ('?g_ReplayStageSeen@@3PAEA', 'DIR32', 0x004A7E68, 0),
    ('?g_ReplayFrame0Value06@@3IA', 'DIR32', 0x004A7DB0, 0),
    ('?g_ReplayFrame1Value06@@3IA', 'DIR32', 0x004A7DE8, 0),
    ('?g_ReplayFrame0Value07@@3IA', 'DIR32', 0x004A7DB8, 0),
    ('?g_ReplayFrame1Value07@@3IA', 'DIR32', 0x004A7DF0, 0),
    ('?g_ReplayFrame0Value0A@@3IA', 'DIR32', 0x004A7DBC, 0),
    ('?g_ReplayFrame1Value0A@@3IA', 'DIR32', 0x004A7DF4, 0),
    ('?g_ReplayFrame0Value0C@@3IA', 'DIR32', 0x004A7E84, 0),
    ('?g_ReplayFrame0Value09@@3HA', 'DIR32', 0x004A7EC8, 0),
    ('?g_ReplayHeaderValue0D7@@3IA', 'DIR32', 0x004A7EAC, 0),
    ('?g_ReplayHeaderValue1E6@@3IA', 'DIR32', 0x004A8108, 0),
    ('?g_ReplayHeaderValue1E7@@3IA', 'DIR32', 0x004A810C, 0),
    ('?g_ReplaySideState0@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DAC, 0),
    ('?g_ReplayHeaderValue1E8@@3IA', 'DIR32', 0x004A7DC0, 0),
    ('?g_ReplayHeaderValue1E9@@3IA', 'DIR32', 0x004A7DF8, 0),
    ('?g_ReplaySideState1@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DE4, 0),
    ('?g_ReplaySideState0@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DAC, 0),
    ('?g_ReplaySideState1@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DE4, 0),
    ('?g_ReplaySideState0@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DAC, 0),
    ('?g_ReplaySideState1@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DE4, 0),
    ('?g_ReplaySideState0@@3PAUReplaySideStateView@@A', 'DIR32', 0x004A7DAC, 0),
    ('?g_ReplayConfigSnapshot@@3PAEA', 'DIR32', 0x004A7E78, 0),
    ('?g_ReplayRng@@3URngRuntimeView@@A', 'DIR32', 0x004ACE0C, 0),
    ('?SetSeed@ReplayRngView@@QAEXG@Z', 'REL32', 0x00420160, 0),
)

def encoded(kind: str, destination: int, offset: int, *, base=BASE) -> int:
    if kind not in ('DIR32', 'REL32'):
        raise ValueError('unsupported playback field kind')
    return (destination - (base + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def validate_original(data: bytes, *, sha256: str = TARGET_SHA256,
                      manifest=TARGET_FIELDS) -> None:
    if len(data) != SIZE or hashlib.sha256(data).hexdigest() != sha256:
        raise ValueError('original TH09 playback complete physical owner SHA differs')
    if len(manifest) != 32:
        raise ValueError('original playback field census must contain all 32 fields')
    previous_end = 0
    for off, kind, dest in manifest:
        if off < previous_end or off + 4 > SIZE:
            raise ValueError('overlapping or out-of-range original playback field')
        previous_end = off + 4
        if struct.unpack_from('<I', data, off)[0] != encoded(kind, dest, off):
            raise ValueError('original playback field destination/encoding differs')


def canonical_destinations(units: dict, *, additional=TARGET_ONLY_BINDINGS) -> dict:
    """Obtain source-symbol identities from exact units, not candidate bytes."""
    known: dict[str, set[int]] = {}
    for unit in units.values():
        if 'symbol' in unit:
            known.setdefault(unit['symbol'], set()).add(int(unit['target_address']))
        for row in unit.get('relocations', []):
            known.setdefault(row['symbol'], set()).add(int(row['target']))
    for symbol, target in additional.items():
        if symbol in known and known[symbol] != {target}:
            raise ValueError('playback local target reference conflicts with exact unit')
        known[symbol] = {target}
    return known


def link_candidate(raw: bytes, rows: list[dict], known: dict) -> tuple[bytes, list[tuple]]:
    if not 500 <= len(raw) <= 600 or len(rows) != 32:
        raise ValueError('playback candidate lacks complete extent or 32 relocation fields')
    expected = [(symbol, kind, addend)
                for symbol, kind, _, addend in SOURCE_FIELDS]
    observed = [(row['symbol'], row['type'], row['addend']) for row in rows]
    # The original has E7 before the SideState0 read. The current source
    # hoists SideState0 exactly one slot earlier; no other permutations are
    # allowed by the target evidence. Permit a future fully corrected order.
    current = expected.copy()
    current[19], current[20] = current[20], current[19]
    if observed not in (expected, current):
        raise ValueError('unreviewed playback source COFF symbol/field order')
    # The source identities themselves are independent of candidate offsets.
    if [(kind, address + addend) for _, kind, address, addend in SOURCE_FIELDS] != [
            (kind, dest) for _, kind, dest in TARGET_FIELDS]:
        raise ValueError('source identity manifest no longer binds original fields')
    linked = bytearray(raw)
    bound = []
    prev_end = 0
    for row in rows:
        off, kind, symbol, addend = (
            row['offset'], row['type'], row['symbol'], row['addend'])
        if (not isinstance(off, int) or off < prev_end or off + 4 > len(raw)
                or not isinstance(addend, int)):
            raise ValueError('source relocation overlap/order/extent changed')
        prev_end = off + 4
        targets = known.get(symbol, set())
        if len(targets) != 1:
            raise ValueError('unreviewed or ambiguous source symbol: ' + symbol)
        dest = next(iter(targets)) + addend
        struct.pack_into('<I', linked, off, encoded(kind, dest, off))
        bound.append((off, kind, dest))
    if sorted((kind, dest) for _, kind, dest in bound) != sorted(
            (kind, dest) for _, kind, dest in TARGET_FIELDS):
        raise ValueError('source has substituted, duplicated or omitted an original field')
    return bytes(linked), bound


def decode(code: bytes):
    import capstone

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(code, BASE))
    if (not ins or sum(i.size for i in ins) != len(code)
            or ins[-1].mnemonic != 'ret' or ins[-1].op_str):
        raise ValueError('incomplete replay owner or unexpected return ABI')
    fields = []
    calls = []
    for inst in ins:
        if inst.mnemonic.startswith('j'):
            continue
        if inst.mnemonic == 'call':
            if (len(inst.operands) != 1 or inst.operands[0].type != capstone.x86.X86_OP_IMM
                    or inst.imm_size != 4):
                raise ValueError('unreviewed indirect/short replay call')
            fields.append((inst.address - BASE + inst.imm_offset, 'REL32', inst.operands[0].imm))
            calls.append(inst.operands[0].imm)
            continue
        for op in inst.operands:
            if (op.type == capstone.x86.X86_OP_MEM and inst.disp_size == 4
                    and 0x00400000 <= op.mem.disp < 0x004E7000):
                fields.append((inst.address - BASE + inst.disp_offset, 'DIR32', op.mem.disp))
            elif (op.type == capstone.x86.X86_OP_IMM and inst.imm_size == 4
                  and 0x00400000 <= op.imm < 0x004E7000):
                fields.append((inst.address - BASE + inst.imm_offset, 'DIR32', op.imm))
    return ins, fields, tuple(calls)


def diagnose(path: Path) -> dict:
    import capstone
    from diagnostic_field_alignment import align_fields
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location('replay_playback_coff',
                                                  ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    validate_original(target)
    ti, tf, tcalls = decode(target)
    if len(ti) != 152 or tuple(tf) != TARGET_FIELDS or tcalls != CALL_ORDER:
        raise ValueError('original playback 152 instructions / 32 fields / 5 calls drifted')

    raw, rows = coff.object_function(path, SYMBOL)
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    linked, bound = link_candidate(raw, rows, canonical_destinations(units))
    ci, cf, ccalls = decode(linked)
    if cf != bound or ccalls != CALL_ORDER:
        raise ValueError('source machine operands/calls differ from bound COFF records')
    target_graph = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    field_alignment = align_fields(TARGET_FIELDS, bound)
    if field_alignment['paired_field_identities'] < 31 or len(target_graph) != 12:
        raise ValueError('original-bound playback field ordering or block graph regressed')
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(linked),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(tf), 'bound_fields': len(bound),
        'ordered_calls': len(tcalls), 'ordered_calls_agree': ccalls == tcalls,
        'target_blocks': len(target_graph), 'candidate_blocks': len(candidate_graph),
        'complete_direct_graphs_agree': target_graph == candidate_graph,
        'aligned_field_identities': field_alignment['paired_field_identities'],
        'field_change_spans': field_alignment['changes'],
        # Independent original PE and source COFF field *positions*, not
        # merely identities: 30 original bytes still hold their exact field
        # offsets. The final two source fields trail by -1/-2 because the
        # original uses XOR+MOV for the replay seed (two extra bytes).
        'field_offset_deltas': [actual[0] - original[0]
                                for original, actual in zip(TARGET_FIELDS, bound)],
        'complete_overlap_differences': sum(a != b for a, b in zip(target, linked)),
        # The complete original PE-linked physical mismatch set, not just
        # one count or matched-prefix sample. Existing exact bytes are sacred.
        'difference_offsets': [i for i, (original, compiled) in
                               enumerate(zip(target, linked)) if original != compiled],
        'missing_target_bytes': max(0, SIZE - len(linked)),
        'extra_source_bytes': max(0, len(linked) - SIZE),
        'complete_bytes_equal': linked == target,
        'target_sha256': TARGET_SHA256, 'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = diagnose(args.object)
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            print('NON-CREDITING ReplayManager::BeginPlaybackStage original-bound diagnostic')
            for field in ('target_bytes', 'candidate_bytes', 'target_instructions',
                          'candidate_instructions', 'original_fields', 'bound_fields',
                          'aligned_field_identities', 'ordered_calls', 'ordered_calls_agree',
                          'target_blocks', 'candidate_blocks', 'complete_direct_graphs_agree',
                          'complete_overlap_differences', 'missing_target_bytes', 'complete_bytes_equal'):
                print(field + ':', result[field])
        return 0
    except (ValueError, KeyError, OSError, ImportError, struct.error) as error:
        print('playback target/COFF input rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
