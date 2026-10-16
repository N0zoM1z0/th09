"""No-target proof guards for the real TH09 Player movement Supervisor owner.

The 1,900-byte whole-owner original PE, 66 actual COFF fields, 18 internal
label targets, and canonical 522-byte exact caller are cold-replayed separately.
"""
import csv
import importlib.util
from pathlib import Path
import tomllib
from unittest import TestCase

ROOT = Path(__file__).resolve().parents[1]

spec = importlib.util.spec_from_file_location(
    'player_movement_true_supervisor', ROOT/'scripts/inspect-player-movement.py')
inspector = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inspector)

class PlayerMovementTrueSupervisor(TestCase):
    def test_real_supervisor_is_shared_and_not_independently_invented(self):
        source = (ROOT/'src/PlayerMovement.cpp').read_text()
        self.assertIn('#include "Supervisor.hpp"',source)
        self.assertIn('g_Supervisor.framerateMultiplier5B8',source)
        self.assertEqual(source.count('g_Supervisor.framerateMultiplier5B8'),2)
        self.assertNotIn('PlayerMovementSupervisorView',source)
        self.assertNotIn('g_PlayerMovementSupervisor',source)
        header = (ROOT/'src/Supervisor.hpp').read_text()
        self.assertIn('extern Supervisor g_Supervisor;', header)
        self.assertIn('float framerateMultiplier5B8;',header)
        self.assertIn('SupervisorFrameMultiplierAt5B8',header)

    def test_unchanged_original_target_global_and_two_operand_addresses(self):
        self.assertEqual(inspector.BASE,0x0041C170)
        self.assertEqual(inspector.PHYSICAL_SIZE,1900)
        self.assertEqual(inspector.CODE_SIZE,1835)
        self.assertEqual(inspector.DESTINATIONS['?g_Supervisor@@3VSupervisor@@A'],0x004B3100)
        self.assertNotIn('?g_PlayerMovementSupervisor@@3UPlayerMovementSupervisorView@@A',inspector.DESTINATIONS)
        self.assertEqual(0x004B3100 + 0x5B8,0x004B36B8)
        self.assertEqual(1900-1835,65)

    def test_same_real_supervisor_registered_and_exact_caller_independent(self):
        with (ROOT/'config/match-units.toml').open('rb') as f:
            units=tomllib.load(f)['units']
        owner=units['player-on-update']
        self.assertEqual(owner['size'],522)
        self.assertEqual(owner['source'],'src/PlayerRuntime.cpp')
        self.assertEqual(owner['target_address'],0x0041E900)
        with (ROOT/'config/functions.csv').open(newline='') as f:
            rows={int(x['address'],16):x for x in csv.DictReader(f)}
        self.assertEqual(rows[0x0041C170]['status'],'unclassified')
        self.assertEqual(rows[0x0041E900]['status'],'matching')
