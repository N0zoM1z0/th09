#!/usr/bin/env python3
"""Compare the two trial PEs with their selected code and generated data objects."""

from collections import defaultdict
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

from target_identity import numeric_resources, parse_pe


def load_script(name):
    path = Path(__file__).with_name(name + '.py')
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


coff = load_script('compare-coff-function')
trial = load_script('trial-link')
callbacks = load_script('report-callback-coverage')
exceptions = load_script('coff_exception_handlers')
MAP_SYMBOL = re.compile(r'^[ \t]+[0-9a-f]{4}:[0-9a-f]{8}[ \t]+(\S+)[ \t]+([0-9a-f]{8})[ \t]+',
                        re.MULTILINE | re.IGNORECASE)


def read_map(path):
    addresses = defaultdict(set)
    for name, address in MAP_SYMBOL.findall(path.read_text()):
        addresses[name].add(int(address, 16))
    return {name: values.pop() for name, values in addresses.items() if len(values) == 1}


def u32(data, offset):
    return struct.unpack_from('<I', data, offset)[0]


def verify_data_object(image, pe, path, addresses, resolve):
    data, sections, symbols = coff.read_coff_object(path)
    definitions = defaultdict(dict)
    for symbol in symbols.values():
        if symbol['storage'] == 2 and symbol['section'] > 0:
            definitions[symbol['section']][symbol['name']] = symbol['value']
    labels = {}
    field_count = 0
    for number, offsets in sorted(definitions.items()):
        section = sections[number - 1]
        name = section['name_raw'].rstrip(b'\0').decode()
        bases = {addresses[label] - offset for label, offset in offsets.items()}
        if len(bases) != 1:
            raise ValueError(f'{path.name} {name} labels do not share one section base')
        base = bases.pop()
        size = section['size']
        rva = base - pe['image_base']
        mapped = next((s for s in pe['sections']
                       if s['rva'] <= rva and rva + size <= s['rva'] + s['virtual_size']), None)
        if mapped is None:
            raise ValueError(f'{path.name} {name} extends outside the linked section')
        start = section['raw_offset']
        if not start:
            if section['reloc_count']:
                raise ValueError(f'{path.name} {name} has relocations in zero-initialized data')
            size = max(0, min(size, mapped['rva'] + mapped['raw_size'] - rva))
        expected = bytearray(data[start:start + size]) if start else bytearray(size)
        actual = coff.pe_bytes_at(image, base, size) if size else b''
        for ordinal in range(section['reloc_count']):
            offset, index, kind = struct.unpack_from('<IIH', data, section['reloc_offset'] + 10 * ordinal)
            symbol = symbols[index]['name']
            dest = resolve(symbol)
            if kind != 6 or dest is None:
                raise ValueError(f'{path.name} {name}+{offset:#x} has unbound relocation to {symbol}')
            value = (dest + u32(expected, offset)) & 0xFFFFFFFF
            if value != u32(actual, offset):
                raise ValueError(f'{path.name} {name}+{offset:#x} does not resolve to {symbol}')
            struct.pack_into('<I', expected, offset, value)
            field_count += 1
        if actual != expected:
            raise ValueError(f'{path.name} {name} differs from its linked bytes')
        labels.update({label: addresses[label] for label in offsets})
    if not labels or not field_count:
        raise ValueError(f'{path.name} is missing labelled storage or pointer fields')
    return labels, field_count


