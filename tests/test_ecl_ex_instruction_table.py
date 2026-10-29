"""Reject incomplete tables and equal-address substitutes for native callbacks."""
import copy
import importlib.util
from pathlib import Path
import struct
import unittest

ROOT = Path(__file__).resolve().parents[1]
def load(name,file):
    spec=importlib.util.spec_from_file_location(name,ROOT/'scripts'/file)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module
m=load('ex_table_test','inspect-ecl-ex-instruction-table.py')
a=load('ex_table_caller_test','audit-ecl-callsite-identities.py')

def fixture():
    units={name:dict(target_address=address,symbol=m.callback_symbol(i)) for i,(address,name) in enumerate(m.SLOTS)}
    fields=[dict(offset=4*i,type_id=6,symbol=m.callback_symbol(i),addend=0,
                 symbol_section=0,symbol_value=0,symbol_type=0x20,storage=2) for i in range(12)]
    original=struct.pack('<12I',*(address for address,_ in m.SLOTS))
    return bytes(48),fields,units,original

def caller():
    return [dict(target_address=address,symbol=a.EX_INSTRUCTION_TABLE_SYMBOL,addend=0,
                 expected_value=0x4A0B14,target_value=0x4A0B14,identity_agrees=True,
                 operand_role_agrees=True,instruction_mnemonics=[mnemonic,mnemonic],
                 candidate_memory_width=4,target_memory_width=4,
                 candidate_memory_access=1,target_memory_access=1,
                 provenance={'native_ecl_callback_table_review':'separate actual COFF'})
            for address,mnemonic in ((0x40B1BD,'call'),(0x40B1D9,'call'),(0x40B21C,'mov'))]

class EclNativeCallbackTable(unittest.TestCase):
    def test_twelve_slots_preserve_bounded_prefix_and_unknown_original_extent(self):
        result=m.validate_binding(*fixture())
        self.assertTrue(result['complete_reviewed_prefix_equal'])
        self.assertEqual(result['reviewed_bytes'],48)
        self.assertTrue(result['original_array_extent'].startswith('unknown'))
        self.assertTrue(result['exactness_credit'].startswith('none'))
        self.assertEqual(a.EX_INSTRUCTION_TABLE_SYMBOL,m.SYMBOL)

    def test_same_numeric_target_cannot_substitute_a_member_or_void_pointer_signature(self):
        payload,fields,units,original=fixture()
        for symbol in ('?SpawnExAttackType1@EnemyExAttackView@@QAEXXZ',
                       '?EnemyEclOpcode2@@YIXPAUEnemyView@@PAX@Z',m.callback_symbol(1)):
            changed=copy.deepcopy(fields);changed[0]['symbol']=symbol
            with self.subTest(symbol=symbol),self.assertRaises(ValueError):m.validate_binding(payload,changed,units,original)
        units[m.SLOTS[0][1]]['symbol']='?SpawnExAttackType1@EnemyExAttackView@@QAEXXZ'
        with self.assertRaisesRegex(ValueError,'canonical'):m.validate_binding(payload,fields,units,original)

    def test_missing_duplicate_shifted_or_nonpointer_fields_fail(self):
        payload,fields,units,original=fixture()
        cases=[fields[:-1],fields+fields[:1]]
        for change in (dict(offset=4),dict(type_id=20),dict(addend=1),dict(addend=False),
                       dict(storage=3),dict(symbol_type=0),dict(symbol_value=4)):
            cases.append([{**fields[0],**change},*fields[1:]])
        for changed in cases:
            with self.subTest(changed=changed[0]),self.assertRaises(ValueError):m.validate_binding(payload,changed,units,original)
        for changed in (payload[:-1],b'\x01'+payload[1:]):
            with self.assertRaises(ValueError):m.validate_binding(changed,fields,units,original)
        with self.assertRaises(ValueError):m.validate_binding(payload,fields,units,original[4:]+original[:4])

    def test_truncated_or_wrong_machine_coff_cannot_supply_data(self):
        for data in (b'',bytes(20),struct.pack('<HHIIIHH',0x14c,1,0,60,0,0,0)+bytes(40)):
            with self.subTest(size=len(data)),self.assertRaises(ValueError):m.read_definition(data,object())

    def test_original_two_calls_and_installation_use_same_real_table(self):
        result=a.validate_ex_instruction_table_fields(caller())
        self.assertEqual((result['indexed_calls'],result['installation_loads']),(2,1))
        for change in (dict(symbol='?g_ExInstructionCallbacks@Th09EclRunState@@3PAP6IXPAUEnemyView@@PAUTh09EclRawInstructionHeaderView@@@ZA'),
                       dict(addend=4),dict(candidate_memory_width=2),dict(candidate_memory_access=2),
                       dict(provenance={'independent_global_review':'equal address'})):
            rows=caller();rows[0].update(change)
            with self.subTest(change=change),self.assertRaises(ValueError):a.validate_ex_instruction_table_fields(rows)
        rows=caller()
        for changed in (rows[:-1],rows+rows[:1],[{**rows[0],'target_address':0x40B1BC},*rows[1:]]):
            with self.assertRaises(ValueError):a.validate_ex_instruction_table_fields(changed)
