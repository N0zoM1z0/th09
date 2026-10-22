#!/usr/bin/env python3
"""Conservative original-only EAX consumption after captured Timer callsites.

Syntactic direct-CFG paths, not feasible runtime paths or a C++ return-type proof.
Unknown callees, indirect transfers, cycles, decode gaps and owner bounds stay open.
"""
from __future__ import annotations
import argparse
import bisect
import collections
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
MASKS = {'eax': 0xffffffff, 'ax': 0xffff, 'al': 0xff, 'ah': 0xff00}


def trace_paths(nodes, start, lower, upper, budget=4096):
    """Stop at the first possible read, full kill, or unknown on each path."""
    pending = [(start, 0xffffffff, ())]
    seen = set()
    outcomes = []
    while pending:
        address, live, trace = pending.pop()
        state = (address, live)
        if len(seen) >= budget:
            outcomes.append(dict(kind='budget-unknown', address=address, live=live, trace=trace))
            continue
        if state in seen:
            outcomes.append(dict(kind='revisit-unknown', address=address, live=live, trace=trace))
            continue
        seen.add(state)
        if not lower <= address < upper:
            outcomes.append(dict(kind='owner-boundary-unknown', address=address, live=live, trace=trace))
            continue
        node = nodes.get(address)
        if node is None:
            outcomes.append(dict(kind='decode-gap-unknown', address=address, live=live, trace=trace))
            continue
        trace = trace+(address,)
        if node['reads'] & live:
            kind = 'possible-explicit-eax-read'
        elif node['kind'] in ('call', 'indirect', 'return', 'unknown'):
            kind = {'call':'callee-ABI-unknown', 'indirect':'indirect-transfer-unknown',
                    'return':'forwarded-register-return', 'unknown':'instruction-unknown'}[node['kind']]
        else:
            live &= ~node['writes']
            if live:
                pending.extend((successor, live, trace) for successor in node['successors'])
                continue
            kind = 'full-eax-overwrite'
        outcomes.append(dict(kind=kind, address=address, live=live, trace=trace))
    return outcomes


def decode_nodes(raw, base):
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    nodes = {}
    for ins in cs.disasm(raw, base):
        reads, writes = ins.regs_access()
        used = 0
        killed = 0
        for reg in reads:
            used |= MASKS.get(cs.reg_name(reg), 0)
        for reg in writes:
            killed |= MASKS.get(cs.reg_name(reg), 0)
        if (ins.mnemonic in ('xor', 'sub') and len(ins.operands) == 2
                and all(op.type == capstone.x86.X86_OP_REG for op in ins.operands)
                and ins.operands[0].reg == ins.operands[1].reg):
            used &= ~MASKS.get(cs.reg_name(ins.operands[0].reg), 0)
        kind, successors = 'ordinary', [ins.address+ins.size]
        if ins.group(capstone.CS_GRP_CALL):
            kind, successors = 'call', []
        elif ins.group(capstone.CS_GRP_RET):
            kind, successors = 'return', []
        elif ins.group(capstone.CS_GRP_JUMP) or ins.mnemonic.startswith('loop'):
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM:
                kind, successors = 'indirect', []
            else:
                successors = [ins.operands[0].imm]
                if ins.mnemonic != 'jmp':
                    successors.append(ins.address+ins.size)
        elif (ins.group(capstone.CS_GRP_INT) or ins.group(capstone.CS_GRP_IRET)
              or ins.mnemonic in ('ud2', 'hlt', 'sysenter', 'sysexit')):
            kind, successors = 'unknown', []
        nodes[ins.address] = dict(kind=kind, reads=used, writes=killed,
                                 successors=successors, text=ins.mnemonic+' '+ins.op_str,
                                 size=ins.size)
    return nodes


def run(xrefs):
    sys.path.insert(0, str(ROOT/'scripts'))
    spec = importlib.util.spec_from_file_location('timer_cfg_coff', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    ledger = (ROOT/'config/functions.csv').read_bytes()
    owners = sorted(csv.DictReader(ledger.decode().splitlines()), key=lambda x:int(x['address'], 0))
    starts = [int(row['address'], 0) for row in owners]
    addresses = [int(row['address'], 0) for row in xrefs['result']]
    if not addresses or len(addresses) != len(set(addresses)):
        raise ValueError('xref inventory must be nonempty and unique')
    rows, cached = [], {}
    for address in sorted(addresses):
        encoded = coff.pe_bytes_at(image, address, 5)
        if encoded[0] not in (0xe8, 0xe9) or address+5+struct.unpack_from('<i',encoded,1)[0] != 0x401500:
            raise ValueError('captured xref is not an original direct Timer transfer')
        index = bisect.bisect_right(starts,address)-1
        if index < 0:
            raise ValueError('xref has no catalogued owner')
        owner = owners[index]
        lower, upper = starts[index], starts[index]+int(owner['size'])
        if address+5 > upper:
            raise ValueError('xref leaves its catalogued owner')
        if lower not in cached:
            raw = coff.pe_bytes_at(image,lower,upper-lower)
            cached[lower] = (decode_nodes(raw,lower),hashlib.sha256(raw).hexdigest())
        nodes, owner_sha = cached[lower]
        if address not in nodes or nodes[address]['size'] != 5:
            raise ValueError('xref is not on an owner instruction boundary')
        if encoded[0] == 0xe9:
            outcomes = [dict(kind='tail-call-ABI-unknown', address=address, live=0xffffffff, trace=())]
        else:
            outcomes = trace_paths(nodes,address+5,lower,upper)
        for outcome in outcomes:
            outcome['trace'] = [dict(address=hex(a),instruction=nodes[a]['text']) for a in outcome['trace']]
            outcome['address'] = hex(outcome['address'])
            outcome['live'] = hex(outcome['live'])
        rows.append(dict(address=hex(address),owner=owner['address'],name=owner['proposed_name'],
                         owner_sha256=owner_sha,outcomes=outcomes))
    counts = collections.Counter('+'.join(sorted({o['kind'] for o in r['outcomes']})) for r in rows)
    return dict(diagnostic_only=True,exactness_credit='none',ABI_inference='none',
                scope='captured IDA xrefs; direct intraprocedural CFG; conditions unconstrained',
                target_sha256=hashlib.sha256(image).hexdigest(),ledger_sha256=hashlib.sha256(ledger).hexdigest(),
                recipe_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                xref_count=len(rows),counts=dict(counts),rows=rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--xrefs',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    output=args.output.resolve()
    if not output.is_relative_to((ROOT/'.analysis').resolve()) or output.exists():
        parser.error('output must be a new file below .analysis')
    data=args.xrefs.read_bytes()
    report=run(json.loads(data))
    report['xref_capture_sha256']=hashlib.sha256(data).hexdigest()
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(xref_count=report['xref_count'],counts=report['counts'])))

if __name__=='__main__':
    main()