def verify_code(image, pe, units, objects, addresses, destinations):
    code_addresses = {}
    private_sections = {}
    handlers = []
    field_count = 0
    private_fields = 0

    def read_linked(address, size):
        return coff.pe_bytes_at(image, address, size)

    for name, unit in units.items():
        symbol = unit['symbol']
        address = addresses[symbol]
        path = objects[unit['source']]
        size = unit.get('compare_size', unit['size'])
        code, fields = coff.object_function(path, symbol, size, include_symbol_locations=True)
        linked = bytearray(read_linked(address, len(code)))
        expected_handlers = {field['symbol'] for field in unit.get('relocations', [])
                             if field['symbol'].startswith('__ehhandler$')}
        actual_handlers = {field['symbol'] for field in fields if field['symbol'].startswith('__ehhandler$')}
        if actual_handlers != expected_handlers:
            raise ValueError(f'{name} has different exception handler bindings')
        for field in fields:
            offset, dest_name, addend = field['offset'], field['symbol'], field['addend']
            section = field['symbol_section']
            value = field['symbol_value']
            if section == field['owner_section']:
                dest = address + value - field['owner_value']
            else:
                dest = destinations.get(dest_name)
            operand = u32(linked, offset)
            if dest is None and field['type'] == 'DIR32' and section > 0:
                # Static read-only constants can be absent from the linker map.
                # All references to their input section must agree on its base.
                base = (operand - addend - value) & 0xFFFFFFFF
                key = (path, section)
                if key not in private_sections:
                    data, sections, _ = coff.read_coff_object(path)
                    info = sections[section - 1]
                    start = info['raw_offset']
                    rva = base - pe['image_base']
                    in_readonly = any(s['name'] == '.rdata' and s['rva'] <= rva
                                      and rva + info['size'] <= s['rva'] + s['virtual_size']
                                      for s in pe['sections'])
                    if (info['name_raw'].rstrip(b'\0') != b'.rdata' or info['reloc_count']
                            or not in_readonly or coff.pe_bytes_at(image, base, info['size'])
                            != data[start:start + info['size']]):
                        raise ValueError(f'{name}+{offset:#x} points outside its private constants')
                    private_sections[key] = base
                if private_sections[key] != base:
                    raise ValueError(f'{name}+{offset:#x} disagrees on the private section base')
                dest = base + value
                private_fields += 1
            if dest is None or field['type'] not in ('DIR32', 'REL32'):
                raise ValueError(f'{name}+{offset:#x} has unbound relocation to {dest_name}')
            relative_base = address + offset + 4 if field['type'] == 'REL32' else 0
            if (dest + addend - relative_base) & 0xFFFFFFFF != operand:
                raise ValueError(f'{name}+{offset:#x} does not resolve to {dest_name}')
            if dest_name in expected_handlers:
                handlers.append(exceptions.verify(
                    coff.read_coff_object(path), field, dest, read_linked, destinations))
            linked[offset:offset + 4] = code[offset:offset + 4]
            field_count += 1
        if linked != code:
            raise ValueError(f'{name} differs from {path.name} outside relocations')
        code_addresses[name] = address
    return code_addresses, field_count, private_fields, handlers


def verify_data_owners(units, labels, object_sizes):
    bindings = defaultdict(dict)
    for unit in units.values():
        for field in unit.get('relocations', []):
            symbol = field['symbol']
            if 'target' in field and symbol in labels:
                bindings[field['target']][symbol] = labels[symbol]
    for owner, size in object_sizes.items():
        start = int(owner, 0)
        origins = set(bindings[start].values())
        if not origins:
            continue
        if len(origins) != 1:
            raise ValueError(f'generated object at {start:#010x} has separate base labels')
        base = origins.pop()
        for target, aliases in bindings.items():
            if start < target < start + size:
                for symbol, address in aliases.items():
                    if address != base + target - start:
                        raise ValueError(f'{symbol} is outside its owning offset in {start:#010x}')


