#!/usr/bin/env python3
"""Compare a VC7 COFF owner and its checked data with the attested target.

Relocation fields are reported separately and excluded from the structural byte
score.  A 100% structural result is useful compiler-oracle evidence, but it is
not an accepted exact match: canonical promotion additionally requires a
reviewed relocation manifest and a replayable match unit.
"""

from __future__ import annotations

import argparse
from functools import lru_cache
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import tomllib
from types import SimpleNamespace


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th09.exe"
TARGET_MANIFEST = ROOT / "config" / "target.toml"
UNITS_MANIFEST = ROOT / "config" / "match-units.toml"
COFF_RELOCATION_NAMES = {0x0006: "DIR32", 0x0014: "REL32"}
PRIVATE_LABEL = re.compile(r"(\$L|\$[A-Za-z_][A-Za-z_0-9]*\$)[0-9]+")
NATIVE_CRT_DESTINATIONS = {
    "_free": 0x0047B163,
    "??3@YAXPAX@Z": 0x0047B249,
    "___CxxFrameHandler": 0x0047B2E5,
}
ECL_INTERPOLATION_UNITS = ("ecl-interpolate-linear", "ecl-interpolate-hermite")
ECL_INTERPOLATION_TABLE = "?g_EclInterpolationCallbacks@Th09EclRunControl@@3PAP6IXPAUEnemyView@@PAUTh09EclInterpolationSlotView@@M@ZA"


def read_c_string(data: bytes) -> str:
    return data.split(b"\0", 1)[0].decode("ascii")


def coff_name(raw: bytes, string_table: bytes) -> str:
    if raw[:4] == b"\0\0\0\0":
        offset = struct.unpack_from("<I", raw, 4)[0]
        if not 4 <= offset < len(string_table):
            raise ValueError(f"invalid COFF string-table offset {offset}")
        return read_c_string(string_table[offset:])
    return read_c_string(raw)


def read_coff_object(path: Path) -> tuple[bytes, list[dict], dict[int, dict]]:
    data = path.read_bytes()
    if len(data) < 20:
        raise ValueError("truncated COFF object")
    machine, section_count, _, symbol_offset, symbol_count, optional_size, _ = struct.unpack_from(
        "<HHIIIHH", data, 0
    )
    if machine != 0x14C or optional_size != 0:
        raise ValueError("expected an i386 COFF object without an optional header")

    sections = []
    section_table = 20
    for index in range(section_count):
        offset = section_table + index * 40
        if offset + 40 > len(data):
            raise ValueError("truncated COFF section table")
        fields = struct.unpack_from("<8sIIIIIIHHI", data, offset)
        sections.append(
            {
                "name_raw": fields[0],
                "size": fields[3],
                "raw_offset": fields[4],
                "reloc_offset": fields[5],
                "reloc_count": fields[7],
                "flags": fields[9],
            }
        )

    strings_offset = symbol_offset + symbol_count * 18
    if strings_offset + 4 > len(data):
        raise ValueError("truncated COFF symbol/string table")
    strings_size = struct.unpack_from("<I", data, strings_offset)[0]
    string_table = data[strings_offset : strings_offset + strings_size]

    symbols: dict[int, dict[str, object]] = {}
    raw_index = 0
    while raw_index < symbol_count:
        offset = symbol_offset + raw_index * 18
        raw_name, value, section_number, type_id, storage_class, aux_count = struct.unpack_from(
            "<8sIhHBB", data, offset
        )
        aux = data[offset + 18 : offset + 18 * (aux_count + 1)]
        symbols[raw_index] = {
            "name": coff_name(raw_name, string_table),
            "value": value,
            "section": section_number,
            "type": type_id,
            "storage": storage_class,
            "aux_count": aux_count,
            "aux": aux,
        }
        raw_index += 1 + aux_count
    return data, sections, symbols


