"""Read-only full relocation diagnostic for the nonexact cleanup owner.

Run from the repository root with a COFF object argument. The four call-facing
views bind independently reviewed reset identities, not teardown aliases.
This diagnostic never updates acceptance ledgers or claims native closure.
"""
import importlib.util,struct,tomllib,json,sys,hashlib
from pathlib import Path
import capstone
sp=importlib.util.spec_from_file_location('c','scripts/compare-coff-function.py');c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
units=tomllib.loads(Path('config/match-units.toml').read_text())['units'];known={}
for v in units.values():
 known.setdefault(v['symbol'],set()).add(v['target_address'])
 for z in v.get('relocations',[]):known.setdefault(z['symbol'],set()).add(z['target'])
views={'?InitializeAddedState@PlayerLifecycleView@@QAEXXZ':0x41EBC0,'?InitializeMessageRuntime@SetupFrontMessageResetView@@QAEXXZ':0x4181E0,'?ClearSpellBackgroundState@SetupBackgroundResetView@@QAEXXZ':0x4011C0,'?RefreshResource@SetupCardAttackResetView@@QAEXXZ':0x403D80,'?ResetTransitionState@SetupFrontSideResetView@@QAEXXZ':0x41A230}
base=0x41B5C6;target=c.pe_bytes_at(c.verified_target(),base,425);raw,rows=c.object_function(Path(sys.argv[1]),'?CleanupGameplayState@GameManagerSetupLayout@@SIXXZ');code=bytearray(raw);out=[];covered=set()
for r in rows:
 sym=r['symbol']
 if sym in views:dest=views[sym]
 else:
  v=known.get(sym,set());assert len(v)==1,(sym,v);dest=next(iter(v))
 off=r['offset'];assert 0<=off<=len(code)-4 and not any(x in covered for x in range(off,off+4));covered.update(range(off,off+4));assert r['type'] in ['DIR32','REL32'];value=dest+r['addend']-(base+off+4 if r['type']=='REL32' else 0);struct.pack_into('<I',code,off,value&0xffffffff);out.append(dict(r,target=hex(dest)))
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
cs=list(md.disasm(code,base));ts=list(md.disasm(target,base));assert sum(i.size for i in cs)==len(code) and sum(i.size for i in ts)==len(target)
def calls(ins):return [hex(i.operands[0].imm) for i in ins if i.mnemonic=='call' and i.operands[0].type==2]
a=calls(cs);b=calls(ts);print(json.dumps(dict(candidate_bytes=len(code),target_bytes=len(target),raw_sha256=hashlib.sha256(raw).hexdigest(),fields=len(rows),complete_equal=code==target,ordered_calls_equal=a==b,candidate_calls=a,target_calls=b,bindings=out),indent=2))
