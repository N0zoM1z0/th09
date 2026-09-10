#!/usr/bin/env python3
"""Shared fail-closed identity helpers for the private TH09 target."""

from __future__ import annotations

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
    if optional + optional_size > len(image):
        raise ValueError("truncated PE optional header")
    if struct.unpack_from("<H", image, optional)[0] != 0x10B:
        raise ValueError("target is not PE32")
    linker_major, linker_minor = struct.unpack_from("<BB", image, optional + 2)
    entry_rva = struct.unpack_from("<I", image, optional + 16)[0]
    image_base = struct.unpack_from("<I", image, optional + 28)[0]
    size_of_image = struct.unpack_from("<I", image, optional + 56)[0]
    size_of_headers = struct.unpack_from("<I", image, optional + 60)[0]
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