def object_function(
    path: Path, wanted: str, expected_size: int | None = None,
    *, include_symbol_locations: bool = False,
) -> tuple[bytearray, list[dict[str, object]]]:
    data, sections, symbols = read_coff_object(path)
    matches = [
        symbol
        for symbol in symbols.values()
        if symbol["name"] == wanted and int(symbol["section"]) > 0
    ]
    if len(matches) != 1:
        raise ValueError(f"expected one defined symbol {wanted!r}, found {len(matches)}")
    symbol = matches[0]
    section = sections[int(symbol["section"]) - 1]
    if int(symbol["aux_count"]) >= 1 and len(symbol["aux"]) >= 8:
        size = struct.unpack_from("<I", symbol["aux"], 4)[0]
    else:
        if expected_size is None:
            raise ValueError("function symbol lacks a definition auxiliary record")
        if int(symbol["type"]) != 0x20 or not int(section["flags"]) & 0x20:
            raise ValueError(
                "aux-less symbol is not a function in a code section"
            )
        next_function = min(
            (int(other["value"]) for other in symbols.values()
             if other["section"] == symbol["section"] and other["type"] == 0x20
             and int(other["value"]) > int(symbol["value"])),
            default=int(section["size"]),
        )
        size = min(next_function, int(section["size"])) - int(symbol["value"])
    if size <= 0 or int(symbol["value"]) + size > int(section["size"]):
        raise ValueError("function extent exceeds its COFF section or is empty")
    start = int(section["raw_offset"]) + int(symbol["value"])
    if start + size > len(data):
        raise ValueError("function body extends beyond the object")
    code = bytearray(data[start : start + size])

    relocations = []
    for index in range(int(section["reloc_count"])):
        offset = int(section["reloc_offset"]) + index * 10
        virtual_address, target_index, type_id = struct.unpack_from("<IIH", data, offset)
        local_offset = virtual_address - int(symbol["value"])
        if not 0 <= local_offset < size:
            continue
        target_symbol = symbols.get(target_index)
        if target_symbol is None:
            raise ValueError(f"relocation references auxiliary/unknown symbol index {target_index}")
        row = {
            "offset": local_offset,
            "type_id": type_id,
            "type": COFF_RELOCATION_NAMES.get(type_id, f"0x{type_id:04X}"),
            "symbol": target_symbol["name"],
            "addend": struct.unpack_from("<I", code, local_offset)[0],
        }
        if include_symbol_locations:
            row.update({
                "symbol_section": target_symbol["section"],
                "symbol_value": target_symbol["value"],
                "owner_section": symbol["section"],
                "owner_value": symbol["value"],
            })
        relocations.append(row)
    return code, relocations


def verified_target() -> bytes:
    with TARGET_MANIFEST.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != expected["sha256"]:
        raise ValueError(f"target identity mismatch: size={len(data)} sha256={digest}")
    return data


def pe_bytes_at(data: bytes, address: int, size: int) -> bytes:
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise ValueError("target lacks a PE signature")
    section_count = struct.unpack_from("<H", data, pe_offset + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe_offset + 20)[0]
    optional = pe_offset + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    rva = address - image_base
    section_table = optional + optional_size
    for index in range(section_count):
        offset = section_table + index * 40
        _, virtual_size, section_rva, raw_size, raw_offset = struct.unpack_from("<8sIIII", data, offset)
        if section_rva <= rva and rva + size <= section_rva + max(virtual_size, raw_size):
            relative = rva - section_rva
            if relative + size > raw_size:
                raise ValueError("target range extends beyond section raw data")
            return data[raw_offset + relative : raw_offset + relative + size]
    raise ValueError("target range is outside PE sections")


