#!/usr/bin/env python3
"""Replay a bounded, immutable Cirno-Labs source sample against our TH09 Oracle.

This is an external-source diagnostic, never a matches.csv acceptance producer.
No relocation is solved from an original operand and no bytes are masked in an
exact result. Source remains in the ignored reference checkout and CP932 mirror.
"""

from __future__ import annotations

import argparse
import ast
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
REVISION = "d7653152becabb8c86f2f43d49015ecc11bd0652"
TOOLS_REVISION = "edf50486ef7f1777f8c097155f7e79b16d1b8592"
PACKET = ROOT / ".analysis/cirno-labs-audit-20261105"
OBJECTS = ROOT / "build/reference-audit"

# Physical ownership comes from our existing reviewed TH09 extents. In
# particular, 844 is only the upstream OnUpdate code size. Its 152-byte
# physical tail must also be compared.
SAMPLES = {
    "title": [
        ("mainmenu", 0x00423D16, 1939, 82),
        ("mainmenu", 0x004276EB, 2045, 135),
        ("mainmenu", 0x00424EDD, 211, 6),
        ("mainmenu", 0x00424FB0, 175, 2),
        ("mainmenu", 0x0042505F, 175, 2),
    ],
    "system": [
        ("app", 0x0042E9E0, 213, 0),
        ("app", 0x00431110, 996, 92),
        ("unsorted", 0x0042B410, 808, 32),
    ],
}
PROFILES = {
    "mainmenu": ["/Oy-", "/Gr", "/GX", "/O1", "/Ob1"],
    "app": ["/O2", "/Ob0", "/Oy-", "/Gr", "/GX"],
    "unsorted": ["/O2", "/Ob0", "/Oy-", "/Gr", "/GX"],
}
LITERAL_EVIDENCE = {
    "mainmenu": ["inspect-title-result-draw.py", "inspect-title-options.py"],
    "app": ["inspect-supervisor-update.py"],
    "unsorted": [],
}


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def load_oracle():
    spec = importlib.util.spec_from_file_location(
        "reference_coff_oracle", ROOT / "scripts/compare-coff-function.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def git(reference: Path, *args: str) -> str:
    return subprocess.check_output(
        ["git", "-C", str(reference), *args], text=True).strip()


def source_inputs(reference: Path) -> dict:
    if git(reference, "rev-parse", "HEAD") != REVISION:
        raise ValueError("reference HEAD differs from the audited revision")
    if git(reference, "status", "--porcelain", "--untracked-files=all"):
        raise ValueError("reference checkout is dirty; preserve it and use a clean pinned copy")
    if git(reference / "tools", "rev-parse", "HEAD") != TOOLS_REVISION:
        raise ValueError("reference tools submodule differs from its audited revision")
    paths = ["project.toml", "config/symbols.csv"]
    paths += sorted(p.relative_to(reference).as_posix()
                    for p in (reference / "src").rglob("*") if p.is_file())
    return {
        "reference_commit": REVISION,
        "reference_tools_commit": TOOLS_REVISION,
        "reference_files": {p: sha((reference / p).read_bytes()) for p in paths},
        "compiler_lock_sha256": sha((ROOT / "config/tools.lock.toml").read_bytes()),
        "compile_driver_sha256": sha((ROOT / "scripts/compile-probe.sh").read_bytes()),
    }


def cp932_source(text: str) -> bytes:
    # The upstream build escapes a CP932 trail byte 0x5c for this old compiler.
    result = bytearray()
    for character in text:
        encoded = character.encode("cp932")
        result.extend(encoded)
        if len(encoded) == 2 and encoded[-1] == 0x5C:
            result.append(0x5C)
    return bytes(result)


def build_objects(reference: Path, names: list[str], inputs: dict) -> None:
    for relative in inputs["reference_files"]:
        path = Path(relative)
        if path.parts[0] == "src" and path.suffix in (".h", ".cpp"):
            destination = PACKET / path
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes(cp932_source((reference / path).read_text()))
    OBJECTS.mkdir(parents=True, exist_ok=True)
    for name in names:
        output = OBJECTS / (name + ".obj")
        with (PACKET / (name + "-build.log")).open("w") as log:
            subprocess.run(
                ["nice", "-n", "10", str(ROOT / "scripts/compile-probe.sh"),
                 str(PACKET / "src" / (name + ".cpp")), str(output), *PROFILES[name]],
                cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, timeout=180, check=True)
        receipt = dict(inputs, profile=PROFILES[name], object_sha256=sha(output.read_bytes()))
        (PACKET / (name + "-build.json")).write_text(json.dumps(receipt, indent=2) + "\n")


def verify_build(name: str, inputs: dict) -> Path:
    output = OBJECTS / (name + ".obj")
    receipt = json.loads((PACKET / (name + "-build.json")).read_text())
    expected = dict(inputs, profile=PROFILES[name], object_sha256=sha(output.read_bytes()))
    if receipt != expected:
        raise ValueError(f"stale source/profile/object proof for {name}; rebuild this group")
    return output


def coff_layout(data: bytes, oracle) -> tuple[list[dict], list[dict]]:
    machine, count, _, symoff, symcount, optional, _ = struct.unpack_from("<HHIIIHH", data)
    if machine != 0x14C or optional:
        raise ValueError("expected ordinary i386 COFF")
    string_start = symoff + symcount * 18
    string_size = struct.unpack_from("<I", data, string_start)[0]
    strings = data[string_start:string_start + string_size]
    sections = []
    for index in range(count):
        row = struct.unpack_from("<8sIIIIIIHHI", data, 20 + index * 40)
        relocations = [struct.unpack_from("<IIH", data, row[5] + i * 10)[0]
                       for i in range(row[7])]
        sections.append(dict(size=row[3], raw=row[4], flags=row[9],
                             relocation_offsets=relocations))
    symbols = []
    index = 0
    while index < symcount:
        raw, value, section, kind, storage, auxiliary = struct.unpack_from(
            "<8sIhHBB", data, symoff + index * 18)
        symbols.append(dict(name=oracle.coff_name(raw, strings), value=value,
                            section=section, kind=kind, storage=storage))
        index += auxiliary + 1
    return sections, symbols


def physical_extent(data: bytes, sections: list[dict], symbols: list[dict],
                    owner: dict, expected: int) -> tuple[int, int]:
    section = sections[owner["section"] - 1]
    next_functions = [s["value"] for s in symbols
                      if s["section"] == owner["section"] and s["value"] > owner["value"]
                      and s["kind"] & 0x30 == 0x20]
    end = min(next_functions, default=section["size"])
    extent = end - owner["value"]
    if extent < 0 or end > section["size"]:
        raise ValueError("invalid independently bounded function contribution")
    padding = data[section["raw"] + owner["value"] + expected:section["raw"] + end]
    # Permit only at most one alignment interval after our reviewed extent.
    # Switch tables and other non-padding owned bytes are never truncated.
    padding_start = owner["value"] + expected
    padding_has_fields = any(offset < end and offset + 4 > padding_start
                             for offset in section.get("relocation_offsets", []))
    if (0 < extent - expected < 16 and not padding_has_fields
            and all(b in (0x90, 0xCC) for b in padding)):
        return expected, len(padding)
    return extent, 0


def literal_bindings(name: str) -> dict[str, int]:
    result = {}
    for evidence in LITERAL_EVIDENCE[name]:
        tree = ast.parse((ROOT / "scripts" / evidence).read_text())
        assignment = next(n for n in tree.body if isinstance(n, ast.Assign)
                          and any(isinstance(t, ast.Name) and t.id == "DESTINATIONS"
                                  for t in n.targets))
        for key, value in zip(assignment.value.keys, assignment.value.values):
            if (isinstance(key, ast.Constant) and isinstance(key.value, str)
                    and key.value.startswith(("??_C@", "__real@"))
                    and isinstance(value, ast.Constant) and isinstance(value.value, int)):
                if key.value in result and result[key.value] != value.value:
                    raise ValueError("conflicting independently recorded literal addresses")
                result[key.value] = value.value
    return result


def replay(raw: bytes, rows: list[dict], base: int, size: int,
           bindings: dict[str, int]) -> tuple[bytes, list[dict], list[dict]]:
    code = bytearray(raw)
    bound, unresolved = [], []
    occupied = set()
    for row in rows:
        offset = row["offset"]
        if offset < 0 or offset + 4 > len(code) or occupied.intersection(range(offset, offset + 4)):
            raise ValueError("partial or overlapping COFF relocation")
        occupied.update(range(offset, offset + 4))
        if row["type"] not in ("DIR32", "REL32"):
            raise ValueError("unsupported relocation kind")
        relative = row["symbol_value"] - row["owner_value"]
        if row["symbol_section"] > 0 and row["symbol_section"] == row["owner_section"] and 0 <= relative < size:
            destination = base + relative
            evidence = "actual owner-local COFF definition"
        elif row["symbol"] in bindings:
            destination = bindings[row["symbol"]]
            evidence = "fixed named binding (not solved from original operand)"
        else:
            unresolved.append(row)
            continue
        value = destination + row["addend"]
        if row["type"] == "REL32":
            value -= base + offset + 4
        struct.pack_into("<I", code, offset, value & 0xFFFFFFFF)
        bound.append(dict(row, target=destination, binding_evidence=evidence))
    return bytes(code), bound, unresolved


def inspect(sample: tuple, reference: Path, inputs: dict, oracle, image: bytes) -> dict:
    name, base, size, expected_fields = sample
    path = verify_build(name, inputs)
    data = path.read_bytes()
    with (reference / "config/symbols.csv").open() as stream:
        inventory = list(csv.DictReader(stream))
    row = next(r for r in inventory if int(r["address"], 16) == base)
    bindings = {s: int(r["address"], 16) for r in inventory for s in r["symbol"].split()}
    bindings.update(literal_bindings(name))
    sections, symbols = coff_layout(data, oracle)
    owners = [s for s in symbols if s["name"] in row["symbol"].split() and s["section"] > 0]
    if len(owners) != 1:
        raise ValueError("expected one upstream function definition")
    extent, padding = physical_extent(data, sections, symbols, owners[0], size)
    raw, relocations = oracle.object_function(path, owners[0]["name"], extent,
                                             include_symbol_locations=True)
    if len(raw) != extent:
        raise ValueError("COFF auxiliary size disagrees with independently bounded extent")
    candidate, bound, unresolved = replay(raw, relocations, base, extent, bindings)
    # This verifies actual COFF literal initializers, not only decorated names.
    literals = oracle.validate_literal_definitions(path, bound, image, bound)
    target = oracle.pe_bytes_at(image, base, size)
    differences = [i for i, (a, b) in enumerate(zip(candidate, target)) if a != b]
    field_bytes = {i for r in relocations for i in range(r["offset"], r["offset"] + 4)}
    ordinary = [i for i, (a, b) in enumerate(zip(raw, target)) if a != b and i not in field_bytes]
    exact = (len(candidate) == size and len(relocations) == expected_fields
             and not unresolved and not differences)
    return dict(
        name=row["name"], symbol=owners[0]["name"], address=f"0x{base:08X}",
        source=f"src/{name}.cpp", profile=PROFILES[name], upstream_code_size=int(row["size"]),
        target_physical_size=size, candidate_physical_size=len(candidate),
        discarded_alignment_bytes=padding, target_extent_sha256=sha(target),
        raw_candidate_sha256=sha(raw), relocated_candidate_sha256=sha(candidate),
        result="complete-byte-relocation-exact" if exact else "nonexact-or-unresolved",
        expected_relocation_count=expected_fields, actual_relocation_count=len(relocations),
        ordinary_differences=len(ordinary), first_ordinary_differences=ordinary[:32],
        relocated_overlap_differences=len(differences),
        first_relocated_overlap_differences=differences[:32],
        unresolved_relocations=unresolved, relocations=bound, literal_definitions=literals,
        canonical_promotion=False)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=ROOT / "_reference/Cirno-Labs-th09")
    parser.add_argument("--group", choices=SAMPLES, action="append", required=True)
    parser.add_argument("--build", action="store_true", help="serially build only the selected translation units")
    args = parser.parse_args()
    try:
        oracle = load_oracle()
        image = oracle.verified_target()
        reference = args.reference.resolve()
        inputs = source_inputs(reference)
        groups = list(dict.fromkeys(args.group))
        samples = [s for group in groups for s in SAMPLES[group]]
        PACKET.mkdir(parents=True, exist_ok=True)
        if args.build:
            build_objects(reference, list(dict.fromkeys(s[0] for s in samples)), inputs)
        results = [inspect(s, reference, inputs, oracle, image) for s in samples]
        if source_inputs(reference) != inputs:
            raise ValueError("reference inputs changed during the audit")
        evidence_paths = {"audit-cirno-reference.py", "compare-coff-function.py",
                          "coff_literal_definitions.py"}
        evidence_paths.update(p for s in samples for p in LITERAL_EVIDENCE[s[0]])
        report = dict(inputs, target_sha256=sha(image), groups=groups, samples=results,
                      local_oracle_files={p: sha((ROOT / "scripts" / p).read_bytes())
                                          for p in sorted(evidence_paths)},
                      canonical_promotion=False, whole_repository_claim=False)
        destination = PACKET / ("-".join(groups) + "-report.json")
        destination.write_text(json.dumps(report, indent=2) + "\n")
        for result in results:
            print(f"{result['address']} {result['name']}: {result['result']}; "
                  f"{result['candidate_physical_size']}/{result['target_physical_size']} bytes; "
                  f"{result['actual_relocation_count']} fields; "
                  f"{result['ordinary_differences']} ordinary differences; "
                  f"{len(result['unresolved_relocations'])} unresolved fields")
        print(f"diagnostic report: {destination.relative_to(ROOT)}")
        # A valid audit may contain a genuine nonexact sample.
        return 0
    except (OSError, ValueError, KeyError, StopIteration, struct.error,
            subprocess.SubprocessError) as exc:
        print(f"error: reference audit rejected: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
