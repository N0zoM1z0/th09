#!/usr/bin/env python3
"""Compare the two trial PEs with their selected code and generated data objects."""

import argparse
from bisect import bisect_right
from collections import defaultdict
from functools import cache
import importlib.util
import json
from pathlib import Path, PureWindowsPath
import re
import struct
import sys
import tomllib

from coff_archive import archive_members
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
frontier_check = load_script('check-frontier')
MAP_SYMBOL = re.compile(r'^[ \t]+[0-9a-f]{4}:[0-9a-f]{8}[ \t]+(\S+)[ \t]+([0-9a-f]{8})[ \t]+',
                        re.MULTILINE | re.IGNORECASE)


def read_map(path):
    addresses = defaultdict(set)
    for name, address in MAP_SYMBOL.findall(path.read_text()):
        addresses[name].add(int(address, 16))
    return {name: values.pop() for name, values in addresses.items() if len(values) == 1}


def u32(data, offset):
    return struct.unpack_from('<I', data, offset)[0]


def original_symbol_addresses(units):
    candidates = defaultdict(set)
    for unit in units.values():
        candidates[unit['symbol']].add(unit['target_address'])
        for field in unit.get('relocations', []):
            if 'target' in field:
                candidates[field['symbol']].add(field['target'])
    return {name: addresses.pop() for name, addresses in candidates.items() if len(addresses) == 1}


def resolve_aliases(aliases, addresses, originals, library_data):
    for alias, real in aliases.items():
        if real not in addresses:
            raise ValueError(f'{alias} has no linked alias provider {real}')
        if alias in library_data:
            if real != library_data[alias]:
                raise ValueError(f'{alias} has the wrong library alias provider {real}')
        elif alias not in originals or originals[alias] != originals.get(real):
            raise ValueError(f'{alias} and alias provider {real} do not share a retail address')
    return {alias: addresses[real] for alias, real in aliases.items()} | addresses


def data_allocation(pe, address, size, flags, owner):
    rva = address - pe['image_base']
    section = next((s for s in pe['sections']
                    if s['rva'] <= rva and rva + size <= s['rva'] + s['virtual_size']), None)
    if section is None:
        raise ValueError(f'{owner} extends outside the linked section')
    if (section['characteristics'] ^ flags) & 0xE0000000:  # read, write and execute flags
        raise ValueError(f'{owner} does not preserve its COFF section access flags')
    return section


def verify_data_object(target, image, pe, path, addresses, resolve, originals, object_sizes):
    data, sections, symbols = coff.read_coff_object(path)
    originals = originals | {
        name: int(name.rsplit('_', 1)[-1], 16)
        for name in (symbol['name'] for symbol in symbols.values())
        if name.startswith(('th09_data_', 'th09_rdata_'))
    }
    definitions = defaultdict(dict)
    for symbol in symbols.values():
        if symbol['storage'] == 2 and symbol['section'] > 0:
            definitions[symbol['section']][symbol['name']] = symbol['value']
    labels = {}
    allocations = []
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
        mapped = data_allocation(pe, base, size, section['flags'], f'{path.name} {name}')
        allocations.append((base, base + size))
        start = section['raw_offset']
        if not start:
            if section['reloc_count']:
                raise ValueError(f'{path.name} {name} has relocations in zero-initialized data')
            size = max(0, min(size, mapped['rva'] + mapped['raw_size'] - rva))
        expected = bytearray(data[start:start + size]) if start else bytearray(size)
        actual = coff.pe_bytes_at(image, base, size) if size else b''
        anchors = {}
        for label, offset in offsets.items():
            if label not in originals:
                raise ValueError(f'{path.name} label {label} has no unique retail address')
            origin = originals[label]
            if anchors.setdefault(offset, origin) != origin:
                raise ValueError(f'{path.name} {name}+{offset:#x} has conflicting original labels')
        anchor_offsets = sorted(anchors)
        for ordinal in range(section['reloc_count']):
            offset, index, kind = struct.unpack_from('<IIH', data, section['reloc_offset'] + 10 * ordinal)
            symbol = symbols[index]['name']
            dest = resolve(symbol)
            if kind != 6 or dest is None:
                raise ValueError(f'{path.name} {name}+{offset:#x} has unbound relocation to {symbol}')
            anchor_index = bisect_right(anchor_offsets, offset) - 1
            if anchor_index < 0 or offset + 4 > size:
                raise ValueError(f'{path.name} {name}+{offset:#x} has no complete labelled pointer field')
            # Blocks can move independently within a COFF section.
            anchor = anchor_offsets[anchor_index]
            original_slot = anchors[anchor] + offset - anchor
            addend = u32(expected, offset)
            original_dest = u32(coff.pe_bytes_at(target, original_slot, 4), 0)
            if symbol not in originals or (originals[symbol] + addend) & 0xFFFFFFFF != original_dest:
                raise ValueError(f'{path.name} {original_slot:#010x} does not match retail pointer to {symbol}')
            value = (dest + addend) & 0xFFFFFFFF
            if value != u32(actual, offset):
                raise ValueError(f'{path.name} {name}+{offset:#x} does not resolve to {symbol}')
            struct.pack_into('<I', expected, offset, value)
            field_count += 1
        if actual != expected:
            raise ValueError(f'{path.name} {name} differs from its linked bytes')
        labels.update({label: addresses[label] for label in offsets})
    verify_data_owners(originals, labels, object_sizes, allocations)
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
        if len(code) != unit.get('candidate_size', len(code)):
            raise ValueError(f'{name} has an unexpected linked carrier extent')
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
                    coff.read_coff_object(path), field, dest, read_linked, destinations,
                    unit.get('unwind_states', (1, 3))))
            linked[offset:offset + 4] = code[offset:offset + 4]
            field_count += 1
        if linked != code:
            raise ValueError(f'{name} differs from {path.name} outside relocations')
        code_addresses[name] = address
    return code_addresses, field_count, private_fields, handlers


