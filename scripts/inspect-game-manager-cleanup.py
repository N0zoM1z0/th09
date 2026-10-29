#!/usr/bin/env python3
"""TH09 Japanese 1.50a GameplayCleanup complete nonexact original/COFF audit.

The original has 425 bytes, 117 instructions, 43 independently decoded data/
call fields, 12 direct calls and 24 control-flow blocks. The maintained source now replays all 425 bytes and 43 bound fields. This diagnostic ties every candidate field to
its real symbol/destination, reports alignment of original identities, and
checks a genuine branch restoring the mode-2 minimum clamp. It never grants
accepted exactness or partial-byte credit.
"""
from __future__ import annotations

import argparse
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT=Path(__file__).resolve().parents[1]
BASE,SIZE=0x0041B5C6,425
SYMBOL='?CleanupGameplayState@GameManagerSetupLayout@@SIXXZ'
TARGET_SHA='e31f3c176b3b735008612b07823982a646248cbae4fb8ac826e2f2cff2d6a798'
SOURCES={
 '?g_GameManager@@3UAsciiGameManagerView@@A':0x004A7D90,
 '?ResetGameManager@@YIXPAUGameManagerSetupLayout@@@Z':0x0041A7C3,
 '?g_AsciiManager@@3VAsciiManager@@A':0x004CE458,
 '?Reset@AsciiManager@@QAEXXZ':0x004343E0,
 '?InitializeMessageRuntime@FrontMessageOwnerView@@QAEXXZ':0x004181E0,
 '?ResetExAttackRecords@@YIXPAUExAttackController@@@Z':0x004156F0,
 '?ClearSpellBackgroundState@SetupBackgroundResetView@@QAEXXZ':0x004011C0,
 '?InitializeAddedState@PlayerLifecycleView@@QAEXXZ':0x0041EBC0,
 '?ResetProjectiles@EtamaController@@QAEXXZ':0x00412470,
 '?ResetPool@EffectManager@@QAEXXZ':0x0040C990,
 '?ResetForGameplayCleanup@EnemyManagerView@@QAEXXZ':0x00411FD0,
 '?RefreshResource@SetupCardAttackResetView@@QAEXXZ':0x00403D80,
 '?ResetTransitionState@SetupFrontSideResetView@@QAEXXZ':0x0041A230,
}


def complete_decode(code: bytes):
    import capstone
    cs=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32)
    cs.detail=True
    instructions=list(cs.disasm(code,BASE))
    if not instructions or sum(i.size for i in instructions)!=len(code):
        raise ValueError('Cleanup instruction coverage incomplete')
    if instructions[-1].mnemonic!='ret' or instructions[-1].op_str:
        raise ValueError('Cleanup return ABI changed')
    return instructions


def physical_fields(instructions):
    import capstone
    items=[];calls=[]
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic=='call':
            if (len(ins.operands)!=1 or ins.operands[0].type!=capstone.x86.X86_OP_IMM
                    or ins.imm_size!=4):
                raise ValueError('Cleanup contains unreviewed indirect call')
            dest=ins.operands[0].imm
            items.append((ins.address-BASE+ins.imm_offset,'REL32',dest))
            calls.append(dest)
            continue
        for arg in ins.operands:
            if arg.type==capstone.x86.X86_OP_MEM and ins.disp_size==4:
                if 0x400000<=arg.mem.disp<0x4E7000:
                    items.append((ins.address-BASE+ins.disp_offset,'DIR32',arg.mem.disp))
            elif arg.type==capstone.x86.X86_OP_IMM and ins.imm_size==4:
                if 0x400000<=arg.imm<0x4E7000:
                    items.append((ins.address-BASE+ins.imm_offset,'DIR32',arg.imm))
    return items,calls


