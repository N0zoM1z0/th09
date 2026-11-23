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

from target_identity import parse_pe


def load_script(name):
    path = Path(__file__).with_name(name + '.py')
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


coff = load_script('compare-coff-function')
trial = load_script('trial-link')
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


def verify_code(image, pe, units, objects, addresses, resolve):
    code_addresses = {}
    private_sections = {}
    field_count = 0
    private_fields = 0
    for name, unit in units.items():
        symbol = unit['symbol']
        address = addresses[symbol]
        path = objects[unit['source']]
        size = unit.get('compare_size', unit['size'])
        code, fields = coff.object_function(path, symbol, size, include_symbol_locations=True)
        linked = bytearray(coff.pe_bytes_at(image, address, len(code)))
        for field in fields:
            offset, dest_name, addend = field['offset'], field['symbol'], field['addend']
            section = field['symbol_section']
            value = field['symbol_value']
            if section == field['owner_section']:
                dest = address + value - field['owner_value']
            else:
                dest = resolve(dest_name)
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
            linked[offset:offset + 4] = code[offset:offset + 4]
            field_count += 1
        if linked != code:
            raise ValueError(f'{name} differs from {path.name} outside relocations')
        code_addresses[name] = address
    return code_addresses, field_count, private_fields


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


def verify_layout(stem, data_name, target, units, objects):
    image = (trial.OUT / f'{stem}.exe').read_bytes()
    pe = parse_pe(image)
    if pe['machine_id'] != 0x14C or pe['linker_version'] != '7.10':
        raise ValueError(f'{stem} is not a VC7.1 i386 link')
    addresses = read_map(trial.OUT / f'{stem}.map')
    aliases = dict(line.removeprefix('/ALTERNATENAME:').split('=', 1)
                   for line in (trial.OUT / f'{stem}.rsp').read_text().splitlines()
                   if line.startswith('/ALTERNATENAME:'))

    def resolve(symbol):
        return addresses.get(symbol, addresses.get(aliases.get(symbol)))

    data_path = trial.ROOT / 'build/product' / data_name
    labels, data_fields = verify_data_object(image, pe, data_path, addresses, resolve)
    product = tomllib.loads((trial.ROOT / 'config/product-link.toml').read_text())
    verify_data_owners(units, labels, product['object_size'])
    input_objects = verify_input_formats(target, image, units, resolve, product['library_data'])
    code, code_fields, private_fields = verify_code(image, pe, units, objects, addresses, resolve)
    print(f'{stem}: {len(code)} code units, {code_fields} code relocations '
          f'({private_fields} into private constants), {len(labels)} data labels, '
          f'{data_fields} data relocations, {input_objects} DirectInput objects')
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
