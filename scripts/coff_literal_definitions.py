"""Validate local read-only literal definitions used by a compared COFF body."""
from __future__ import annotations

import hashlib
import re
import struct


def literal_payload(data: bytes, row: dict, size: int) -> bytes:
    """Read the actual defined section payload, without trusting a symbol name."""
    if len(data) < 20:
        raise ValueError('truncated literal COFF header')
    machine, count = struct.unpack_from('<HH', data)
    optional = struct.unpack_from('<H', data, 16)[0]
    section, value = row.get('symbol_section'), row.get('symbol_value')
    if (machine != 0x14C or optional != 0 or type(section) is not int
            or not 1 <= section <= count or type(value) is not int or value < 0
            or type(size) is not int or size <= 0):
        raise ValueError('literal lacks an actual i386 COFF definition')
    header = 20 + (section - 1) * 40
    if header + 40 > len(data):
        raise ValueError('truncated literal COFF section header')
    extent, start = struct.unpack_from('<II', data, header + 16)
    flags = struct.unpack_from('<I', data, header + 36)[0]
    if (start < 20 + count * 40 or value + size > extent
            or start + extent > len(data)):
        raise ValueError('literal definition leaves its actual section')
    if not flags & 0x40 or not flags & 0x40000000 or flags & 0x80000020:
        raise ValueError('literal definition is not initialized read-only data')
    return data[start + value:start + value + size]


def check_literal_definitions(data, rows, image, coff, destinations):
    """Compare referenced scalar/C-string definitions with the original image.

    Destinations must be an independently reviewed symbol/target manifest. The
    caller still verifies every encoded operand and the complete function body.
    External globals and functions have separate ownership, not local payloads.
    """
    definitions = {}
    for row in rows:
        symbol = row['symbol']
        if not symbol.startswith(('__real@', '??_C@')):
            continue
        if symbol not in destinations:
            raise ValueError('unreviewed literal target: ' + symbol)
        target = destinations[symbol]
        if symbol.startswith('__real@'):
            if re.fullmatch(r'__real@[0-9a-fA-F]{8}', symbol) is None:
                raise ValueError('unreviewed scalar literal type: ' + symbol)
            expected = struct.pack('<I', int(symbol[7:], 16))
            if coff.pe_bytes_at(image, target, 4) != expected:
                raise ValueError('literal name/original scalar value differs: ' + symbol)
        else:
            # These reviewed Title format/heading literals are short, narrow,
            # NUL-terminated strings. Do not infer arbitrary object extents.
            original = coff.pe_bytes_at(image, target, 256)
            end = original.find(b'\0')
            if end < 0:
                raise ValueError('unbounded original C-string literal: ' + symbol)
            expected = original[:end + 1]
        if literal_payload(data, row, len(expected)) != expected:
            raise ValueError('source/original literal definition differs: ' + symbol)
        addend = row.get('addend')
        if (type(addend) is not int or not 0 <= addend < len(expected)
                or symbol.startswith('__real@') and addend != 0):
            raise ValueError('literal reference leaves its verified definition')
        identity = dict(symbol=symbol, target=target, size=len(expected),
                        sha256=hashlib.sha256(expected).hexdigest(),
                        section=row['symbol_section'], value=row['symbol_value'])
        if symbol in definitions:
            if any(definitions[symbol][key] != value for key, value in identity.items()):
                raise ValueError('inconsistent literal definition provenance')
        else:
            definitions[symbol] = dict(identity, references=[])
        definitions[symbol]['references'].append(row['offset'])
    return list(definitions.values())
