#!/usr/bin/env python3
"""Original-bound Effect 34 byte/field/call/COM-argument diagnostics; no exact credit."""
from pathlib import Path
import hashlib,importlib.util,json,struct,sys,tomllib
ROOT=Path(__file__).resolve().parents[1]
s=importlib.util.spec_from_file_location('family_coff',ROOT/'scripts/compare-coff-function.py');c=importlib.util.module_from_spec(s);s.loader.exec_module(c)
OWNERS={'initialize':(0x444960,178,'?EffectInitializeCallbackType34@@YIHPAUEffect@@@Z','e1cd1a2867299092ed3e86ac43c03538760723bef50feda4d3b6150d9a150f40'),'draw':(0x444AF0,946,'?EffectDrawCallbackType34@@YIHPAUEffect@@@Z','d32652c2e08ffbd02a103e379724cde773716f964bbb962e9e4cbd12f58c88a2')}
def string_at_symbol(path,row):
 data=Path(path).read_bytes();machine,sections,_,_,_,opt,_=struct.unpack_from('<HHIIIHH',data);assert machine==0x14c and 1<=row['symbol_section']<=sections
 sec=20+opt+(row['symbol_section']-1)*40;length,pointer=struct.unpack_from('<II',data,sec+16);offset=row['symbol_value'];assert 0<=offset<length;start=pointer+offset;end=data.index(b'\0',start,pointer+length);return data[start:end+1]
def optional_pe_bytes(image,address,size):
 try:return c.pe_bytes_at(image,address,size)
 except ValueError:return None
