#!/usr/bin/env python3
"""Complete, non-crediting TH09 TitleScreen::DrawReplayMenu original PE/COFF audit.

Retail Japanese TH09 v1.50a:
  0x004234F6, 1,280 physical bytes, 376 decoded x86 instructions,
  57 independently decoded code/data address fields, 13 calls, 72 CFG blocks.

The authentic source is currently a 1,258-byte, 368-instruction, 70-block
candidate. Neither the source's field count nor a normalized alignment score
can certify exactness. Strict exactness needs a separate source-commit-bound
Factory replay of the entire original-owned physical byte range.
"""

from __future__ import annotations

import argparse
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004234F6
SIZE = 1280
SYMBOL = '?DrawReplayMenu@TitleScreenView@@QAEHXZ'
ORIGINAL_SHA256 = 'd0201b3aae3727f7aa773187ac9ac6d21727c86342b657c780cb9353cdc39ed0'
ORIGINAL_INSTRUCTIONS = 376
ORIGINAL_FIELDS = 57
ORIGINAL_CALLS = 13
ORIGINAL_BLOCKS = 72

# One-time reviewed mappings from ORIGINAL PE absolute references, direct
# call xrefs, literal values and exact existing TH09 implementation owners.
# Distinct COFF spellings can have nonzero addends; every original destination
# is re-disassembled from the immutable SHA-pinned PE before source binding.
# These are original addresses, not relocated candidate byte guesses.
DESTINATIONS = {
    '??_C@_02DKCKIIND@?$CFs?$AA@': 0x0048F538,
    '??_C@_08FLEDHNDO@?9?9?1?9?9?1?9?9?$AA@': 0x0048F588,
    '??_C@_08GGNFKEJM@?9?9?9?9?9?9?9?9?$AA@': 0x0048F57C,
    '??_C@_08ODHOLODL@?$CFs?5?5?9?$CFs?9?$AA@': 0x0048F53C,
    '??_C@_0BA@KHLACNNE@No?4?$CF?42d?5?$CF8s?5?$CF8s?$AA@': 0x0048F56C,
    '??_C@_0BC@JOBPAHOE@?$CFc?$CFc?$CFc?$CFc?5?5?$CF8s?5?$CF8s?$AA@': 0x0048F558,
    '??_C@_0N@HFEJOOOP@?$CF?48s?5vs?5?$CF?48s?$AA@': 0x0048F520,
    '?AddFormatText@AsciiManager@@QAAXPAUFloat3@@PBDZZ': 0x00434330,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x004CE458,
    '?g_ReplayCharacterNames@@3PAPBDA': 0x004A1CF8,
    '?g_ReplayDifficultyNames@@3PAPBDA': 0x004A1D3C,
    '?g_ReplayExtraModeName@@3PAPBDA': 0x004A1D54,
    '?g_ReplayModeNames@@3PAPBDA': 0x004A1D58,
    '?g_ReplayStoryModeName@@3PAPBDA': 0x004A1D50,
    '__real@00000000': 0x0048E314,
    '__real@3dcccccd': 0x0048E4B0,
    '__real@40900000': 0x0048F534,
    '__real@41500000': 0x0048F554,
    '__real@42b40000': 0x0048F550,
    '__real@43200000': 0x0048F548,
    '__real@43588000': 0x0048F54C,
    '__real@43a00000': 0x0048F530,
    '__real@4f800000': 0x0048E2A8,
    '_sprintf': 0x0047C47B,
    '_strlen': 0x0047C3F0,
}


def complete_instructions(bytecode: bytes, *, capstone, base: int = BASE):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(bytecode, base))
    if not ins or sum(i.size for i in ins) != len(bytecode):
        raise ValueError('DrawReplayMenu incomplete original/source x86 decode')
    if ins[-1].mnemonic != 'ret' or ins[-1].op_str:
        raise ValueError('DrawReplayMenu source/original return ABI changed')
    return ins


def original_address_fields(instructions, *, capstone, base: int = BASE):
    """List RETAIL-PE-encoded real absolute data and direct call operands."""
    fields = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('DrawReplayMenu target contains an unreviewed indirect call')
            fields.append((ins.address - base + ins.imm_offset, 'REL32', ins.operands[0].imm))
        else:
            for op in ins.operands:
                if op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                    if 0x00400000 <= op.mem.disp < 0x00500000:
                        fields.append((ins.address - base + ins.disp_offset, 'DIR32', op.mem.disp))
                elif op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                    if 0x00400000 <= op.imm < 0x00500000:
                        fields.append((ins.address - base + ins.imm_offset, 'DIR32', op.imm))
    return fields


def normalized(ins):
    if ins.mnemonic.startswith('j') or ins.mnemonic == 'call':
        return ins.mnemonic, 'DIRECT'
    return ins.mnemonic, re.sub(r'0x[0-9a-f]+', 'CONST', ins.op_str)


