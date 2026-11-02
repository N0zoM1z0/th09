"""TH09 source-side guard for the native Enemy/ECL Player and side views.

Full source-cold original/COFF comparison is separately performed by the
focused runecl and enemy owner checks.  These tests prevent reintroducing
conflicting private storage classes or numerically retargeted Etama calls.
"""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class NativeEnemyEclPlayerStorage(unittest.TestCase):
    def test_one_player_field_layout_shared_by_enemy_and_ecl(self):
        native = (ROOT/'src/EnemyManager.hpp').read_text()
        player = (ROOT/'src/EnemyCorePlayerView.hpp').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        late = (ROOT/'src/EclRunLate.inl').read_text()
        self.assertIn('typedef EnemyRewardPlayerView EnemyCorePlayerView;', player)
        self.assertNotIn('struct EnemyCorePlayerView\n{', player)
        self.assertIn('#include "EnemyCorePlayerView.hpp"', state)
        self.assertIn('typedef ::EnemyCorePlayerView PlayerLateView;', late)
        for name, offset in (('focusEffect364','0x364'),
                             ('position1B88','0x1B88'),
                             ('tracking30364','0x30364'),
                             ('stateTimer303C8','0x303C8'),
                             ('tableValue303F4','0x303F4')):
            with self.subTest(name=name):
                self.assertIn(name, native)
                self.assertIn(offset, native)
        self.assertIn('EnemyRewardPlayerHomingAt3037C', native)
        self.assertIn('EnemyCorePlayerHomingAt3037C', player)
        self.assertIn('Th09EclLateSharedPlayerWindow', late)

    def test_run_ecl_random_movement_uses_shared_native_player_position(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        movement=(ROOT/'src/EclRunMovement.inl').read_text()
        ecl=(ROOT/'src/EclManager.cpp').read_text()
        self.assertIn('EnemyRewardPlayerPositionAt1B88',native)
        self.assertIn('EnemyRewardPlayerView *Player(EnemyView *enemy)',movement)
        self.assertIn('EnemyRewardPlayerView *player = Player(enemy);',ecl)
        self.assertGreaterEqual(ecl.count('Th09EclRunMovement::Player(enemy)'),5)
        self.assertNotIn('struct BoundaryMovePlayerView',ecl)
        self.assertNotIn('Th09EclRunMovement::BoundaryMovePlayerView',ecl)
        self.assertIn('player->position1B88.x',ecl)

    def test_side_state_has_one_native_etama_owner(self):
        state = (ROOT/'src/EclRunState.inl').read_text()
        late = (ROOT/'src/EclRunLate.inl').read_text()
        native = (ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('typedef EnemySideStateView EnemyCoreSideStateView;', state)
        self.assertNotIn('struct EnemyCoreSideStateView\n{', state)
        self.assertIn('typedef ::EnemyCoreSideStateView SideStateView;', late)
        for member in ('Background *background00;', 'EtamaController *etama08;',
                       'EnemyCoreSideOwnerView *owner14;',
                       'unsigned int flags34;'):
            self.assertIn(member, native)
        self.assertIn('return Manager(enemy)->sideState320->etama08;', late)
        self.assertIn('Th09EclLateSharedNativeSideLayout', late)

    def test_ecl_manager_protocol_fields_share_native_offsets(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        state=(ROOT/'src/EclRunState.inl').read_text()
        late=(ROOT/'src/EclRunLate.inl').read_text()
        self.assertIn('typedef ::EnemyManagerView ManagerLateView;', late)
        for member in ('protocolValue2AC3DC','timelineSpawnsSuppressed2AC440'):
            self.assertIn(member, native)
            self.assertIn(member, state)
        self.assertIn('Th09EclLateSharedNativeManagerFields', late)
        self.assertIn('EnemyManagerProtocolAt2AC3DC', native)
        self.assertIn('EnemyManagerTimelineAt2AC440', native)

    def test_late_run_ecl_opcodes_use_native_enemy_and_manager(self):
        late=(ROOT/'src/EclRunLate.inl').read_text()
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertNotIn('Th09EclRunLate::View(enemy)->',late)
        self.assertNotIn('inline Th09EclRunState::EnemyStateView *View(',late)
        self.assertIn('typedef ::EnemyManagerView ManagerLateView;',late)
        self.assertIn('return enemy->manager00;',late)
        self.assertIn('return Manager(enemy)->sideState320->etama08;',late)
        self.assertIn('&enemy->primaryAnmVm0008)->rotationZ08;',late)
        self.assertIn('sizeof(ManagerLateView) == 0x2AC450',late)
        for field in ('damageReductionTimer53A8','worldPosition2DD4',
                      'position2D74','primaryFlags337C',
                      'phaseStartingLife2E50','secondaryFlags3380'):
            with self.subTest(field=field):
                self.assertIn('enemy->'+field,late)
                self.assertIn(field,native)

    def test_reuse_retains_bounded_fields_not_a_claim_of_full_player_identity(self):
        native=(ROOT/'src/EnemyManager.hpp').read_text()
        self.assertIn('unknown00368[0x1B88 - 0x368]', native)
        self.assertIn('unknown1B94[0x30364 - 0x1B94]', native)
        self.assertIn('unknown303D4[0x303F4 - 0x303D4]', native)
        self.assertIn('sizeof(PlayerEnemyTrackingStateView)',
                      (ROOT/'src/PlayerEnemyTrackingStateView.hpp').read_text())
        self.assertNotIn('struct PlayerLateView\n{',
                         (ROOT/'src/EclRunLate.inl').read_text())


if __name__ == '__main__':
    unittest.main()