LIMIT_GLOBAL=0x004A7E44  # original g_GameManager.limitB4, true +0xB4 field
ORIGINAL_LIMIT_TRACE={
    'default_reads': [0x169],
    'all_writes': [0x0A2, 0x179, 0x180],
    'normal_clamp_branch': [0x17E],
}


def cleanup_limit_trace(instructions):
    """Decode the REAL global reads/writes, not C++ comments or field counts.

    Target +0x179 publishes the unclamped limit BEFORE +0x17E JGE, and
    +0x180 conditionally publishes one. The maintained source now preserves
    that pair of observable writes together with the complete original
    425-byte register and field schedule.
    """
    import capstone
    reads, writes, branches=[], [], []
    for ins in instructions:
        offset=ins.address-BASE
        if ins.mnemonic=='mov' and len(ins.operands)==2:
            dst,src=ins.operands
            if (dst.type==capstone.x86.X86_OP_MEM and
                    dst.mem.base==0 and dst.mem.index==0 and
                    dst.mem.disp==LIMIT_GLOBAL):
                writes.append(offset)
            if (src.type==capstone.x86.X86_OP_MEM and
                    src.mem.base==0 and src.mem.index==0 and
                    src.mem.disp==LIMIT_GLOBAL):
                reads.append(offset)
        if ins.mnemonic=='jge' and 0x150<=offset<0x190:
            branches.append(offset)
    return {'default_reads':reads,'all_writes':writes,
            'normal_clamp_branch':branches}


def validate_original_limit_trace(trace):
    """Fail closed on this separately confirmed original store chronology."""
    if (not isinstance(trace,dict) or trace!=ORIGINAL_LIMIT_TRACE or
            not (trace['all_writes'][1] < trace['normal_clamp_branch'][0] <
                 trace['all_writes'][2])):
        raise ValueError('Cleanup original initial limit publication / clamp order differs')
    return trace


def original_attest(target: bytes):
    if len(target)!=SIZE or hashlib.sha256(target).hexdigest()!=TARGET_SHA:
        raise ValueError('Cleanup original 425-byte function SHA256/extent changed')
    ins=complete_decode(target)
    fields,calls=physical_fields(ins)
    if (len(ins),len(fields),len(calls))!=(117,43,12):
        raise ValueError('Cleanup original 117-instruction/43-field/12-call census changed')
    for off,kind,dest in fields:
        encoded=(dest-(BASE+off+4 if kind=='REL32' else 0))&0xffffffff
        if struct.unpack_from('<I',target,off)[0]!=encoded:
            raise ValueError('Cleanup original address encoding invalid')
    return ins,fields,calls


def link_coff(raw: bytes, rows: list[dict]):
    if not 360<=len(raw)<=470 or len(rows) not in (41, 42, 43):
        raise ValueError('Cleanup candidate has unreviewed length or 41/42/43-field inventory')
    bound=bytearray(raw);actual=[];prev=0
    for row in rows:
        offset=row['offset'];kind=row['type'];symbol=row['symbol'];addend=row['addend']
        if symbol not in SOURCES or kind not in ('DIR32','REL32'):
            raise ValueError('unknown Cleanup candidate external referent')
        if not isinstance(offset,int) or offset<prev or offset+4>len(raw):
            raise ValueError('Cleanup candidate field overlap/out-of-order/beyond extent')
        prev=offset+4
        dest=SOURCES[symbol]+addend
        value=(dest-(BASE+offset+4 if kind=='REL32' else 0))&0xffffffff
        struct.pack_into('<I',bound,offset,value)
        actual.append((offset,kind,dest))
    return bytes(bound),actual


def norm_instructions(ins,fields):
    offs=set(x[0] for x in fields)
    values=[]
    for i in ins:
        start=i.address-BASE
        b=bytearray(i.bytes)
        for off in offs:
            if start<=off<start+i.size:
                if off-start+4>i.size:raise ValueError('Cleanup relocation crosses an instruction')
                b[off-start:off-start+4]=bytes(4)
        if i.mnemonic.startswith('j') and i.imm_size:
            b[i.imm_offset:i.imm_offset+i.imm_size]=bytes(i.imm_size)
        values.append(bytes(b))
    return values


