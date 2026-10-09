#!/usr/bin/env python3
"""Audit RunEcl call identities through its complete opcode-rooted CFG.

This read-only diagnostic does not compile, patch, or grant exact-byte credit.
Call pairing is established from complete ordered block graphs and independently
read opcode/easing tables before independently sourced callee addresses are used.
It does not prove conditional predicates, general argument values, source typing,
callee side effects, runtime behavior, original TU ownership, or byte equality.
"""
import argparse
import subprocess
import sys

import collections
import hashlib
import importlib.util
import json
import struct
import tomllib
from pathlib import Path
try:
    import capstone
except ModuleNotFoundError:
    capstone = None  # Public CI exercises pure guards without the private decoder.

ROOT = Path(__file__).resolve().parents[1]

def require(condition, message):
    """Do not let python -O disable validation of private binary inputs."""
    if not condition:
        raise ValueError(message)

BASE = 0x4086c0
ALIASES = {
    # Same folded +8 getter used by the reviewed canonical charge timer.
    # This is a diagnostic ABI binding, not a second physical/source owner.
    '??BZunTimer@@QAEHXZ': 0x435f00,
    '??BTh09EclTimerStorageView@@QAEHXZ': 0x435f00,
    '??FTh09EclTimerStorageView@@QAEXH@Z': 0x406670,
    '??YTh09EclTimerStorageView@@QAEXM@Z': 0x406650,
    '??PTh09EclTimerStorageView@@QAEIH@Z': 0x401520,
    '??4Th09EclTimerStorageView@@QAEXH@Z': 0x401500,
    '??BTh09EclTimerStorageView@@QAEMXZ': 0x4014f0,
    '??8Th09EclTimerStorageView@@QAEIH@Z': 0x404950,
    '?NextU16@RngView@Th09EclRunControl@@QAEGXZ': 0x42ae20,
    '?FloatModulo@Th09EclRunControl@@YGMMM@Z': 0x405710,
    '?Sin@Th09EclRunControl@@YGMM@Z': 0x401070,
    '?Cos@Th09EclRunControl@@YGMM@Z': 0x401060,
    '?AddNormalizeAngle@Th09EclRunControl@@YGMMM@Z': 0x42aed0,
    '?SquareRoot@Th09EclRunControl@@YGMM@Z': 0x401080,
    '?ClampPosition@Th09EclRunMovement@@YIXPAUEnemyView@@@Z': 0x4101d0,
    '?MoveRandomBiased@Th09EclRunLate@@YIXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@@Z': 0x407e30,
    '?GetRandomU32InRange@RngView@Th09EclRunControl@@QAEII@Z': 0x4048e0,
    '?SetBossMarkerState@BossUiView@Th09EclRunState@@QAEXHH@Z': 0x4067d0,
    '?SetBossMarkerPosition@BossUiView@Th09EclRunState@@QAEXHPAUFloat3@@@Z': 0x4067f0,
    '?PlaySoundPositionedByIdx@SoundPlayerView@Th09EclRunState@@QAEXHM@Z': 0x43e380,
    '??YTh09EclTimerStorageView@@QAEXH@Z': 0x406640,
    '?ClearBulletsForTransition@Th09EclRunBullet@@YIXPAUEtamaController@@@Z': 0x405650,
    '?RemoveBulletsInRadius@BulletControllerLateView@Th09EclRunLate@@QAEXPBUFloat3@@M@Z': 0x4125a0,
    '?RemoveAllBullets@BulletControllerLateView@Th09EclRunLate@@QAEXH@Z': 0x412590,
    '?GetRandomF32InRange@RngView@Th09EclRunControl@@QAEMM@Z': 0x404910,
    '?StartSpellBackground@BackgroundLateView@Th09EclRunLate@@QAEXXZ': 0x4067b0,
}
DATA_ALIASES = {
    '?g_DifficultyMask@Th09EclRunOwner@@3IA': 0x4a7eb0,
    '?g_BossUi@Th09EclRunState@@3UBossUiView@1@A': 0x4ce458,
    '?g_SoundPlayer@Th09EclRunState@@3USoundPlayerView@1@A': 0x4dc698,
    '??_C@_06IPGJPKLK@ECLInt?$AA@': 0x48e468,
    '?g_ExInstructionCallbacks@Th09EclRunState@@3PAP6IXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@@ZA': 0x4a0b14,
    '?g_BossLifeMarkerProtocolValue@Th09EclRunState@@3HA': 0x4a8110,
}