def verify_input_formats(target, image, units, resolve, library_data):
    formats = {
        symbol: {field['target'] for field in units['supervisor-setup-dinput']['relocations']
                 if field['symbol'] == symbol}
        for symbol in library_data
    }
    total = 0
    for symbol, origins in formats.items():
        if len(origins) != 1:
            raise ValueError(f'{symbol} has ambiguous DirectInput format ownership')
        address = resolve(symbol)
        if address is None:
            raise ValueError(f'{symbol} has no linked DirectInput format')
        original = struct.unpack('<6I', coff.pe_bytes_at(target, origins.pop(), 24))
        linked = struct.unpack('<6I', coff.pe_bytes_at(image, address, 24))
        if original[:2] != (24, 16) or original[:5] != linked[:5]:
            raise ValueError(f'{symbol} has a different DirectInput format header')
        count = original[4]
        original_rows = coff.pe_bytes_at(target, original[5], count * 16)
        linked_rows = coff.pe_bytes_at(image, linked[5], count * 16)
        if len(original_rows) != count * 16 or len(linked_rows) != count * 16:
            raise ValueError(f'{symbol} has a truncated DirectInput object table')
        guid_bindings = {}
        for index, (first, second) in enumerate(zip(struct.iter_unpack('<4I', original_rows),
                                                    struct.iter_unpack('<4I', linked_rows))):
            first_guid, second_guid = first[0], second[0]
            if (first[1:] != second[1:] or bool(first_guid) != bool(second_guid)
                    or guid_bindings.setdefault(first_guid, second_guid) != second_guid
                    or (first_guid and coff.pe_bytes_at(target, first_guid, 16)
                        != coff.pe_bytes_at(image, second_guid, 16))):
                raise ValueError(f'{symbol} differs at DirectInput object {index}')
        total += count
    return total


def verify_callback_tables(target, image, units, resolve):
    code_symbols = {unit['target_address']: unit['symbol'] for unit in units.values()}
    executable = callbacks.executable_intervals(target)
    total = 0
    for name, (base, count, stride, first, roles, digest, factory, _) in callbacks.TABLES.items():
        owners = {field['symbol'] for field in units[factory]['relocations'] if field.get('target') == base}
        if len(owners) != 1:
            raise ValueError(f'{name} has ambiguous callback table ownership')
        address = resolve(owners.pop())
        if address is None:
            raise ValueError(f'{name} has no linked callback table')
        original = coff.pe_bytes_at(target, base, count * stride)
        slots = callbacks.decode_prefix(original, base, count, stride, first,
                                        roles, digest, executable)
        expected = bytearray(original)
        for slot in slots:
            dest = resolve(code_symbols[slot['destination']])
            if dest is None:
                raise ValueError(f"{name}[{slot['row']}] has no linked {slot['role']} callback")
            struct.pack_into('<I', expected, slot['pointer_address'] - base, dest)
        if coff.pe_bytes_at(image, address, len(expected)) != expected:
            raise ValueError(f'{name} linked callback table differs from the original rows')
        total += len(slots)
    return total


def verify_ecl_tables(target, image, units, resolve):
    code_symbols = {unit['target_address']: unit['symbol'] for unit in units.values()}
    tables = (
        ('ECL interpolation', coff.ECL_INTERPOLATION_TABLE, 0x004A0AF4, 8),
        ('ECL extra instruction', '?g_EnemyEclExInstructions@@3PAP6IXPAUEnemyView@@PAUEnemyEclExInstructionView@@@ZA',
         0x004A0B14, 12),
    )
    total = 0
    for name, symbol, base, count in tables:
        address = resolve(symbol)
        if address is None:
            raise ValueError(f'{name} has no linked callback table')
        original = coff.pe_bytes_at(target, base, count * 4)
        linked = coff.pe_bytes_at(image, address, count * 4)
        for index in range(count):
            callback = code_symbols[u32(original, index * 4)]
            destination = resolve(callback)
            if destination is None or u32(linked, index * 4) != destination:
                raise ValueError(f'{name}[{index}] linked callback differs from {callback}')
        total += count
    return total


def verify_crt_initializers(image, addresses):
    c_initializers = ('___onexitinit', '___initstdio', '___sse2_available_init', '___initmbctable',
                      '?__CxxSetUnhandledExceptionFilter@@YAHXZ')
    cxx_initializers = ('___security_init_cookie', '_$E1')
    for section, names in (('xi', c_initializers), ('xc', cxx_initializers)):
        expected = struct.pack(f'<{len(names) + 2}I', 0, *(addresses[name] for name in names), 0)
        start, end = addresses[f'___{section}_a'], addresses[f'___{section}_z']
        if end - start != len(expected) - 4 or coff.pe_bytes_at(image, start, len(expected)) != expected:
            raise ValueError(f'linked CRT {section} initializer table differs from the expected order')


