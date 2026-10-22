#!/usr/bin/env python3
"""Complete, non-crediting TH09 EnemyManager::OnUpdate PE/VC7.1 diagnostic.

Target identity, literal payloads, owner-local label destinations and all 97
relocation fields are checked independently. External name/address identities
come from already-reviewed exact match units, plus the specifically reviewed
non-exact call boundaries below. Do not derive addresses by fitting target bytes.
This script only inspects an object supplied by the caller: it does not attest
its current source or grant exact, runtime, or partial-owner byte credit.
"""

import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

import capstone

from diagnostic_switch_graph import switch_graph
from diagnostic_byte_spans import describe_difference_regions

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00410730
OWNER_SIZE = 3900  # 3883 instructions + one alignment + four-entry table
CODE_SIZE = 3883
TARGET_SHA256 = "a7f01c2ff0bda79d1fd17ac9170dc3560d4089457455d7928776df8556f1bc75"
SYMBOL = '?OnUpdate@EnemyManagerView@@SIHPAU1@@Z'
RUNECL_SYMBOL = '?RunEcl@EnemyEclManagerView@@QAEHPAUEnemyView@@@Z'
PLAYER_STATE_CALLS = (
    (0x36B, '?GetUpdateState@PlayerLifecycleView@@QAEHXZ', 0x00435EC0),
    (0xBE5, '?GetUpdateState@PlayerLifecycleView@@QAEHXZ', 0x00435EC0),
    (0xC06, '?SetUpdateState@PlayerState4OpsView@@QAEXH@Z', 0x00403C20),
)
PLAYER_DAMAGE_SYMBOL = '?CalcDamageToEnemy@PlayerDamagePlayerView@@QAEHPAUPlayerDamageFloat3@@0PAH11@Z'
PLAYER_DAMAGE_FIELDS = (0x8BF, 0x8FF)
TIMER_GREATER_SYMBOL = '??OZunTimer@@QAEIH@Z'
TIMER_GREATER_FIELDS = (0x0A1, 0x276, 0x921, 0xEC6)
TIMER_GREATER_SHA256 = 'f4946c0b7b22e556c3b758435c2c7bdd468104acc285e40dda5ec2a737d36126'


def require_timer_greater_definition(fields, units, body, body_fields, target):
    """Preserve the real co-visible comparison body, not merely its address."""
    unit = units['zun-timer-greater']
    if (unit['source'] != 'src/EclManager.cpp'
            or unit['object'] != 'build/matching/EclManager.obj'
            or unit['symbol'] != TIMER_GREATER_SYMBOL
            or unit['target_address'] != 0x403DE0 or unit['size'] != 20
            or unit['profile'] != units['enemy-attached-effect-update']['profile']
            or unit.get('relocations', []) != []):
        raise ValueError('native Timer greater source/profile/extent changed')
    if (len(body) != 20 or body_fields
            or hashlib.sha256(body).hexdigest() != TIMER_GREATER_SHA256):
        raise ValueError('native Timer greater complete body differs from original')
    observed = [row for row in fields if row['symbol'] == TIMER_GREATER_SYMBOL
                or row['offset'] in TIMER_GREATER_FIELDS]
    if len(observed) != 4:
        raise ValueError('Enemy native Timer greater call inventory changed')
    coordinates = set()
    for row, offset in zip(observed, TIMER_GREATER_FIELDS):
        section = row.get('symbol_section')
        if (row['offset'] != offset or row['symbol'] != TIMER_GREATER_SYMBOL
                or row['type'] != 'REL32' or row['addend'] != 0
                or type(section) is not int or section <= 0
                or section == row.get('owner_section')
                or row.get('symbol_value') != 0 or target[offset-1] != 0xE8
                or BASE+offset+4+struct.unpack_from('<i', target, offset)[0] != 0x403DE0):
            raise ValueError('Enemy Timer greater call lacks its native definition')
        coordinates.add(section)
    if len(coordinates) != 1:
        raise ValueError('Enemy Timer greater definitions disagree')
    return dict(unit='zun-timer-greater', source='src/EclManager.cpp', calls=4,
                bytes=20, fields=0, same_object_definition=True,
                original_body_sha256=TIMER_GREATER_SHA256)


