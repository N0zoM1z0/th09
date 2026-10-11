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
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

import capstone

from diagnostic_switch_graph import switch_graph

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00410730
OWNER_SIZE = 3900  # 3883 instructions + one alignment + four-entry table
CODE_SIZE = 3883
SYMBOL = '?OnUpdate@EnemyManagerView@@SIHPAU1@@Z'


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
 '?IsAfter@EnemyCoreTimerView@@QAEHH@Z':'zun-timer-greater',
 '?Set@EnemyCoreTimerView@@QAEXH@Z':'zun-timer-assign-int',
 '??EEnemyCoreTimerView@@QAEXH@Z':'zun-timer-post-increment',
 '?HasTicked@EnemyCoreTimerView@@QAEHH@Z':'zun-timer-has-ticked-every',
 '??0EnemyCoreBulletSpawnDescriptorView@@QAE@XZ':'bullet-spawn-descriptor-constructor',
 '?SpawnBulletPatternPrimary@EnemyCoreEtamaControllerView@@QAEPAXPAUEnemyCoreBulletSpawnDescriptorView@@@Z':'bullet-pattern-primary',
 '??FEnemyCoreTimerView@@QAEXH@Z':'zun-timer-post-decrement',
 '?IsWithinPlayfield@EnemyCoreGameManagerPlayfieldView@@QAEHMMMM@Z':'game-manager-is-within-playfield',
 '?EnemyCoreRunLifeCallback@@YIHPAUEnemyCoreView@@@Z':'player-check-state2',
 '?EnemyCoreRunTimerCallback@@YIHPAUEnemyCoreView@@@Z':'player-check-state2',
 '?ExecuteScript@EnemyCoreAnmManagerView@@QAEHPAUEnemyCoreAnmVmView@@@Z':'anm-manager-execute-script',
 '?SetState@EnemyCorePlayerView@@QAEXH@Z':'player-set-update-state',
 '?AddScore@EnemyCoreSideStateView@@QAEXH@Z':'game-manager-side-add-score',
 '?EnemyCoreReleaseAttachedEffects@@YIXPAUEnemyCoreView@@@Z':'enemy-ecl-release-attached-effects',
 '?PlaySoundPositionedByIdx@EnemyCoreSoundPlayerView@@QAEXHM@Z':'soundplayer-play-positioned-sound-by-idx',
 '?SetBossMarkerPosition@EnemyCoreUiView@@QAEXHPAUFloat3@@@Z':'ascii-boss-marker-set-position',
 '?SetBossMarkerState@EnemyCoreUiView@@QAEXHH@Z':'ascii-manager-set-boss-marker-state',
}
symbol_aliases={
 '?g_EnemyCoreRuntimeFlags@@3IA':'?g_PlayerUpdateGateFlags@@3IA',
 '?g_EnemyCoreFront@@3PAUEnemyCoreFrontView@@A':'?g_PlayerSharedRuntime@@3PAUPlayerSharedRuntimeView@@A',
 '?g_EnemyCoreSchedule@@3PAEA':'?g_EnemySubroutineSchedule@@3PAEA',
 '?g_EnemyCoreDifficultyValue@@3HA':'?g_PlayerRewardBaseValue@@3HA',
 '?GetState@EnemyCorePlayerView@@QAEHXZ':'?GetUpdateState@PlayerLifecycleView@@QAEHXZ',
 '?RunEcl@EnemyEclManagerView@@QAEHPAUEnemyView@@@Z':'?RunEcl@EnemySpawnEclManagerBoundary@@QAEHPAUEnemyView@@@Z',
 '?g_EnemyCoreGameManager@@3UEnemyCoreGameManagerPlayfieldView@@A':'?g_GameManager@@3UGameManagerSetupLayout@@A',
 '?g_EnemyCoreAnmManager@@3PAUEnemyCoreAnmManagerView@@A':'?g_AnmManager@@3PAVAnmManager@@A',
 '?Current@EnemyCoreTimerView@@QAEHXZ':'??BZunTimer@@QAEHXZ',
 '?g_EnemyCoreSoundPlayer@@3UEnemyCoreSoundPlayerView@@A':'?g_SoundPlayer@@3VSoundPlayer@@A',
 '?g_EnemyCoreUi@@3UEnemyCoreUiView@@A':'?g_AsciiManager@@3VAsciiManager@@A',
}
semantic_aliases={
 '?AppendPlayerCollisionBox@EnemyAppendCollisionView@@QAEXPBUFloat3@@0@Z':'EnemyAppendCollisionView::AppendPlayerCollisionBox',
 '?CalcDamageToEnemy@EnemyCorePlayerView@@QAEHPAUFloat3@@0PAH11@Z':'PlayerDamagePlayerView::CalcDamageToEnemy',
}


def external_destination(row, units, functions, registry):
    symbol = row['symbol']
    if symbol in semantic_aliases:
        name = semantic_aliases[symbol]
        function = functions.get(name)
        if function is None or function['owner'] != 'authored' or not function['source_file']:
            raise ValueError('unreviewed non-exact call boundary: ' + symbol)
        return int(function['address'], 0), 'reviewed source-present non-exact owner ' + name
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


def diagnose(path):
    compare = load_compare()
    units, functions, registry = reference_index()
    target_file = compare.verified_target()
    target = compare.pe_bytes_at(target_file, BASE, OWNER_SIZE)
    source, relocations = compare.object_function(
        path, SYMBOL, include_symbol_locations=True)
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
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'object': str(path),
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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    report = diagnose(args.object)
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


if __name__ == '__main__':
    main()