def solved_relocations(
    code: bytearray, target: bytes, relocations: list[dict[str, object]], target_address: int
) -> list[dict[str, object]]:
    rows = []
    for relocation in relocations:
        offset = int(relocation["offset"])
        if offset + 4 > len(code):
            raise ValueError(f"relocation at {offset:#x} crosses the function boundary")
        type_id = int(relocation["type_id"])
        addend = int(relocation["addend"])
        target_field = struct.unpack_from("<I", target, offset)[0]
        if type_id == 0x0006:
            destination = (target_field - addend) & 0xFFFFFFFF
        elif type_id == 0x0014:
            signed_field = struct.unpack("<i", struct.pack("<I", target_field))[0]
            signed_addend = struct.unpack("<i", struct.pack("<I", addend))[0]
            destination = (target_address + offset + 4 + signed_field - signed_addend) & 0xFFFFFFFF
        else:
            destination = None
        rows.append(
            {
                "offset": f"0x{offset:X}",
                "type": relocation["type"],
                "symbol": relocation["symbol"],
                "object_addend": f"0x{addend:08X}",
                "target_field": f"0x{target_field:08X}",
                "solved_destination": None if destination is None else f"0x{destination:08X}",
            }
        )
    return rows


def compare(path: Path, symbol: str, address: int, expected_size: int) -> dict[str, object]:
    code, relocations = object_function(path, symbol, expected_size)
    target = pe_bytes_at(verified_target(), address, expected_size)
    if len(code) != expected_size:
        return {
            "result": "size-mismatch",
            "object_size": len(code),
            "target_size": expected_size,
            "symbol": symbol,
        }

    ignored = set()
    for relocation in relocations:
        offset = int(relocation["offset"])
        ignored.update(range(offset, min(offset + 4, expected_size)))
    differences = [
        {"offset": f"0x{index:X}", "object": left, "target": right}
        for index, (left, right) in enumerate(zip(code, target))
        if index not in ignored and left != right
    ]
    comparable = expected_size - len(ignored)
    return {
        "result": "structural-exact" if not differences else "mismatch",
        "symbol": symbol,
        "target_address": f"0x{address:08X}",
        "size": expected_size,
        "comparable_bytes": comparable,
        "matched_comparable_bytes": comparable - len(differences),
        "relocation_field_bytes": len(ignored),
        "relocations": solved_relocations(code, target, relocations, address),
        "first_differences": differences[:32],
    }


@lru_cache(maxsize=2)
def canonical_function_destinations(manifest_text: str) -> dict[str, frozenset[int]]:
    owners: dict[str, set[int]] = {}
    for unit in tomllib.loads(manifest_text)["units"].values():
        owners.setdefault(unit["symbol"], set()).add(int(unit["target_address"]))
    return {symbol: frozenset(addresses) for symbol, addresses in owners.items()}


def validate_symbol_destinations(expected: list[dict[str, object]]) -> None:
    """One COFF symbol has one base, before each reference's addend is applied.

    Per-reference bases can otherwise conceal a wrong source array stride or
    member offset while still reproducing every linked operand independently.
    Distinct symbols may share a destination, for example after code folding.
    A reference cannot redirect a symbol with a canonical function owner to
    another entry, even when that reference would reproduce its target bytes.
    """
    owners = canonical_function_destinations(UNITS_MANIFEST.read_text()) if expected else {}
    bindings: dict[str, tuple[int, int]] = {}
    for row in expected:
        symbol = str(row["symbol"])
        target, offset = int(row["target"]), int(row["offset"])
        if symbol in owners and target not in owners[symbol]:
            raise ValueError(
                f"canonical function owner destination differs: {symbol}, "
                f"field {offset:#x} -> {target:#x}; owners "
                + ", ".join(f"{address:#x}" for address in sorted(owners[symbol]))
            )
        if symbol in bindings and bindings[symbol][0] != target:
            previous, first_offset = bindings[symbol]
            raise ValueError(
                f"one COFF symbol has multiple manifest destinations: {symbol}, "
                f"field {first_offset:#x} -> {previous:#x}, "
                f"field {offset:#x} -> {target:#x}"
            )
        bindings[symbol] = (target, offset)


