#!/usr/bin/env python3
"""Pure, target-independent grouping of decoded machine-code byte differences.

Does not decode x86, alter relocations or award fractional exactness credit.
Callers supply already-decoded, complete instruction objects and sorted real
byte offsets from their own target-bound, fail-closed PE/COFF Oracle.
"""

def nearby_difference_regions(offsets, gap=0x30):
    """Group strictly increasing owned-byte differences by locality.

    The regions explain machine-code deltas only: none receives independent
    exactness credit. A gap of 0x28 groups the four actual TH09 Enemy owner
    difference families without assuming their source semantics or CFG shape.
    """
    if gap < 0 or any(not isinstance(x, int) or x < 0 for x in offsets):
        raise ValueError('invalid Enemy difference offsets or grouping gap')
    if any(a >= b for a, b in zip(offsets, offsets[1:])):
        raise ValueError('Enemy differences must be strictly increasing')
    result = []
    for off in offsets:
        if not result or off - result[-1]['offsets'][-1] > gap:
            result.append({'start': off, 'end_exclusive': off + 1,
                           'offsets': [off]})
        else:
            result[-1]['offsets'].append(off)
            result[-1]['end_exclusive'] = off + 1
    return result


def describe_difference_regions(target_instructions, candidate_instructions,
                                offsets, *, base, context=12):
    """Show complete intersecting target/candidate instructions, never mask bytes."""
    if context < 0:
        raise ValueError('negative disassembly context')
    regions = nearby_difference_regions(offsets)
    def selected(instructions, start, end):
        return [
            {'offset': '0x%X' % (i.address - base),
             'bytes': bytes(i.bytes).hex(),
             'mnemonic': i.mnemonic, 'operands': i.op_str}
            for i in instructions
            if i.address - base < end + context and
               i.address - base + i.size > max(0, start - context)
        ]
    return [
        {'offset_start': '0x%X' % r['start'],
         'offset_end_exclusive': '0x%X' % r['end_exclusive'],
         'different_bytes': len(r['offsets']),
         'offsets': ['0x%X' % n for n in r['offsets']],
         'target_instructions': selected(target_instructions,
                                         r['start'], r['end_exclusive']),
         'candidate_instructions': selected(candidate_instructions,
                                            r['start'], r['end_exclusive'])}
        for r in regions
    ]
