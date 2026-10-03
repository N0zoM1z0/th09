"""Read-only, non-crediting TH09 SupervisorServiceUpdate comparison.

Run from the repository root with a COFF object argument. Every relocation is
bound independently before comparing the complete nonexact owner. This does
not update match ledgers, promote an ABI or infer native product closure.
"""
import importlib.util,json,struct,sys,re,tomllib,hashlib
from pathlib import Path
import capstone
sp=importlib.util.spec_from_file_location('c','scripts/compare-coff-function.py');c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
units=tomllib.loads(Path('config/match-units.toml').read_text())['units'];known={}
for unit in units.values():
 known.setdefault(unit['symbol'],set()).add(unit['target_address'])
 for row in unit.get('relocations',[]):known.setdefault(row['symbol'],set()).add(row['target'])
# Target call identities and packet addresses reviewed from their consumers,
# not inferred by subtracting candidate relocation addends from target bytes.
reviewed={
 '?GetInput@Controller@@YIGH@Z':0x42BE40,
 '?DrawNetworkMessage@@YAHHHKHPBDZZ':0x43BE50,
 '?g_NetworkMessageManager@@3HA':0x4DC550,
 '?g_NetworkSearchingMessage@@3QBDB':0x490AC4,
 '?g_NetworkWaitingMessage@@3QBDB':0x490AA8,
 '?g_NetworkDisconnectedMessage@@3QBDB':0x490A88,
}
# Anonymous-namespace suffix depends on source path, so normalize only that.
anon={
 '?GetRandomU32InRange@ReplayRngNetworkView@ANON@@QAEII@Z':0x4048E0,
 '?GetSeed@ReplayRngNetworkView@ANON@@QAEGXZ':0x41A89E,
 '?g_ReplayRng@@3UReplayRngNetworkView@ANON@@A':0x4ACE0C,
 '?g_NetworkFramePacket@@3UNetworkFramePacket@ANON@@A':0x4B409C,
 '?g_NetworkHandshakePacket@@3UNetworkHandshakePacket@ANON@@A':0x4B40A8,
}
im=c.verified_target()
for va,hexbytes in [(0x490AC4,'90da91b189c2945c82c8976489f682f0925482b582c482a282dc82b700'),(0x490AA8,'976489f682cc90da91b182f091d282c182c482a282dc82b700'),(0x490A88,'976489f682a982e782cc92ca904d82aa937290e282a682c482a282dc82b700')]:
 expected=bytes.fromhex(hexbytes);assert c.pe_bytes_at(im,va,len(expected))==expected
base=0x430AA0;size=1633
raw,rows=c.object_function(Path(sys.argv[1]),'?SupervisorServiceUpdate@@YIHPAVSupervisor@@@Z');code=bytearray(raw);bindings=[];seen=set()
for row in rows:
 sym=row['symbol'];normal=re.sub(r'\?A0x[0-9a-fA-F]+','ANON',sym)
 if sym in reviewed:dest=reviewed[sym];authority='reviewed target identity'
 elif normal in anon:dest=anon[normal];authority='reviewed view/packet consumer'
 else:
  values=known.get(sym,set());assert len(values)==1,(sym,values);dest=next(iter(values));authority='canonical manifest'
 off=row['offset'];assert 0<=off<=len(code)-4 and not any(i in seen for i in range(off,off+4));seen.update(range(off,off+4));assert row['type'] in ('DIR32','REL32')
 value=dest+row['addend'];value-=base+off+4 if row['type']=='REL32' else 0
 struct.pack_into('<I',code,off,value&0xffffffff);bindings.append(dict(row,destination=hex(dest),authority=authority))
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
target=c.pe_bytes_at(im,base,size);ti=list(md.disasm(target,base));ci=list(md.disasm(code,base));assert sum(i.size for i in ti)==len(target) and sum(i.size for i in ci)==len(code)
def calls(ins):return [i.operands[0].imm for i in ins if i.mnemonic=='call' and i.operands[0].type==2]
tcalls=calls(ti);ccalls=calls(ci)
out=dict(object=sys.argv[1],raw_sha256=hashlib.sha256(raw).hexdigest(),candidate_bytes=len(code),target_bytes=size,candidate_instructions=len(ci),target_instructions=len(ti),complete_equal=code==target,overlapping_differences=sum(a!=b for a,b in zip(code,target)),relocations=len(rows),ordered_direct_calls_equal=tcalls==ccalls,target_direct_calls=[hex(x) for x in tcalls],candidate_direct_calls=[hex(x) for x in ccalls],bindings=bindings)
print(json.dumps(out,indent=2))