def verify_data_owners(originals, labels, object_sizes, allocations):
    bindings = defaultdict(dict)
    for symbol, address in labels.items():
        bindings[originals[symbol]][symbol] = address
    for owner, size in object_sizes.items():
        start = int(owner, 0)
        origins = set(bindings[start].values())
        if not origins:
            continue
        if len(origins) != 1:
            raise ValueError(f'generated object at {start:#010x} has separate base labels')
        base = origins.pop()
        if not any(lo <= base and base + size <= hi for lo, hi in allocations):
            raise ValueError(f'generated object at {start:#010x} extends outside its COFF allocation')
        owned = set(bindings[start])
        for target, aliases in bindings.items():
            if start < target < start + size:
                for symbol, address in aliases.items():
                    if address != base + target - start:
                        raise ValueError(f'{symbol} is outside its owning offset in {start:#010x}')
                    owned.add(symbol)
        for symbol, address in labels.items():
            if base <= address < base + size and symbol not in owned:
                raise ValueError(f'{symbol} overlaps generated object at {start:#010x}')


def verify_enemy_schedule(target, image, pe, objects, addresses, originals):
    name = '?g_EnemySubroutineSchedule@@3PAEA'
    _, sections, symbols = coff.read_coff_object(objects['src/EclManager.cpp'])
    symbol, = (row for row in symbols.values() if row['name'] == name and row['section'] > 0)
    section = sections[symbol['section'] - 1]
    size = 256
    if (symbol['storage'] != 2 or symbol['value'] != 0 or section['name_raw'].rstrip(b'\0') != b'.bss'
            or section['size'] != size or section['raw_offset'] or section['reloc_count']):
        raise ValueError('enemy schedule does not occupy its complete zero-initialized COFF section')
    address = addresses[name]
    mapped = data_allocation(pe, address, size, section['flags'], 'enemy schedule')
    if any(address <= other < address + size for label, other in addresses.items() if label != name):
        raise ValueError('a linked symbol overlaps the enemy schedule')
    rva = address - pe['image_base']
    raw = min(size, mapped['rva'] + mapped['raw_size'] - rva)
    if (any(coff.pe_bytes_at(target, originals[name], size))
            or (raw > 0 and any(coff.pe_bytes_at(image, address, raw)))):
        raise ValueError('enemy schedule is not zero-initialized')
    return size


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


def verify_focus_effect_ids(target, image, addresses):
    symbol = '?g_PlayerFocusEffectIds@@3PAHA'
    size = 16 * 4
    if coff.pe_bytes_at(image, addresses[symbol], size) != coff.pe_bytes_at(target, 0x004A1A10, size):
        raise ValueError('linked player focus effect IDs differ from the retail table')


def verify_frontiers(target, frontiers, objects):
    for name, entry in frontiers.items():
        size, differing, _, _, _ = frontier_check.measure(coff, target, entry, objects[entry['source']])
        new = differing - frontier_check.parse_offsets(entry['residual'])
        if size != entry['candidate_size'] or new:
            offsets = ', '.join(frontier_check.format_offsets(new)) or 'none'
            raise ValueError(f'{name} linked candidate has size {size}, expected {entry["candidate_size"]}, '
                             f'and new retail differences at {offsets}')


def verify_replay_captions(target, image, units, objects, addresses):
    slots = 0
    for name, title_menu in (('replay-save', False), ('title-screen-draw-replay-menu', True)):
        unit = units[name]
        path = objects[unit['source']]
        _, fields = coff.object_function(path, unit['symbol'], unit['size'], include_symbol_locations=True)
        proof = coff.verify_replay_name_tables(path, target, unit['relocations'], fields, title_menu=title_menu)
        original = int(proof['target_address'], 16)
        base = addresses[proof['symbols'][0]]
        for field in unit['relocations']:
            symbol = field['symbol']
            if symbol in proof['symbols'] and addresses[symbol] != base + field['target'] - original:
                raise ValueError(f'{name}: {symbol} moved outside its replay caption table offset')
        linked = coff.pe_bytes_at(image, base, proof['size'])
        for literal in proof['literal_definitions']:
            pointer = addresses[literal['symbol']]
            for offset in literal['references']:
                if u32(linked, offset) != pointer:
                    raise ValueError(f'{name}: replay caption at +{offset:#x} points to the wrong literal')
            original_text = coff.pe_bytes_at(target, literal['target'], literal['size'])
            if coff.pe_bytes_at(image, pointer, literal['size']) != original_text:
                raise ValueError(f'{name}: linked replay caption bytes differ from retail')
        slots += proof['relocations']
    return slots


def crt_xcu_wrappers(objects):
    startup = 'product/ProductStaticInit.cpp'
    wrappers = []
    for source, path in sorted(objects.items()):
        data, sections, symbols = coff.read_coff_object(path)
        for section in sections:
            if section['name_raw'].rstrip(b'\0') != b'.CRT$XCU':
                continue
            if source != startup:
                raise ValueError(f'{source} contributes unexpected .CRT$XCU static initializers')
            entries = []
            for index in range(section['reloc_count']):
                offset, symbol_index, kind = struct.unpack_from(
                    '<IIH', data, section['reloc_offset'] + 10 * index)
                if kind != 6 or offset % 4:
                    raise ValueError(f'{startup} .CRT$XCU has an invalid relocation at +{offset:#x}')
                entries.append((offset, symbols[symbol_index]['name']))
            wrappers.extend(name for _, name in sorted(entries))
    if not wrappers:
        raise ValueError(f'{startup} has no .CRT$XCU static initializers')
    return wrappers


