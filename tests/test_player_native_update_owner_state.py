"""Exact fastcall state updater uses native PlayerOwnerState and side score.

Original VC7.1 PE/COFF whole-function comparator is the binary oracle.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]
SCORE='?AddScore@AsciiGameManagerSideView@@QAEXH@Z'

class PlayerNativeUpdateOwnerState(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT/'config/match-units.toml').open('rb') as file:
            cls.units=tomllib.load(file)['units']

    def test_one_native_0x44_byte_state_instead_of_runtime_shadow(self):
        core=(ROOT/'src/PlayerRuntime.cpp').read_text()
        header=(ROOT/'src/PlayerLifecycleView.hpp').read_text()
        self.assertNotIn('struct PlayerOwnerStateUpdateView',core)
        self.assertNotIn('struct PlayerOwnerStatePlayerView',core)
        self.assertNotIn('struct PlayerOwnerStateSideView',core)
        self.assertNotIn('AdjustOwnerMetric',core)
        self.assertIn('PlayerRuntimeNativeOwnerStateAt30410',core)
        self.assertIn('void __fastcall PlayerUpdateOwnerState(void *opaqueState)',core)
        self.assertIn('PlayerOwnerStateView *state = static_cast<PlayerOwnerStateView *>(opaqueState);',core)
        self.assertIn('state->playerOwner00->sideState->AddScore(state->maxValue10);',core)
        self.assertIn('state->maxValue10 = 0;',core)
        self.assertIn('state->value0C = 0;',core)
        self.assertIn('state->timer14 = 0;',core)
        self.assertIn('state->activeFrameCounter3C = 0;',core)
        self.assertIn('PlayerOwnerStateView ownerState30410;',header)
        self.assertIn('PlayerOwnerStateNativeFullMembers',header)

    def test_existing_exact_fastcall_126_bytes_and_real_addscore_call(self):
        u=self.units['player-update-owner-state']
        self.assertEqual((u['target_address'],u['size'],len(u['relocations'])),
                         (0x0041D760,126,9))
        self.assertEqual(u['symbol'],'?PlayerUpdateOwnerState@@YIXPAX@Z')
        calls=[r for r in u['relocations'] if r['symbol']==SCORE]
        self.assertEqual([(r['offset'],r['type'],r['target']) for r in calls],
                         [(0x68,'REL32',0x0040F860)])
        self.assertEqual(self.units['game-manager-side-add-score']['size'],33)
        self.assertEqual(self.units['player-owner-apply-reward']['compare_size'],1540)
        self.assertEqual(self.units['player-on-update']['size'],522)
        player=self.units['player-on-update']['relocations']
        self.assertTrue(any(r['symbol']=='?PlayerUpdateOwnerState@@YIXPAX@Z'
                            and r['target']==0x0041D760 for r in player))

if __name__=='__main__':unittest.main()