def validate_native_crt_destinations(expected: list[dict[str, object]]) -> None:
    """Keep the original CRT free entry distinct from its scalar-delete thunk.

    The pinned libcmt.lib delete.obj defines ??3 as a five-byte JMP to _free.
    Binding a game's _free reference to that thunk can yield identical linked
    bytes while misrepresenting the actual source interface across owners.
    These are reviewed identities in the one supported Japanese v1.50a target.
    """
    for row in expected:
        symbol = str(row["symbol"])
        if symbol in NATIVE_CRT_DESTINATIONS and int(row["target"]) != NATIVE_CRT_DESTINATIONS[symbol]:
            raise ValueError(f"native CRT destination differs: {symbol}, "
                             f"expected {NATIVE_CRT_DESTINATIONS[symbol]:#x}, "
                             f"manifest {int(row['target']):#x}")


@lru_cache(maxsize=1)
def target_iat(image: bytes) -> dict[int, str]:
    spec = importlib.util.spec_from_file_location(
        "match_unit_target_identity", Path(__file__).with_name("target_identity.py"))
    identity = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(identity)
    return identity.import_iat(image, lambda address, size: pe_bytes_at(image, address, size))


def validate_import_destinations(expected: list[dict], actual: list[dict], image: bytes) -> None:
    imports = [row for row in expected if row["symbol"].startswith("__imp_")]
    if not imports:
        return
    destinations: dict[str, list[int]] = {}
    for address, qualified_name in target_iat(image).items():
        name = qualified_name.split("::", 1)[1]
        destinations.setdefault(name, []).append(address)
    addends = {row["offset"]: row["addend"] for row in actual}
    for row in imports:
        symbol = row["symbol"]
        match = re.fullmatch(r"__imp__([A-Za-z_][A-Za-z0-9_]*)(?:@[0-9]+)?", symbol)
        if (not match or destinations.get(match.group(1)) != [row["target"]]
                or row["type"] != "DIR32" or addends[row["offset"]] != 0):
            raise ValueError(f"PE import identity differs for {symbol} at field {row['offset']:#x}")


def owner_local_offset(row: dict[str, object], owner_size: int) -> int | None:
    if int(row["symbol_section"]) <= 0 or row["symbol_section"] != row["owner_section"]:
        return None
    relative = int(row["symbol_value"]) - int(row["owner_value"])
    return relative if 0 <= relative < owner_size else None


def validate_owner_local_destinations(
    actual: list[dict[str, object]], expected: list[dict[str, object]],
    target_address: int, owner_size: int,
) -> None:
    """Do not let a manifest redirect a label inside the compared COFF owner.

    The symbol's position is independent evidence; recovering its destination
    from the target relocation field would conceal a misrouted switch case.
    Compare symbol bases here, before applying the object's unchanged addend.
    Other sections and symbols outside this owner still need reviewed external
    bindings; proximity or a private-looking name is not sufficient evidence.
    """
    by_offset = {int(row["offset"]): row for row in expected}
    for row in actual:
        relative = owner_local_offset(row, owner_size)
        if relative is None:
            continue
        destination = target_address + relative
        if int(by_offset[int(row["offset"])]["target"]) != destination:
            raise ValueError(
                f"owner-local COFF destination differs from manifest: "
                f"{row['symbol']} at field {int(row['offset']):#x}, "
                f"actual destination {destination:#x}"
            )


def validate_literal_definitions(path: Path, actual: list[dict], image: bytes,
                                 expected: list[dict]) -> list[dict]:
    rows = [row for row in actual
            if row["symbol"].startswith(("__real@", "??_C@"))]
    if not rows:
        return []
    spec = importlib.util.spec_from_file_location(
        "match_unit_literals", ROOT / "scripts/coff_literal_definitions.py")
    literals = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(literals)
    by_offset = {row["offset"]: row for row in expected}
    data = path.read_bytes()
    definitions = []
    for row in rows:
        destination = by_offset[row["offset"]]
        definitions.extend(literals.check_literal_definitions(
            data, [row], image,
            SimpleNamespace(pe_bytes_at=pe_bytes_at),
            {row["symbol"]: destination["target"]}))
    return definitions