def verify_crt_initializers(image, objects, addresses):
    c_initializers = ('___onexitinit', '___initstdio', '___sse2_available_init', '___initmbctable',
                      '?__CxxSetUnhandledExceptionFilter@@YAHXZ')
    cxx_initializers = ('___security_init_cookie', *crt_xcu_wrappers(objects))
    for section, names in (('xi', c_initializers), ('xc', cxx_initializers)):
        expected = struct.pack(f'<{len(names) + 2}I', 0, *(addresses[name] for name in names), 0)
        start, end = addresses[f'___{section}_a'], addresses[f'___{section}_z']
        if end - start != len(expected) - 4 or coff.pe_bytes_at(image, start, len(expected)) != expected:
            raise ValueError(f'linked CRT {section} initializer table differs from the expected order')


@cache
def crt_entry_object():
    archive = trial.pinned_libraries()['libcmt.lib']
    member, = (data for name, data in archive_members(archive)
               if PureWindowsPath(name).name.lower() == 'wincrt0.obj')
    return member


def verify_crt_entry(target, image, pe, addresses, retail_winmain):
    symbol = '_WinMainCRTStartup'
    entry = addresses[symbol]
    if pe['subsystem_id'] != 2 or pe['entry_point'] != entry:  # IMAGE_SUBSYSTEM_WINDOWS_GUI
        raise ValueError('PE entry or subsystem does not select the VC7.1 GUI startup')
    member = crt_entry_object()
    code, fields = coff.object_function(member, symbol, 469, include_symbol_locations=True)
    if len(code) != 469 or not any(
            s['characteristics'] & 0x20000000 and s['rva'] <= pe['entry_rva']
            and pe['entry_rva'] + len(code) <= s['rva'] + min(s['virtual_size'], s['raw_size'])
            for s in pe['sections']):
        raise ValueError('CRT entry extent is outside executable file-backed storage')

    retail_entry = parse_pe(target)['entry_point']
    original = bytearray(coff.pe_bytes_at(target, retail_entry, len(code)))
    linked = bytearray(coff.pe_bytes_at(image, entry, len(code)))
    original_iat, linked_iat = coff.target_iat(target), coff.target_iat(image)
    # The private SEH scope table has no public linker label.
    scope, = (field for field in fields if field['symbol'] == '$T20649')
    scope_code, scope_fields = exceptions.owned_section(
        coff.read_coff_object(member), scope['symbol_section'], scope['owner_section'], '.rdata', 0x40)
    if scope['type'] != 'DIR32' or scope['symbol_value'] or scope['addend'] or set(scope_fields) != {4, 8}:
        raise ValueError('unexpected CRT scope table reference')
    for binary, start, operand in ((target, retail_entry, original), (image, entry, linked)):
        expected = scope_code.copy()
        for offset, (label, kind) in scope_fields.items():
            local = label['value'] - scope['owner_value']
            if kind != 6 or label['section'] != scope['owner_section'] or not 0 <= local < len(code):
                raise ValueError('CRT scope table refers outside the entry function')
            exceptions.relocate_zero_field(expected, offset, start + local)
        address = u32(operand, scope['offset'])
        if coff.pe_bytes_at(binary, address, len(expected)) != expected:
            raise ValueError('CRT scope table does not select the expected filter and handler')

    for field in fields:
        offset, name = field['offset'], field['symbol']
        if field != scope:
            addend = field['addend']
            if field['type'] == 'REL32':
                if name == '_WinMain@16':
                    retail_target = (u32(original, offset) + retail_entry + offset + 4 - addend) & 0xFFFFFFFF
                    if retail_target != retail_winmain:
                        raise ValueError('retail CRT entry does not select canonical WinMain')
                expected = addresses[name] + addend - (entry + offset + 4)
            elif field['type'] == 'DIR32':
                expected = addresses[name] + addend
                if name.startswith('__imp__'):
                    identity = original_iat.get(u32(original, offset) - addend)
                    if identity is None or linked_iat.get(addresses[name]) != identity:
                        raise ValueError(f'CRT entry import binding differs for {name}')
            else:
                raise ValueError(f'unsupported CRT entry relocation for {name}')
            if u32(linked, offset) != expected & 0xFFFFFFFF:
                raise ValueError(f'CRT entry+{offset:#x} does not resolve to {name}')
        original[offset:offset + 4] = code[offset:offset + 4]
        linked[offset:offset + 4] = code[offset:offset + 4]
    if linked != code or original != code:
        raise ValueError('CRT entry bytes differ from pinned wincrt0.obj outside relocations')
    return len(code), len(fields) + len(scope_fields)


def verify_exit_registrations(path, constructor, size, cleanup_names):
    code, relocations = coff.object_function(path, constructor, size)
    fields = {field['offset']: field for field in relocations}
    registered = []
    for offset, field in sorted(fields.items()):
        if field['symbol'] != '_atexit':
            continue
        argument = fields.get(offset - 5)
        if (offset < 6 or code[offset - 6] != 0x68 or code[offset - 1] != 0xE8
                or field['type'] != 'REL32' or field['addend'] or argument is None
                or argument['type'] != 'DIR32' or argument['addend']):
            raise ValueError(f'{constructor}+{offset:#x} has an unexpected _atexit call')
        registered.append(argument['symbol'])
    if registered != [f'?Destroy{name}@@YAXXZ' for name in cleanup_names]:
        raise ValueError(f'{constructor} does not preserve its exit registration order')


