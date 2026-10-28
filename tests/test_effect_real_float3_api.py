"""Effect draw relocation identities for the real Float3 API."""
from copy import deepcopy
from pathlib import Path
import tomllib
import unittest
ROOT=Path(__file__).resolve().parents[1]
ADD='??YFloat3@@QAEAAU0@ABU0@@Z'
SITES={'effect-manager-draw':(0x7E,183),'effect-manager-draw-shared':(0x66,146),'effect-draw-bullet-layer':(0x70,169)}
def validate(units):
 leaf=units['float3-add-assignment']
 assert (leaf['symbol'],leaf['target_address'],leaf['size'],leaf['source'])==(ADD,0x405730,36,'src/Float3.cpp')
 assert leaf.get('relocations',[])==[]
 for name,(offset,size) in SITES.items():
  u=units[name];assert u['size']==size
  sites=[r for r in u['relocations'] if r['target']==0x405730]
  assert len(sites)==1
  r=sites[0];assert(r['offset'],r['type'],r['symbol'],r['target'],r.get('addend',0))==(offset,'REL32',ADD,0x405730,0)
class EffectRealFloat3Api(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
 def test_actual_api_relocations(self):validate(self.units)
 def test_reject_wrong_boundary_field_and_symbol(self):
  for name in SITES:
   for key,bad in [('offset',0),('type','DIR32'),('symbol','?Add@EffectFloat3@@QAEPAU1@PBU1@@Z'),('target',0x405731),('addend',1)]:
    wrong=deepcopy(self.units);r=next(r for r in wrong[name]['relocations'] if r['target']==0x405730);r[key]=bad
    with self.subTest(unit=name,key=key),self.assertRaises(AssertionError):validate(wrong)
if __name__=='__main__':unittest.main()
