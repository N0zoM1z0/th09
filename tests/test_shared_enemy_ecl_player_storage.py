"""TH09 source-side guard for the native Enemy/ECL Player and side views.

Full source-cold original/COFF comparison is separately performed by the
focused runecl and enemy owner checks.  These tests prevent reintroducing
conflicting private storage classes or numerically retargeted Etama calls.
"""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class NativeEnemyEclPlayerStorage(unittest.TestCase):
    def test_one_player_view_shared_by_enemy_and_ecl(self):
        player = (ROOT/'src/EnemyCorePlayerView.hpp').read_text()
        enemy = (ROOT/'src/EnemyManagerCore.inl').read_text()
        state = (ROOT/'src/EclRunState.inl').read_text()
        late = (ROOT/'src/EclRunLate.inl').read_text()

        self.assertIn('struct EnemyCorePlayerView\n{', player)
        self.assertNotIn('struct EnemyCorePlayerView\n{', enemy)
        self.assertIn('#include "EnemyCorePlayerView.hpp"', state)
        self.assertIn('typedef ::EnemyCorePlayerView PlayerLateView;', late)
        self.assertNotIn('struct PlayerLateView\n{', late)

        for member, offset in (
            ('focusEffect364', '0x364'),
            ('position1B88', '0x1B88'),
            ('tracking30364', '0x30364'),
            ('stateTimer303C8', '0x303C8'),
            ('tableValue303F4', '0x303F4'),
        ):
            with self.subTest(member=member):
                self.assertIn(member, player)
                self.assertIn(offset, player)

        self.assertIn('EnemyCorePlayerHomingAt3037C', player)
        self.assertIn('EnemyCorePlayerSideTableAt303F4', player)

    def test_late_ecl_consumes_native_side_etama_and_owner(self):
        state = (ROOT/'src/EclRunState.inl').read_text()
        late = (ROOT/'src/EclRunLate.inl').read_text()

        self.assertIn('typedef ::EnemyCoreSideStateView SideStateView;', late)
        self.assertNotIn('struct SideStateView\n{', late)
        self.assertIn('EnemyCorePlayerView *player04;', state)
        self.assertIn('EtamaController *etama08;', state)
        self.assertIn('EnemyCoreSideOwnerView *owner14;', state)
        self.assertIn('return Manager(enemy)->sideState320->etama08;', late)
        self.assertIn('(offsetof(SideStateView, etama08) == 0x08)', late)
        self.assertIn('(offsetof(SideStateView, owner14) == 0x14)', late)
        self.assertIn('player04->tableValue303F4;', late)

    def test_late_ecl_manager_uses_one_typed_owner_with_two_native_fields(self):
        state = (ROOT/'src/EclRunState.inl').read_text()
        late = (ROOT/'src/EclRunLate.inl').read_text()
        enemy = (ROOT/'src/EnemyManagerCore.inl').read_text()

        self.assertIn('typedef ::EnemyManagerCoreView ManagerLateView;', late)
        self.assertNotIn('struct ManagerLateView\n{', late)
        self.assertIn('return View(enemy)->manager0000;', late)
        self.assertIn('int protocolValue2AC3DC;', state)
        self.assertIn('int timelineSpawnsSuppressed2AC440;', state)
        self.assertIn('unknown2AC3D5[0x2AC3DC - 0x2AC3D5]', state)
        self.assertIn('unknown2AC420[0x20]', state)
        self.assertIn('EnemyManagerCoreLateProtocolAt2AC3DC', enemy)
        self.assertIn('EnemyManagerCoreTimelineAt2AC440', enemy)
        self.assertIn('offsetof(ManagerLateView, protocolValue2AC3DC) == 0x2AC3DC', late)
        self.assertIn('offsetof(ManagerLateView, timelineSpawnsSuppressed2AC440) == 0x2AC440', late)

    def test_new_header_is_only_a_bounded_observed_storage_view(self):
        player = (ROOT/'src/EnemyCorePlayerView.hpp').read_text()
        self.assertIn('unknown00368[0x1B88 - 0x368]', player)
        self.assertIn('unknown01B94[0x30364 - 0x1B94]', player)
        self.assertIn('unknown303D4[0x303F4 - 0x303D4]', player)
        self.assertIn('sizeof(PlayerEnemyTrackingStateView)', (
            ROOT/'src/PlayerEnemyTrackingStateView.hpp').read_text())
        self.assertNotIn('struct PlayerLateView\n{',
                         (ROOT/'src/EclRunLate.inl').read_text())


if __name__ == '__main__':
    unittest.main()
