"""Player movement reads the true 0x390-byte GameManager coordinate owner.

An independent already-exact GameManager coordinate initializer sets the same
four values; original whole boss bytes and all 66 bound fields are replayed in
focused cold audits, not accepted by these pure-input assertions.
"""
import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('player_game_bounds_audit',ROOT/'scripts/inspect-player-movement.py')
inspector=importlib.util.module_from_spec(spec);spec.loader.exec_module(inspector)

class RealPlayerGameBounds(TestCase):
    def test_gamemanager_four_playfield_bounds_are_actual_fields(self):
        cpp=(ROOT/'src/PlayerMovement.cpp').read_text()
        header=(ROOT/'src/GameManagerResetView.hpp').read_text()
        self.assertIn('#include "GameManagerResetView.hpp"',cpp)
        self.assertIn('extern GameManagerResetView g_GameManager;',cpp)
        self.assertIn('GameManagerResetSize390',header)
        for field,offset,uses in (
            ('coordinate360',0x360,4),('coordinate364',0x364,4),
            ('coordinate368',0x368,2),('coordinate36C',0x36C,2)):
            with self.subTest(field=field):
                self.assertEqual(cpp.count('g_GameManager.'+field),uses)
                self.assertIn(field+';',header)
                self.assertIn('== 0x%X'%offset,cpp)
        for legacy in ('g_PlayerPlayfieldMinX','g_PlayerPlayfieldMinY',
                       'g_PlayerPlayfieldWidth','g_PlayerPlayfieldHeight'):
            self.assertNotIn(legacy,cpp)

    def test_same_original_gamemanager_global_for_all_four_real_boundaries(self):
        self.assertEqual(inspector.DESTINATIONS['?g_GameManager@@3UGameManagerResetView@@A'],
                         0x004A7D90)
        for offset,address in (
            (0x360,0x004A80F0),(0x364,0x004A80F4),
            (0x368,0x004A80F8),(0x36C,0x004A80FC)):
            self.assertEqual(inspector.DESTINATIONS['?g_GameManager@@3UGameManagerResetView@@A'] + offset,address)
        self.assertEqual(inspector.PHYSICAL_SIZE,1900)
        self.assertEqual(inspector.CODE_SIZE,1835)
        for fake in ('?g_PlayerPlayfieldMinX@@3MA','?g_PlayerPlayfieldMinY@@3MA',
                     '?g_PlayerPlayfieldWidth@@3MA','?g_PlayerPlayfieldHeight@@3MA'):
            self.assertNotIn(fake,inspector.DESTINATIONS)

    def test_exact_coordinate_initializer_writes_same_original_field_values(self):
        with (ROOT/'config/match-units.toml').open('rb') as stream:
            units=tomllib.load(stream)['units']
        x=units['game-manager-initialize-coordinate-bounds']
        self.assertEqual(x['target_address'],0x0041A9B8)
        self.assertEqual(x['size'],79)
        self.assertEqual(x['source'],'src/GameManagerCoordinateBounds.cpp')
        body=(ROOT/x['source']).read_text()
        for field,value in (('coordinate360','-136.0f'),('coordinate364','16.0f'),
                            ('coordinate368','272.0f'),('coordinate36C','416.0f')):
            self.assertIn(field+' = '+value+';',body)
        with (ROOT/'config/functions.csv').open(newline='') as f:
            rows={int(r['address'],16):r for r in csv.DictReader(f)}
        self.assertEqual(rows[0x0041A9B8]['status'],'matching')
        self.assertEqual(rows[0x0041E900]['status'],'matching')
        self.assertEqual(rows[0x0041C170]['status'],'unclassified')