def validate_relocation_fields(raw, rows):
    """Validate non-overlapping four-byte fields before any target pairing."""
    rows = sorted(rows, key=lambda row: row['offset'])
    for index, row in enumerate(rows):
        require(row['type'] in ('REL32', 'DIR32'), 'unsupported relocation type')
        require(0 <= row['offset'] <= len(raw) - 4, 'field outside owner')
        if index:
            require(rows[index - 1]['offset'] + 4 <= row['offset'], 'overlapping relocation fields')
    return rows


def direct_identity(symbol, canonical, aliases, observed_destination):
    """Compare one graph-paired call with an independent symbol binding."""
    records = canonical.get(symbol, [])
    destinations = {address for address, unit in records}
    if len(destinations) > 1:
        raise ValueError('conflicting canonical call identities: ' + symbol)
    if destinations:
        expected = next(iter(destinations))
        provenance = {'canonical_units': [unit for address, unit in records]}
    elif symbol in aliases:
        expected = aliases[symbol]
        review = ('October 9 charge timer conversion and RunEcl timer-family review: '
                  'folded four-byte +8 getter ABI; original source spelling/ownership unknown'
                  if symbol == '??BZunTimer@@QAEHXZ' else
                  'Packet 785: callee implementation, target-body and xref review; not caller-order inference')
        provenance = {'independent_alias_review': review}
    else:
        expected = None
        provenance = {'unresolved': True}
    agrees = expected == observed_destination if expected is not None else None
    return expected, provenance, agrees


def cfg(ins, tables, candidate, by_offset=None):
    """Decode a complete graph; switch roots come from independently read tables."""
    require(bool(ins), 'empty instruction stream')
    require(all(a.address + a.size == b.address for a, b in zip(ins, ins[1:])), 'noncontiguous instruction stream')
    pos = {i.address: n for n, i in enumerate(ins)}
    require(len(pos) == len(ins), 'duplicate instruction addresses')
    leaders = {0}
    branches = {}
    switches = {}
    calls = {}
    indirect = []
    for n, i in enumerate(ins):
        require(i.mnemonic not in ('loop', 'loope', 'loopne', 'retf', 'iret', 'iretd', 'int', 'int3', 'sysenter', 'sysexit', 'ud2'), 'unsupported control transfer')
        if i.mnemonic == 'call':
            calls[n] = i
            if i.operands[0].type != capstone.x86.X86_OP_IMM:
                indirect.append(n)
        if i.mnemonic.startswith('j'):
            if i.operands[0].type == capstone.x86.X86_OP_IMM:
                dest = [i.operands[0].imm]
            else:
                require(i.mnemonic == 'jmp' and len(i.operands) == 1 and (i.operands[0].type == capstone.x86.X86_OP_MEM), 'unreviewed input shape or incomplete coverage')
                o = i.operands[0]
                require(o.mem.scale == 4 and o.mem.base == 0, 'unreviewed input shape or incomplete coverage')
                address = o.mem.disp
                if candidate:
                    r = by_offset[i.address - BASE + i.disp_offset]
                    require(r['type'] == 'DIR32' and r['symbol_section'] == r['owner_section'] and (r['addend'] == 0), 'unreviewed input shape or incomplete coverage')
                    address = BASE + r['symbol_value'] - r['owner_value']
                names = [k for k, v in tables.items() if v['address'] == address]
                require(len(names) == 1, 'unreviewed input shape or incomplete coverage')
                switches[n] = names[0]
                dest = tables[names[0]]['entries']
            require(all((d in pos for d in dest)), 'unreviewed input shape or incomplete coverage')
            branches[n] = [pos[d] for d in dest]
            leaders.update(branches[n])
        if i.mnemonic.startswith('j') or i.mnemonic == 'ret':
            if n + 1 < len(ins):
                leaders.add(n + 1)
    starts = sorted(leaders)
    spans = list(zip(starts, starts[1:] + [len(ins)]))
    owner = {n: b for b, (a, z) in enumerate(spans) for n in range(a, z)}
    blocks = []
    for a, z in spans:
        end = ins[z - 1]
        kind = end.mnemonic
        edges = []
        cleanup = []
        if kind.startswith('j'):
            edges = [owner[n] for n in branches[z - 1]]
            if z - 1 in switches:
                kind = 'switch:' + switches[z - 1]
            elif kind != 'jmp':
                require(z < len(ins), 'missing conditional fallthrough')
                edges.append(owner[z])
        elif kind == 'ret':
            require(all(op.type == capstone.x86.X86_OP_IMM for op in end.operands), 'unreviewed return operand')
            cleanup = [o.imm for o in end.operands]
        else:
            kind = 'fallthrough'
            require(z < len(ins), 'unreviewed input shape or incomplete coverage')
            edges = [owner[z]]
        callsites = [n for n in range(a, z) if n in calls]
        blocks.append({'start': ins[a].address, 'end': end.address + end.size, 'kind': kind, 'edges': edges, 'cleanup': cleanup, 'calls': callsites})
    roots = {k: [owner[pos[a]] for a in v['entries']] for k, v in tables.items()}
    return {'blocks': blocks, 'roots': roots, 'calls': calls, 'indirect': indirect, 'positions': pos, 'instruction_owner': owner, 'table_use_counts': dict(collections.Counter(switches.values()))}

