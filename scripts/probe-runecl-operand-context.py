#!/usr/bin/env python3
"""Probe current RunEcl operand expressions with the pinned full-owner Oracle.

Diagnostic only. A rejected layout never grants exact credit. Both qualified
and unqualified calls are transformed; only reviewed stable arguments may be
repeated by expression expansion. The maintained game source is untouched.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import re
import sys
ROOT = Path(__file__).resolve().parents[1]
CASES = ('operand-macros', 'operand-macros-plain-jump',
         'operand-macros-native-timeout', 'operand-macros-native-plain')
expressions={
 'ReadInt': ('TH09_ECL_READ_INT','(((instruction)->parameterMask0A & (1U << (index))) ? Th09EclRunControl::ResolveInt((enemy), Th09EclRunControl::RawInt((instruction), (index))) : Th09EclRunControl::RawInt((instruction), (index)))'),
 'ReadFloat': ('TH09_ECL_READ_FLOAT','(((instruction)->parameterMask0A & (1U << (index))) ? (enemy)->ResolveFloat(Th09EclRunControl::RawFloat((instruction), (index))) : Th09EclRunControl::RawFloat((instruction), (index)))'),
 'WriteInt': ('TH09_ECL_WRITE_INT','(Th09EclRunControl::ResolveIntLValue((enemy), &Th09EclRunControl::RawInt((instruction), (index)), (instruction)->parameterMask0A, (index)))'),
 'WriteFloat': ('TH09_ECL_WRITE_FLOAT','(Th09EclRunControl::ResolveFloatLValue((enemy), &Th09EclRunControl::RawFloat((instruction), (index)), (instruction)->parameterMask0A, (index)))'),
}
def render(case, source):
 counts={}
 original=source['EclRunControl.inl'].decode().replace('\r\n','\n')
 begin=original.index('__forceinline int ReadInt(')
 end=original.index('\n} // namespace Th09EclRunControl',begin)
 if hashlib.sha256(original[begin:end].encode()).hexdigest() != '6914bdf8d4fae611da73a3ce0380fd28e3361fbf108ddd7d817b9630abb7de2f':
  raise ValueError('reviewed operand wrapper bodies changed')
 changed={}
 for name in ('EclManager.cpp','EclRunLate.inl','EclRunState.inl','EclRunBullet.inl','EclRunRemote.inl','EclRunMovement.inl','EclRunControl.inl'):
  text=source[name].decode().replace('\r\n','\n')
  for function,(macro,expression) in expressions.items():
   pattern=r'(?<![\w:])(?:Th09EclRunControl::)?'+function+r'\(\s*enemy,\s*instruction,\s*([0-9]+)\s*\)'
   indices=re.findall(pattern,text)
   assert all(0<=int(i)<=7 for i in indices)
   counts[function]=counts.get(function,0)+len(indices)
   text=re.sub(pattern,lambda m: macro+'(enemy, instruction, '+m.group(1)+')',text)
   if name=='EclRunControl.inl':
    pattern=r'__forceinline [^\n]*\b'+function+r'\([\s\S]*?\n}\n'
    text,count=re.subn(pattern,'#define '+macro+'(enemy, instruction, index) '+expression+'\n',text)
    assert count==1,(name,function,count)
  changed[name]=text.encode()
 if case in ('operand-macros-plain-jump','operand-macros-native-plain'):
  name='EclRunControl.inl';text=changed[name].decode()
  old='int value;\n        memcpy(&value, &Th09EclRunControl::RawInt(instruction, 0), sizeof(value));'
  assert text.count(old)==1
  changed[name]=text.replace(old,'int value = Th09EclRunControl::RawInt(instruction, 0);').encode()
 if case in ('operand-macros-native-timeout','operand-macros-native-plain'):
  name='EclRunState.inl';text=changed[name].decode()
  old='state.primaryFlags337C = Th09EclRunState::AssignTimeoutSpellBit(state.primaryFlags337C, value);';assert text.count(old)==1
  changed[name]=text.replace(old,'state.primaryFlagBits337C.timeoutSpell24 = value;').encode()
 for data in changed.values():
  if re.search(r'(?<![\w:])(?:Th09EclRunControl::)?(?:ReadInt|ReadFloat|WriteInt|WriteFloat)\s*\(', data.decode()):
   raise ValueError('unreviewed operand call remains')
 assert counts == {'ReadInt':157, 'ReadFloat':116, 'WriteInt':16, 'WriteFloat':25}, counts
 return changed

def main():
 parser=argparse.ArgumentParser(description=__doc__)
 parser.add_argument('--case', choices=CASES, required=True)
 parser.add_argument('--output', type=Path, required=True)
 args=parser.parse_args()
 args.output=args.output.resolve()
 analysis=(ROOT/'.analysis').resolve()
 if not args.output.is_relative_to(analysis) or args.output==analysis:
  parser.error('output must be below this repository .analysis directory')
 args.output.mkdir(parents=True, exist_ok=True)
 report_path=args.output/(args.case+'.json')
 if report_path.is_symlink():
  parser.error('refusing symlinked report')
 report_path.write_text(json.dumps(dict(state='incomplete', diagnostic_only=True,
                                      exactness_credit='none', case=args.case))+'\n')
 sys.path.insert(0,str(ROOT/'scripts'))
 spec=importlib.util.spec_from_file_location('operand_raw_context',ROOT/'scripts/probe-runecl-raw-context.py')
 provider=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(provider)
 provider.render=render
 try:
  report=provider.run(args.case,args.output)
 except (OSError,ValueError,KeyError,RuntimeError) as error:
  path=args.output/(args.case+'.json')
  if not path.is_file():
   raise
  report=json.loads(path.read_text())
  report.update(state='rejected', diagnostic_error=str(error))
 report['operand_recipe_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 (args.output/(args.case+'.json')).write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'state':report['state'], 'exactness_credit':'none',
                   'case':args.case, 'report':str(args.output/(args.case+'.json'))}))
 return 0 if report['state']=='complete-diagnostic' else 2

if __name__=='__main__':
 raise SystemExit(main())