def verify_ecl_interpolation_table(path: Path, image: bytes, units: dict) -> dict:
    """Read all eight callbacks from the COFF object that owns the table."""
    target_address = 0x004A0AF4
    table_size = 8 * 4
    data, sections, symbols = read_coff_object(path)
    owners = [symbol for symbol in symbols.values()
              if symbol["name"] == ECL_INTERPOLATION_TABLE and symbol["section"] > 0]
    if len(owners) != 1:
        raise ValueError("expected one defined ECL interpolation table")
    owner = owners[0]
    section = sections[owner["section"] - 1]
    start = section["raw_offset"]
    if (owner["value"] != 0 or section["size"] != table_size
            or not section["flags"] & 0x40 or start <= 0 or start + table_size > len(data)):
        raise ValueError("ECL interpolation table does not occupy its full initialized COFF section")

    fields = sorted(struct.unpack_from("<IIH", data, section["reloc_offset"] + index * 10)
                    for index in range(section["reloc_count"]))
    if [offset for offset, _, _ in fields] != list(range(0, table_size, 4)):
        raise ValueError("expected one COFF relocation per ECL interpolation table slot")
    destinations = {units[name]["symbol"]: units[name]["target_address"]
                    for name in ECL_INTERPOLATION_UNITS}
    original = pe_bytes_at(image, target_address, table_size)
    for offset, index, kind in fields:
        symbol = symbols.get(index)
        addend = struct.unpack_from("<I", data, start + offset)[0]
        if (kind != 0x0006 or addend != 0 or not symbol or symbol["section"] <= 0
                or not sections[symbol["section"] - 1]["flags"] & 0x20):
            raise ValueError(f"invalid ECL interpolation table relocation at +{offset:#x}")
        target = struct.unpack_from("<I", original, offset)[0]
        if destinations.get(symbol["name"]) != target:
            raise ValueError(f"ECL interpolation table callback differs at +{offset:#x}")
    return {"symbol": ECL_INTERPOLATION_TABLE, "target_address": f"0x{target_address:08X}",
            "size": table_size, "relocations": len(fields)}


def verify_title_result_character_order(path: Path, image: bytes,
                                        expected: list[dict], actual: list[dict]) -> dict:
    name = "?g_TitleResultCharacterOrder@@3PADA"
    address, size = 0x004A1DAC, 16
    data, sections, symbols = read_coff_object(path)
    owners = [symbol for symbol in symbols.values()
              if symbol["name"] == name and symbol["section"] > 0]
    if len(owners) != 1:
        raise ValueError("expected one defined Title result character order")
    owner = owners[0]
    section = sections[owner["section"] - 1]
    start = section["raw_offset"]
    if (owner["value"] != 0 or section["size"] != size
            or not section["flags"] & 0x40 or section["reloc_count"]
            or not 0 < start <= len(data) - size):
        raise ValueError("Title result character order does not occupy its full initialized COFF section")
    if data[start:start + size] != pe_bytes_at(image, address, size):
        raise ValueError("Title result character order bytes differ from the original")

    offsets = {row["offset"] for row in expected if row["symbol"] == name
               and row["target"] == address}
    references = [row for row in actual if row["symbol"] == name]
    if (len(references) != 2 or len(offsets) != 2
            or any(row["offset"] not in offsets or row["type_id"] != 0x0006
                   or row["addend"] != 0 or row["symbol_section"] != owner["section"]
                   or row["symbol_value"] != owner["value"] for row in references)):
        raise ValueError("Title result character order references differ from the original")
    return {"symbol": name, "target_address": f"0x{address:08X}", "size": size, "relocations": 0}


