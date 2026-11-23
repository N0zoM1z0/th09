#!/usr/bin/env python3
"""Shared fail-closed identity helpers for the private TH09 target."""

from __future__ import annotations

from collections.abc import Callable
import hashlib
import os
from pathlib import Path
import struct
import tomllib
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "target.toml"


def load_manifest() -> dict[str, Any]:
    with MANIFEST.open("rb") as stream:
        return tomllib.load(stream)


def resolve_target(explicit: Path | None = None) -> Path:
    manifest = load_manifest()
    configured = os.environ.get("TH09_TARGET_PATH")
    candidate = explicit or (Path(configured) if configured else None)
    if candidate is None:
        candidate = ROOT / "resources" / str(manifest["target"]["filename"])
    return candidate.expanduser().resolve()


def file_hashes(path: Path) -> dict[str, str]:
    digests = {
        "sha256": hashlib.sha256(),
        "sha1": hashlib.sha1(),
        "md5": hashlib.md5(usedforsecurity=False),
    }
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            for digest in digests.values():
                digest.update(block)
    return {name: digest.hexdigest() for name, digest in digests.items()}


def parse_pe(image: bytes) -> dict[str, Any]:
    if len(image) < 0x40 or image[:2] != b"MZ":
        raise ValueError("target lacks an MZ header")
    pe_offset = struct.unpack_from("<I", image, 0x3C)[0]
    if pe_offset + 24 > len(image) or image[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise ValueError("target lacks a PE signature")
    machine, section_count, timestamp = struct.unpack_from("<HHI", image, pe_offset + 4)
    optional_size, characteristics = struct.unpack_from("<HH", image, pe_offset + 20)
    optional = pe_offset + 24
    if optional_size < 70 or optional + optional_size > len(image):
        raise ValueError("truncated PE optional header")
    if struct.unpack_from("<H", image, optional)[0] != 0x10B:
        raise ValueError("target is not PE32")
    linker_major, linker_minor = struct.unpack_from("<BB", image, optional + 2)
    entry_rva = struct.unpack_from("<I", image, optional + 16)[0]
    image_base = struct.unpack_from("<I", image, optional + 28)[0]
    size_of_image = struct.unpack_from("<I", image, optional + 56)[0]
    size_of_headers = struct.unpack_from("<I", image, optional + 60)[0]
    subsystem_id = struct.unpack_from("<H", image, optional + 68)[0]
    section_table = optional + optional_size
    sections = []
    for index in range(section_count):
        offset = section_table + index * 40
        if offset + 40 > len(image):
            raise ValueError("truncated PE section table")
        name = image[offset : offset + 8].rstrip(b"\0").decode("ascii")
        virtual_size, rva, raw_size, raw_offset = struct.unpack_from(
            "<IIII", image, offset + 8
        )
        if raw_offset + raw_size > len(image):
            raise ValueError(f"section {name} raw bytes exceed the file")
        sections.append(
            {
                "name": name,
                "rva": rva,
                "virtual_size": virtual_size,
                "raw_offset": raw_offset,
                "raw_size": raw_size,
                "characteristics": struct.unpack_from("<I", image, offset + 36)[0],
            }
        )
    return {
        "machine_id": machine,
        "section_count": section_count,
        "timestamp_unix": timestamp,
        "linker_version": f"{linker_major}.{linker_minor}",
        "entry_rva": entry_rva,
        "entry_point": image_base + entry_rva,
        "image_base": image_base,
        "size_of_image": size_of_image,
        "headers_raw_size": size_of_headers,
        "relocations_stripped": bool(characteristics & 0x0001),
        "subsystem_id": subsystem_id,
        "sections": sections,
    }


def verify_target(path: Path) -> tuple[dict[str, Any], list[str]]:
    manifest = load_manifest()
    expected_target = manifest["target"]
    expected_pe = manifest["pe"]
    image = path.read_bytes()
    observed = {
        "path": str(path),
        "size": len(image),
        **file_hashes(path),
        "pe": parse_pe(image),
    }
    problems: list[str] = []
    for field in ("size", "sha256", "sha1", "md5"):
        if observed[field] != expected_target[field]:
            problems.append(
                f"{field}: {observed[field]!r} (expected {expected_target[field]!r})"
            )
    numeric = {
        "machine_id": int(expected_pe["machine_id"]),
        "section_count": int(expected_pe["section_count"]),
        "timestamp_unix": int(expected_pe["timestamp_unix"]),
        "entry_rva": int(str(expected_pe["entry_rva"]), 0),
        "entry_point": int(str(expected_pe["entry_point"]), 0),
        "image_base": int(str(expected_pe["image_base"]), 0),
        "size_of_image": int(expected_pe["size_of_image"]),
        "headers_raw_size": int(expected_pe["headers_raw_size"]),
    }
    for field, wanted in numeric.items():
        actual = observed["pe"][field]
        if actual != wanted:
            problems.append(f"PE {field}: {actual!r} (expected {wanted!r})")
    for field in ("linker_version", "relocations_stripped"):
        if observed["pe"][field] != expected_pe[field]:
            problems.append(
                f"PE {field}: {observed['pe'][field]!r} "
                f"(expected {expected_pe[field]!r})"
            )
    expected_sections = expected_pe["sections"]
    if len(observed["pe"]["sections"]) == len(expected_sections):
        for actual, wanted in zip(observed["pe"]["sections"], expected_sections):
            normalized = {
                "name": wanted["name"],
                "rva": int(str(wanted["rva"]), 0),
                "virtual_size": int(wanted["virtual_size"]),
                "raw_offset": int(str(wanted["raw_offset"]), 0),
                "raw_size": int(wanted["raw_size"]),
            }
            if actual != normalized:
                problems.append(
                    f"PE section {actual.get('name', '?')}: {actual!r} "
                    f"(expected {normalized!r})"
                )
    else:
        problems.append("PE section manifest length mismatch")
    return observed, problems


def pe_bytes_at(image: bytes, address: int, size: int) -> bytes:
    facts = parse_pe(image)
    rva = address - facts["image_base"]
    for section in facts["sections"]:
        relative = rva - section["rva"]
        if 0 <= relative and relative + size <= section["raw_size"]:
            start = section["raw_offset"] + relative
            return image[start : start + size]
        if 0 <= relative < section["virtual_size"]:
            raise ValueError(f"address {address:#x} has no complete raw-byte extent")
    raise ValueError(f"address {address:#x} is not mapped by a PE section")


def import_iat(image: bytes, read: Callable[[int, int], bytes]) -> dict[int, str]:
    """Resolve named PE32 IAT slots without consulting instruction operands."""
    pe_offset = struct.unpack_from("<I", image, 0x3C)[0]
    optional_size = struct.unpack_from("<H", image, pe_offset + 20)[0]
    optional = pe_offset + 24
    if optional_size < 112 or struct.unpack_from("<H", image, optional)[0] != 0x10B:
        raise ValueError("expected PE32 import directory")
    image_base = struct.unpack_from("<I", image, optional + 28)[0]
    import_rva, import_size = struct.unpack_from("<II", image, optional + 104)
    if not import_rva or not import_size:
        return {}

    def read_string(rva: int) -> str:
        result = bytearray()
        for offset in range(4096):
            value = read(image_base + rva + offset, 1)[0]
            if value == 0:
                return result.decode("ascii")
            result.append(value)
        raise ValueError("unterminated PE import name")

    result: dict[int, str] = {}
    for offset in range(0, import_size - 19, 20):
        original, _, _, name_rva, first_thunk = struct.unpack(
            "<IIIII", read(image_base + import_rva + offset, 20))
        if not any((original, name_rva, first_thunk)):
            return result
        dll = read_string(name_rva)
        names = original or first_thunk
        thunk_offset = 0
        while True:
            value, = struct.unpack("<I", read(image_base + names + thunk_offset, 4))
            if value == 0:
                break
            name = f"Ordinal_{value & 0xFFFF}" if value & 0x80000000 else read_string(value + 2)
            result[image_base + first_thunk + thunk_offset] = f"{dll}::{name}"
            thunk_offset += 4
    raise ValueError("unterminated PE import directory")


def numeric_resources(image: bytes, read: Callable[[int, int], bytes]) -> dict[tuple[int, ...], tuple[int, bytes]]:
    """Read the type, ID and language leaves of TH09's numeric resource tree."""
    pe_offset = struct.unpack_from("<I", image, 0x3C)[0]
    optional_size = struct.unpack_from("<H", image, pe_offset + 20)[0]
    optional = pe_offset + 24
    if optional_size < 120 or struct.unpack_from("<H", image, optional)[0] != 0x10B:
        raise ValueError("expected a PE32 resource directory")
    image_base = struct.unpack_from("<I", image, optional + 28)[0]
    count = struct.unpack_from("<I", image, optional + 92)[0]
    rva, size = struct.unpack_from("<II", image, optional + 112)
    if count < 3 or not rva or not size:
        return {}
    directory = read(image_base + rva, size)
    if len(directory) != size:
        raise ValueError("truncated resource directory")

    def extent(offset, length):
        if not 0 <= offset <= len(directory) - length:
            raise ValueError("resource data extends outside the directory")
        return directory[offset:offset + length]

    entries = {}

    def walk(offset, trail):
        named, ids = struct.unpack_from("<HH", extent(offset, 16), 12)
        if named:
            raise ValueError("named resources are not expected in TH09")
        for key, pointer in struct.iter_unpack("<II", extent(offset + 16, 8 * ids)):
            path = (*trail, key)
            is_directory = bool(pointer & 0x80000000)
            if key > 0xFFFF or is_directory != (len(path) < 3):
                raise ValueError("unexpected TH09 resource tree entry")
            if is_directory:
                walk(pointer & 0x7FFFFFFF, path)
            else:
                payload, length, codepage, reserved = struct.unpack("<IIII", extent(pointer, 16))
                if reserved or path in entries:
                    raise ValueError("invalid or duplicate TH09 resource leaf")
                entries[path] = (codepage, extent(payload - rva, length))

    walk(0, ())
    return entries
