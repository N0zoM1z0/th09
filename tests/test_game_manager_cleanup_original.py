"""Independent original-identity and real-COFF input gates for TH09 Cleanup."""
import hashlib
import importlib.util
from pathlib import Path
import struct
import unittest

try:
    import capstone
except ImportError:
    capstone=None

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('cleanup_checked', ROOT/'scripts/inspect-game-manager-cleanup.py')
mod=importlib.util.module_from_spec(spec)
spec.loader.exec_module(mod)


class CleanupOriginalIdentityTests(unittest.TestCase):
    @staticmethod
    def rows():
        key='?g_GameManager@@3UGameManagerSetupLayout@@A'
        return [dict(offset=2+10*i, type='DIR32',symbol=key,addend=0)
                for i in range(41)]

    def test_original_sha_is_noncrediting_independent_from_source_fields(self):
        self.assertEqual(mod.SIZE,425)
        self.assertEqual(mod.TARGET_SHA,'e31f3c176b3b735008612b07823982a646248cbae4fb8ac826e2f2cff2d6a798')
        self.assertEqual(mod.SOURCES['?ResetGameManager@@YIXPAUGameManagerSetupLayout@@@Z'],0x41A7C3)
        self.assertEqual(mod.SOURCES['?InitializeMessageRuntime@SetupFrontMessageResetView@@QAEXXZ'],0x4181E0)
        self.assertEqual(mod.SOURCES['?ResetTransitionState@SetupFrontSideResetView@@QAEXXZ'],0x41A230)
        # Fabricated original image identity may never be accepted, even when
        # the size is correct or the source happens to have 41 COFF fields.
        with self.assertRaisesRegex(ValueError,'SHA256'):
            mod.original_attest(bytes(425))
        with self.assertRaisesRegex(ValueError,'SHA256'):
            mod.original_attest(bytes(424))

    def test_original_limit_publication_order_is_not_a_final_value_assumption(self):
        trace={'default_reads':[0x169], 'all_writes':[0x0A2,0x179,0x180],
               'normal_clamp_branch':[0x17E]}
        self.assertEqual(mod.validate_original_limit_trace(trace),trace)
        # A single post-clamp store or a swap of the two memory publications
        # is not the original, even if the eventual numeric result is equal.
        for broken in (
                {**trace,'all_writes':[0x0A2,0x180]},
                {**trace,'all_writes':[0x0A2,0x180,0x179]},
                {**trace,'all_writes':[0x0A2,0x179,0x181]},
                {**trace,'normal_clamp_branch':[0x174]},
                {**trace,'default_reads':[0x0C8]},
                {**trace,'all_writes':[0x0A2,0x179,0x180,0x185]},
        ):
            with self.subTest(broken=broken),self.assertRaisesRegex(
                    ValueError,'publication / clamp order'):
                mod.validate_original_limit_trace(broken)

    def test_linked_candidate_uses_actual_symbol_addends(self):
        rows=self.rows()
        bound, actual=mod.link_coff(bytes(406),rows)
        self.assertEqual(len(actual),41)
        self.assertEqual(len(bound),406)
        self.assertEqual(struct.unpack_from('<I',bound,2)[0],0x4A7D90)
        self.assertEqual(actual[-1][0],402)
        rows[0]=dict(rows[0],addend=0x100)
        bound,actual=mod.link_coff(bytes(406),rows)
        self.assertEqual(struct.unpack_from('<I',bound,2)[0],0x4A7E90)
        self.assertEqual(actual[0][-1],0x4A7E90)

    def test_real_second_preclamp_publication_is_representable_in_coff(self):
        rows = self.rows()
        rows.append(dict(rows[0], offset=407))
        linked, bound = mod.link_coff(bytes(411), rows)
        self.assertEqual(len(bound), 42)
        self.assertEqual(bound[-1][0], 407)
        self.assertEqual(struct.unpack_from('<I',linked,407)[0], 0x4A7D90)
        with self.assertRaisesRegex(ValueError, 'overlap'):
            mod.link_coff(bytes(411), rows + [dict(rows[-1], offset=407)])
        with self.assertRaisesRegex(ValueError, 'overlap'):
            mod.link_coff(bytes(411), rows[:-1] + [dict(rows[-1], offset=402)])

    def test_missing_wrong_kind_unknown_referent_and_overlap_fail_closed(self):
        rows=self.rows()
        cases=[rows[:-1],rows+[rows[-1]],
               [dict(rows[0],type='BASEREL32')]+rows[1:],
               [dict(rows[0],symbol='?invented@@3HA')]+rows[1:],
               rows[:1]+[dict(rows[1],offset=3)]+rows[2:],
               rows[:-1]+[dict(rows[-1],offset=404)]]
        for trial in cases:
            with self.subTest(change=trial[:2]),self.assertRaises(ValueError):
                mod.link_coff(bytes(406),trial)
        for size in (359,471):
            with self.assertRaises(ValueError):
                mod.link_coff(bytes(size),rows)

    @unittest.skipIf(capstone is None,'Capstone unavailable under python -S')
    def test_decoder_return_abi_and_branch_operand_census(self):
        self.assertEqual(len(mod.complete_decode(b'\xc3')),1)
        with self.assertRaises(ValueError):
            mod.complete_decode(b'\xc2\x04\x00')
        # This unconditional branch is a control destination, not absolute
        # external data. The extra MOV carries the only DIR32 field.
        code=bytes.fromhex('a1907d4a00e900000000c3')
        ins=mod.complete_decode(code)
        fields,calls=mod.physical_fields(ins)
        self.assertEqual(fields,[(1,'DIR32',0x4A7D90)])
        self.assertEqual(calls,[])
        with self.assertRaises(ValueError):
            mod.physical_fields(mod.complete_decode(bytes.fromhex('ffd0c3')))


if __name__=='__main__':
    unittest.main()
