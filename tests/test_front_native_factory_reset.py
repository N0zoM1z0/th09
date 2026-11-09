"""Native constructed Front owns the original 14-byte reset entry."""
from pathlib import Path
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = '?Reset@FrontFactoryConstructView@@QAEHXZ'


class NativeFrontResetOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with (ROOT / 'config/match-units.toml').open('rb') as fd:
            cls.units = tomllib.load(fd)['units']

    def test_actual_factory_object_not_shadow_byte_array(self):
        header = (ROOT / 'src/FrontFactoryConstructView.hpp').read_text()
        factory = (ROOT / 'src/FrontFactory.cpp').read_text()
        source = (ROOT / 'src/FrontSupport.cpp').read_text()
        self.assertIn('int Reset();', header)
        self.assertIn('int FrontFactoryConstructView::Reset()', source)
        self.assertIn('memset(this, 0, sizeof(*this));', source)
        self.assertIn('front->Reset();', factory)
        self.assertNotIn('reinterpret_cast<Front *>(front)', factory)
        self.assertFalse((ROOT / 'src/FrontSupport.hpp').exists())
        self.assertIn('sizeof(FrontFactoryConstructView) == 0x1293C', header)

    def test_original_call_identity_and_complete_function_boundaries(self):
        reset = self.units['front-reset']
        factory = self.units['front-factory']
        self.assertEqual((reset['symbol'], reset['target_address'], reset['size']),
                         (SYMBOL, 0x00415D40, 14))
        self.assertEqual(reset['relocations'], [])
        calls = [r for r in factory['relocations'] if r['offset'] == 0x61]
        self.assertEqual([(r['type'], r['symbol'], r['target']) for r in calls],
                         [('REL32', SYMBOL, 0x00415D40)])
        self.assertEqual((factory['target_address'], factory['size'],
                          len(factory['relocations'])),
                         (0x00418840, 220, 22))


if __name__ == '__main__':
    unittest.main()