def verify_replay_name_tables(path: Path, image: bytes, expected: list[dict],
                              actual: list[dict], *, title_menu: bool = False) -> dict:
    """Verify the original replay caption section and its string definitions."""
    if title_menu:
        address = 0x004A1CF8
        tables = (
            ("?g_TitleReplayCharacterNames@@3PAPBDA", 17),
            ("?g_TitleReplayDifficultyNames@@3PAPBDA", 5),
            ("?g_TitleReplayModeNames@@3PAPBDA", 2),
            ("?g_TitleReplayMatchNames@@3PAPBDA", 4),
        )
    else:
        address = 0x004A1C78
        tables = (
            ("?g_ReplayCharacterNames@@3PAPBDA", 16),
            ("?g_ReplayModeNames@@3PAPBDA", 6),
            ("?g_ReplayDifficultyNames@@3PAPBDA", 5),
        )
    offsets = {}
    size = 0
    for symbol, count in tables:
        offsets[symbol] = size
        size += count * 4
    data, sections, symbols = read_coff_object(path)
    owners = [symbol for symbol in symbols.values()
              if symbol["name"] in offsets and symbol["section"] > 0]
    if (len(owners) != len(offsets)
            or {owner["name"]: owner["value"] for owner in owners} != offsets
            or len({owner["section"] for owner in owners}) != 1):
        raise ValueError("invalid COFF definitions for the replay name tables")
    section_index = owners[0]["section"]
    targets = {row["symbol"]: row["target"] for row in expected if row["symbol"] in offsets}
    if targets != {name: address + offset for name, offset in offsets.items()}:
        raise ValueError("replay name table bindings differ from the original")
    expected_addends = {
        row["offset"]: row.get("addend", 0) if title_menu else 0
        for row in expected if row["symbol"] in offsets
    }
    for row in actual:
        if row["symbol"] in offsets and (
                row["type_id"] != 0x0006 or row["addend"] != expected_addends.get(row["offset"])
                or row["symbol_section"] != section_index
                or row["symbol_value"] != offsets[row["symbol"]]):
            raise ValueError(f"invalid caller reference to replay name table at +{row['offset']:#x}")

    section = sections[section_index - 1]
    start = section["raw_offset"]
    if section["size"] != size or not section["flags"] & 0x40 or not 0 < start <= len(data) - size:
        raise ValueError("replay name tables do not occupy their full initialized COFF section")
    fields = sorted(struct.unpack_from("<IIH", data, section["reloc_offset"] + index * 10)
                    for index in range(section["reloc_count"]))
    if [offset for offset, _, _ in fields] != list(range(0, size, 4)):
        raise ValueError("expected one COFF relocation per replay name table slot")

    original = pe_bytes_at(image, address, size)
    literal_fields = []
    expected_literals = []
    literal_targets = {}
    for offset, index, kind in fields:
        symbol = symbols.get(index)
        if (kind != 0x0006 or data[start + offset:start + offset + 4] != bytes(4)
                or not symbol or not symbol["name"].startswith("??_C@_0")):
            raise ValueError(f"invalid replay name table entry at +{offset:#x}")
        name = symbol["name"]
        target = struct.unpack_from("<I", original, offset)[0]
        if literal_targets.setdefault(target, name) != name:
            raise ValueError(f"inconsistent literal identity for replay name at {target:#x}")
        literal_fields.append(dict(offset=offset, symbol=name, addend=0,
                                   symbol_section=symbol["section"], symbol_value=symbol["value"]))
        expected_literals.append(dict(offset=offset, target=target))

    literals = validate_literal_definitions(path, literal_fields, image, expected_literals)
    return {"symbols": list(offsets), "target_address": f"0x{address:08X}",
            "size": size, "relocations": len(fields), "literal_definitions": literals}


def verify_exception_handlers(path, owner, expected, actual, image):
    roots = [field for field in expected if field["symbol"].startswith("__ehhandler$")]
    if not roots:
        return []
    if len(roots) != 1 or roots[0]["symbol"] != "__ehhandler$" + owner:
        raise ValueError("EH handler symbol does not identify the compared caller")
    root = roots[0]
    handler = next(field for field in actual if field["offset"] == root["offset"])
    spec = importlib.util.spec_from_file_location(
        "match_unit_eh", Path(__file__).with_name("coff_exception_handlers.py"))
    verifier = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verifier)
    destinations = {
        name: next(iter(addresses)) for name, addresses in
        canonical_function_destinations(UNITS_MANIFEST.read_text()).items()
        if len(addresses) == 1
    }
    destinations.update(NATIVE_CRT_DESTINATIONS)
    return [verifier.verify(
        read_coff_object(path), handler, root["target"],
        lambda address, size: pe_bytes_at(image, address, size), destinations)]


