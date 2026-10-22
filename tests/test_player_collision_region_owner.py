"""Guard the shared cancellation-region producer/consumer source connection.

Target-independent source tests do not grant exactness. Full original-bound
caller and canonical sibling replays remain separate requirements.
"""
from pathlib import Path
import re
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
REGION = 'PlayerCollisionRegionCreateRegionView'
OWNER = 'PlayerCollisionRegionCreateView'


def require(condition, message):
    if not condition:
        raise ValueError(message)


def struct_layout(source, name, known):
    """Read the limited ordinary scalar/array declarations in these two views.

    Use target i386 widths, never host pointer size; unknown declarations fail.
    This checks the actual maintained field order instead of trusting suffixes.
    """
    match = re.search(r'\bstruct\s+' + name + r'\s*\{(.*?)^\};', source, re.S | re.M)
    require(match is not None, 'missing shared structure ' + name)
    body = re.sub(r'//[^\n]*|/\*.*?\*/', '', match.group(1), flags=re.S)
    fields = {}
    size = 0
    alignment = 1
    for declaration in body.split(';'):
        declaration = ' '.join(declaration.split())
        if not declaration or '(' in declaration:
            continue  # Declared methods contribute no storage.
        member = re.fullmatch(
            r'(unsigned char|float|int|PlayerCollisionRegionCreateRegionView)'
            r'\s+(\*)?\s*(\w+)(?:\[(0x[0-9A-Fa-f]+|\d+)\])?', declaration)
        require(member is not None, 'unreviewed structure field: ' + declaration)
        kind, pointer, field, count = member.groups()
        width, align = (4, 4) if pointer else known[kind]
        size = (size + align - 1) // align * align
        require(field not in fields, 'duplicate structure field')
        fields[field] = (size, kind + ('*' if pointer else ''), int(count, 0) if count else 1)
        size += width * (int(count, 0) if count else 1)
        alignment = max(alignment, align)
    return fields, ((size + alignment - 1) // alignment * alignment, alignment)


def validate_connection(consumer, header, producer, units):
    require('#include "PlayerCollisionRegionCreate.hpp"' in consumer, 'missing producer header')
    require(not re.search(r'\bPlayerCancel(?:RegionView|RuntimeView)\b', consumer),
            'duplicated private region/storage overlay')
    body = consumer.split('int PlayerLifecycleView::CheckBulletCollision(', 1)[1]
    body = body.split('\nint PlayerLifecycleView::CheckGrazeCollision(', 1)[0]
    require(re.search(REGION + r'\s*\*\*slot\s*=\s*reinterpret_cast<' + OWNER +
                      r'\s*\*>\(player\)->activeB100;', body) is not None,
            'consumer must scan the actual producer pointer array')
    scalar = {'unsigned char': (1, 1), 'float': (4, 4), 'int': (4, 4)}
    fields, shape = struct_layout(header, REGION, scalar)
    require(shape == (0x44, 4), 'region physical size/alignment')
    expected = {
        'centerX00': (0x00, 'float'), 'centerY04': (0x04, 'float'),
        'radius08': (0x08, 'float'), 'halfWidth10': (0x10, 'float'),
        'halfHeight14': (0x14, 'float'), 'angle20': (0x20, 'float'),
        'hitCount2C': (0x2C, 'int'), 'type38': (0x38, 'int'),
        'delay40': (0x40, 'int'),
    }
    consumed = set(re.findall(r'\(\*slot\)->(\w+)', body))
    require(consumed == set(expected), 'unreviewed or missing consumed region fields')
    for field, (offset, kind) in expected.items():
        require(fields.get(field) == (offset, kind, 1), 'wrong region field ' + field)
    owner, owner_shape = struct_layout(header, OWNER, dict(scalar, **{REGION: shape}))
    require(owner['activeB100'] == (0xB100, REGION + '*', 514), 'wrong active pointer pool')
    require(owner['embeddedB0BC'] == (0xB0BC, REGION, 1), 'wrong fallback record')
    require(owner['releasedB908'] == (0xB908, REGION + '*', 514), 'wrong released pointer pool')
    require(owner['activeCountC110'] == (0xC110, 'int', 1), 'wrong active count')
    require(owner['releasedCountC114'] == (0xC114, 'int', 1), 'wrong released count')
    allocation = producer.split(OWNER + '::AllocateCollisionRegion(', 1)[1]
    allocation = allocation.split(OWNER + '::CreateRectType1(', 1)[0]
    normalized = re.sub(r'\s+', '', allocation)
    for expression in (
        'region=&embeddedB0BC;', 'region=releasedB908[releasedCountC114];',
        'activeB100[activeCountC110]=region;', '++activeCountC110;',
        'region->Reset();', 'region->centerX00=center->x;',
        'region->centerY04=center->y;', 'region->delay40=delay;', 'returnregion;'):
        require(expression in normalized, 'producer connection changed: ' + expression)
    unit = units['player-allocate-collision-region']
    require((unit['source'], unit['target_address'], unit['size']) ==
            ('src/PlayerCollisionRegionCreate.cpp', 0x0041CF30, 110), 'allocator owner changed')
    require(unit['relocations'] == [dict(offset=0x46, type='REL32',
        symbol='?Reset@PlayerCollisionRegionCreateRegionView@@QAEHXZ', target=0x0041BDA0)],
        'actual allocator Reset callee changed')


class PlayerCollisionRegionOwner(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.consumer = (ROOT / 'src/PlayerCollisionHelpers.cpp').read_text()
        cls.header = (ROOT / 'src/PlayerCollisionRegionCreate.hpp').read_text()
        cls.producer = (ROOT / 'src/PlayerCollisionRegionCreate.cpp').read_text()
        cls.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']

    def check(self, consumer=None, header=None, producer=None):
        validate_connection(self.consumer if consumer is None else consumer,
                            self.header if header is None else header,
                            self.producer if producer is None else producer, self.units)

    def test_current_shared_producer_consumer_connection(self):
        self.check()

    def test_reject_old_private_overlay(self):
        for old in ('PlayerCancelRegionView', 'PlayerCancelRuntimeView'):
            with self.subTest(old=old), self.assertRaisesRegex(ValueError, 'private'):
                self.check(consumer=self.consumer + '\nstruct ' + old + ' {};\n')

    def test_reject_wrong_pool_offset_or_capacity(self):
        for old, new in (('0xB0BC', '0xB0B8'), ('*activeB100[514]', '*activeB100[513]')):
            with self.subTest(old=old), self.assertRaises(ValueError):
                self.check(header=self.header.replace(old, new, 1))

    def test_reject_shifted_or_wrongly_typed_consumed_field(self):
        for old, new in (('float radiusGrowth0C;', 'unsigned char radiusGrowth0C[8];'),
                         ('int hitCount2C;', 'float hitCount2C;')):
            with self.subTest(old=old), self.assertRaises(ValueError):
                self.check(header=self.header.replace(old, new, 1))

    def test_reject_producer_publishing_to_other_pool(self):
        with self.assertRaisesRegex(ValueError, 'producer connection'):
            self.check(producer=self.producer.replace('activeB100[activeCountC110] = region;',
                                                     'releasedB908[activeCountC110] = region;', 1))

    def test_reject_consumer_scanning_other_pool(self):
        with self.assertRaisesRegex(ValueError, 'actual producer pointer array'):
            self.check(consumer=self.consumer.replace('->activeB100;', '->releasedB908;', 1))


if __name__ == '__main__':
    unittest.main()
