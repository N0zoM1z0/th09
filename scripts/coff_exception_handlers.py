"""Verify the VC7.1 unwind contributions owned by a TH09 COFF function."""

import struct


def owned_section(coff, number, owner, name, flag):
    data, sections, symbols = coff
    if not 1 <= number <= len(sections):
        raise ValueError(f"missing EH {name} section")
    section = sections[number - 1]
    definitions = [
        symbol for symbol in symbols.values()
        if symbol["section"] == number and symbol["name"] == name
        and symbol["storage"] == 3 and len(symbol["aux"]) >= 15
    ]
    if (len(definitions) != 1 or section["name_raw"].rstrip(b"\0") != name.encode()
            or not section["flags"] & flag or not section["flags"] & 0x1000
            or struct.unpack_from("<HB", definitions[0]["aux"], 12) != (owner, 5)):
        raise ValueError(f"EH {name} section is not associated with the caller")
    start, size = section["raw_offset"], section["size"]
    if start <= 0 or start + size > len(data):
        raise ValueError(f"EH {name} section is not file backed")
    rows = sorted(
        struct.unpack_from("<IIH", data, section["reloc_offset"] + index * 10)
        for index in range(section["reloc_count"])
    )
    fields = {}
    previous_end = 0
    for offset, index, kind in rows:
        if offset < previous_end or offset + 4 > size or index not in symbols:
            raise ValueError(f"invalid or overlapping EH {name} relocation")
        fields[offset] = (symbols[index], kind)
        previous_end = offset + 4
    return bytearray(data[start:start + size]), fields


def relocate_zero_field(code, offset, value):
    if struct.unpack_from("<I", code, offset)[0] != 0:
        raise ValueError(f"unexpected EH COFF addend at +{offset:#x}")
    struct.pack_into("<I", code, offset, value & 0xFFFFFFFF)


def compare_bytes(code, target, address, name):
    if len(code) != len(target):
        raise ValueError(f"truncated EH {name} range at {address:#010x}")
    if code != target:
        offset = next(i for i, (a, b) in enumerate(zip(code, target)) if a != b)
        raise ValueError(f"EH {name} differs at {address + offset:#010x} (+{offset:#x})")


def verify(coff, handler, target_address, read_target, destinations, expected_states=(1, 3)):
    """Follow the caller's handler through its complete code and unwind data."""
    if handler["type_id"] != 6 or handler["addend"] != 0:
        raise ValueError("EH handler reference must be a direct DIR32 field")
    code_number, entry = handler["symbol_section"], handler["symbol_value"]
    owner = handler["owner_section"]
    code, code_fields = owned_section(coff, code_number, owner, ".text$x", 0x20)

    # VC7.1 emits a MOV EAX, FuncInfo and JMP __CxxFrameHandler thunk
    # after contiguous cleanup funclets. Its unwind map precedes FuncInfo.
    thunk = read_target(target_address, 10)
    if thunk[0] != 0xB8 or thunk[5] != 0xE9:
        raise ValueError("unreviewed EH handler shape")
    info_address, = struct.unpack_from("<I", thunk, 1)
    magic, count, map_address, *extra = struct.unpack("<7I", read_target(info_address, 28))
    map_size = count * 8
    if (magic != 0x19930520 or count not in expected_states or any(extra)
            or info_address != map_address + map_size):
        raise ValueError("unreviewed EH FuncInfo shape")
    metadata = read_target(map_address, map_size + 28)
    states = list(struct.iter_unpack("<iI", metadata[:map_size]))
    actions = sorted({address for _, address in states})
    code_address = actions[0]
    if (any(state != -1 for state, _ in states) or len(actions) != count
            or code_address == 0 or actions[-1] >= target_address
            or entry != target_address - code_address or len(code) != entry + 10):
        raise ValueError("EH cleanup/handler extent differs from the target")

    info, kind = code_fields.pop(entry + 1, ({}, 0))
    if kind != 6 or info["value"] != map_size:
        raise ValueError("EH handler does not reference the FuncInfo record")
    data_number = info["section"]
    unwind, data_fields = owned_section(coff, data_number, owner, ".xdata$x", 0x40)
    if len(unwind) != len(metadata) or set(data_fields) != {
        map_size + 8, *range(4, map_size, 8)
    }:
        raise ValueError("EH unwind metadata extent or relocation set differs")
    symbol, kind = data_fields[map_size + 8]
    if kind != 6 or symbol["section"] != data_number or symbol["value"] != 0:
        raise ValueError("EH FuncInfo does not reference its unwind map")
    relocate_zero_field(unwind, map_size + 8, map_address)
    for offset, (_, address) in zip(range(4, map_size, 8), states):
        symbol, kind = data_fields[offset]
        if kind != 6 or symbol["section"] != code_number or code_address + symbol["value"] != address:
            raise ValueError(f"EH unwind action differs at slot {offset // 8}")
        relocate_zero_field(unwind, offset, code_address + symbol["value"])
    compare_bytes(unwind, metadata, map_address, "unwind metadata")

    frame_handler, kind = code_fields.pop(entry + 6, ({}, 0))
    if kind != 20 or frame_handler["name"] != "___CxxFrameHandler" or frame_handler["section"] != 0:
        raise ValueError("EH thunk does not tail-call the pinned CRT frame handler")
    relocate_zero_field(code, entry + 1, info_address)
    relocate_zero_field(code, entry + 6, destinations["___CxxFrameHandler"] - (target_address + 10))
    calls = sorted(offset for offset, (_, kind) in code_fields.items() if kind == 20)
    if len(calls) != count:
        raise ValueError("EH cleanup relocation count differs")
    for start, end, offset in zip(actions, actions[1:] + [target_address], calls):
        if not start < code_address + offset <= end - 4 or code[offset - 1] not in (0xE8, 0xE9):
            raise ValueError(f"unreviewed EH cleanup call at {code_address + offset:#010x}")
    for offset, (symbol, kind) in code_fields.items():
        destination = destinations.get(symbol["name"])
        if destination is None or not 0 < offset <= entry - 4:
            raise ValueError(f"unbound EH cleanup relocation at {code_address + offset:#010x}")
        if kind == 20:
            destination -= code_address + offset + 4
        elif kind != 6 or code[offset - 1] not in (0x05, 0x68):
            raise ValueError(f"unreviewed EH cleanup operand at {code_address + offset:#010x}")
        relocate_zero_field(code, offset, destination)
    compare_bytes(code, read_target(code_address, len(code)), code_address, "cleanup/handler code")
    return {"handler_address": f"0x{target_address:08X}", "states": count,
            "code_bytes": len(code), "metadata_bytes": len(unwind),
            "relocations": len(code_fields) + count + 3}