def require_player_damage_calls(fields, units, target):
    """Require the shared damage interface and both original thiscall operands."""
    owner = units['player-calc-damage-to-enemy']
    if (owner['source'] != 'src/PlayerDamage.cpp'
            or owner['symbol'] != PLAYER_DAMAGE_SYMBOL
            or owner['target_address'] != 0x41FCD0 or owner['size'] != 996):
        raise ValueError('native Player damage owner changed')
    observed = [row for row in fields if row['offset'] in PLAYER_DAMAGE_FIELDS]
    if len(observed) != 2:
        raise ValueError('Enemy Player damage call inventory changed')
    for row, offset in zip(observed, PLAYER_DAMAGE_FIELDS):
        if (row['offset'] != offset or row['type'] != 'REL32'
                or row['symbol'] != PLAYER_DAMAGE_SYMBOL or row['addend'] != 0
                or row.get('symbol_section') != 0 or target[offset-1] != 0xE8
                or BASE+offset+4+struct.unpack_from('<i', target, offset)[0] != 0x41FCD0):
            raise ValueError('Enemy damage call lost its native source/target identity')
    return dict(calls=2, unit='player-calc-damage-to-enemy', bytes=996)


def require_player_state_calls(fields, units, target):
    """Bind the real interfaces independently of numeric alias resolution."""
    expected = units['player-set-update-state']
    if (expected['source'] != 'src/PlayerSetUpdateState.cpp'
            or expected['symbol'] != PLAYER_STATE_CALLS[2][1]
            or expected['target_address'] != 0x403C20 or expected['size'] != 12):
        raise ValueError('native Player state setter owner changed')
    offsets = {row[0] for row in PLAYER_STATE_CALLS}
    symbols = {row[1] for row in PLAYER_STATE_CALLS}
    observed = [row for row in fields
                if row['offset'] in offsets or row['symbol'] in symbols]
    if len(observed) != 3:
        raise ValueError('Enemy Player state call inventory changed')
    for row, (offset, symbol, destination) in zip(observed, PLAYER_STATE_CALLS):
        if (row['type'] != 'REL32' or row['symbol'] != symbol
                or row['addend'] != 0):
            raise ValueError('Enemy state call lost its native source/target identity')
        if row['offset'] != offset:
            raise ValueError('Enemy native Player state call moved from original offset '
                             f'{offset:#x} to {row["offset"]:#x}')
        if row.get('symbol_section') != 0:
            raise ValueError('Enemy state call has an unreviewed same-object definition')
        if (target[offset-1] != 0xE8
                or BASE+offset+4+struct.unpack_from('<i', target, offset)[0] != destination):
            raise ValueError('Enemy state call lost its native source/target identity')
    return dict(calls=3, setter_unit='player-set-update-state', setter_bytes=12,
                getter_address='0x00435EC0', getter_exact_credit='none; shared/folded origin unresolved')