def verify_product_startup(target, image, pe, objects, addresses, destinations):
    startup = 'product/ProductStaticInit.cpp'
    archive_ctor = '??0PbgArchive@@QAE@XZ'
    xcu_wrappers = crt_xcu_wrappers(objects)
    functions = {}
    for source in (startup, 'product/PbgArchiveCtor.cpp'):
        _, sections, symbols = coff.read_coff_object(objects[source])
        for symbol in symbols.values():
            number = symbol['section']
            if number <= 0 or symbol['type'] != 0x20:
                continue
            section = sections[number - 1]
            name = symbol['name']
            sec_name = section['name_raw'].rstrip(b'\0')
            if sec_name not in (b'.text', b'.text$yc', b'.text$yd'):
                continue  # Associative EH sections have separate checks.
            if name not in addresses or name == '??_H@YGXPAXIHP6EPAX0@Z@Z':
                continue  # Unreferenced COMDAT or separately verified canonical helper.
            functions[name] = {'source': source, 'symbol': name, 'size': section['size']}
    eh_ctors = (
        ('??0ScoreRecordsStaticInit@@QAE@XZ', 2),
        ('??0AsciiManagerStaticInit@@QAE@XZ', 1),
    )
    for constructor, states in eh_ctors:
        functions[constructor].update(unwind_states=(states,),
                                      relocations=[{'symbol': '__ehhandler$' + constructor}])

    # Verify the 19 .CRT$XCU wrappers construct and register teardown in exact retail order.
    expected_init_order = [
        '_s_gameManagerStaticInit',
        '?g_GameManagerCalcChain@@3VChainElem@@A',
        '?g_GameManagerDrawChain@@3VChainElem@@A',
        '?g_ScoreFileHeader@@3UScoreFileView@@A',
        '_s_scoreRecordsStaticInit',
        '?g_Chain@@3VChain@@A',
        '?g_ZunMemory@@3VZunMemory@@A',
        '?g_ReplayInputStates@@3PAUReplayInputState@@A',
        '?g_PbgArchive@@3VPbgArchive@@A',
        '?g_GameErrorContext@@3VGameErrorContext@@A',
        '?g_GameWindow@@3UGameWindow@@A',
        '?g_Supervisor@@3VSupervisor@@A',
        '?g_SupervisorLoadingVms@@3PAUAnmVm@@A',
        '_s_auxiliaryArchives',
        '_s_asciiManagerStaticInit',
        '?g_AsciiManagerCalcChain@@3VChainElem@@A',
        '?g_AsciiManagerDrawChainLowPrio@@3VChainElem@@A',
        '?g_AsciiManagerDrawChainHighPrio@@3VChainElem@@A',
        '?g_SoundPlayer@@3VSoundPlayer@@A',
    ]
    expected_dtor_order = [
        '?g_GameManagerCalcChain@@3VChainElem@@A',
        '?g_GameManagerDrawChain@@3VChainElem@@A',
        '?g_Chain@@3VChain@@A',
        '?g_ZunMemory@@3VZunMemory@@A',
        '?g_PbgArchive@@3VPbgArchive@@A',
        '_s_auxiliaryArchives',
        '?g_AsciiManagerCalcChain@@3VChainElem@@A',
        '?g_AsciiManagerDrawChainLowPrio@@3VChainElem@@A',
        '?g_AsciiManagerDrawChainHighPrio@@3VChainElem@@A',
    ]
    actual_init_order = []
    actual_dtor_order = []
    aux_ctor_wrapper = None
    aux_dtor_wrapper = None
    for wrapper in xcu_wrappers:
        _, rels = coff.object_function(objects[startup], wrapper, functions[wrapper]['size'])
        dir_syms = [r['symbol'] for r in rels if r['type'] == 'DIR32']
        rel_syms = [r['symbol'] for r in rels if r['type'] == 'REL32']
        target_obj = next((s for s in dir_syms if s in expected_init_order), None)
        if target_obj is None:
            raise ValueError(f'{wrapper} does not target an expected static global')
        actual_init_order.append(target_obj)
        if '_atexit' in rel_syms:
            by_off = {r['offset']: r for r in rels}
            atexit_off = next(r['offset'] for r in rels if r['symbol'] == '_atexit')
            dtor_field = by_off.get(atexit_off - 5)
            dtor_fn = dtor_field['symbol'] if dtor_field and dtor_field['type'] == 'DIR32' else None
            if dtor_fn is None or dtor_fn not in functions:
                raise ValueError(f'{wrapper} calls _atexit without a local teardown wrapper')
            _, dtor_rels = coff.object_function(objects[startup], dtor_fn, functions[dtor_fn]['size'])
            dtor_obj = next((r['symbol'] for r in dtor_rels if r['type'] == 'DIR32' and r['symbol'] in expected_init_order), None)
            if dtor_obj != target_obj:
                raise ValueError(f'{dtor_fn} tears down {dtor_obj}, expected {target_obj}')
            actual_dtor_order.append(dtor_obj)
            if target_obj == '_s_auxiliaryArchives':
                aux_ctor_wrapper, aux_dtor_wrapper = wrapper, dtor_fn
    if actual_init_order != expected_init_order:
        raise ValueError(f'static constructor order differs from retail: {actual_init_order}')
    if actual_dtor_order != expected_dtor_order:
        raise ValueError(f'static atexit teardown order differs from retail: {actual_dtor_order}')

    _, sections, symbols = coff.read_coff_object(objects[startup])
    array_name = '_s_auxiliaryArchives'
    array, = (symbol for symbol in symbols.values() if symbol['name'] == array_name)
    bss = sections[array['section'] - 1]
    # Anchor the .bss contribution from its public base symbol and bind private locals.
    anchors = {addresses[s['name']] - s['value'] for s in symbols.values()
               if s['section'] == array['section'] and s['storage'] == 2 and s['name'] in addresses}
    if len(anchors) != 1:
        raise ValueError('ProductStaticInit.obj .bss symbols do not share one section base')
    base = anchors.pop()
    allocation = data_allocation(pe, base, bss['size'], bss['flags'], 'product startup storage')
    receivers = ('_s_gameManagerStaticInit', '_s_scoreRecordsStaticInit', '_s_asciiManagerStaticInit')
    storage = {symbol['name']: base + symbol['value'] for symbol in symbols.values()
               if symbol['section'] == array['section'] and symbol['name'] in (array_name, *receivers)}
    raw = min(bss['size'], pe['image_base'] + allocation['rva'] + allocation['raw_size'] - base)
    if (bss['name_raw'].rstrip(b'\0') != b'.bss' or bss['raw_offset'] or bss['reloc_count']
            or (raw > 0 and any(coff.pe_bytes_at(image, base, raw)))):
        raise ValueError('ProductStaticInit.obj .bss storage is not cleanly zero-initialized')
    # VC7.1 uses FS:[0] for the x86 exception registration chain.
    bindings = destinations | storage | {'__except_list': 0}
    code, fields, _, handlers = verify_code(image, pe, functions, objects, addresses, bindings)

    # Resolve the compiler's complete wrappers against the retail CRT and objects.
    retail = {archive_ctor: 0x00433040, '??1PbgArchive@@QAE@XZ': 0x004335D0,
              '??_L@YGXPAXIHP6EX0@Z1@Z': 0x0047CEC9, '??_M@YGXPAXIHP6EX0@Z@Z': 0x0047CE69,
              '_atexit': 0x0047C323, aux_dtor_wrapper: 0x0048DA90, array_name: 0x004B42E0}
    for name, address, size in ((aux_ctor_wrapper, 0x0048D920, 36), (aux_dtor_wrapper, 0x0048DA90, 20),
                                (archive_ctor, retail[archive_ctor], 16)):
        expected, relocations = coff.object_function(objects[functions[name]['source']], name, size)
        for field in relocations:
            offset = field['offset']
            destination = retail[field['symbol']] + field['addend']
            if field['type'] == 'REL32':
                destination -= address + offset + 4
            elif field['type'] != 'DIR32':
                raise ValueError(f'unsupported product startup relocation in {name}')
            struct.pack_into('<I', expected, offset, destination & 0xFFFFFFFF)
        if len(expected) != size or coff.pe_bytes_at(target, address, size) != expected:
            raise ValueError(f'{name} differs from its retail startup or teardown body')
    return len(code), fields, handlers