def bind_candidate(candidate: bytes, rows: list[dict],
                   original_fields: list[tuple[int, str, int]], *, target: bytes) -> bytes:
    """Allow only the 57 known retail code/data field identities and order."""
    if not 1230 <= len(candidate) <= SIZE:
        raise ValueError('DrawReplayMenu unreviewed source physical extent')
    if len(rows) != ORIGINAL_FIELDS or len(original_fields) != ORIGINAL_FIELDS:
        raise ValueError('DrawReplayMenu changed original 57-field census')
    bound = bytearray(candidate)
    seen = set()
    for row, (original_offset, original_kind, original_destination) in zip(rows, original_fields):
        offset, kind = row['offset'], row['type']
        symbol = row['symbol']
        if (kind not in ('REL32', 'DIR32') or kind != original_kind or
                not isinstance(offset, int) or offset < 0 or offset+4 > len(candidate) or
                set(range(offset, offset+4)).intersection(seen)):
            raise ValueError('DrawReplayMenu source COFF kind/offset/extent changed')
        seen.update(range(offset, offset+4))
        if symbol not in DESTINATIONS or DESTINATIONS[symbol] + row['addend'] != original_destination:
            raise ValueError('DrawReplayMenu unknown or original-mismatching relocation: ' + symbol)
        # Check the independent unmodified PE encoding at its own physical
        # field offset, not the candidate offset (different code lengths).
        expected_original = (original_destination - (BASE+original_offset+4)
                             if kind == 'REL32' else original_destination)
        if struct.unpack_from('<I', target, original_offset)[0] != (expected_original & 0xFFFFFFFF):
            raise ValueError('DrawReplayMenu original PE operand encoding disagrees')
        linked = (original_destination - (BASE+offset+4)
                  if kind == 'REL32' else original_destination)
        struct.pack_into('<I', bound, offset, linked & 0xFFFFFFFF)
    return bytes(bound)


def diagnose(path: Path) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    co = importlib.util.spec_from_file_location('th09_menu_coff', ROOT/'scripts/compare-coff-function.py')
    tool = importlib.util.module_from_spec(co)
    co.loader.exec_module(tool)
    pe = tool.verified_target()
    target = tool.pe_bytes_at(pe, BASE, SIZE)
    actual_sha = hashlib.sha256(target).hexdigest()
    if actual_sha != ORIGINAL_SHA256:
        raise ValueError('DrawReplayMenu original retail SHA256 changed')
    ti = complete_instructions(target, capstone=capstone)
    target_fields = original_address_fields(ti, capstone=capstone)
    if len(ti) != ORIGINAL_INSTRUCTIONS or len(target_fields) != ORIGINAL_FIELDS:
        raise ValueError('DrawReplayMenu original instruction/field census changed')
    source, rows = tool.object_function(path, SYMBOL)
    linked = bind_candidate(source, rows, target_fields, target=target)
    ci = complete_instructions(linked, capstone=capstone)
    source_fields = original_address_fields(ci, capstone=capstone)
    if len(source_fields) != ORIGINAL_FIELDS:
        raise ValueError('DrawReplayMenu source decoded field coverage changed')
    for row, real in zip(rows, source_fields):
        if real != (row['offset'], row['type'], DESTINATIONS[row['symbol']] + row['addend']):
            raise ValueError('DrawReplayMenu COFF field does not own a decoded source operand')
    def calls(items):
        return tuple(i.operands[0].imm for i in items if i.mnemonic == 'call')
    tc,cc = calls(ti),calls(ci)
    if len(tc) != ORIGINAL_CALLS:
        raise ValueError('DrawReplayMenu original ordered call census changed')
    tgraph = direct_control_flow(ti,capstone.x86.X86_OP_IMM)
    cgraph = direct_control_flow(ci,capstone.x86.X86_OP_IMM)
    if len(tgraph) != ORIGINAL_BLOCKS:
        raise ValueError('DrawReplayMenu original CFG boundary changed')
    aligned = difflib.SequenceMatcher(None,list(map(normalized,ti)),
                 list(map(normalized,ci)),autojunk=False)
    mismatches = [i for i,(a,b) in enumerate(zip(target,linked)) if a!=b]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': actual_sha,
        'target_bytes': SIZE, 'candidate_bytes':len(linked),
        'target_instructions':len(ti), 'candidate_instructions':len(ci),
        'original_fields':len(target_fields), 'bound_fields':len(rows),
        'original_calls':len(tc), 'candidate_calls':len(cc),
        'ordered_calls_agree':tc==cc,
        'original_blocks':len(tgraph), 'candidate_blocks':len(cgraph),
        'direct_graphs_agree':tgraph==cgraph,
        'aligned_instructions':sum(b.size for b in aligned.get_matching_blocks()),
        'overlap_differences':len(mismatches),
        'difference_offsets':mismatches,
        'missing_target_bytes':max(0,SIZE-len(linked)),
        'extra_candidate_bytes':max(0,len(linked)-SIZE),
        'complete_bytes_equal':target == linked,
    }


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object',type=Path)
    parser.add_argument('--json',action='store_true')
    args=parser.parse_args()
    result=diagnose(args.object)
    if args.json:
        print(json.dumps(result,indent=2))
    else:
        print('NON-CREDITING Title DrawReplayMenu complete original PE/COFF')
        for key in ('target_sha256','target_bytes','candidate_bytes',
                    'target_instructions','candidate_instructions',
                    'original_fields','bound_fields','original_calls',
                    'candidate_calls','ordered_calls_agree',
                    'original_blocks','candidate_blocks','direct_graphs_agree',
                    'aligned_instructions','overlap_differences',
                    'missing_target_bytes','complete_bytes_equal'):
            print(key+':',result[key])


if __name__=='__main__':
    main()
