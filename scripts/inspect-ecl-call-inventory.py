#!/usr/bin/env python3
"""Non-crediting RunEcl call census, including rejected inline/profile models.

A source-only wrapper can replace a real resolver without changing the number
of calls. Resolve symbols independently of candidate positions and compare the
complete direct-call multiset. This does NOT pair call sites, check the CFG or
prove any byte equality. The canonical full-owner Oracle remains separate.
"""
from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004086C0
PHYSICAL_BYTES = 15564
CODE_BYTES = 14792
TABLE_ENTRIES = 193
OWNER_SHA256 = 'e9433504219119eba40ed4d11537782178135effeb6c95d74b6274e0e7f5e7ea'
SYMBOL = '?RunEcl@EnemyEclManagerView@@QAEHPAUEnemyView@@@Z'


def symbol_destinations(units: dict, aliases: dict) -> dict:
    result = defaultdict(set)
    for unit in units.values():
        result[unit['symbol']].add(int(unit['target_address']))
    for symbol, address in aliases.items():
        result[symbol].add(int(address))
    return dict(result)


def compare_calls(symbols: list[str], target_calls: list[int], known: dict) -> dict:
    actual = Counter()
    unresolved = []
    for symbol, count in sorted(Counter(symbols).items()):
        destinations = known.get(symbol, set())
        if len(destinations) != 1:
            unresolved.append({'symbol': symbol, 'count': count,
                               'possible_destinations': sorted(destinations)})
        else:
            actual[next(iter(destinations))] += count
    expected = Counter(target_calls)

    def rows(counter):
        return [{'destination': hex(address), 'count': count}
                for address, count in sorted(counter.items())]

    missing = expected - actual
    extra = actual - expected
    return {
        'candidate_direct_calls': len(symbols),
        'target_direct_calls': len(target_calls),
        'same_direct_call_count': len(symbols) == len(target_calls),
        'same_direct_call_multiset': not unresolved and not missing and not extra,
        'unresolved_symbols': unresolved,
        'missing_target_calls': rows(missing),
        'extra_known_calls': rows(extra),
        'exactness_credit': 'none',
        'ordering_or_cfg_identity': 'not established',
    }


def table_destinations(raw_size: int, relocations: list[dict]) -> tuple[int, list[int]]:
    """Read the actual complete COFF table tail; never move its local labels."""
    start = raw_size - TABLE_ENTRIES * 4
    if start <= 0:
        raise ValueError('candidate cannot contain the complete two-table tail')
    lookup = {row['offset']: row for row in relocations}
    if len(lookup) != len(relocations):
        raise ValueError('duplicate COFF fields')
    expected = set(range(start, raw_size, 4))
    if {off for off in lookup if off >= start} != expected:
        raise ValueError('missing or excess candidate table fields')
    destinations = []
    for offset in sorted(expected):
        row = lookup[offset]
        if (row['type'] != 'DIR32' or row['addend'] != 0 or
                row['symbol_section'] <= 0 or
                row['symbol_section'] != row['owner_section']):
            raise ValueError('candidate table field is not a local code label')
        relative = row['symbol_value'] - row['owner_value']
        if not 0 <= relative < start:
            raise ValueError('candidate table label outside code')
        destinations.append(relative)
    return start, destinations


def load(name: str, filename: str):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def inspect(path: Path) -> dict:
    import capstone

    coff = load('call_inventory_coff', 'compare-coff-function.py')
    audit = load('call_inventory_aliases', 'audit-ecl-callsite-identities.py')
    pe = coff.verified_target()
    original = coff.pe_bytes_at(pe, BASE, PHYSICAL_BYTES)
    if hashlib.sha256(original).hexdigest() != OWNER_SHA256:
        raise ValueError('complete original RunEcl owner hash changed')
    raw, relocations = coff.object_function(path, SYMBOL, include_symbol_locations=True)
    relocations = audit.validate_relocation_fields(raw, relocations)
    table_start, labels = table_destinations(len(raw), relocations)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    target = list(decoder.disasm(original[:CODE_BYTES], BASE))
    candidate = list(decoder.disasm(raw[:table_start], BASE))
    if (sum(i.size for i in target) != CODE_BYTES or
            sum(i.size for i in candidate) != table_start):
        raise ValueError('incomplete full code decode')
    positions = {ins.address - BASE for ins in candidate}
    if any(label not in positions for label in labels):
        raise ValueError('candidate table label is not an instruction boundary')
    if {i.op_str for i in target if i.mnemonic == 'ret'} != {'4'} or {
            i.op_str for i in candidate if i.mnemonic == 'ret'} != {'4'}:
        raise ValueError('RunEcl return cleanup ABI changed')
    calls = [i for i in candidate if i.mnemonic == 'call']
    direct = [i for i in calls if i.operands[0].type == capstone.x86.X86_OP_IMM]
    lookup = {r['offset']: r for r in relocations if r['type'] == 'REL32'}
    symbols = []
    used = set()
    for ins in direct:
        field = ins.address - BASE + ins.imm_offset
        row = lookup.get(field)
        if ins.imm_size != 4 or row is None or row['addend'] != 0:
            raise ValueError('unreviewed direct-call field or addend')
        used.add(field)
        symbols.append(row['symbol'])
    if used != set(lookup):
        raise ValueError('REL32 field is not an accounted direct call')
    target_calls = [i.operands[0].imm for i in target if i.mnemonic == 'call'
                    and i.operands[0].type == capstone.x86.X86_OP_IMM]
    if len(target_calls) != 375:
        raise ValueError('original direct-call inventory changed')
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    compiler_math = audit.validate_compiler_math_wrappers(coff, path, pe, symbols)
    result = compare_calls(symbols, target_calls, symbol_destinations(units, audit.ALIASES))
    result.update(
        diagnostic_only=True, object=str(path), compiler_math_wrappers=compiler_math,
        object_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
        raw_sha256=hashlib.sha256(raw).hexdigest(), target_owner_sha256=OWNER_SHA256,
        physical_bytes=len(raw), target_physical_bytes=PHYSICAL_BYTES,
        fields=len(relocations), candidate_table_start=table_start,
        target_table_start=CODE_BYTES, local_table_entries=len(labels),
        candidate_indirect_calls=len(calls)-len(direct),
        indirect_target_identity='not established',
        source_binding='Supplied object inspected; this tool does not compile source.',
    )
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (OSError, ValueError, KeyError, ImportError) as error:
        print('RunEcl call census rejected input:', error, file=sys.stderr)
        return 2
    print(json.dumps(result, indent=2))
    return 0 if result['same_direct_call_multiset'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