def inspect(obj,role,*,owners=None):
 import capstone
 base,size,symbol,expected=(OWNERS if owners is None else owners)[role];image=c.verified_target();target=c.pe_bytes_at(image,base,size);assert hashlib.sha256(target).hexdigest()==expected
 cs=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);cs.detail=True;ti=list(cs.disasm(target,base));assert sum(i.size for i in ti)==size and ti[-1].mnemonic=='ret'
 original_fields=[];literal_registry={};absolute_addresses=set()
 for i in ti:
  if i.mnemonic=='call' and i.operands[0].type==capstone.x86.X86_OP_IMM:original_fields.append({'offset':i.address-base+i.imm_offset,'type':'REL32','destination':i.operands[0].imm})
  for op in i.operands:
   if op.type==capstone.x86.X86_OP_MEM and op.mem.base==0 and op.mem.index==0 and i.disp_size==4:
    original_fields.append({'offset':i.address-base+i.disp_offset,'type':'DIR32','destination':op.mem.disp});absolute_addresses.add(op.mem.disp)
    if i.mnemonic.startswith('f'):
     bits=struct.unpack('<I',c.pe_bytes_at(image,op.mem.disp,4))[0];literal_registry.setdefault('__real@%08x'%bits,set()).add(op.mem.disp)
   elif op.type==capstone.x86.X86_OP_IMM and not i.mnemonic.startswith(('j','call')) and i.imm_size==4 and 0x48D000<=op.imm<0x4E7000:
    original_fields.append({'offset':i.address-base+i.imm_offset,'type':'DIR32','destination':op.imm});absolute_addresses.add(op.imm)
 registry={};units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
 for u in units.values():
  for r in [{'symbol':u['symbol'],'target':u['target_address']}]+u.get('relocations',[]):registry.setdefault(r['symbol'],set()).add(r['target'])
 raw,fields=c.object_function(Path(obj),symbol,include_symbol_locations=True);linked=bytearray(raw);bindings=[]
 for r in fields:
  if r['symbol'].startswith('__real@'):values=literal_registry.get(r['symbol'],set());basis='original decoded FPU address and literal payload'
  elif r['symbol'].startswith(('$SG','??_C@')):
   payload=string_at_symbol(obj,r);values={a for a in absolute_addresses if optional_pe_bytes(image,a,len(payload))==payload};basis='actual COFF literal bytes matched to original decoded string address'
  elif r['symbol_section']==r['owner_section'] and r['symbol_section']>0:
   relative=r['symbol_value']-r['owner_value'];assert 0<=relative<len(raw);values={base+relative};basis='actual owner-local COFF label position'
  else:values=registry.get(r['symbol'],set());basis='pre-existing canonical C++ symbol identity'
  assert len(values)==1,(r['symbol'],values);destination=next(iter(values));effective=destination+r['addend'];value=effective
  if r['type']=='REL32':value-=base+r['offset']+4
  else:assert r['type']=='DIR32'
  struct.pack_into('<I',linked,r['offset'],value&0xffffffff);bindings.append({'offset':r['offset'],'type':r['type'],'symbol':r['symbol'],'target':destination,'addend':r['addend'],'effective_target':effective,'basis':basis})
 ci=list(cs.disasm(linked,base));assert sum(i.size for i in ci)==len(raw)
 def calls(ii):
  return [('direct',i.operands[0].imm) if i.operands[0].type==capstone.x86.X86_OP_IMM else ('indirect-slot',i.operands[0].mem.disp) for i in ii if i.mnemonic=='call']
 def com_args(ii):
  result=[];pushes=[]
  for i in ii:
   if i.mnemonic=='push':pushes.append(i.operands[0].imm if i.operands[0].type==capstone.x86.X86_OP_IMM else None)
   if i.mnemonic=='call':
    if i.operands[0].type==capstone.x86.X86_OP_MEM:
     slot=i.operands[0].mem.disp;n={0xfc:4,0xc8:3,0x130:2,0x120:5}[slot];result.append({'slot':slot,'reverse_stack_arguments':pushes[-n:]})
    pushes=[]
  return result
 def graph(ii):
  ends={i.address for i in ii};splits={base}
  for i in ii:
   if i.mnemonic.startswith(('j','ret')):
    if i.address+i.size in ends:splits.add(i.address+i.size)
    if i.mnemonic.startswith('j'):
     assert i.operands[0].type==capstone.x86.X86_OP_IMM and i.operands[0].imm in ends;splits.add(i.operands[0].imm)
  starts=sorted(splits);result=[]
  for n,start in enumerate(starts):
   end=starts[n+1] if n+1<len(starts) else ii[-1].address+ii[-1].size;tail=next(i for i in reversed(ii) if start<=i.address<end);out=[]
   if tail.mnemonic.startswith('j'):out.append(tail.operands[0].imm-base)
   if tail.mnemonic!='jmp' and not tail.mnemonic.startswith('ret') and tail.address+tail.size in ends:out.append(tail.address+tail.size-base)
   result.append((start-base,tail.mnemonic if tail.mnemonic.startswith(('j','ret')) else 'fallthrough',out))
  return result
 original_fields.sort(key=lambda r:r['offset']);actual=[{'offset':r['offset'],'type':r['type'],'destination':r['effective_target']} for r in bindings];diff=[n for n,(a,b) in enumerate(zip(target,linked)) if a!=b]
 report={'role':role,'diagnostic_only':True,'exactness_credit':'none; complete non-crediting original/COFF diagnostic','target_address':base,'target_bytes':size,'candidate_bytes':len(raw),'target_sha256':expected,'raw_sha256':hashlib.sha256(raw).hexdigest(),'resolved_sha256':hashlib.sha256(linked).hexdigest(),'target_instructions':len(ti),'candidate_instructions':len(ci),'fields':len(bindings),'bindings':bindings,'original_fields':original_fields,'all_original_fields_agree':actual==original_fields,'field_referent_order_agrees':[(r['type'],r['destination']) for r in actual]==[(r['type'],r['destination']) for r in original_fields],'target_calls':calls(ti),'candidate_calls':calls(ci),'ordered_calls_agree':calls(ti)==calls(ci),'target_blocks':len(graph(ti)),'candidate_blocks':len(graph(ci)),'direct_graphs_agree':graph(ti)==graph(ci),'target_COM_arguments':com_args(ti),'candidate_COM_arguments':com_args(ci),'COM_immediate_arguments_agree':com_args(ti)==com_args(ci),'overlapping_differences':len(diff),'difference_offsets':diff,'missing_bytes':max(0,size-len(raw)),'extra_bytes':max(0,len(raw)-size),'complete_bytes_agree':bytes(linked)==target}
 return report,ti,ci
def main():
 import argparse
 parser=argparse.ArgumentParser(description="Non-crediting original TH09 Effect 34 callback diagnostic")
 parser.add_argument('object',type=Path)
 parser.add_argument('--role',choices=['initialize','draw','all'],default='draw')
 parser.add_argument('--json',action='store_true')
 args=parser.parse_args()
 try:
  roles=list(OWNERS) if args.role=='all' else [args.role]
  rows=[inspect(args.object,role)[0] for role in roles]
 except (OSError,ValueError,AssertionError,KeyError,struct.error) as e:
  print(json.dumps({'result':'error','message':str(e)}));return 2
 result=rows if args.role=='all' else rows[0]
 if args.json:print(json.dumps(result,indent=2))
 else:
  for r in rows:
   print(r['role'],r['candidate_bytes'],'/',r['target_bytes'],'bytes;',r['fields'],'fields;',r['overlapping_differences'],'overlap differences;',r['extra_bytes'],'extra bytes; exact=',r['complete_bytes_agree'])
 return 0
if __name__=='__main__':raise SystemExit(main())
