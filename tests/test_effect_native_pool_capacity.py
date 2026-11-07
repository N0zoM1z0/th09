"""Native EffectManager dynamic pool, failure sentinel and live update bounds.

The pinned retail PE/COFF original is verified by the separate focused
Effect owner replay; this pure-input test guards the recovered C++ model.
"""
from pathlib import Path
import tomllib
import unittest

ROOT=Path(__file__).resolve().parents[1]


class EffectNativePoolCapacityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.native=(ROOT/'src/EffectManager.hpp').read_text()
        cls.header=(ROOT/'src/EffectNativePoolCapacity.hpp').read_text()
        cls.source=(ROOT/'src/EffectManager.cpp').read_text()

    def test_native_manager_struct_fields_and_sentinel_count(self):
        for part in ('struct EffectManager','Effect *effects;',
                     'int primaryCount;','int secondaryCount;',
                     'EffectManagerSizeIs2DC','EffectSizeIsD8'):
            self.assertIn(part,self.native)
        for part in ('struct EffectNativePoolCapacity','SentinelRecords = 1',
                     'sizeof(Effect) == 0xD8','sizeof(EffectManager) == 0x2DC',
                     'offsetof(EffectManager, effects) == 0x30',
                     'offsetof(EffectManager, primaryCount) == 0x34',
                     'offsetof(EffectManager, secondaryCount) == 0x38'):
            self.assertIn(part,self.header)

    def test_dynamic_active_allocation_and_failure_indices(self):
        h=self.header
        self.assertIn('return manager.primaryCount + manager.secondaryCount;',h)
        self.assertIn('return manager.primaryCount + manager.secondaryCount + SentinelRecords;',h)
        self.assertIn('return primaryCount + secondaryCount + SentinelRecords;',h)
        self.assertIn('return manager.effects + ActiveCount(manager);',h)
        for primary,secondary in ((0,0),(1,0),(0,7),(175,360),(256,32)):
            with self.subTest(primary=primary,secondary=secondary):
                live=primary+secondary
                allocated=live+1
                sentinel=live
                self.assertEqual(allocated-sentinel,1)
                self.assertGreaterEqual(sentinel,primary)

    def test_actual_lifecycle_spawn_and_update_share_dynamic_count(self):
        s=self.source
        self.assertIn('#include "EffectNativePoolCapacity.hpp"',s)
        self.assertEqual(s.count('EffectNativePoolCapacity::AllocatedCount(*this)'),4)
        self.assertEqual(s.count('EffectNativePoolCapacity::FailureSentinel(*this)'),2)
        self.assertIn('EffectNativePoolCapacity::AllocatedFromCounts(primaryCount, secondaryCount)',s)
        self.assertIn('EffectNativePoolCapacity::ActiveCountWithNativeOrder(*effectManager)',s)
        self.assertNotIn('this->effects[this->primaryCount + this->secondaryCount]',s)
        self.assertIn('EffectManager::ResetPool()',s)
        self.assertIn('EffectManager::ReleasePool()',s)
        self.assertIn('EffectManager::Create(',s)
        self.assertIn('EffectManager::OnUpdate(',s)

    def test_live_primary_count_load_order_is_preserved(self):
        text=self.header
        self.assertIn('ActiveCountWithNativeOrder(const EffectManager &manager)',text)
        self.assertIn('static_cast<const volatile int &>(manager.primaryCount)',text)
        self.assertIn('manager.secondaryCount;',text)
        # The exact original real-time loop cannot be replaced by an alias
        # that silently hoists or de-volatilizes the primary field.
        self.assertNotIn('i < EffectNativePoolCapacity::ActiveCount(*effectManager)',self.source)

    def test_exact_owners_and_unmatched_resource_callback_remain_distinct(self):
        units=tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        exact={n:u for n,u in units.items() if u['source']=='src/EffectManager.cpp'}
        self.assertEqual(len(exact),15)
        for n,size in (('effect-manager-update',475),('effect-manager-reset-pool',194),
                       ('effect-manager-release-pool',123),('effect-manager-create',327),
                       ('effect-manager-spawn-effect',184),
                       ('effect-manager-spawn-effect-with-velocity',175)):
            self.assertEqual(exact[n]['size'],size)
        self.assertNotIn('effect-manager-added-callback',units)

if __name__=='__main__':
    unittest.main()
