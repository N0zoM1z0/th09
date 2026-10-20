#!/usr/bin/env python3
"""Original-bound Type29 Effect draw boundary and non-crediting COFF audit.

The callback is authored but has no maintained source or canonical match unit.
An arbitrary supplied object is a compiler experiment, never source attestation.
"""
from pathlib import Path
import argparse
import csv
import hashlib
import importlib.util
import json
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00448750
SIZE = 2220
SHA256 = '8ecf5d55f2ef0de67316f09b3a6d9422b4475bf9a7fdf1ea9dd10919247a3455'
SYMBOL = '?EffectDrawCallbackType29@@YIHPAUEffect@@@Z'
PREFIX_BASE = 0x004A0B48
PREFIX_SHA256 = '318a2c5a7e469179735911b94d8837b8b96651b2c89153ea2a15023ec20942b8'


def direct_extent(base, decode, *, limit=0x1800):
    """Walk direct branches, fallthrough and returns; do not follow calls.

    decode(address) returns (length, flow, destination). Indirect/unrecognized
    control flow and overlapping instruction interpretations fail closed.
    """
    pending = [base]
    seen = {}
    while pending:
        address = pending.pop()
        if address in seen:
            continue
        if not base <= address < base + limit:
            raise ValueError('direct control flow leaves bounded review')
        length, flow, destination = decode(address)
        if (type(length) is not int or length <= 0 or
                address + length > base + limit or
                flow not in ('normal', 'call', 'ret', 'jmp', 'jcc')):
            raise ValueError('invalid or unreviewed instruction flow')
        if any(address < previous + row[0] and previous < address + length
               for previous, row in seen.items()):
            raise ValueError('overlapping instruction interpretations')
        seen[address] = (length, flow, destination)
        if flow == 'ret':
            continue
        if flow in ('jmp', 'jcc'):
            if type(destination) is not int:
                raise ValueError('indirect branch needs independent review')
            pending.append(destination)
        if flow != 'jmp':
            pending.append(address + length)
    if not any(row[1] == 'ret' for row in seen.values()):
        raise ValueError('bounded graph has no return')
    return max(address + row[0] for address, row in seen.items()), seen


def load_auditor():
    spec = importlib.util.spec_from_file_location(
        'effect29_coff_audit', ROOT / 'scripts/inspect-effect-type34.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def target_review(auditor):
    import capstone

    coff = auditor.c
    image = coff.verified_target()
    prefix = coff.pe_bytes_at(image, PREFIX_BASE, 38 * 24)
    if hashlib.sha256(prefix).hexdigest() != PREFIX_SHA256:
        raise ValueError('original Effect prefix identity changed')
    slot = PREFIX_BASE + 29 * 24 + 20
    if struct.unpack_from('<I', prefix, 29 * 24 + 20)[0] != BASE:
        raise ValueError('original row29 draw pointer changed')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(address):
        ins = next(decoder.disasm(
            coff.pe_bytes_at(image, address, 15), address, count=1), None)
        if ins is None or ins.mnemonic in ('int', 'int3'):
            raise ValueError('invalid instruction reached from callback')
        if ins.group(capstone.CS_GRP_RET):
            flow = 'ret'
        elif ins.group(capstone.CS_GRP_JUMP):
            flow = 'jmp' if ins.mnemonic == 'jmp' else 'jcc'
        elif ins.group(capstone.CS_GRP_CALL):
            flow = 'call'
        else:
            flow = 'normal'
        destination = (ins.operands[0].imm if ins.operands and
                       ins.operands[0].type == capstone.x86.X86_OP_IMM else None)
        return ins.size, flow, destination

    end, reached = direct_extent(BASE, decode)
    raw = coff.pe_bytes_at(image, BASE, end - BASE)
    if end - BASE != SIZE or hashlib.sha256(raw).hexdigest() != SHA256:
        raise ValueError('complete original callback extent/hash changed')
    if coff.pe_bytes_at(image, end, 4) != b'\xCC' * 4:
        raise ValueError('following four CC bytes changed')
    instructions = list(decoder.disasm(raw, BASE))
    if sum(ins.size for ins in instructions) != SIZE:
        raise ValueError('complete physical owner does not decode')
    return {
        'target_address': BASE, 'target_bytes': SIZE, 'target_sha256': SHA256,
        'table_pointer_address': slot, 'prefix_sha256': PREFIX_SHA256,
        'reachable_instructions': len(reached),
        'physical_instructions': len(instructions),
        'returns': sorted(address for address, row in reached.items()
                          if row[1] == 'ret'),
        'following_CC_bytes': 4,
        'complete_executable_inventory_proven': False,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path, nargs='?')
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        auditor = load_auditor()
        with (ROOT / 'config/functions.csv').open(newline='') as stream:
            catalog = next((row for row in csv.DictReader(stream)
                            if int(row['address'], 0) == BASE), None)
        result = {
            'diagnostic_only': True, 'exactness_credit': 'none',
            'maintained_source_present': bool(catalog and catalog['source_file']),
            'object_source_attested': False,
            'original': target_review(auditor),
        }
        if args.object:
            report, _, _ = auditor.inspect(args.object, 'draw29', owners={
                'draw29': (BASE, SIZE, SYMBOL, SHA256)})
            result['comparison'] = report
    except (OSError, ValueError, AssertionError, KeyError, struct.error) as exc:
        print(json.dumps({'result': 'error', 'message': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('Original Type29 draw:', SIZE, 'bytes; complete boundary/hash pass')
        if args.object:
            report = result['comparison']
            print('Candidate:', report['candidate_bytes'], 'bytes;',
                  report['overlapping_differences'], 'overlap differences;',
                  report['missing_bytes'], 'missing;',
                  report['extra_bytes'], 'extra; no exactness credit')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