def diagnose(path: Path):
    from diagnostic_field_alignment import align_fields
    from diagnostic_branch_alignment import direct_control_flow
    import capstone
    sp=importlib.util.spec_from_file_location('cleanup_coff',ROOT/'scripts/compare-coff-function.py')
    coff=importlib.util.module_from_spec(sp);sp.loader.exec_module(coff)
    target=coff.pe_bytes_at(coff.verified_target(),BASE,SIZE)
    ti,tf,tc=original_attest(target)
    limit_target_trace=validate_original_limit_trace(cleanup_limit_trace(ti))
    raw,rr=coff.object_function(path,SYMBOL)
    linked,expected_fields=link_coff(raw,rr)
    ci=complete_decode(linked)
    cf,cc=physical_fields(ci)
    if cf!=expected_fields:
        raise ValueError('Cleanup decoded candidate operands disagree with actual COFF records')
    if tc!=cc:
        raise ValueError('Cleanup candidate changes original 12 ordered calls')
    tg=direct_control_flow(ti,capstone.x86.X86_OP_IMM)
    cg=direct_control_flow(ci,capstone.x86.X86_OP_IMM)
    if len(tg)!=24:
        raise ValueError('Cleanup original 24-block CFG changed')
    align=align_fields(tf,cf)
    norm_t=norm_instructions(ti,tf)
    norm_c=norm_instructions(ci,cf)
    matches=difflib.SequenceMatcher(None,norm_t,norm_c,autojunk=False)
    # Check the genuine mode-2 branch, not just any late conditional clamp.
    early=[(x.address-BASE,x.mnemonic,x.op_str) for x in ci if 0x86<=x.address-BASE<0xAE]
    has_mode2_clamp=any(0x98<=off<=0xA3 and op=='jge' for off,op,_ in early)
    return {
        'diagnostic_only':True,'exactness_credit':'none',
        'target_bytes':SIZE,'candidate_bytes':len(raw),
        'target_instructions':len(ti),'candidate_instructions':len(ci),
        'original_fields':len(tf),'bound_fields':len(cf),
        'ordered_calls':len(tc),'ordered_calls_agree':tc==cc,
        'original_blocks':len(tg),'candidate_blocks':len(cg),
        'direct_graphs_agree':tg==cg,
        'aligned_field_identities':align['paired_field_identities'],
        'field_change_spans':len(align['changes']),
        'aligned_instructions':sum(q.size for q in matches.get_matching_blocks()),
        'overlap_differences':sum(a!=b for a,b in zip(linked,target)),
        'difference_offsets':[n for n,(a,b) in enumerate(zip(linked,target)) if a!=b],
        'missing_target_bytes':max(0,len(target)-len(linked)),
        'extra_candidate_bytes':max(0,len(linked)-len(target)),
        # Preserve the original publication order as an independent observation;
        # the canonical complete byte/field comparison grants exact credit.
        'original_limit_publication':limit_target_trace,
        'candidate_limit_publication':cleanup_limit_trace(ci),
        'mode2_lower_bound_branch_present':has_mode2_clamp,
        'original_limit_double_store':True,
        'complete_bytes_equal':linked==target,
        'target_sha256':TARGET_SHA,
        'source_raw_sha256':hashlib.sha256(raw).hexdigest(),
        'source_linked_sha256':hashlib.sha256(linked).hexdigest(),
    }


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('object',type=Path)
    args=p.parse_args()
    try:print(json.dumps(diagnose(args.object),indent=2));return 0
    except (ValueError,OSError,ImportError,KeyError,struct.error) as ex:
        print('Cleanup original-bound diagnostic rejected input:',ex,file=sys.stderr)
        return 2

if __name__=='__main__':
    raise SystemExit(main())