def compare_unit(name: str) -> dict[str, object]:
    with UNITS_MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)
    units = manifest.get("units", {})
    if not isinstance(units, dict) or name not in units:
        raise ValueError(f"unknown match unit {name!r}")
    unit = units[name]
    if not isinstance(unit, dict):
        raise ValueError(f"invalid match unit {name!r}")

    size = int(unit["size"])
    compare_size = int(unit.get("compare_size", size))
    if compare_size < size:
        raise ValueError(
            f"comparison extent {compare_size:#x} is smaller than coverage size {size:#x}"
        )
    object_path = (ROOT / str(unit["object"])).resolve()
    object_path.relative_to(ROOT)
    code, actual = object_function(
        object_path, str(unit["symbol"]), compare_size, include_symbol_locations=True
    )
    address = int(unit["target_address"])
    if len(code) != compare_size:
        raise ValueError(
            f"object function size {len(code):#x} differs from manifest "
            f"comparison extent {compare_size:#x}"
        )
    image = verified_target()
    target = pe_bytes_at(image, address, compare_size)

    expected = list(unit.get("relocations", []))
    normalized_expected = []
    for relocation in expected:
        type_name = str(relocation["type"])
        type_ids = {name: value for value, name in COFF_RELOCATION_NAMES.items()}
        if type_name not in type_ids:
            raise ValueError(f"unsupported manifest relocation type {type_name!r}")
        field = {
            "offset": int(relocation["offset"]),
            "type_id": type_ids[type_name],
            "type": type_name,
            "symbol": str(relocation["symbol"]),
            "target": int(relocation["target"]),
        }
        if "addend" in relocation:
            field["addend"] = int(relocation["addend"]) & 0xFFFFFFFF
        normalized_expected.append(field)
    # Numbered VC7.1 labels can be renamed on rebuild. Their COFF definitions
    # still have to reproduce the reviewed destination within this owner.
    actual_names = {}
    expected_names = {int(row["offset"]): str(row["symbol"]) for row in normalized_expected}
    actual_key = []
    for row in actual:
        offset, type_id, symbol = int(row["offset"]), int(row["type_id"]), str(row["symbol"])
        actual_names[offset] = symbol
        expected_symbol = expected_names.get(offset, "")
        if symbol != expected_symbol:
            label = PRIVATE_LABEL.fullmatch(symbol)
            recorded_label = PRIVATE_LABEL.fullmatch(expected_symbol)
            if (label and recorded_label and label.group(1) == recorded_label.group(1)
                    and owner_local_offset(row, compare_size) is not None):
                symbol = expected_symbol
        actual_key.append((offset, type_id, symbol))
    actual_key.sort()
    expected_key = sorted(
        (int(row["offset"]), int(row["type_id"]), str(row["symbol"]))
        for row in normalized_expected
    )
    if actual_key != expected_key:
        raise ValueError(f"COFF relocations differ from manifest: actual={actual_key!r} expected={expected_key!r}")

    validate_symbol_destinations(normalized_expected)
    validate_native_crt_destinations(normalized_expected)
    validate_import_destinations(normalized_expected, actual, image)
    validate_owner_local_destinations(actual, normalized_expected, address, compare_size)
    literal_definitions = validate_literal_definitions(
        object_path, actual, image, normalized_expected)
    data_initializers = []
    if name in ECL_INTERPOLATION_UNITS:
        data_initializers.append(verify_ecl_interpolation_table(object_path, image, units))
    if name == "title-screen-update-result":
        data_initializers.append(verify_title_result_character_order(
            object_path, image, normalized_expected, actual))
    if name in ("replay-save", "title-screen-draw-replay-menu"):
        data_initializers.append(verify_replay_name_tables(
            object_path, image, normalized_expected, actual,
            title_menu=name == "title-screen-draw-replay-menu"))
    exception_handlers = verify_exception_handlers(
        object_path, unit["symbol"], normalized_expected, actual, image)

    replay = []
    for relocation in normalized_expected:
        offset = int(relocation["offset"])
        addend = struct.unpack_from("<I", code, offset)[0]
        if addend != relocation.get("addend", addend):
            raise ValueError(f"COFF addend differs from manifest at {offset:#x}")
        if relocation["type_id"] == 0x0006:
            value = int(relocation["target"]) + addend
        else:
            value = int(relocation["target"]) + addend - (address + offset + 4)
        struct.pack_into("<I", code, offset, value & 0xFFFFFFFF)
        replay.append(
            {
                "offset": f"0x{offset:X}",
                "type": relocation["type"],
                "symbol": actual_names[offset],
                "target": f"0x{int(relocation['target']):08X}",
                "addend": f"0x{addend:08X}",
            }
        )
    differences = [
        {"offset": f"0x{index:X}", "object": left, "target": right}
        for index, (left, right) in enumerate(zip(code, target))
        if left != right
    ]
    coverage_differences = sum(int(row["offset"], 0) < size for row in differences)
    return {
        "unit": name,
        "result": "exact" if not differences else "mismatch",
        "symbol": unit["symbol"],
        "target_address": f"0x{address:08X}",
        "size": size,
        "matched_bytes": size - coverage_differences,
        "compared_size": compare_size,
        "matched_compared_bytes": compare_size - len(differences),
        "object": str(object_path.relative_to(ROOT)),
        "relocations": replay,
        "literal_definitions": literal_definitions,
        "data_initializers": data_initializers,
        "exception_handlers": exception_handlers,
        "first_differences": differences[:32],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path, nargs="?")
    parser.add_argument("symbol", nargs="?")
    parser.add_argument("target_address", type=lambda value: int(value, 0), nargs="?")
    parser.add_argument("size", type=lambda value: int(value, 0), nargs="?")
    parser.add_argument("--unit")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        if args.unit:
            if any(value is not None for value in (args.object, args.symbol, args.target_address, args.size)):
                raise ValueError("--unit cannot be combined with positional diagnostic arguments")
            report = compare_unit(args.unit)
        else:
            if any(value is None for value in (args.object, args.symbol, args.target_address, args.size)):
                raise ValueError("diagnostic mode requires OBJECT SYMBOL TARGET_ADDRESS SIZE")
            report = compare(args.object.resolve(), args.symbol, args.target_address, args.size)
    except (OSError, KeyError, TypeError, ValueError, struct.error, tomllib.TOMLDecodeError) as exc:
        report = {"result": "error", "error": str(exc)}
    if args.json:
        print(json.dumps(report, indent=2))
    elif report["result"] == "error":
        print(f"error: {report['error']}")
    elif report["result"] == "size-mismatch":
        print(f"{args.symbol}: size mismatch ({report['object_size']}/{report['target_size']} bytes)")
    elif report["result"] in {"exact", "mismatch"} and "unit" in report:
        if report.get("compared_size") == report["size"]:
            detail = f"{report['matched_bytes']}/{report['size']} bytes"
        else:
            detail = (
                f"{report['matched_bytes']}/{report['size']} coverage bytes; "
                f"{report['matched_compared_bytes']}/{report['compared_size']} compared bytes"
            )
        print(f"{report['unit']}: {report['result']} ({detail})")
    else:
        print(
            f"{args.symbol}: {report['result']} "
            f"({report['matched_comparable_bytes']}/{report['comparable_bytes']} non-relocation bytes)"
        )
    return 0 if report["result"] in {"structural-exact", "exact"} else 1


if __name__ == "__main__":
    raise SystemExit(main())