def verify_resources(target, image):
    original = numeric_resources(target, lambda address, size: coff.pe_bytes_at(target, address, size))
    linked = numeric_resources(image, lambda address, size: coff.pe_bytes_at(image, address, size))
    if linked != original:
        raise ValueError('linked PE resource identities or payloads differ from the original')
    return len(linked)


def import_thunk_identity(image, address, imports):
    thunk = coff.pe_bytes_at(image, address, 6)
    if thunk[:2] == b'\xff\x25':
        return imports.get(u32(thunk, 2))
    return None


def verify_imports(target, image, units, addresses):
    original_imports = coff.target_iat(target)
    linked_imports = coff.target_iat(image)
    if sorted(linked_imports.values()) != sorted(original_imports.values()):
        raise ValueError('linked DLL and import identities differ from the original')

    fields = 0
    for name, unit in units.items():
        for field in unit.get('relocations', []):
            original = field.get('target')
            if original is None:
                continue
            relative = field['type'] == 'REL32'
            if relative:
                expected = import_thunk_identity(target, original, original_imports)
            else:
                expected = original_imports.get(original)
            if expected is None:
                continue
            offset = field['offset']
            operand = addresses[unit['symbol']] + offset
            value = u32(coff.pe_bytes_at(image, operand, 4), 0)
            if relative:
                entry = (operand + 4 + value) & 0xFFFFFFFF
                actual = import_thunk_identity(image, entry, linked_imports)
            else:
                actual = linked_imports.get(value)
            if actual != expected:
                raise ValueError(f'{name}+{offset:#x} imports {actual}, expected {expected}')
            fields += 1
    return len(linked_imports), fields


def verify_vtables(image, units, objects, addresses, resolve):
    owners = defaultdict(set)
    for unit in units.values():
        for field in unit.get('relocations', []):
            if field['symbol'].startswith('??_7'):
                owners[unit['source']].add(field['symbol'])

    verified = set()
    slots = 0
    for source, tables in owners.items():
        data, sections, symbols = coff.read_coff_object(objects[source])
        for symbol in symbols.values():
            name = symbol['name']
            if name not in tables or symbol['section'] <= 0:
                continue
            table = sections[symbol['section'] - 1]
            if (table['name_raw'].rstrip(b'\0') != b'.rdata' or symbol['value']
                    or table['size'] != 4 * table['reloc_count'] or not table['reloc_count']):
                raise ValueError(f'{name} has an unexpected VC7.1 vtable extent')
            start = table['raw_offset']
            expected = bytearray(data[start:start + table['size']])
            relocations = table['reloc_offset']
            for index in range(table['reloc_count']):
                offset, target, kind = struct.unpack_from('<IIH', data, relocations + index * 10)
                destination = resolve(symbols[target]['name'])
                if offset != index * 4 or kind != 6 or destination is None:
                    raise ValueError(f'{name} has an unbound vtable slot at +{offset:#x}')
                struct.pack_into('<I', expected, offset, (destination + u32(expected, offset)) & 0xFFFFFFFF)
            if coff.pe_bytes_at(image, addresses[name], len(expected)) != expected:
                raise ValueError(f'{name} differs from its linked virtual functions')
            if name not in verified:
                slots += table['reloc_count']
            verified.add(name)
    if verified != set().union(*owners.values()):
        raise ValueError('a tracked vtable has no selected COFF definition')
    return len(verified), slots