def verify_resources(target, image):
    original = numeric_resources(target, lambda address, size: coff.pe_bytes_at(target, address, size))
    linked = numeric_resources(image, lambda address, size: coff.pe_bytes_at(image, address, size))
    if linked != original:
        raise ValueError('linked PE resource identities or payloads differ from the original')
    return len(linked)


def verify_d3dx_constants(target, image, pe):
    retail = parse_pe(target)
    original, = [section for section in retail['sections'] if section['name'] == '.data1']
    linked = next((section for section in pe['sections'] if section['name'] == '.data1'), None)
    size = original['virtual_size']
    if linked is None or linked['virtual_size'] != size:
        raise ValueError('linked D3DX8 constant section extent differs from retail')
    original_data = coff.pe_bytes_at(target, retail['image_base'] + original['rva'], size)
    if coff.pe_bytes_at(image, pe['image_base'] + linked['rva'], size) != original_data:
        raise ValueError('linked D3DX8 constant section bytes differ from retail')
    return size


@cache
def d3dx_math_object():
    archive = trial.pinned_libraries()['d3dx8.lib']
    member, = (data for name, data in archive_members(archive)
               if PureWindowsPath(name).name.lower() == 'd3dxmath.obj')
    return member


def verify_d3dx_fast_table(target, image, pe, addresses, originals):
    member = d3dx_math_object()
    data, sections, symbols = coff.read_coff_object(member)
    name = '?g_D3DXFastTable@@3UD3DXFASTTABLE@@A'
    table, = (symbol for symbol in symbols.values()
              if symbol['name'] == name and symbol['section'] > 0)
    section = sections[table['section'] - 1]
    start, size = section['raw_offset'], section['size']
    if table['value'] or section['name_raw'].rstrip(b'\0') != b'.data' or not start or size % 4:
        raise ValueError('unexpected D3DX8 math dispatch table contribution')
    linked_base = addresses[name]
    data_allocation(pe, linked_base, size, section['flags'], name)

    # The SDK's public math entries jump through distinct slots of this table.
    entries = ('_D3DXVec3Normalize@8', '_D3DXVec3Project@24', '_D3DXMatrixMultiply@12',
               '_D3DXMatrixRotationX@8', '_D3DXMatrixRotationY@8', '_D3DXMatrixRotationZ@8')
    retail_base = 0x004A3630
    entry_slots = set()
    for entry in entries:
        code, fields = coff.object_function(member, entry, 6)
        field, = fields
        addend = field['addend']
        if (len(code) != 6 or code[:2] != b'\xff\x25'
                or (field['offset'], field['type_id'], field['symbol']) != (2, 6, name)):
            raise ValueError(f'{entry} is not a D3DX8 math dispatch thunk')
        entry_slots.add(addend)
        for binary, address, base in ((target, originals[entry], retail_base),
                                      (image, addresses[entry], linked_base)):
            struct.pack_into('<I', code, 2, base + addend)
            if coff.pe_bytes_at(binary, address, len(code)) != code:
                raise ValueError(f'{entry} does not select D3DX8 math dispatch slot +{addend:#x}')

    expected = bytearray(data[start:start + size])
    linked = coff.pe_bytes_at(image, linked_base, size)
    slots = set()
    for index in range(section['reloc_count']):
        offset, symbol_index, kind = struct.unpack_from('<IIH', data, section['reloc_offset'] + 10 * index)
        symbol = symbols[symbol_index]
        if (kind != 6 or offset % 4 or offset + 4 > size or offset in slots
                or symbol['section'] < 1 or symbol['type'] != 0x20):
            raise ValueError(f'invalid D3DX8 math dispatch field at +{offset:#x}')
        destination = addresses[symbol['name']] + u32(expected, offset)
        flags = sections[symbol['section'] - 1]['flags']
        data_allocation(pe, destination, 1, flags, symbol['name'])
        if u32(linked, offset) != destination:
            raise ValueError(f'D3DX8 math dispatch slot +{offset:#x} does not select {symbol["name"]}')
        struct.pack_into('<I', expected, offset, destination)
        slots.add(offset)
    if linked != expected or len(entry_slots) != len(entries) or not entry_slots <= slots:
        raise ValueError('D3DX8 math dispatch table does not preserve its COFF contents or entry slots')
    return len(entries), len(slots)


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


