"""Original +0x04 resource presence shared by Etama and Effect callbacks.

The formerly separate 8-byte AnmLoadedResidentView is superseded by the
original-bound native 0x1C AnmLoaded class. The original Etama callback is
exact; the Effect callback remains an explicitly nonexact 51-byte owner.
"""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SharedEtamaResidentLayout(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.header = (ROOT / 'src/AnmManager.hpp').read_text()
        cls.etama = (ROOT / 'src/EtamaControllerDraw.cpp').read_text()
        cls.effect = (ROOT / 'src/EffectManager.cpp').read_text()
        with (ROOT / 'config/match-units.toml').open('rb') as file:
            cls.units = tomllib.load(file)['units']

    def test_actual_resource_owner_replaces_false_resident_prefix(self):
        self.assertIn('class AnmLoaded', self.header)
        self.assertIn('AnmRawEntry *rawData04;',self.header)
        self.assertIn('AnmLoadedNativeSizeIs1C',self.header)
        slot = (ROOT/'src/EtamaAnmResourceSlot.hpp').read_text()
        self.assertIn('ANM_FILE_SLOT_ETAMA = 8',slot)
        self.assertIn('enum EtamaAnmResourceSlot',slot)
        self.assertNotIn('struct AnmLoadedResidentView',self.header)
        self.assertFalse((ROOT/'src/AnmLoadedResidentView.hpp').exists())

    def test_etama_and_effect_read_the_same_real_native_offset(self):
        for label,source,expression in (
            ('Etama',self.etama,'anm->rawData04 == NULL'),
            ('Effect',self.effect,'effectAnm->rawData04 == NULL'),
        ):
            with self.subTest(label=label):
                self.assertNotIn('#include "AnmLoadedResidentView.hpp"',source)
                self.assertIn(expression,source)
                self.assertIn('->PreloadAnm(ANM_FILE_SLOT_ETAMA,',source)
                self.assertIn('->GetAnm(ANM_FILE_SLOT_ETAMA)',source)
                self.assertIn('#include "EtamaAnmResourceSlot.hpp"',source)
                self.assertIn('"etama.anm"',source)
        self.assertNotIn('EtamaAnmLoadedStateView',self.etama)
        self.assertNotIn('reinterpret_cast<void **>(effectAnm)[1]',self.effect)
        self.assertIn('controller->bulletAnm =',self.etama)
        self.assertIn('manager->effectAnm = effectAnm;',self.effect)

    def test_etama_startup_and_shutdown_use_the_same_native_slot(self):
        lifecycle = (ROOT/'src/BulletEtamaLifecycle.cpp').read_text()
        self.assertIn('g_AnmManager->PreloadAnm(ANM_FILE_SLOT_ETAMA, "etama.anm")',
                      lifecycle)
        self.assertIn('g_AnmManager->ReleaseAnm(ANM_FILE_SLOT_ETAMA)', lifecycle)
        for source in (self.etama,self.effect,lifecycle):
            self.assertNotIn('GetAnm(8)',source)
            self.assertNotIn('PreloadAnm(8,',source)
            self.assertNotIn('ReleaseAnm(8)',source)

    def test_original_callers_retain_real_addresses_and_sizes(self):
        target={
            'bullet-draw-single':(0x00413AF0,232,'src/EtamaControllerDraw.cpp'),
            'etama-controller-added-callback':(0x00413E40,832,'src/EtamaControllerDraw.cpp'),
            'effect-manager-update':(0x0040CDD0,475,'src/EffectManager.cpp'),
        }
        for name,(address,size,source) in target.items():
            with self.subTest(name=name):
                spec=self.units[name]
                self.assertEqual(spec['source'],source)
                self.assertEqual(spec['target_address'],address)
                self.assertEqual(spec.get('compare_size',spec['size']),size)
        # The original Effect AddedCallback remains an authored nonexact boss,
        # not an additional exact unit in the canonical match manifest.
        self.assertNotIn('effect-manager-added-callback',self.units)

    def test_compiler_private_label_destinations_are_frozen(self):
        target={
            'bullet-draw-single':{
                0x017:0x00413BC8,
                0x0D8:0x00413B0B,0x0DC:0x00413B13,
                0x0E0:0x00413B1B,0x0E4:0x00413B23,
            },
            'etama-controller-added-callback':{
                0x17F:0x004140A4,0x186:0x00414098,
                0x1EB:0x0041411C,0x1F2:0x00414110,
                0x258:0x00413FCA,0x25C:0x00413FDB,
                0x260:0x00413FFC,0x2D0:0x00414036,
                0x2D4:0x00414044,0x2D8:0x00414052,
            },
        }
        for name,expected in target.items():
            with self.subTest(name=name):
                actual={r['offset']:r for r in self.units[name]['relocations']
                        if r['symbol'].startswith('$L')}
                self.assertEqual(set(actual),set(expected))
                for offset,va in expected.items():
                    self.assertEqual(actual[offset]['target'],va)
                    self.assertEqual(actual[offset]['type'],'DIR32')


if __name__=='__main__':
    unittest.main()
