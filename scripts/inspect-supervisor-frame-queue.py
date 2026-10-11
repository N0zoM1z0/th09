#!/usr/bin/env python3
"""TH09 v1.50a Supervisor frame insertion complete non-crediting COFF Oracle.

The 0x0042E9E0 owner has 213 source-owned code bytes and *no* relocations.
Compare the full function, all decoded instructions, direct control-flow edges,
return stack cleanup and physical bytes against the independently verified
original Japanese PE. The caller must separately attest that a COFF object was
compiled from its claimed maintained source. No partial score grants exact.
"""

import argparse
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0042E9E0
SIZE = 213
SYMBOL = '?InsertReceivedFrame@SupervisorFrameQueueView@@QAEXHHHF@Z'


def require_owner(candidate, relocations):
    if len(candidate) != SIZE:
        raise ValueError('not the complete 213-byte reviewed function extent')
    if relocations:
        raise ValueError('unexpected COFF relocation in zero-field owner')


def differing_offsets(candidate, target):
    if len(candidate) != len(target) or len(target) != SIZE:
        raise ValueError('cannot compare incomplete physical owner')
    return [i for i, (left, right) in enumerate(zip(candidate, target))
            if left != right]


def diagnose(path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location(
        'th09_supervisor_frame_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    verified = coff.verified_target()
    target = coff.pe_bytes_at(verified, BASE, SIZE)
    code, relocations = coff.object_function(path, SYMBOL)
    require_owner(code, relocations)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    target_instructions = list(decoder.disasm(target, BASE))
    candidate_instructions = list(decoder.disasm(code, BASE))
    if (sum(ins.size for ins in target_instructions) != SIZE or
            sum(ins.size for ins in candidate_instructions) != SIZE):
        raise ValueError('incomplete or non-boundary-aligned instruction decoding')
    target_graph = direct_control_flow(target_instructions, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_instructions, capstone.x86.X86_OP_IMM)
    differences = differing_offsets(code, target)
    return dict(
        diagnostic_only=True,
        credit='none; exact requires a separate cold canonical producer/ledger replay',
        object=str(path),
        target_bytes=SIZE,
        candidate_bytes=len(code),
        complete_bytes_equal=code == target,
        complete_differences=len(differences),
        differing_offsets=[f'0x{i:02X}' for i in differences],
        relocation_fields=0,
        target_instructions=len(target_instructions),
        candidate_instructions=len(candidate_instructions),
        target_calls=sum(i.mnemonic == 'call' for i in target_instructions),
        candidate_calls=sum(i.mnemonic == 'call' for i in candidate_instructions),
        target_blocks=len(target_graph),
        candidate_blocks=len(candidate_graph),
        complete_direct_graphs_agree=target_graph == candidate_graph,
    )


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('object', type=Path)
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args()
    report = diagnose(args.object)
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('NON-CREDITING SupervisorFrameQueueView::InsertReceivedFrame')
        for field in ('target_bytes', 'candidate_bytes', 'relocation_fields',
                      'complete_differences', 'target_instructions',
                      'candidate_instructions', 'target_calls',
                      'candidate_calls', 'target_blocks', 'candidate_blocks',
                      'complete_direct_graphs_agree', 'complete_bytes_equal'):
            print(field + ':', report[field])
        print('differing offsets:', ', '.join(report['differing_offsets']))


if __name__ == '__main__':
    main()
