"""Guard the C++ owner that complete code bytes cannot distinguish."""
import copy
import importlib.util
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'title_config_frontier', ROOT / 'scripts/check-boss-frontiers.py')
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)


class TitleConfigSnapshotOwner(unittest.TestCase):
    def units(self):
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            return tomllib.load(stream)['units']

    def test_real_root_owner_at_original_copy_operand(self):
        frontier.guard_title_config_snapshot_owner(self.units())

    def test_rejects_byte_equivalent_fake_independent_global(self):
        units = self.units()
        field = next(r for r in units['title-screen-start-menu']['relocations']
                     if r['offset'] == 0x47)
        # This fake target plus zero COFF addend encodes the very same
        # 0x004B3488 as the real root plus config388's 0x388 addend.
        field.update(symbol='?g_TitleConfigSnapshot@@3UTitleConfigSnapshotView@@A',
                     target=0x004B3488)
        with self.assertRaises(RuntimeError):
            frontier.guard_title_config_snapshot_owner(units)

    def test_rejects_wrong_original_extent_source_or_operand(self):
        original = self.units()
        for bad in ({'source': 'src/TitleScreenOptions.cpp'},
                    {'object': 'build/matching/Other.obj'},
                    {'symbol': '?FakeStartMenu@@YAHXZ'},
                    {'target_address': 0x00429E24}, {'size': 1559},
                    {'compare_size': 1591}):
            units = copy.deepcopy(original)
            units['title-screen-start-menu'].update(bad)
            with self.subTest(bad=bad), self.assertRaises(RuntimeError):
                frontier.guard_title_config_snapshot_owner(units)
        for bad in ({'offset': 0x43}, {'type': 'REL32'},
                    {'symbol': '?g_Supervisor@@3UTitleSupervisorView@@A'},
                    {'target': 0x004B3488}):
            units = copy.deepcopy(original)
            field = next(r for r in units['title-screen-start-menu']['relocations']
                         if r['offset'] == 0x47)
            field.update(bad)
            with self.subTest(bad=bad), self.assertRaises(RuntimeError):
                frontier.guard_title_config_snapshot_owner(units)

    def test_rejects_missing_or_duplicated_original_field(self):
        for duplicate in (False, True):
            units = self.units()
            rows = units['title-screen-start-menu']['relocations']
            field = next(r for r in rows if r['offset'] == 0x47)
            if duplicate:
                rows.append(dict(field))
            else:
                rows.remove(field)
            with self.subTest(duplicate=duplicate), self.assertRaises(RuntimeError):
                frontier.guard_title_config_snapshot_owner(units)