def verify_opponent_lists(target, image, units, labels, object_sizes):
    symbol = '?g_OpponentLists@@3PAPAUOpponentSelectionEntry@@A'
    fields = units['game-manager-select-opponent']['relocations']
    pointer_table, = {field['target'] for field in fields if field['symbol'] == symbol}
    extent = object_sizes[f'0x{pointer_table:08X}']
    original = coff.pe_bytes_at(target, pointer_table, extent)
    linked = coff.pe_bytes_at(image, labels[symbol], extent)
    for slot, ((source,), (destination,)) in enumerate(
            zip(struct.iter_unpack('<I', original), struct.iter_unpack('<I', linked), strict=True)):
        size = object_sizes[f'0x{source:08X}']
        expected = coff.pe_bytes_at(target, source, size)
        keys = [row[0] for row in struct.iter_unpack('<8h', expected)]
        if len(expected) != size or not keys or keys[-1] >= 0 or any(key < 0 for key in keys[:-1]):
            raise ValueError(f'opponent list {slot} has an invalid original extent')
        if (destination != labels.get(f'th09_data_{source:08X}')
                or coff.pe_bytes_at(image, destination, size) != expected):
            raise ValueError(f'opponent list {slot} differs from the attested {source:#010x} extent')


def verify_layout(stem, data_name, target, units, objects):
    image = (trial.OUT / f'{stem}.exe').read_bytes()
    pe = parse_pe(image)
    if pe['machine_id'] != 0x14C or pe['linker_version'] != '7.10':
        raise ValueError(f'{stem} is not a VC7.1 i386 link')
    addresses = read_map(trial.OUT / f'{stem}.map')
    verify_crt_initializers(image, addresses)
    resources = verify_resources(target, image)
    imports, import_fields = verify_imports(target, image, units, addresses)
    aliases = dict(line.removeprefix('/ALTERNATENAME:').split('=', 1)
                   for line in (trial.OUT / f'{stem}.rsp').read_text().splitlines()
                   if line.startswith('/ALTERNATENAME:'))

    destinations = {alias: addresses[real] for alias, real in aliases.items() if real in addresses} | addresses
    resolve = destinations.get

    data_path = trial.ROOT / 'build/product' / data_name
    labels, data_fields = verify_data_object(image, pe, data_path, addresses, resolve)
    product = tomllib.loads((trial.ROOT / 'config/product-link.toml').read_text())
    verify_data_owners(units, labels, product['object_size'])
    verify_opponent_lists(target, image, units, labels, product['object_size'])
    input_objects = verify_input_formats(target, image, units, resolve, product['library_data'])
    callback_slots = verify_callback_tables(target, image, units, resolve)
    ecl_slots = verify_ecl_tables(target, image, units, resolve)
    vtables, virtual_slots = verify_vtables(image, units, objects, addresses, resolve)
    code, code_fields, private_fields, handlers = verify_code(image, pe, units, objects, addresses, destinations)
    print(f'{stem}: {len(code)} code units, {code_fields} code relocations '
          f'({private_fields} into private constants), {len(labels)} data labels, '
          f'{data_fields} data relocations, {input_objects} DirectInput objects, '
          f'{callback_slots} Effect/ExAttack callback slots, {ecl_slots} ECL callback slots')
    print(f'{stem}: {len(handlers)} C++ exception handler references, '
          f'{sum(row["states"] for row in handlers)} unwind states, '
          f'{sum(row["relocations"] for row in handlers)} unwind relocations')
    print(f'{stem}: {resources} PE resources, {imports} PE imports, {import_fields} checked import operands, '
          f'{vtables} vtables, {virtual_slots} virtual slots')
    return code, labels


def main():
    target = coff.verified_target()
    index = json.loads((trial.OUT / 'index.json').read_text())
    objects = {entry['source']: trial.OUT / entry['object'] for entry in trial.selected_carriers(index)}
    units = trial.load_units()
    normal = verify_layout('trial', 'ProductData.obj', target, units, objects)
    reversed_layout = verify_layout('trial-reversed', 'ProductDataReversed.obj', target, units, objects)
    for role, first, second in zip(('code units', 'data labels'), normal, reversed_layout):
        if first.keys() != second.keys():
            raise ValueError(f'{role} differ between link layouts')
        moved = sum(address != second[name] for name, address in first.items())
        if not moved:
            raise ValueError(f'no {role} moved')
        print(f'{moved} of {len(first)} {role} moved')


if __name__ == '__main__':
    try:
        main()
    except (OSError, KeyError, ValueError, struct.error) as exc:
        sys.exit(f'trial layout verification failed: {exc}')