def verify_guids(target, image, units, addresses):
    local = {'_g_Th09NetworkApplicationGuid', '?g_KeyboardDeviceGuid@@3U_GUID@@B'}
    destinations = {}
    fields = 0
    for name, unit in units.items():
        for field in unit.get('relocations', []):
            symbol = field['symbol']
            if symbol not in local and not symbol.startswith(('_CLSID_', '_IID_', '_GUID_')):
                continue
            offset = field['offset']
            original = field['target']
            original_operand = u32(coff.pe_bytes_at(target, unit['target_address'] + offset, 4), 0)
            # The retail operand can name a DWORD inside the GUID.
            addend = original_operand - original
            if field['type'] != 'DIR32' or not 0 <= addend <= 12:
                raise ValueError(f'{name}+{offset:#x} has an unexpected retail GUID operand')
            operand = u32(coff.pe_bytes_at(image, addresses[unit['symbol']] + offset, 4), 0)
            destination = operand - addend
            if destinations.setdefault(symbol, destination) != destination or (
                    coff.pe_bytes_at(image, destination, 16) != coff.pe_bytes_at(target, original, 16)):
                raise ValueError(f'{name}+{offset:#x} differs from the retail GUID {symbol}')
            fields += 1
    return len(destinations), fields


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


def verify_opponent_lists(target, image, units, addresses, labels, object_sizes):
    symbol = '?g_OpponentLists@@3PAPAUOpponentSelectionEntry@@A'
    fields = units['game-manager-select-opponent']['relocations']
    pointer_table, = {field['target'] for field in fields if field['symbol'] == symbol}
    extent = object_sizes[f'0x{pointer_table:08X}']
    original = coff.pe_bytes_at(target, pointer_table, extent)
    linked = coff.pe_bytes_at(image, addresses[symbol], extent)
    for slot, ((source,), (destination,)) in enumerate(
            zip(struct.iter_unpack('<I', original), struct.iter_unpack('<I', linked), strict=True)):
        size = object_sizes[f'0x{source:08X}']
        expected = coff.pe_bytes_at(target, source, size)
        keys = [row[0] for row in struct.iter_unpack('<8h', expected)]
        if len(expected) != size or not keys or keys[-1] >= 0 or any(key < 0 for key in keys[:-1]):
            raise ValueError(f'opponent list {slot} has an invalid original extent')
        generated = labels.get(f'th09_data_{source:08X}')
        if (generated is not None and destination != generated) or coff.pe_bytes_at(image, destination, size) != expected:
            raise ValueError(f'opponent list {slot} differs from the attested {source:#010x} extent')


def highlow_relocations(image, pe):
    header = u32(image, 0x3C)
    optional = header + 24
    optional_size, = struct.unpack_from('<H', image, header + 20)
    directory = optional + 96 + 5 * 8  # IMAGE_DIRECTORY_ENTRY_BASERELOC
    if (pe['relocations_stripped'] or directory + 8 > optional + optional_size
            or u32(image, optional + 92) < 6):
        raise ValueError('PE lacks a base relocation directory')
    rva, size = struct.unpack_from('<II', image, directory)
    if not rva or not size:
        raise ValueError('PE has an empty base relocation directory')
    data = coff.pe_bytes_at(image, pe['image_base'] + rva, size)
    fields = set()
    cursor = 0
    while cursor < len(data):
        page, block_size = struct.unpack_from('<II', data, cursor)
        end = cursor + block_size
        if page % 0x1000 or block_size < 8 or block_size % 4 or end > len(data):
            raise ValueError(f'invalid base relocation block at directory offset {cursor:#x}')
        for (entry,) in struct.iter_unpack('<H', data[cursor + 8:end]):
            kind, offset = entry >> 12, entry & 0xFFF
            if kind == 0:  # IMAGE_REL_BASED_ABSOLUTE padding
                continue
            field = page + offset
            if kind != 3 or field in fields:  # IMAGE_REL_BASED_HIGHLOW
                raise ValueError(f'unsupported or repeated base relocation at RVA {field:#x}')
            fields.add(field)
        cursor = end
    if not fields:
        raise ValueError('PE has no HIGHLOW relocations')
    return fields


