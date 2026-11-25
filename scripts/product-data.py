#!/usr/bin/env python3
"""Generate the labelled data closure for the trial whole-image link.

The exact units reference many original globals that no maintained source
defines yet. This tool emits build/product/ProductData.asm: one labelled
storage block per missing object, initial contents taken from the attested
original, and every pointer-sized word that names an image address written
as a symbol reference so the data follows its referents when the layout
changes. Interior names (fields that consumers address as globals) become
labels inside their owner's storage, which keeps shared storage identical.

It is a closure diagnostic for the Windows i386 product phase. Natural C++
reconstruction of these tables remains open; nothing here is an exactness
Oracle. The output contains original data and stays under build/.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import csv
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

from target_identity import numeric_resources, pe_bytes_at

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'build/product'
PROBE = ROOT / 'build/link-probe'
IMAGE_LO, IMAGE_HI = 0x00401000, 0x004E58E0
TEXT_HI = 0x0048DADA


def load_compare():
    spec = importlib.util.spec_from_file_location('compare_coff', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def pe_sections(image: bytes):
    pe = struct.unpack_from('<I', image, 0x3C)[0]
    count = struct.unpack_from('<H', image, pe + 6)[0]
    optional = struct.unpack_from('<H', image, pe + 20)[0]
    sections = []
    for index in range(count):
        offset = pe + 24 + optional + 40 * index
        name = image[offset:offset + 8].rstrip(b'\0').decode()
        vsize, va, rsize, raw = struct.unpack_from('<IIII', image, offset + 8)
        sections.append((name, 0x400000 + va, vsize, rsize, raw))
    return sections


def library_data_addresses(compare, image) -> set[int]:
    """Data addresses that library or compiler-generated code references."""
    import capstone
    from capstone import x86
    origins = {int(r['address'], 16): r['origin'] for r in csv.DictReader(open(ROOT / 'config/function-origins.csv'))}
    disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    disassembler.detail = True
    found = set()
    for row in csv.DictReader(open(ROOT / 'config/functions.csv')):
        address = int(row['address'], 16)
        # Third-party and compiler-generated code address game objects,
        # including their interiors; only archive code owns separate data.
        if origins.get(address) != 'library':
            continue
        try:
            code = compare.pe_bytes_at(image, address, int(row['size']))
        except ValueError:
            continue
        for instruction in disassembler.disasm(code, address):
            for operand in instruction.operands:
                value = operand.imm if operand.type == x86.X86_OP_IMM else (
                    operand.mem.disp if operand.type == x86.X86_OP_MEM else None)
                if value is not None and 0x0048E000 <= (value & 0xFFFFFFFF) < IMAGE_HI:
                    found.add(value & 0xFFFFFFFF)
    return found


def write_resources(image: bytes, path: Path) -> int:
    """Copy the original resource tree into a .res file for the linker."""
    entries = numeric_resources(image, lambda address, size: pe_bytes_at(image, address, size))

    def ident(key):
        return struct.pack('<HH', 0xFFFF, key)

    out = bytearray(struct.pack('<II', 0, 32) + ident(0) + ident(0) + struct.pack('<IHHII', 0, 0, 0, 0, 0))
    for (kind, ident_key, language), (_, data) in entries.items():
        flags = 0x1030 if kind == 14 else 0x1010
        header = ident(kind) + ident(ident_key) + struct.pack('<IHHII', 0, flags, language, 0, 0)
        out += struct.pack('<II', len(data), 8 + len(header)) + header + data
        out += b'\0' * (-len(out) % 4)
    path.write_bytes(bytes(out))
    return len(entries)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--report', action='store_true', help='print unresolved pointer words')
    parser.add_argument('--reverse', action='store_true',
                        help='emit blocks in reverse order as ProductDataReversed for layout checks')
    args = parser.parse_args()
    compare = load_compare()
    image = compare.verified_target()
    sections = pe_sections(image)

    def section_of(address):
        for name, va, vsize, rsize, raw in sections:
            if va <= address < va + max(vsize, rsize):
                return name, va, vsize, rsize, raw
        raise ValueError(f'{address:#x} outside sections')

    def initial(address, size):
        name, va, vsize, rsize, raw = section_of(address)
        out = bytearray(size)
        file_end = va + rsize
        take = max(0, min(address + size, file_end) - address)
        out[:take] = image[raw + address - va:raw + address - va + take]
        return bytes(out), take

    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    product = tomllib.loads((ROOT / 'config/product-link.toml').read_text())
    library_bound = product.get('library_data', {})
    function_starts = {int(r['address'], 16) for r in csv.DictReader(open(ROOT / 'config/functions.csv'))}

    # Address bindings from the canonical manifest.
    names_at = collections.defaultdict(set)
    for unit in units.values():
        names_at[unit['target_address']].add(unit['symbol'])
        for field in unit.get('relocations', []):
            if 'target' in field:
                names_at[field['target']].add(field['symbol'])
    trial = importlib.util.spec_from_file_location('trial_link', ROOT / 'scripts/trial-link.py')
    trial_module = importlib.util.module_from_spec(trial)
    trial.loader.exec_module(trial_module)
    defined, referenced = set(), set()
    for entry in json.loads((PROBE / 'index.json').read_text()):
        d, r = trial_module.coff_externals(PROBE / entry['object'])
        defined |= d
        referenced |= r
    data_floor = min(va for name, va, *_ in sections if name == '.rdata')
    archive = trial_module.library_symbols()
    missing_names = set()
    for address, names in names_at.items():
        if address < data_floor or any(n in defined for n in names):
            continue
        if any(n in archive for n in names):
            continue
        missing_names |= {n for n in names if n in referenced and n not in library_bound}
    # Library data addresses are owned by the linked archives; never emit them.
    library_data = library_data_addresses(compare, image)

    data_lo = data_floor
    missing = collections.defaultdict(set)
    for address, names in names_at.items():
        for name in names:
            if name in missing_names:
                missing[address].add(name)
    object_sizes = {int(owner, 16): size for owner, size in product.get('object_size', {}).items()}
    pointer_only = {}
    for address in object_sizes.keys() - names_at.keys():
        section, va, vsize, rsize, _ = section_of(address)
        if (section != '.data' or address in library_data
                or not 0 < object_sizes[address] <= va + min(vsize, rsize) - address):
            raise ValueError(f'pointer-only object at {address:#010x} has no game data extent')
        pointer_only[address] = object_sizes[address]
    known_starts = sorted({a for a in names_at if a >= data_lo}
                          | {a for a in library_data if a >= data_lo}
                          | missing.keys() | pointer_only.keys())

    # Group interior names: a missing address strictly inside a larger missing
    # owner's referenced extent becomes a label of that owner.
    extent = {}
    for address in sorted(missing):
        name, va, vsize, rsize, raw = section_of(address)
        index = bisect.bisect_right(known_starts, address)
        nxt = known_starts[index] if index < len(known_starts) else va + vsize
        # Extend across interior missing names covered by an owner's addends.
        extent[address] = min(nxt, va + max(vsize, rsize)) - address
    span = collections.defaultdict(int)
    for unit in units.values():
        for field in unit.get('relocations', []):
            if 'target' in field and field['target'] in missing:
                span[field['target']] = max(span[field['target']], field.get('addend', 0) + 1)
    for owner, size in object_sizes.items():
        if owner in missing:
            span[owner] = max(span[owner], size)
            extent[owner] = max(extent[owner], size)
    owners = {}
    for address in sorted(missing):
        for other in sorted(missing):
            if other < address < other + span[other] and section_of(other)[0] == section_of(address)[0]:
                owners[address] = other
                break
    blocks = collections.OrderedDict()
    for address in sorted(missing):
        if address in owners:
            continue
        end = address + max(extent[address], span[address])
        blocks[address] = end
    # Merge interior addresses into their owner's block end.
    for address, owner in owners.items():
        root = owner
        while root in owners:
            root = owners[root]
        blocks[root] = max(blocks[root], address + extent[address])

    unit_symbol_at = {u['target_address']: u['symbol'] for u in units.values()}

    def looks_like_pointer(word):
        if not IMAGE_LO <= word < IMAGE_HI:
            return False
        if word <= TEXT_HI:
            return word in function_starts
        return True

    # Data referenced only from these tables (mostly file-name strings) gets
    # its own anonymous labelled block: a NUL-terminated run in .rdata.
    def covered(value):
        if value in unit_symbol_at or any(n in defined for n in names_at.get(value, ())):
            return True
        return any(start <= value < end for start, end in blocks.items())
    anonymous = {}
    pending = list(blocks.items())
    while pending:
        start, end = pending.pop()
        content, filed = initial(start, end - start)
        for offset in range(0, filed - 3, 4):
            if (start + offset) % 4:
                continue
            word = struct.unpack_from('<I', content, offset)[0]
            if word > TEXT_HI and looks_like_pointer(word) and not covered(word) and word not in anonymous:
                if word in pointer_only:
                    length = pointer_only[word]
                    anonymous[word] = word + length
                    blocks[word] = word + length
                    missing[word] = {f'th09_data_{word:08X}'}
                    pending.append((word, word + length))
                    continue
                section = section_of(word)[0]
                if section != '.rdata':
                    continue
                probe, _ = initial(word, 0x200)
                length = probe.index(0) + 1 if 0 in probe else 1
                anonymous[word] = word + length
                blocks[word] = word + length
                missing[word] = {f'th09_rdata_{word:08X}'}
                pending.append((word, word + length))
    blocks = collections.OrderedDict(sorted(blocks.items()))

    labels_at = {address: sorted(missing[address]) for address in missing}

    def symbol_for(value):
        """Symbolic name for an image address, or None."""
        if value in unit_symbol_at:
            return unit_symbol_at[value], 0
        for candidate in sorted(names_at.get(value, ())):
            if candidate in defined:
                return candidate, 0
        if value in labels_at:
            return labels_at[value][0], 0
        for start in blocks:
            if start <= value < blocks[start]:
                return labels_at[start][0], value - start
        return None

    unresolved = []
    lines = ['; Generated by scripts/product-data.py from the attested original.', '.386', '']
    for segment, wanted in (('CONST', '.rdata'), ('_DATA', '.data'), ('_BSS', None)):
        emitted = []
        for start, end in blocks.items():
            section, va, vsize, rsize, raw = section_of(start)
            content, filed = initial(start, end - start)
            # PE file alignment can cover the start of a zero-fill object.
            crosses_raw_end = 0 < filed < len(content)
            known_extent = object_sizes.get(start) == len(content)
            is_bss = filed == 0 or (
                section == '.data' and crosses_raw_end and known_extent and not any(content)
            )
            if wanted == '.rdata' and section != '.rdata':
                continue
            if wanted == '.data' and (section == '.rdata' or is_bss):
                continue
            if wanted is None and (section == '.rdata' or not is_bss):
                continue
            emitted.append((start, end, content))
        if not emitted:
            continue
        kind = {'CONST': 'CONST', '_DATA': 'DATA', '_BSS': 'BSS'}[segment]
        lines.append(f"{segment} SEGMENT PARA PUBLIC USE32 '{kind}'")
        if args.reverse:
            emitted.reverse()
        for start, end, content in emitted:
            align = 16 if start % 16 == 0 else 8 if start % 8 == 0 else 4 if start % 4 == 0 else 2 if start % 2 == 0 else 1
            lines.append(f'ALIGN {align}')
            inner = sorted(a for a in labels_at if start <= a < end)
            offset = 0
            for label_address in inner + [end]:
                chunk_end = label_address - start
                while offset < chunk_end:
                    absolute = start + offset
                    if segment == '_BSS':
                        lines.append(f'    db {chunk_end - offset} dup (?)')
                        offset = chunk_end
                        break
                    if absolute % 4 == 0 and offset + 4 <= chunk_end:
                        word = struct.unpack_from('<I', content, offset)[0]
                        if looks_like_pointer(word):
                            resolved = symbol_for(word)
                            if resolved:
                                name, add = resolved
                                lines.append(f'    dd {name}' + (f'+{add}' if add else ''))
                            else:
                                unresolved.append((start, absolute, word))
                                lines.append(f'    dd {word:#010x}h ; UNRESOLVED image address'.replace('0x', '0'))
                            offset += 4
                            continue
                    lines.append(f'    db {content[offset]:#04x}h'.replace('0x', '0'))
                    offset += 1
                if label_address < end:
                    for label in labels_at[label_address]:
                        lines.append(f'PUBLIC {label}')
                        lines.append(f'{label} LABEL BYTE')
        lines.append(f'{segment} ENDS')
        lines.append('')
    externs = set()
    for line in lines:
        if line.strip().startswith('dd ') and not line.strip().startswith('dd 0'):
            externs.add(line.split()[1].split('+')[0])
    publics = {l.split()[1] for l in lines if l.startswith('PUBLIC ')}
    header = [f'EXTERN {name}:BYTE' for name in sorted(externs - publics)]
    lines = lines[:3] + header + [''] + lines[3:] + ['END', '']
    OUT.mkdir(parents=True, exist_ok=True)
    print('resources', write_resources(image, OUT / 'ProductResources.res'))
    stem = 'ProductDataReversed' if args.reverse else 'ProductData'
    (OUT / f'{stem}.asm').write_text('\r\n'.join(lines))
    import os, subprocess
    env = dict(os.environ, DISPLAY='', WAYLAND_DISPLAY='', WINEDEBUG='-all')
    for key in ('WINEPREFIX', 'WINEARCH'):
        env.pop(key, None)
    env['PATH'] = str(ROOT / '.tools/factory-bin') + ':' + env['PATH']
    (OUT / f'{stem}.obj').unlink(missing_ok=True)
    result = subprocess.run(['nice', '-n', '10', 'wine', str(ROOT / '.tools/msvc710/Vc7/bin/ml.exe'), '/nologo', '/c',
                             '/coff', '/Cp', f'/Fo{stem}.obj', f'{stem}.asm'],
                            cwd=OUT, env=env, capture_output=True, text=True, errors='replace')
    if result.returncode:
        print(result.stdout[-2000:])
        raise SystemExit('assembly failed')
    total = sum(end - start for start, end in blocks.items())
    print(f'blocks {len(blocks)}, labels {sum(len(v) for v in labels_at.values())}, bytes {total:#x}, '
          f'unresolved pointer words {len(unresolved)}')
    if args.report:
        for start, absolute, word in unresolved[:80]:
            print(f'  in {labels_at[start][0][:50]} +{absolute - start:#x}: {word:#x}')
    (OUT / 'product-data.json').write_text(json.dumps(dict(
        blocks={f'{s:#x}': f'{e:#x}' for s, e in blocks.items()},
        unresolved=[[f'{s:#x}', f'{a:#x}', f'{w:#x}'] for s, a, w in unresolved]), indent=1))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
