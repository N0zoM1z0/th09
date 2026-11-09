# The BossMarker VMs are native AsciiManager members

The original TH09 v1.50a BossMarker position and interrupt routines at
0x4067F0 and 0x4067D0 address four `AnmVm` records at
`AsciiManager + 0x17C4`, with each VM's `pos` at +0x208. Those records now
live in `AsciiManager::bossMarkerVms17C4[4]`, replacing a separate prefix
class. The four associated state words at +0x2254 are also exposed on the
same owner, while their existing setter can be migrated independently.

Both native member bodies compile exact: position **42/42 bytes, zero
relocations**; interrupt **32/32 bytes, one relocation** to the original
`AnmVm::SetInterrupt` at 0x406790. The three RunEcl original PE REL32
operands (+0x2504/+0x2543/+0x2573) and Enemy OnUpdate +0xDFC were checked
against unchanged 0x4067D0/0x4067F0 targets. All three RunEcl calls and the
Enemy call now use ordinary AsciiManager methods, with no shadow cast.

The complete 15,564-byte RunEcl retains 598 bound fields and its prior
30 ordinary unlike bytes; Enemy OnUpdate retains 3,900 bytes, 97 fields
and 13 unlike bytes. The 77 exact ECL siblings (24,368 bytes/1,096 fields)
remain exact, as do the affected ASCII Reset, Draw, Popup and OnUpdate owners.
The ECL callsite and owner tests pass. No full cold replay or additional
canonical exact credit is claimed.