def section_payloads(image, pe):
    return {(s['name'], s['rva'], s['virtual_size'], s['characteristics']):
            image[s['raw_offset']:s['raw_offset'] + s['raw_size']]
            for s in pe['sections'] if s['name'] != '.reloc'}


def decode_x86_chunks(decoder, payload, address):
    cursor = 0
    while cursor < len(payload):
        limit = min(cursor + 16 * 1024, len(payload))
        start = cursor
        # An x86 instruction can extend up to 14 bytes past the boundary.
        for instruction in decoder.disasm(payload[start:limit + 14], address + start):
            offset = instruction.address - address
            if offset >= limit:
                break
            yield instruction
            cursor = offset + instruction.size
        if cursor == start:
            raise ValueError(f'x86 decoder stopped at {address + cursor:#010x}')


def verify_absolute_memory_operands(image, pe, fields, former_range=None):
    try:
        import capstone
        from capstone.x86 import X86_OP_MEM
    except ImportError as exc:
        raise ValueError('Capstone is required for the base-relocation operand check') from exc

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    decoder.skipdata = True
    base = pe['image_base']
    end = base + pe['size_of_image']
    count = 0
    for section in pe['sections']:
        if not section['characteristics'] & 0x20000000:
            continue
        start = section['raw_offset']
        size = min(section['raw_size'], section['virtual_size'])
        payload = image[start:start + size]
        for instruction in decode_x86_chunks(decoder, payload, base + section['rva']):
            if not instruction.id or instruction.disp_size != 4:
                continue
            # An indexed absolute address may have an index register but no base register.
            if not any(op.type == X86_OP_MEM and not op.mem.base and not op.mem.segment
                       for op in instruction.operands):
                continue
            address = instruction.disp & 0xFFFFFFFF
            if former_range and former_range[0] <= address < former_range[1]:
                raise ValueError(f'absolute memory operand at {instruction.address:#010x} retains the old image base')
            if base <= address < end:
                site = instruction.address + instruction.disp_offset - base
                if site not in fields:
                    raise ValueError(f'absolute memory operand at {instruction.address:#010x} has no HIGHLOW field')
                count += 1
    return count


def verify_base_pair(fixed, preferred, shifted):
    fixed_pe, preferred_pe, shifted_pe = map(parse_pe, (fixed, preferred, shifted))
    low, high = trial.IMAGE_BASES
    if (fixed_pe['image_base'], preferred_pe['image_base'], shifted_pe['image_base']) != (low, low, high):
        raise ValueError('unexpected PE image bases in the base comparison')
    if not fixed_pe['relocations_stripped']:
        raise ValueError('base comparison requires the fixed trial as its reference')
    if low + preferred_pe['size_of_image'] > high:
        raise ValueError('the trial image ranges overlap')
    if len({pe['entry_rva'] for pe in (fixed_pe, preferred_pe, shifted_pe)}) != 1:
        raise ValueError('PE entry RVA changed between bases')
    fields = highlow_relocations(preferred, preferred_pe)
    if fields != highlow_relocations(shifted, shifted_pe):
        raise ValueError('HIGHLOW field RVAs differ between bases')
    original = section_payloads(preferred, preferred_pe)
    moved = section_payloads(shifted, shifted_pe)
    if original != section_payloads(fixed, fixed_pe):
        raise ValueError('preferred-base relocatable sections differ from the fixed link')
    if original.keys() != moved.keys():
        raise ValueError('linked section identities or attributes differ between bases')
    counts = {}
    for key, payload in original.items():
        name, start, size, _ = key
        expected = bytearray(payload)
        count = 0
        for field in fields:
            offset = field - start
            if 0 <= offset and offset + 4 <= min(size, len(payload)):
                value = (u32(payload, offset) + high - low) & 0xFFFFFFFF
                struct.pack_into('<I', expected, offset, value)
                count += 1
        if expected != moved[key]:
            raise ValueError(f'{name} bytes differ after applying the HIGHLOW base delta')
        if count:
            counts[name] = count
    if sum(counts.values()) != len(fields):
        raise ValueError('HIGHLOW fields extend outside the file-backed image sections')
    old_range = (low, low + preferred_pe['size_of_image'])
    preferred_operands = verify_absolute_memory_operands(preferred, preferred_pe, fields)
    shifted_operands = verify_absolute_memory_operands(shifted, shifted_pe, fields, old_range)
    return counts, (preferred_operands, shifted_operands)


