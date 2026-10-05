#!/usr/bin/env python3
"""Complete target-bound Etama update diagnostic; no exactness or prefix credit."""
import argparse
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct


ROOT = Path(__file__).resolve().parents[1]
BASE = 0x4146F0
BODY_SIZE = 2195
PHYSICAL_SIZE = 2216  # body, one alignment byte, five switch-table DWORDs
SYMBOL = '?OnUpdate@EtamaController@@SIHPAU1@@Z'

# TH09 direct call destinations, checked against the complete physical call
# order and direct IDA callees; these are not inferred from candidate bytes.
DESTINATIONS = {
    '??4ZunTimer@@QAEXH@Z': 0x401500,
    '??BFloat3@@QAEPAMXZ': 0x4343D0,
    '??BZunTimer@@QAEMXZ': 0x4014F0,
    '??EZunTimer@@QAEXH@Z': 0x401510,
    '??FZunTimer@@QAEXH@Z': 0x406670,
    '??KFloat3@@QBE?AU0@M@Z': 0x40F5A0,
    '??MZunTimer@@QAEIH@Z': 0x401540,
    '??NZunTimer@@QAEIH@Z': 0x406720,
    '??PZunTimer@@QAEIH@Z': 0x401520,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '?AddNormalizeAngle@@YGMMM@Z': 0x42AED0,
    '?AdvanceTransformProgram@Bullet@@QAEXXZ': 0x4141C0,
    '?AppendBoxRecord@PlayerCollisionQueryStateView@@QAEXPBUPlayerPositionView@@0H@Z': 0x40F8E0,
    '?AppendLaserRecord@PlayerCollisionQueryStateView@@QAEXPBUPlayerPositionView@@00MH@Z': 0x412830,
    '?CheckBulletCollision@BulletPlayerView@@QAEHPAUFloat3@@0PAUBullet@@@Z': 0x41DFF0,
    '?CheckGrazeCollision@BulletPlayerView@@QAEHPAUFloat3@@0PAUBullet@@@Z': 0x41E350,
    '?ClearDrawBuckets@EtamaController@@QAEHXZ': 0x412390,
    '?Deactivate@Bullet@@QAEXXZ': 0x412930,
    '?ExecuteScript@BulletAnmManagerView@@QAEHPAUAnmVm@@@Z': 0x436F30,
    '?GetCurrent@BulletTimerCurrentView@@QAEHXZ': 0x435F00,
    '?IsWithinPlayfield@BulletGameManagerView@@QAEHMMMM@Z': 0x41A6EB,
    '?SelectSide@BulletSupervisorView@@QAEXH@Z': 0x401440,
    '?SetZRotation@AnmVm@@QAEXM@Z': 0x40F840,
    '?UpdateBulletAbsoluteDirectionChange@@YIXPAUBullet@@@Z': 0x413700,
    '?UpdateBulletAimedDirectionChange@@YIXPAUBullet@@@Z': 0x4137D0,
    '?UpdateBulletBoundaryBounce@@YIXPAUBullet@@@Z': 0x4138C0,
    '?UpdateBulletDeceleration@@YIXPAUBullet@@@Z': 0x413460,
    '?UpdateBulletHorizontalWrap@@YIXPAUBullet@@@Z': 0x4139F0,
    '?UpdateBulletPolarAcceleration@@YIXPAUBullet@@@Z': 0x413590,
    '?UpdateBulletRelativeDirectionChange@@YIXPAUBullet@@@Z': 0x413630,
    '?UpdateBulletVectorAcceleration@@YIXPAUBullet@@@Z': 0x4134D0,
    '?UpdateBulletVerticalWrap@@YIXPAUBullet@@@Z': 0x413A70,
    '__ftol2': 0x47B1D4,
    # Globals and target literal cells. Compiler-private labels are verified
    # below by their section-relative location, not their unstable spelling.
    '?g_AnmManager@@3PAUBulletAnmManagerView@@A': 0x4DC550,
    '?g_GameManager@@3UBulletGameManagerView@@A': 0x4A7D90,
    '?g_Supervisor@@3UBulletSupervisorView@@A': 0x4B3100,
    '__real@00000000': 0x48E314,
    '__real@3d800000': 0x48E828,
    '__real@3f000000': 0x490F54,
    '__real@3f99999a': 0x48E830,
    '__real@3fc90fdb': 0x48E450,
    '__real@437f0000': 0x48E834,
    '__real@44200000': 0x48E82C,
}

