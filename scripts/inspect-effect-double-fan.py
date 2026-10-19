#!/usr/bin/env python3
"""Complete original-bound Effect26/28 diagnostics; canonical comparisons grant credit."""
from pathlib import Path
import importlib.util,json
ROOT=Path(__file__).resolve().parents[1]
s=importlib.util.spec_from_file_location('double_fan_audit',ROOT/'scripts/inspect-effect-type34.py')
a=importlib.util.module_from_spec(s);s.loader.exec_module(a)
OWNERS={'update26': [4471344, 209, '?EffectUpdateCallbackType26@@YIHPAUEffect@@@Z', 'e22152173d39a96b45457ee3d79faec4d4e53a8afa309dfad53cc3509419f7a4'], 'draw26': [4474624, 602, '?EffectDrawCallbackType26@@YIHPAUEffect@@@Z', 'fed154e98c83676846b71c9e6eaf498548b571cc01542be565e0ea36e67eab59'], 'update28': [4486592, 91, '?EffectUpdateCallbackType28@@YIHPAUEffect@@@Z', '92ef3faf939622fa756982f4ca0f9e76a949f353183c3200bb777c0e03c0a63b'], 'draw28': [4488416, 602, '?EffectDrawCallbackType28@@YIHPAUEffect@@@Z', '8022aab33bae86908522ea4ba117832f3077d88bc91d8814b96585210c93f85d']}
def inspect(obj,role):
 import capstone
 r,original,candidate=a.inspect(obj,role,owners=OWNERS);indexed=[]
 for i in original:
  for op in i.operands:
   if op.type==capstone.x86.X86_OP_MEM and i.disp_size==4 and (op.mem.base or op.mem.index) and 0x400000<=op.mem.disp<0x4e7000:indexed.append(dict(offset=i.address-OWNERS[role][0]+i.disp_offset,type='DIR32',destination=op.mem.disp))
 shape={i.address-OWNERS[role][0]:(i.mnemonic,i.op_str) for i in original}
 if role=='update26':
  assert shape[0x2f]==('imul','ecx, ecx, 0x38') and shape[0x32]==('mov','edx, dword ptr [ecx + 0x4a7d94]') and shape[0x38]==('add','edx, 0x1b88')
  assert indexed==[dict(offset=0x34,type='DIR32',destination=0x4a7d94)]
  assert shape[0x52]==('mov','eax, dword ptr [eax + 0x10]') and shape[0x55]==('mov','ecx, dword ptr [eax + 4]') and shape[0x58]==('mov','edx, dword ptr [ecx + 0x1cd8]')
 elif role=='update28':
  assert shape[0x28]==('imul','edx, edx, 0x38') and shape[0x2b]==('mov','eax, dword ptr [edx + 0x4a7d94]') and shape[0x31]==('add','eax, 0x1b88')
  assert indexed==[dict(offset=0x2d,type='DIR32',destination=0x4a7d94)]
 else:assert indexed==[]
 fields=sorted(r['original_fields']+indexed,key=lambda r:r['offset']);assert len({f['offset'] for f in fields})==len(fields);actual=[dict(offset=b['offset'],type=b['type'],destination=b['effective_target']) for b in r['bindings']]
 r.update(original_fields=fields,all_original_fields_agree=actual==fields,field_referent_order_agrees=[(f['type'],f['destination']) for f in fields]==[(f['type'],f['destination']) for f in actual],indexed_field_basis='Independently decoded original side stride0x38/static player04 operand and position1B88; Type26 also decodes EffectManager sideState10/player04/movementAngle1CD8. Candidate rows never supply original fields.')
 return r

def main():
 import argparse
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('object',type=Path)
 parser.add_argument('--role',choices=list(OWNERS)+['all'],default='update26')
 parser.add_argument('--json',action='store_true')
 args=parser.parse_args()
 try:
  roles=list(OWNERS) if args.role=='all' else [args.role]
  rows={role:inspect(args.object,role) for role in roles}
 except (OSError,ValueError,AssertionError,KeyError) as error:
  print(json.dumps({'result':'error','message':str(error)}));return 2
 result=rows if args.role=='all' else rows[args.role]
 if args.json:print(json.dumps(result,indent=2))
 else:
  for role,r in rows.items():print(role,r['candidate_bytes'],'/',r['target_bytes'],'bytes;',r['fields'],'fields;',r['overlapping_differences'],'differences')
 return 0
if __name__=='__main__':raise SystemExit(main())