def load_compare():
    spec = importlib.util.spec_from_file_location(
        'enemy_core_coff', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def reference_index():
    with (ROOT / 'config/match-units.toml').open('rb') as file:
        units = tomllib.load(file)['units']
    with (ROOT / 'config/functions.csv').open(newline='') as file:
        functions = {
            row['proposed_name']: row for row in csv.DictReader(file)
            if row['proposed_name']
        }
    registry = {}
    for name, unit in units.items():
        for row in ([{'symbol': unit['symbol'], 'target': unit['target_address']}]
                    + list(unit.get('relocations', []))):
            registry.setdefault(row['symbol'], []).append(
                {'target': row['target'], 'proof': 'units.' + name})
    return units, functions, registry


unit_aliases={
 '?SetSideEnemyIndicatorPosition@EnemyCoreFrontView@@QAEXHPAUFloat3@@@Z':'front-enemy-indicator',
 '?EnemyCoreRunLifeCallback@@YIHPAUEnemyCoreView@@@Z':'player-check-state2',
 '?EnemyCoreRunTimerCallback@@YIHPAUEnemyCoreView@@@Z':'player-check-state2',
 '?ExecuteScript@EnemyCoreAnmManagerView@@QAEHPAUEnemyCoreAnmVmView@@@Z':'anm-manager-execute-script',
 '?AddScore@EnemyCoreSideStateView@@QAEXH@Z':'game-manager-side-add-score',
 '?EnemyCoreReleaseAttachedEffects@@YIXPAUEnemyCoreView@@@Z':'enemy-ecl-release-attached-effects',
 '?PlaySoundPositionedByIdx@EnemyCoreSoundPlayerView@@QAEXHM@Z':'soundplayer-play-positioned-sound-by-idx',
 '?SetBossMarkerPosition@EnemyCoreUiView@@QAEXHPAUFloat3@@@Z':'ascii-boss-marker-set-position',
 '?SetBossMarkerState@EnemyCoreUiView@@QAEXHH@Z':'ascii-manager-set-boss-marker-state',
}
symbol_aliases={
 '?g_EnemyCoreFront@@3PAUEnemyCoreFrontView@@A':'?g_PlayerSharedRuntime@@3PAUPlayerSharedRuntimeView@@A',
 '?g_EnemyCoreSchedule@@3PAEA':'?g_EnemySubroutineSchedule@@3PAEA',
 '?g_EnemyCoreAnmManager@@3PAUEnemyCoreAnmManagerView@@A':'?g_AnmManager@@3PAVAnmManager@@A',
 '?g_EnemyCoreSoundPlayer@@3UEnemyCoreSoundPlayerView@@A':'?g_SoundPlayer@@3VSoundPlayer@@A',
 '?g_EnemyCoreUi@@3UEnemyCoreUiView@@A':'?g_AsciiManager@@3VAsciiManager@@A',
}
semantic_aliases={
 RUNECL_SYMBOL:'EclManager::RunEcl',
 '?AppendPlayerCollisionBox@EnemyAppendCollisionView@@QAEXPBUFloat3@@0@Z':'EnemyAppendCollisionView::AppendPlayerCollisionBox',
}


def require_runecl_definition(caller_fields, callee_raw, callee_fields):
    """Require the caller's native symbol to resolve in this actual COFF.

    A source-present callee may remain nonexact. A matching numeric address
    through an unrelated class declaration does not prove this source link.
    """
    calls = [row for row in caller_fields if row['symbol'] == RUNECL_SYMBOL]
    if len(calls) != 1 or len(callee_raw) != 15564 or len(callee_fields) != 598:
        raise ValueError('incomplete native RunEcl caller/definition inventory')
    call = calls[0]
    coordinates = {(row['owner_section'], row['owner_value']) for row in callee_fields}
    if len(coordinates) != 1:
        raise ValueError('inconsistent native RunEcl owner coordinates')
    section, value = next(iter(coordinates))
    if (section <= 0 or call['type'] != 'REL32'
            or call['addend'] != 0 or call['symbol_section'] != section
            or call['symbol_value'] != value or call['owner_section'] == section):
        raise ValueError('Enemy RunEcl call does not resolve to its native definition')
    if call['offset'] != 0x397:
        raise ValueError('Enemy native RunEcl call moved from original offset 0x397')
    return dict(symbol=RUNECL_SYMBOL, call_offset=call['offset'],
                same_object_definition=True, definition_bytes=len(callee_raw),
                definition_fields=len(callee_fields))


def require_external_runecl_call(caller_fields, target):
    """Check only a genuine external call for the explicit standalone probe.

    This proves no callee visibility or codegen. The normal inspector still
    requires the complete same-object RunEcl definition.
    """
    observed = [row for row in caller_fields
                if row['symbol'] == RUNECL_SYMBOL or row['offset'] == 0x397]
    if len(observed) != 1:
        raise ValueError('standalone Enemy RunEcl call inventory differs')
    row = observed[0]
    if (type(row['offset']) is not int or row['offset'] != 0x397
            or row['symbol'] != RUNECL_SYMBOL
            or row['type'] != 'REL32' or row['addend'] != 0
            or type(row['addend']) is not int
            or type(row.get('symbol_section')) is not int or row['symbol_section'] != 0
            or type(row.get('symbol_value')) is not int or row['symbol_value'] != 0
            or type(row.get('owner_section')) is not int or row['owner_section'] <= 0
            or len(target) != OWNER_SIZE or target[0x396] != 0xE8
            or BASE+0x397+4+struct.unpack_from('<i', target, 0x397)[0] != 0x4086C0):
        raise ValueError('standalone Enemy lost its true external RunEcl call')
    return dict(symbol=RUNECL_SYMBOL, call_offset=0x397,
                same_object_definition=False, callee_codegen='not inspected',
                original_call_destination=0x4086C0, exactness_credit='none')


GAME_MANAGER_SYMBOL = '?g_GameManager@@3UAsciiGameManagerView@@A'
PLAYFIELD_SYMBOL = '?IsWithinPlayfield@AsciiGameManagerView@@QAEHMMMM@Z'
GAME_MANAGER_FIELDS = {0x0A: 0x134, 0x28F: 0xB4,
                       0x60E: 0, 0x669: 0, 0x69D: 0, 0x6D4: 0}
PLAYFIELD_CALLS = (0x613, 0x66E, 0x6A2, 0x6D9)


def require_game_manager_fields(rows, units, target):
    """Require the shared native global and member, including real COFF addends."""
    unit = units['game-manager-is-within-playfield']
    if (unit['symbol'], unit['target_address'], unit['size']) != (
            PLAYFIELD_SYMBOL, 0x41A6EB, 116):
        raise ValueError('canonical GameManager playfield contract changed')
    globals_ = [r for r in rows if r['symbol'] == GAME_MANAGER_SYMBOL
                or r['offset'] in GAME_MANAGER_FIELDS]
    calls = [r for r in rows if r['symbol'] == PLAYFIELD_SYMBOL
             or r['offset'] in PLAYFIELD_CALLS]
    if (len(target) != OWNER_SIZE or len(globals_) != 6 or len(calls) != 4
            or {r['offset'] for r in globals_} != set(GAME_MANAGER_FIELDS)
            or {r['offset'] for r in calls} != set(PLAYFIELD_CALLS)):
        raise ValueError('native GameManager field/call inventory differs')
    for row in globals_ + calls:
        offset = row['offset']
        is_global = offset in GAME_MANAGER_FIELDS
        addend = GAME_MANAGER_FIELDS[offset] if is_global else 0
        if (type(offset) is not int or type(row['addend']) is not int
                or row['addend'] != addend
                or row['type'] != ('DIR32' if is_global else 'REL32')
                or row['symbol'] != (GAME_MANAGER_SYMBOL if is_global else PLAYFIELD_SYMBOL)
                or type(row.get('symbol_section')) is not int or row['symbol_section'] != 0
                or type(row.get('symbol_value')) is not int or row['symbol_value'] != 0):
            raise ValueError('native GameManager root/member identity differs')
        actual = struct.unpack_from('<I', target, offset)[0]
        expected = (0x4A7D90 + addend if is_global else
                    (0x41A6EB - (BASE + offset + 4)) & 0xFFFFFFFF)
        if actual != expected or (not is_global and target[offset-1] != 0xE8):
            raise ValueError('original GameManager field/call target differs')
    return dict(global_symbol=GAME_MANAGER_SYMBOL, fields=6, calls=4,
                valueB4_addend=0xB4, flags_addend=0x134,
                playfield_unit='game-manager-is-within-playfield',
                playfield_bytes=116, same_object_definition=False)


def external_destination(row, units, functions, registry):
    symbol = row['symbol']
    if symbol in semantic_aliases:
        name = semantic_aliases[symbol]
        function = functions.get(name)
        if function is None or function['owner'] != 'authored' or not function['source_file']:
            raise ValueError('unreviewed non-exact call boundary: ' + symbol)
        state = 'matching' if function['status'] == 'matching' else 'non-exact'
        return int(function['address'], 0), 'reviewed source-present ' + state + ' owner ' + name
    if symbol in unit_aliases:
        unit_name = unit_aliases[symbol]
        unit = units.get(unit_name)
        if unit is None:
            raise ValueError('unavailable exact unit: ' + unit_name)
        return int(unit['target_address']), 'canonical exact unit ' + unit_name
    canonical = symbol_aliases.get(symbol, symbol)
    possibilities = {int(record['target']) for record in registry.get(canonical, [])}
    if len(possibilities) != 1:
        raise ValueError('absent or ambiguous independent destination for ' + symbol)
    return next(iter(possibilities)), 'canonical exact unit identity for ' + canonical


def bind_field(row, units, functions, registry, compare, target):
    off = int(row['offset'])
    if (row['symbol_section'] > 0 and
            row['symbol_section'] == row['owner_section']):
        local = int(row['symbol_value']) - int(row['owner_value'])
        if not 0 <= local < OWNER_SIZE:
            raise ValueError('owner-local symbol leaves reviewed extent')
        destination = BASE + local
        basis = 'actual same-section COFF label position (not target-derived)'
    else:
        destination, basis = external_destination(row, units, functions, registry)
    if row['symbol'].startswith('__real@'):
        bits = row['symbol'][7:]
        if len(bits) != 8 or compare.pe_bytes_at(target, destination, 4) != struct.pack(
                '<I', int(bits, 16)):
            raise ValueError('unreviewed or incorrect float literal: ' + row['symbol'])
    if row['type'] not in ('DIR32', 'REL32'):
        raise ValueError('unreviewed relocation kind')
    value = destination + row['addend']
    if row['type'] == 'REL32':
        value -= BASE + off + 4
    return value & 0xFFFFFFFF, destination, basis


def diagnose(path, *, explain=False):
    return _diagnose(path, explain=explain, external_runecl=False)


def diagnose_standalone(path, *, explain=False):
    """Explicit bounded diagnostic; never claim a same-object RunEcl proof."""
    return _diagnose(path, explain=explain, external_runecl=True)


def _diagnose(path, *, explain, external_runecl):
    compare = load_compare()
    units, functions, registry = reference_index()
    target_file = compare.verified_target()
    target = compare.pe_bytes_at(target_file, BASE, OWNER_SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError("original EnemyManager full-owner SHA changed")
    source, relocations = compare.object_function(
        path, SYMBOL, include_symbol_locations=True)
    if external_runecl:
        native_run_owner = require_external_runecl_call(relocations, target)
    else:
        callee_raw, callee_fields = compare.object_function(
            path, RUNECL_SYMBOL, include_symbol_locations=True)
        native_run_owner = require_runecl_definition(relocations, callee_raw, callee_fields)
    native_player_state = require_player_state_calls(relocations, units, target)
    native_player_damage = require_player_damage_calls(relocations, units, target)
    native_game_manager = require_game_manager_fields(relocations, units, target)
    greater_raw, greater_fields = compare.object_function(path, TIMER_GREATER_SYMBOL)
    native_timer_greater = require_timer_greater_definition(
        relocations, units, greater_raw, greater_fields, target)
    if len(source) != OWNER_SIZE:
        raise ValueError('unreviewed candidate physical extent: ' + str(len(source)))
    if len(relocations) != 97:
        raise ValueError('candidate relocation count differs from reviewed 97')
    data = bytearray(source)
    covered = set()
    bindings = []
    for row in relocations:
        off = int(row['offset'])
        field = set(range(off, off + 4))
        if off < 0 or off + 4 > len(source) or covered.intersection(field):
            raise ValueError('overlapping or out-of-owner relocation field')
        covered.update(field)
        value, destination, basis = bind_field(
            row, units, functions, registry, compare, target_file)
        struct.pack_into('<I', data, off, value)
        bindings.append({
            'offset': off, 'symbol': row['symbol'], 'type': row['type'],
            'destination': destination, 'basis': basis,
            'target_field_agrees': data[off:off + 4] == target[off:off + 4]
        })
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    candidate_instructions = list(decoder.disasm(data[:CODE_SIZE], BASE))
    target_instructions = list(decoder.disasm(target[:CODE_SIZE], BASE))
    if (sum(ins.size for ins in candidate_instructions) != CODE_SIZE or
            sum(ins.size for ins in target_instructions) != CODE_SIZE):
        raise ValueError('instruction decoding incomplete across the target-owned code')
    def calls(instructions):
        result = []
        for ins in instructions:
            if ins.mnemonic != 'call':
                continue
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM:
                raise ValueError('unreviewed indirect call in Enemy update')
            result.append(('direct', ins.operands[0].imm))
        return result
    # The source-bound target proves one four-entry switch with a NOP before
    # its table at 0x41165C. Bind labels from COFF before validating the graph.
    if target[CODE_SIZE] != 0x90 or data[CODE_SIZE] != 0x90:
        raise ValueError('unreviewed ECL-independent Enemy update table alignment')
    target_table = struct.unpack_from('<4I', target, CODE_SIZE + 1)
    candidate_table = struct.unpack_from('<4I', data, CODE_SIZE + 1)
    switch_va = 0x00411284
    switch_operand = 'dword ptr [ecx*4 + 0x41165c]'
    target_graph = switch_graph(target_instructions, capstone.x86.X86_OP_IMM,
                                switch_va, switch_operand, target_table)
    candidate_graph = switch_graph(candidate_instructions, capstone.x86.X86_OP_IMM,
                                   switch_va, switch_operand, candidate_table)
    differences = [i for i, (a, b) in enumerate(zip(data, target)) if a != b]
    explanation = describe_difference_regions(
        target_instructions, candidate_instructions, differences, base=BASE) if explain else None
    result = {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'object': str(path),
        'native_run_owner': native_run_owner,
        'native_player_state': native_player_state,
        'native_player_damage': native_player_damage,
        'native_game_manager': native_game_manager,
        'native_timer_greater': native_timer_greater,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'target_bytes': OWNER_SIZE,
        'candidate_bytes': len(source),
        'code_bytes': CODE_SIZE,
        'relocations': len(relocations),
        'relocation_fields_agree': sum(r['target_field_agrees'] for r in bindings),
        'target_instructions': len(target_instructions),
        'candidate_instructions': len(candidate_instructions),
        'target_calls': len(calls(target_instructions)),
        'candidate_calls': len(calls(candidate_instructions)),
        'ordered_calls_agree': calls(target_instructions) == calls(candidate_instructions),
        'target_direct_blocks': len(target_graph),
        'candidate_direct_blocks': len(candidate_graph),
        'reviewed_control_flow_agrees': target_graph == candidate_graph,
        'switch_table_destinations_agree': target_table == candidate_table,
        'complete_bytes_agree': data == target,
        'complete_differences': len(differences),
        'difference_offsets': [f'0x{i:X}' for i in differences],
        'bindings': bindings,
    }
    if explanation is not None:
        result['difference_regions'] = explanation
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--explain', action='store_true',
                        help='Include full instructions around grouped byte differences')
    args = parser.parse_args()
    report = diagnose(args.object, explain=args.explain)
    if args.json:
        print(json.dumps(report, indent=2))
        return
    print('NON-CREDITING EnemyManager::OnUpdate complete owner:',
          report['candidate_bytes'], 'bytes,', report['relocations'], 'bound fields')
    print('all bound operand fields agree:', report['relocation_fields_agree'],
          '/', report['relocations'])
    print('target/candidate instructions:', report['target_instructions'],
          '/', report['candidate_instructions'])
    print('target/candidate calls:', report['target_calls'], '/', report['candidate_calls'],
          'ordered identities agree:', report['ordered_calls_agree'])
    print('target/candidate graph blocks:', report['target_direct_blocks'], '/',
          report['candidate_direct_blocks'],
          'reviewed control flow agrees:', report['reviewed_control_flow_agrees'])
    print('complete target-linked byte differences:', report['complete_differences'],
          'at', ','.join(report['difference_offsets']))
    print('exact:', report['complete_bytes_agree'], '(never grants exactness credit)')
    if args.explain:
        for region in report['difference_regions']:
            print('\nEnemy difference group', region['offset_start'],
                  '..', region['offset_end_exclusive'],
                  'different bytes:', region['different_bytes'])
            for label, field in (('TARGET', 'target_instructions'),
                                 ('CANDIDATE', 'candidate_instructions')):
                print(' ', label)
                for ins in region[field]:
                    print('   %-7s %-20s %-8s %s' %
                          (ins['offset'], ins['bytes'],
                           ins['mnemonic'], ins['operands']))


if __name__ == '__main__':
    main()