def verify_layout(stem, data_name, target, units, objects):
    image = (trial.OUT / f'{stem}.exe').read_bytes()
    pe = parse_pe(image)
    if pe['machine_id'] != 0x14C or pe['linker_version'] != '7.10':
        raise ValueError(f'{stem} is not a VC7.1 i386 link')
    retail_pe = parse_pe(target)
    for alignment in ('file_alignment', 'section_alignment'):
        if pe[alignment] != retail_pe[alignment]:
            raise ValueError(f'{stem} {alignment} differs from retail')
    addresses = read_map(trial.OUT / f'{stem}.map')
    entry_bytes, entry_fields = verify_crt_entry(target, image, pe, addresses, units['win-main']['target_address'])
    verify_crt_initializers(image, objects, addresses)
    resources = verify_resources(target, image)
    d3dx_constants = verify_d3dx_constants(target, image, pe)
    imports, import_fields = verify_imports(target, image, units, addresses)
    guids, guid_fields = verify_guids(target, image, units, addresses)
    aliases = dict(line.removeprefix('/ALTERNATENAME:').split('=', 1)
                   for line in (trial.OUT / f'{stem}.rsp').read_text().splitlines()
                   if line.startswith('/ALTERNATENAME:'))
    product = tomllib.loads((trial.ROOT / 'config/product-link.toml').read_text())
    originals = original_symbol_addresses(units)
    d3dx_entries, d3dx_slots = verify_d3dx_fast_table(target, image, pe, addresses, originals)
    destinations = resolve_aliases(aliases, addresses, originals, product['library_data'])
    resolve = destinations.get
    startup_functions, startup_fields, startup_handlers = verify_product_startup(
        target, image, pe, objects, addresses, destinations)

    data_path = trial.ROOT / 'build/product' / data_name
    labels, data_fields = verify_data_object(target, image, pe, data_path, addresses, resolve, originals,
                                            product['object_size'])
    schedule_bytes = verify_enemy_schedule(target, image, pe, objects, addresses, originals)
    verify_opponent_lists(target, image, units, addresses, labels, product['object_size'])
    input_objects = verify_input_formats(target, image, units, resolve, product['library_data'])
    callback_slots = verify_callback_tables(target, image, units, resolve)
    ecl_slots = verify_ecl_tables(target, image, units, resolve)
    verify_focus_effect_ids(target, image, addresses)
    replay_captions = verify_replay_captions(target, image, units, objects, addresses)
    vtables, virtual_slots = verify_vtables(image, units, objects, addresses, resolve)
    frontiers = trial.load_frontiers()
    verify_frontiers(target, frontiers, objects)
    code, code_fields, private_fields, handlers = verify_code(image, pe, units | frontiers, objects,
                                                            addresses, destinations)
    cpp_data = {}
    for path in objects.values():
        _, sections, symbols = coff.read_coff_object(path)
        for symbol in symbols.values():
            if symbol['storage'] != 2 or symbol['section'] <= 0 or symbol['type'] == 0x20:
                continue
            sec_name = sections[symbol['section'] - 1]['name_raw'].rstrip(b'\0')
            name = symbol['name']
            if sec_name in (b'.data', b'.rdata', b'.bss') and name in originals and name in addresses and name not in labels:
                cpp_data[name] = addresses[name]
    print(f'{stem}: {len(code)} code units ({len(frontiers)} from frontiers), {code_fields} code relocations '
          f'({private_fields} into private constants), {len(cpp_data)} C++ data globals, '
          f'{len(labels)} generated data labels, '
          f'{data_fields} data relocations, {input_objects} DirectInput objects, '
          f'{callback_slots} CardAttack/Effect/ExAttack callback slots, {ecl_slots} ECL callback slots, '
          f'{replay_captions} replay captions, {schedule_bytes} enemy schedule bytes')
    print(f'{stem}: {len(handlers)} C++ exception handler references, '
          f'{sum(row["states"] for row in handlers)} unwind states, '
          f'{sum(row["relocations"] for row in handlers)} unwind relocations')
    print(f'{stem}: {startup_functions} product startup/exit functions, {startup_fields} code relocations, '
          f'{sum(row["states"] for row in startup_handlers)} startup unwind states, '
          f'{sum(row["relocations"] for row in startup_handlers)} unwind relocations')
    print(f'{stem}: {entry_bytes} CRT entry bytes, {entry_fields} entry and scope relocations')
    print(f'{stem}: {d3dx_entries} D3DX8 math dispatch entries, {d3dx_slots} initial function pointers')
    print(f'{stem}: {resources} PE resources, {imports} PE imports, {import_fields} checked import operands, '
          f'{guids} GUIDs across {guid_fields} operands, {vtables} vtables, {virtual_slots} virtual slots, '
          f'{len(aliases)} checked link aliases, {d3dx_constants} D3DX8 constant bytes')
    return code, cpp_data, labels


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--verify-bases', action='store_true',
                        help='also verify both relocatable links at each of the two trial bases')
    args = parser.parse_args()
    target = coff.verified_target()
    index = json.loads((trial.OUT / 'index.json').read_text())
    objects = {entry['source']: trial.OUT / entry['object'] for entry in trial.selected_carriers(index)}
    units = trial.load_units()
    normal = verify_layout('trial', 'ProductData.obj', target, units, objects)
    reversed_layout = verify_layout('trial-reversed', 'ProductDataReversed.obj', target, units, objects)
    for role, first, second in zip(('code units', 'C++ data globals', 'generated data labels'), normal, reversed_layout):
        if first.keys() != second.keys():
            raise ValueError(f'{role} differ between link layouts')
        if not first and role == 'generated data labels':
            print('0 generated data labels (100% native C++ data ownership)')
            continue
        moved = sum(address != second[name] for name, address in first.items())
        if not moved:
            raise ValueError(f'no {role} moved')
        print(f'{moved} of {len(first)} {role} moved')
    if args.verify_bases:
        for reverse, data_name in ((False, 'ProductData.obj'), (True, 'ProductDataReversed.obj')):
            stem = trial.link_stem(reverse)
            variants = [trial.link_stem(reverse, base) for base in trial.IMAGE_BASES]
            for variant in variants:
                verify_layout(variant, data_name, target, units, objects)
            images = [(trial.OUT / f'{name}.exe').read_bytes() for name in (stem, *variants)]
            counts, operands = verify_base_pair(*images)
            print(f'{stem}: base delta verified {sum(counts.values())} HIGHLOW fields {counts}')
            print(f'{stem}: checked {operands[0]} preferred-base and {operands[1]} shifted absolute memory operands')


if __name__ == '__main__':
    try:
        main()
    except (OSError, KeyError, RuntimeError, ValueError, struct.error) as exc:
        sys.exit(f'trial layout verification failed: {exc}')
