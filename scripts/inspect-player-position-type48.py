#!/usr/bin/env python3
"""Full TH09 Type4/Type8 position-callback source-to-PE diagnostic.

Both original fastcall callbacks own 249 bytes and an identical six-call
sequence. Their current VC7.1 source objects are 250 bytes, with an extra
LEA for the positive-branch vector argument. Relocation and call identities
come from the independently verified Japanese TH09 1.50a target, not a
candidate address solve. This inspector never grants function or byte credit.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
SIZE = 249
TYPES = {
    '4': (0x00443B10, '3f000000', 0x00490F54),
    '8': (0x00447620, '3f5db3d7', 0x00491630),
}
CALLS = (0x00401140, 0x0042AED0, 0x00441890,
         0x00401140, 0x00401100, 0x00401140)

# Physical field offsets/identities are fixed by each *target* function.
# Candidate offsets are read from COFF independently and must keep the same
# identity/kind/order; they may shift during legitimate compiler experiments.
FIELD_LAYOUT = (
    (0x01B, 'REL32', '??GFloat3@@QBE?AU0@ABU0@@Z', 0x00401140),
    (0x032, 'REL32', '?AddNormalizeAngle@@YIMMM@Z', 0x0042AED0),
    (0x045, 'DIR32', None, None),  # per-callback magnitude scalar
    (0x054, 'REL32', '?FromAngleMagnitude@Float3@@QAEXMM@Z', 0x00441890),
    (0x068, 'DIR32', '__real@00000000', 0x0048E314),
    (0x07D, 'REL32', '??GFloat3@@QBE?AU0@ABU0@@Z', 0x00401140),
    (0x08E, 'REL32', '??HFloat3@@QBE?AU0@ABU0@@Z', 0x00401100),
    (0x0AC, 'REL32', '??GFloat3@@QBE?AU0@ABU0@@Z', 0x00401140),
)


def expected_fields(callback_type: str):
    _, real_bits, real_address = TYPES[callback_type]
    return tuple((offset, kind, '__real@' + real_bits if name is None else name,
                  real_address if address is None else address)
                 for offset, kind, name, address in FIELD_LAYOUT)


def encoded_value(base: int, offset: int, kind: str, address: int) -> int:
    if kind not in ('DIR32', 'REL32'):
        raise ValueError('unreviewed relocation type')
    return (address - (base + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def validate_target_fields(target: bytes, callback_type: str) -> None:
    base = TYPES[callback_type][0]
    if len(target) != SIZE:
        raise ValueError('unreviewed target callback extent')
    for offset, kind, _, address in expected_fields(callback_type):
        if struct.unpack_from('<I', target, offset)[0] != encoded_value(base, offset, kind, address):
            raise ValueError('target field identity changed at %s' % hex(offset))
    if target[0x73:0x75] != b'\x8b\xd1':
        raise ValueError('target positive-branch MOV EDX,ECX changed')


def bind_candidate(raw: bytes, rows: list[dict], callback_type: str) -> tuple[bytes, list[int]]:
    """Link only the eight independently identified original addresses."""
    fields = expected_fields(callback_type)
    if len(rows) != len(fields):
        raise ValueError('candidate does not contain exactly eight reviewed fields')
    if len(raw) not in (SIZE, SIZE + 1):
        raise ValueError('unreviewed callback candidate extent')
    code = bytearray(raw)
    covered: set[int] = set()
    offsets: list[int] = []
    base = TYPES[callback_type][0]
    for row, (_, kind, symbol, address) in zip(rows, fields):
        offset = row['offset']
        if (row['type'], row['symbol'], row['addend']) != (kind, symbol, 0):
            raise ValueError('candidate field identity/kind/addend changed')
        if not isinstance(offset, int) or offset < 0 or offset + 4 > len(code):
            raise ValueError('candidate field lies outside owned function')
        occupied = set(range(offset, offset + 4))
        if occupied & covered or (offsets and offset <= offsets[-1]):
            raise ValueError('candidate fields overlap or are reordered')
        covered.update(occupied)
        offsets.append(offset)
        struct.pack_into('<I', code, offset, encoded_value(base, offset, kind, address))
    return bytes(code), offsets


def analyze(path: Path, callback_type: str) -> dict:
    spec = importlib.util.spec_from_file_location('type48_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    base = TYPES[callback_type][0]
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, base, SIZE)
    validate_target_fields(target, callback_type)
    # The two source constants are distinct physical target data owners,
    # not interchangeable symbols with numerically similar float values.
    _, literal_bits, literal_address = TYPES[callback_type]
    if coff.pe_bytes_at(pe, literal_address, 4) != struct.pack('<I', int(literal_bits, 16)):
        raise ValueError('target callback magnitude literal changed')
    if coff.pe_bytes_at(pe, 0x0048E314, 4) != bytes(4):
        raise ValueError('target callback floating zero literal changed')
    symbol = ('?PlayerPositionCallback30404Type' + callback_type +
              '@@YIHPAUPlayerPositionCallbackType4View@@PBUFloat3@@@Z')
    raw, rows = coff.object_function(path, symbol)
    candidate, candidate_offsets = bind_candidate(raw, rows, callback_type)

    import capstone
    from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    def decode(code):
        instructions = list(decoder.disasm(code, base))
        if not instructions or sum(i.size for i in instructions) != len(code):
            raise ValueError('incomplete function instruction decoding')
        if instructions[-1].mnemonic != 'ret' or instructions[-1].op_str:
            raise ValueError('unexpected fastcall return or stack cleanup')
        return instructions

    original, generated = decode(target), decode(candidate)
    def direct_calls(instructions):
        out = []
        for ins in instructions:
            if ins.mnemonic == 'call':
                if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM:
                    raise ValueError('unreviewed indirect callback call')
                out.append(ins.operands[0].imm)
        return tuple(out)
    if direct_calls(original) != CALLS:
        raise ValueError('target callback call order changed')
    if direct_calls(generated) != CALLS:
        raise ValueError('candidate callback call identities changed')

    target_graph = direct_control_flow(original, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(generated, capstone.x86.X86_OP_IMM)
    def normalize(ins):
        if ins.mnemonic == 'call' or ins.mnemonic.startswith('j'):
            return ins.mnemonic, 'bound-control'
        return ins.mnemonic, ins.op_str
    matcher = difflib.SequenceMatcher(None, [normalize(i) for i in original],
                                      [normalize(i) for i in generated], autojunk=False)
    blocks = matcher.get_matching_blocks()
    conflicts, unpaired = aligned_branch_conflicts(
        original, generated, blocks, capstone.x86.X86_OP_IMM)
    difference_spans = []
    for kind, a, b, x, y in matcher.get_opcodes():
        if kind != 'equal':
            difference_spans.append({
                'kind': kind,
                'target': [(hex(i.address - base), i.mnemonic, i.op_str) for i in original[a:b]],
                'candidate': [(hex(i.address - base), i.mnemonic, i.op_str) for i in generated[x:y]],
            })
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none; canonical unit and accepted cold replay required',
        'callback_type': callback_type,
        'target_address': hex(base),
        'target_bytes': SIZE,
        'candidate_bytes': len(raw),
        'target_instructions': len(original),
        'candidate_instructions': len(generated),
        'target_fields': len(expected_fields(callback_type)),
        'candidate_field_offsets': [hex(x) for x in candidate_offsets],
        'target_calls': len(CALLS),
        'candidate_calls_agree': True,
        'target_direct_blocks': len(target_graph),
        'candidate_direct_blocks': len(candidate_graph),
        'direct_control_flow_equal': target_graph == candidate_graph,
        'paired_branch_conflicts': [(hex(a), hex(b), hex(c)) for a,b,c in conflicts],
        'unpaired_branch_destinations': unpaired,
        'normalized_matching_instructions': sum(block.size for block in blocks),
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'complete_bytes_equal': candidate == target,
        'complete_differing_bytes': (sum(a != b for a,b in zip(target, candidate))
                                     if len(candidate) == SIZE else None),
        'overlap_differences': sum(a != b for a,b in zip(target,candidate)),
        'difference_spans': difference_spans,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--type', required=True, choices=sorted(TYPES))
    parser.add_argument('--explain', action='store_true', help='show differing aligned instructions')
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    report = analyze(args.object, args.type)
    if not args.explain:
        report.pop('difference_spans')
    print(json.dumps(report, indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