# Each switch relocation must name an internal label at this precise owner
# offset. The names themselves drift when unrelated source context changes.
PRIVATE_FIELDS = {
    0x0D0: 0x894,
    0x894: 0x1B0,
    0x898: 0x0D4,
    0x89C: 0x10A,
    0x8A0: 0x140,
    0x8A4: 0x574,
}


def relocation_destination(row):
    if row['offset'] in PRIVATE_FIELDS:
        if (not row['symbol'].startswith('$L') or row['type'] != 'DIR32'
                or row['addend'] != 0
                or row['symbol_section'] != row['owner_section']
                or row['symbol_value'] - row['owner_value']
                != PRIVATE_FIELDS[row['offset']]):
            raise ValueError('unreviewed compiler-private destination')
        return BASE + PRIVATE_FIELDS[row['offset']]
    if row['symbol'].startswith('$L'):
        raise ValueError('unreviewed compiler-private relocation field')
    try:
        return DESTINATIONS[row['symbol']] + row['addend']
    except KeyError as error:
        raise ValueError('unreviewed external relocation symbol') from error


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    # Pure relocation guards and --help must work in dependency-free public CI.
    import capstone

    spec = importlib.util.spec_from_file_location(
        'coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, PHYSICAL_SIZE)
    raw, rows = coff.object_function(
        args.object, SYMBOL, include_symbol_locations=True)
    if len(raw) != PHYSICAL_SIZE or len(rows) != 88:
        raise ValueError('candidate physical extent/relocation census changed')
    if target[BODY_SIZE] != 0x90:
        raise ValueError('target body/table boundary changed')
    code = bytearray(raw)
    seen_private_fields = set()
    for row in rows:
        destination = relocation_destination(row)
        if row['offset'] in PRIVATE_FIELDS:
            seen_private_fields.add(row['offset'])
        if row['type'] == 'REL32':
            if row['addend']:
                raise ValueError('unreviewed nonzero direct-call addend')
            destination -= BASE + row['offset'] + 4
        elif row['type'] != 'DIR32':
            raise ValueError('unreviewed relocation type')
        struct.pack_into('<I', code, row['offset'], destination & 0xffffffff)
    if seen_private_fields != set(PRIVATE_FIELDS):
        raise ValueError('incomplete compiler-private relocation coverage')

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(blob):
        result = list(decoder.disasm(blob[:BODY_SIZE], BASE))
        if (not result or result[-1].address + result[-1].size != BASE + BODY_SIZE
                or result[-1].mnemonic != 'ret'):
            raise ValueError('incomplete function body decoding')
        return result

    target_ins, candidate_ins = decode(target), decode(code)

    def calls(ins):
        result = []
        for item in ins:
            if item.mnemonic == 'call':
                if item.operands[0].type != capstone.x86.X86_OP_IMM:
                    raise ValueError('unreviewed indirect call')
                result.append(item.operands[0].imm)
        return result

    target_calls, candidate_calls = calls(target_ins), calls(candidate_ins)
    if len(target_calls) != 62 or len(candidate_calls) != 62:
        raise ValueError('direct-call census changed')
    if len(target_ins) != 553 or len(candidate_ins) != 553:
        raise ValueError('instruction census changed')
    normal = lambda i: (i.mnemonic, 'control') if i.mnemonic.startswith('j') or i.mnemonic == 'call' else (i.mnemonic, i.op_str)
    aligned = sum(block.size for block in difflib.SequenceMatcher(
        None, list(map(normal, target_ins)), list(map(normal, candidate_ins)),
        autojunk=False).get_matching_blocks())
    print(json.dumps({
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'body_bytes': BODY_SIZE,
        'physical_bytes': PHYSICAL_SIZE,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'relocations': len(rows),
        'ordered_calls_agree': target_calls == candidate_calls,
        'complete_bytes_agree': code == target,
        'complete_differences': sum(a != b for a, b in zip(code, target)),
        'post_entry_suffix_agrees': code[0x53:] == target[0x53:],
        'normalized_aligned_instructions': aligned,
    }, indent=2))


if __name__ == '__main__':
    main()
