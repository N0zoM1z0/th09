#!/usr/bin/env python3
"""Cold-test the real Float3 callee visibility hypothesis for Player types 4/8.

Compare complete original owners in both the standalone caller context and the
actual EclManager carrier. This diagnostic never grants exactness credit. The
callee's real void ABI, compiler flags and all source bodies remain unchanged.
Only a compact JSON report survives; temporary source/OBJ/PDB files are removed.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
OWNERS = (
    (4, 0x00443B10, '6dca2309682745de05d0884094886ee901f833e6535250f41676945421703a5f'),
    (8, 0x00447620, '998011d1cc6e1fdf3ca20cdc6dbedf43cc422d49e82440d8a531e881e342ce40'),
)


def load_coff():
    spec = importlib.util.spec_from_file_location(
        'player_visibility_coff', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def source_snapshot() -> dict[str, str]:
    paths = subprocess.check_output(
        ['git', 'ls-files', '-z', '--', 'src', 'config/match-units.toml',
         'config/tools.lock.toml', 'scripts/compile-probe.sh'], cwd=ROOT)
    return {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest()
            for name in paths.decode().split('\0') if name}


def original_fields(target: bytes, base: int) -> list[tuple[int, str, int]]:
    import capstone

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(target, base))
    if sum(i.size for i in instructions) != 249 or instructions[-1].mnemonic != 'ret':
        raise ValueError('incomplete original Player owner')
    fields = []
    for ins in instructions:
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or ins.imm_size != 4 or
                    ins.operands[0].type != capstone.x86.X86_OP_IMM):
                raise ValueError('unreviewed original Player call')
            fields.append((ins.address - base + ins.imm_offset,
                           'REL32', ins.operands[0].imm))
        else:
            for operand in ins.operands:
                if (operand.type == capstone.x86.X86_OP_MEM and
                        0x400000 <= operand.mem.disp < 0x4E7000):
                    if ins.disp_size != 4:
                        raise ValueError('unreviewed original Player data field')
                    fields.append((ins.address - base + ins.disp_offset,
                                   'DIR32', operand.mem.disp))
    if len(fields) != 8:
        raise ValueError('original Player owner no longer has eight reviewed fields')
    return fields


def compare(coff, obj: Path, owner: tuple, units: dict) -> dict:
    kind, base, expected_sha = owner
    target = coff.pe_bytes_at(coff.verified_target(), base, 249)
    if hashlib.sha256(target).hexdigest() != expected_sha:
        raise ValueError('wrong original Player function identity')
    symbol = (f'?PlayerPositionCallback30404Type{kind}'
              '@@YIHPAUPlayerPositionCallbackType4View@@PBUFloat3@@@Z')
    raw, rows = coff.object_function(obj, symbol)
    fields = original_fields(target, base)
    if len(rows) != len(fields):
        raise ValueError('candidate Player field count changed')
    destinations = {}
    for unit in units.values():
        destinations.setdefault(unit['symbol'], set()).add(unit['target_address'])
    linked = bytearray(raw)
    bindings, used = [], set()
    for row, (original_offset, kind, destination) in zip(rows, fields):
        offset, name = row['offset'], row['symbol']
        coverage = set(range(offset, offset + 4))
        if (row['type'] != kind or row['addend'] != 0 or offset < 0 or
                offset + 4 > len(raw) or used.intersection(coverage)):
            raise ValueError('unreviewed candidate Player field kind/range/addend')
        used.update(coverage)
        if name.startswith('__real@'):
            if (kind != 'DIR32' or
                    coff.pe_bytes_at(coff.verified_target(), destination, 4) !=
                    struct.pack('<I', int(name[7:], 16))):
                raise ValueError('candidate float does not match original literal')
        elif kind != 'REL32' or destinations.get(name) != {destination}:
            raise ValueError('candidate call is not the real independently exact callee')
        value = destination - (base + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, value & 0xFFFFFFFF)
        bindings.append(dict(offset=offset, original_offset=original_offset,
                             type=kind, symbol=name, destination=destination))
    offsets = [i for i, (a, b) in enumerate(zip(linked, target)) if a != b]
    return dict(address=hex(base), target_bytes=249, candidate_bytes=len(raw),
                target_sha256=expected_sha, raw_sha256=hashlib.sha256(raw).hexdigest(),
                resolved_sha256=hashlib.sha256(linked).hexdigest(),
                overlapping_differences=len(offsets), difference_offsets=offsets,
                missing_bytes=max(0, 249 - len(raw)), extra_bytes=max(0, len(raw) - 249),
                complete_bytes_equal=linked == target, bindings=bindings)


def run(output: Path) -> dict:
    coff = load_coff()
    coff.verified_target()
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    callee = units['float3-from-angle-magnitude']
    profile = units['ecl-pop-context']['profile']
    if (callee['source'] != 'src/EclManager.cpp' or callee['size'] != 32 or
            callee['target_address'] != 0x00441890 or callee['profile'] != profile):
        raise ValueError('real Float3 owner/profile changed; review probe first')
    before = source_snapshot()
    callers = [(ROOT / f'src/PlayerPositionCallback30404Type{k}.cpp').read_text()
               for k in (4, 8)]
    starts = [text.index(f'int __fastcall PlayerPositionCallback30404Type{k}(')
              for text, k in zip(callers, (4, 8))]
    if callers[0][:starts[0]] != callers[1][:starts[1]]:
        raise ValueError('Player callers no longer share the same reviewed prefix')
    shared = callers[0] + '\n' + callers[1][starts[1]:]
    ecl = (ROOT / 'src/EclManager.cpp').read_text()
    if '#include "ExAttackInitType8.inl"' not in ecl:
        raise ValueError('actual Float3 definition is no longer in the ECL carrier')
    contexts = {'standalone': shared, 'real_ecl_carrier': ecl + '\n' + shared}
    result = dict(diagnostic_only=True, exactness_credit='none', profile=profile,
                  source_head=subprocess.check_output(
                      ['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
                  source_sha256=before, contexts={})
    with tempfile.TemporaryDirectory(prefix='player-visibility-', dir=output) as temporary:
        scratch = Path(temporary)
        for name, text in contexts.items():
            source, obj = scratch / f'{name}.cpp', scratch / f'{name}.obj'
            source.write_text(text)
            invocation = subprocess.run(
                [str(ROOT / 'scripts/compile-probe.sh'), str(source), str(obj), *profile],
                cwd=ROOT, text=True, capture_output=True)
            if invocation.returncode or not obj.is_file():
                raise ValueError('cold compiler failed: ' +
                                 (invocation.stdout + invocation.stderr)[-1500:])
            context = dict(carrier_sha256=hashlib.sha256(text.encode()).hexdigest(),
                           callers=[compare(coff, obj, owner, units) for owner in OWNERS])
            if name == 'real_ecl_carrier':
                raw, fields = coff.object_function(obj, callee['symbol'])
                original = coff.pe_bytes_at(coff.verified_target(), 0x441890, 32)
                if fields or raw != original:
                    raise ValueError('exposed Float3 callee is not the actual exact 32-byte body')
                context['real_callee_32_bytes_equal'] = True
                context['real_callee_sha256'] = hashlib.sha256(raw).hexdigest()
            result['contexts'][name] = context
    if before != source_snapshot():
        raise ValueError('source changed during the experiment')
    a, b = (result['contexts'][name]['callers'] for name in contexts)
    result['caller_bytes_and_fields_unchanged_by_visibility'] = all(
        (x['raw_sha256'], x['bindings']) == (y['raw_sha256'], y['bindings'])
        for x, y in zip(a, b))
    result['compiler_artifacts_retained'] = False
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True,
                        help='ignored .analysis/ directory for the compact result')
    args = parser.parse_args()
    try:
        output = args.output.resolve()
        if (not output.is_relative_to((ROOT / '.analysis').resolve()) or
                output == (ROOT / '.analysis').resolve() or args.output.is_symlink()):
            raise ValueError('output must be a nonsymlink directory below .analysis/')
        output.mkdir(parents=True, exist_ok=True)
        report_path = output / 'player-position-visibility.json'
        if report_path.is_symlink():
            raise ValueError('refusing symlinked report')
        result = run(output)
        report_path.write_text(json.dumps(result, indent=2) + '\n')
        print(json.dumps({k: v for k, v in result.items()
                          if k not in ('source_sha256', 'contexts')}, indent=2))
        for name, context in result['contexts'].items():
            for caller in context['callers']:
                print(name, caller['address'], caller['candidate_bytes'],
                      'bytes;', caller['overlapping_differences'], 'overlap differences;',
                      caller['extra_bytes'], 'extra;', len(caller['bindings']), 'bound fields')
        return 0
    except (OSError, ValueError, KeyError, ImportError, struct.error) as error:
        print('Player visibility probe rejected input:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