def target_data(ins):
    """Collect only the reviewed forms of target address operands."""
    # This PE has stripped base relocations. Address-range membership alone
    # is not pointer evidence: AND ...,0x00400000 is a scalar flag mask.
    # The independently reviewed body uses MOV/PUSH immediate pointers and
    # absolute memory displacements. Other candidate field forms fail below.
    result = []
    for i in ins:
        for op in i.operands:
            if op.type == capstone.x86.X86_OP_MEM and i.disp_size == 4 and (0x400000 <= op.mem.disp < 0x4e7000):
                result.append({'address': i.address + i.disp_offset, 'value': op.mem.disp, 'role': 'displacement', 'mnemonic': i.mnemonic})
            elif op.type == capstone.x86.X86_OP_IMM and i.imm_size == 4 and i.mnemonic in ('mov', 'push') and (0x400000 <= op.imm < 0x4e7000):
                result.append({'address': i.address + i.imm_offset, 'value': op.imm, 'role': 'immediate', 'mnemonic': i.mnemonic})
    require(len({row['address'] for row in result}) == len(result), 'overlapping target operand fields')
    return sorted(result, key=lambda row: row['address'])

def audit(object_path):
    require(capstone is not None, 'Capstone is required for this optional private-target audit')
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    require(coff.pe_bytes_at(image, 0x435f00, 4) == bytes.fromhex('8b4108c3'),
            'folded timer-current getter body changed')
    obj = Path(object_path)
    raw, rows = coff.object_function(obj, '?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z', include_symbol_locations=True)
    # The two compiler tables follow the complete logical instruction stream.
    # Every relocation must own one distinct, recognized four-byte field.
    rows = validate_relocation_fields(raw, rows)
    table_offset = len(raw) - 193 * 4
    require(table_offset == 14792 and len(rows) == 598, 'unreviewed input shape or incomplete coverage')
    # The compiler may place one or more alignment-NOP instructions between the
    # final return and the two switch tables.  Treat only a fully decoded,
    # side-effect-free trailing NOP sequence as alignment; do not infer a code
    # length from the table offset itself.
    probe_md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    probe_md.detail = True
    probe_ins = list(probe_md.disasm(raw[:table_offset], BASE))
    require(sum((i.size for i in probe_ins)) == table_offset,
            'unreviewed RunEcl code/alignment split')
    final_ret = max((n for n, i in enumerate(probe_ins) if i.mnemonic == 'ret'),
                    default=-1)
    require(final_ret >= 0, 'unreviewed RunEcl code/alignment split')
    trailing = probe_ins[final_ret + 1:]
    for i in trailing:
        is_plain_nop = i.mnemonic == 'nop'
        is_self_lea = (
            i.mnemonic == 'lea' and len(i.operands) == 2
            and i.operands[0].type == capstone.x86.X86_OP_REG
            and i.operands[1].type == capstone.x86.X86_OP_MEM
            and i.operands[1].mem.base == i.operands[0].reg
            and i.operands[1].mem.index == 0
            and i.operands[1].mem.disp == 0
        )
        require(is_plain_nop or is_self_lea,
                'unreviewed RunEcl code/alignment split')
    logical = probe_ins[final_ret].address + probe_ins[final_ret].size - BASE
    require(14784 <= logical <= 14792,
            'unreviewed RunEcl code/alignment split')
    by_offset = {r['offset']: r for r in rows}
    require(len(by_offset) == len(rows), 'unreviewed input shape or incomplete coverage')
    expected_table_offsets = set(range(table_offset, len(raw), 4))
    actual_table_offsets = {row['offset'] for row in rows if row['offset'] >= table_offset}
    require(actual_table_offsets == expected_table_offsets, 'incomplete or excess table fields')
    ctables = {}
    ttables = {}
    for key, start, count, taddr in [('easing', table_offset, 6, 0x40c088), ('opcode', table_offset + 24, 187, 0x40c0a0)]:
        entries = []
        for off in range(start, start + count * 4, 4):
            r = by_offset[off]
            require(r['type'] == 'DIR32' and r['symbol_section'] == r['owner_section'] and (r['addend'] == 0), 'unreviewed input shape or incomplete coverage')
            entries.append(BASE + r['symbol_value'] - r['owner_value'])
        ctables[key] = {'address': BASE + start, 'entries': entries}
        ttables[key] = {'address': taddr, 'entries': list(struct.unpack('<' + 'I' * count, coff.pe_bytes_at(image, taddr, count * 4)))}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ci = list(md.disasm(raw[:logical], BASE))
    ti = list(md.disasm(coff.pe_bytes_at(image, BASE, 14792), BASE))
    require(sum((i.size for i in ci)) == logical and sum((i.size for i in ti)) == 14792, 'unreviewed input shape or incomplete coverage')

    C = cfg(ci, ctables, True, by_offset)
    T = cfg(ti, ttables, False)

    require(C['table_use_counts'] == T['table_use_counts'] == {'easing': 1, 'opcode': 1}, 'unexpected table-use multiplicity')

    def shape(b, ins):
        return (b['kind'], b['edges'], b['cleanup'], [ins[n].operands[0].type for n in b['calls']])
    cs = [shape(b, ci) for b in C['blocks']]
    ts = [shape(b, ti) for b in T['blocks']]
    diffs = []
    for n, (cb, tb) in enumerate(zip(C['blocks'], T['blocks'])):
        if shape(cb, ci) != shape(tb, ti):
            diffs.append({'block': n, 'candidate': cb, 'target': tb})
    units = tomllib.load(open(ROOT / 'config/match-units.toml', 'rb'))['units']
    canonical = collections.defaultdict(list)
    for key, u in units.items():
        canonical[u['symbol']].append((u['target_address'], key))
    # Establish the graph correspondence before inspecting any destination.
    # This proves a block/call-ordinal correspondence, not scheduling among
    # non-call instructions or equivalence of predicates/argument values.
    require(cs == ts and C['roots'] == T['roots'], 'graph pairing unavailable; do not guess identities')
    seed_labels = collections.defaultdict(list)
    seed_labels[0].append('entry')
    for family, roots in C['roots'].items():
        for n, block in enumerate(roots, 1):
            seed_labels[block].append(f'{family}:{n}')
    regions = collections.defaultdict(set)
    for start, labels in seed_labels.items():
        pending = [start]
        seen = set()
        while pending:
            b = pending.pop()
            if b in seen or (b != start and b in seed_labels):
                continue
            seen.add(b)
            regions[b].update(labels)
            if C['blocks'][b]['kind'].startswith('switch:'):
                continue
            pending.extend(C['blocks'][b]['edges'])
    call_matrix = []
    accounted = set()
    indirect_matrix = []
    for bn, (cb, tb) in enumerate(zip(C['blocks'], T['blocks'])):
        for ordinal, (cn, tn) in enumerate(zip(cb['calls'], tb['calls']), 1):
            cins, tins = (ci[cn], ti[tn])
            if cins.operands[0].type != capstone.x86.X86_OP_IMM:
                indirect_matrix.append({'block': bn, 'regions': sorted(regions[bn]), 'candidate_offset': cins.address - BASE, 'target_address': tins.address, 'candidate_operand': cins.op_str, 'target_operand': tins.op_str})
                continue
            off = cins.address - BASE + 1
            r = by_offset[off]
            require(r['type'] == 'REL32' and r['addend'] == 0 and (cins.bytes[0] == 232), 'unreviewed input shape or incomplete coverage')
            sym = r['symbol']
            actual = tins.operands[0].imm
            expected, provenance, agrees = direct_identity(sym, canonical, ALIASES, actual)
            accounted.add(off)
            call_matrix.append({'block': bn, 'regions': sorted(regions[bn]), 'call_in_block': ordinal, 'candidate_offset': cins.address - BASE, 'target_address': tins.address, 'symbol': sym, 'expected_destination': expected, 'target_destination': actual, 'identity_agrees': agrees, 'provenance': provenance})
    refs = collections.defaultdict(list)
    for key, u in units.items():
        for r in u.get('relocations', []):
            refs[r['symbol']].append((r['target'], key))

    body_data = []
    unpaired = []
    for bn, (cb, tb) in enumerate(zip(C['blocks'], T['blocks'])):
        cr = [r for r in rows if r['type'] == 'DIR32' and cb['start'] <= BASE + r['offset'] < cb['end']]
        tr = target_data([i for i in ti if tb['start'] <= i.address < tb['end']])
        if len(cr) != len(tr):
            unpaired.append({'block': bn, 'candidate_fields': cr, 'target_fields': tr})
            continue
        for r, t in zip(cr, tr):
            sym = r['symbol']
            expected = None
            if r['symbol_section'] == r['owner_section'] and r['symbol_section'] > 0:
                local = BASE + r['symbol_value'] - r['owner_value'] + r['addend']
                names = [k for k, v in ctables.items() if v['address'] == local]
                if len(names) == 1:
                    expected = ttables[names[0]]['address']
                    proof = {'table_base': names[0]}
                else:
                    proof = {'unresolved_local_symbol': sym}
            elif sym in DATA_ALIASES:
                expected = DATA_ALIASES[sym] + r['addend']
                proof = {'independent_global_review': 'Packet 785: independent callee/global source, target-body and xref review'}
            else:
                destinations = {a for a, k in refs[sym]}
                if len(destinations) > 1:
                    raise ValueError('conflicting canonical data identities: ' + sym)
                if len(destinations) == 1:
                    expected = next(iter(destinations)) + r['addend']
                    proof = {'canonical_reference_units': sorted(set((k for a, k in refs[sym])))}
                else:
                    proof = {'unresolved_external_symbol': sym}
            owner_ins = next((i for i in ci if i.address <= BASE + r['offset'] < i.address + i.size))
            role = 'displacement' if r['offset'] - (owner_ins.address - BASE) == owner_ins.disp_offset and owner_ins.disp_size == 4 else 'immediate' if r['offset'] - (owner_ins.address - BASE) == owner_ins.imm_offset and owner_ins.imm_size == 4 else 'unrecognized'
            require(role != 'unrecognized', 'relocation is not a decoded four-byte operand')
            require(role != 'immediate' or owner_ins.mnemonic in ('mov', 'push'), 'unreviewed immediate-address form')
            accounted.add(r['offset'])
            body_data.append({'block': bn, 'candidate_offset': r['offset'], 'target_address': t['address'], 'symbol': sym, 'addend': r['addend'], 'expected_value': expected, 'target_value': t['value'], 'identity_agrees': expected == t['value'] if expected is not None else None, 'operand_role_agrees': role == t['role'], 'instruction_mnemonics_agree': owner_ins.mnemonic == t['mnemonic'], 'instruction_mnemonics': [owner_ins.mnemonic, t['mnemonic']], 'provenance': proof})
    table_fields = []
    for family, ct in ctables.items():
        for n, (cd, td) in enumerate(zip(ct['entries'], ttables[family]['entries'])):
            off = ct['address'] - BASE + n * 4
            r = by_offset[off]
            accounted.add(off)
            cb = C['instruction_owner'][C['positions'][cd]]
            tb = T['instruction_owner'][T['positions'][td]]
            table_fields.append({'family': family, 'index': n + 1, 'candidate_offset': off, 'symbol': r['symbol'], 'candidate_destination': cd, 'target_destination': td, 'destination_block': cb, 'root_edge_agrees': cb == tb})
    object_bytes = obj.read_bytes()
    optional_size = struct.unpack_from('<H', object_bytes, 16)[0]

    def object_symbol_data(row, size):
        section = row['symbol_section']
        require(0 < section <= struct.unpack_from('<H', object_bytes, 2)[0], 'unreviewed input shape or incomplete coverage')
        header = 20 + optional_size + (section - 1) * 40
        raw_size, pointer = struct.unpack_from('<II', object_bytes, header + 16)
        value = row['symbol_value']
        require(value >= 0 and value + size <= raw_size, 'unreviewed input shape or incomplete coverage')
        require(pointer + value + size <= len(object_bytes), 'literal outside object file')
        return object_bytes[pointer + value:pointer + value + size]
    literals = []
    for sym in sorted({r['symbol'] for r in rows if r['symbol'].startswith('__real@') or r['symbol'].startswith('??_C@')}):
        r = next((r for r in rows if r['symbol'] == sym))
        binding = next((x for x in body_data if x['symbol'] == sym))
        address = binding['expected_value']
        require(address is not None, 'unreviewed input shape or incomplete coverage')
        value = bytes.fromhex(sym.split('@')[1])[::-1] if sym.startswith('__real@') else b'ECLInt\x00'
        actual = object_symbol_data(r, len(value))
        target_value = coff.pe_bytes_at(image, address, len(value))
        literals.append({'symbol': sym, 'bytes': len(value), 'object_and_target_match_declared_bits': actual == value == target_value})

    # Counts are observed, not copied from the historical expected totals.
    failures = []
    if any(row['identity_agrees'] is not True for row in call_matrix):
        failures.append('direct_call_identity')
    if unpaired or any(row['identity_agrees'] is not True or not row['operand_role_agrees'] or not row['instruction_mnemonics_agree'] for row in body_data):
        failures.append('body_data_identity_or_pairing')
    if any(not row['root_edge_agrees'] for row in table_fields):
        failures.append('table_entry_identity')
    if any(not row['object_and_target_match_declared_bits'] for row in literals):
        failures.append('literal_contents')
    if accounted != set(by_offset):
        failures.append('incomplete_relocation_accounting')
    source_paths = ['src/EclManager.cpp', 'src/EclOpcodes.hpp']
    source_paths += [str(path.relative_to(ROOT)) for path in sorted((ROOT / 'src').glob('EclRun*.inl'))]
    source_paths += ['config/match-units.toml', 'config/functions.csv']
    source_observation = {
        'git_commit': subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip(),
        'worktree_dirty': bool(subprocess.check_output(['git', 'status', '--porcelain'], cwd=ROOT, text=True)),
        'selected_file_sha256': {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest() for name in source_paths},
        'object_source_binding': 'Existing object inspected; this diagnostic performs no cold compile and does not establish that these source files produced it.',
    }
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'audit_passed': not failures,
        'failures': failures,
        'target_sha256': hashlib.sha256(image).hexdigest(),
        'candidate_object': str(obj),
        'candidate_object_sha256': hashlib.sha256(object_bytes).hexdigest(),
        'candidate_function_sha256': hashlib.sha256(raw).hexdigest(),
        'source_observation': source_observation,
        'candidate_code_bytes': logical,
        'target_code_bytes': len(coff.pe_bytes_at(image, BASE, 14792)),
        'candidate_physical_bytes': len(raw),
        'target_physical_bytes': 15564,
        'candidate_instructions': len(ci),
        'target_instructions': len(ti),
        'candidate_blocks': len(C['blocks']),
        'target_blocks': len(T['blocks']),
        'pairing_scope': 'Same verified opcode-rooted graph block and call ordinal; no within-block instruction scheduling proof.',
        'complete_graph_shapes_agree': cs == ts,
        'observed_table_use_counts': C['table_use_counts'],
        'table_roots_agree': C['roots'] == T['roots'],
        'direct_calls': call_matrix,
        'indirect_calls': indirect_matrix,
        'manual_indirect_contracts_candidate_sha256': '20fd67dfb8b6e1f066823dc11159f133af022d09e617ef02d4b3eb7b75ae522f',
        'manual_indirect_contracts_apply_to_candidate': hashlib.sha256(raw).hexdigest() == '20fd67dfb8b6e1f066823dc11159f133af022d09e617ef02d4b3eb7b75ae522f',
        'indirect_contract_scope': 'Packet 785 independently reviews the four observed sites; this tool records actual operands and graph placement, not runtime callback target or full argument proof.',
        'body_data_fields': body_data,
        'unpaired_data_blocks': unpaired,
        'table_fields': table_fields,
        'literal_contents': literals,
        'accounted_relocations': len(accounted),
        'total_relocations': len(rows),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--output', type=Path, help='Write the full diagnostic JSON to this path')
    args = parser.parse_args()
    try:
        result = audit(args.object)
        if args.output:
            args.output.write_text(json.dumps(result, indent=2) + '\n')
        summary = {key: value for key, value in result.items() if key not in (
            'direct_calls', 'indirect_calls', 'body_data_fields', 'table_fields',
            'source_observation', 'unpaired_data_blocks')}
        summary['direct_call_count'] = len(result['direct_calls'])
        summary['indirect_call_count'] = len(result['indirect_calls'])
        summary['body_data_field_count'] = len(result['body_data_fields'])
        summary['table_field_count'] = len(result['table_fields'])
        print(json.dumps(summary, indent=2))
        return 0 if result['audit_passed'] else 1
    except (OSError, ValueError, KeyError, IndexError, struct.error) as exc:
        print('error: ' + str(exc), file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
