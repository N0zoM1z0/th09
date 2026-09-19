#!/usr/bin/env python3
"""Apply strong pinned-library origin findings to TH09 tracking ledgers."""
from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import sys

from tracking_csv import rewrite_selected_rows, rows_by_address


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
STRONG_EVIDENCE = {
    "pe_import_thunk",
    "pinned_coff_nonrelocation_exact",
    "pinned_associative_coff_nonrelocation_exact",
    "pinned_coff_contribution_exact",
    "pinned_associative_coff_contribution_exact",
}
EXPECTED_STRONG_FINDINGS = 1050


def load_audit():
    path = ROOT / "scripts" / "audit-runtime-origins.py"
    spec = importlib.util.spec_from_file_location("th09_runtime_origin_audit", path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def effective_origin(finding: dict[str, object]) -> str:
    if finding["evidence"] == "pe_import_thunk":
        return "import_thunk"
    if finding["classification"] == "compiler":
        return "compiler_generated"
    if finding["classification"] == "library":
        return "library"
    raise ValueError(f"unsupported strong classification: {finding['classification']}")


def subsystem(finding: dict[str, object]) -> str:
    origin = effective_origin(finding)
    if origin == "import_thunk":
        return "Imports"
    if origin == "compiler_generated":
        return "Compiler"
    archives = {
        str(row["archive"])
        for key in ("matches", "contributions")
        for row in finding.get(key, [])
    }
    if archives == {"d3dx8"}:
        return "D3DX8"
    if archives and archives.issubset({"libcmt", "libcpmt"}):
        return "CRT"
    if archives:
        return "Library"
    raise ValueError(
        f"cannot determine subsystem for {finding['address_text']}: {sorted(archives)}"
    )


def apply(write: bool, included_subsystems: set[str] | None = None) -> dict[str, object]:
    audit_module = load_audit()
    report = audit_module.audit(5)
    all_strong = [
        row for row in report["findings"] if row["evidence"] in STRONG_EVIDENCE
    ]
    if len(all_strong) != EXPECTED_STRONG_FINDINGS:
        raise ValueError(
            f"expected {EXPECTED_STRONG_FINDINGS} strong findings, got {len(all_strong)}"
        )
    findings = [
        row
        for row in all_strong
        if included_subsystems is None or subsystem(row) in included_subsystems
    ]
    if not findings:
        raise ValueError("subsystem filter selected no strong findings")
    by_address = {str(row["address_text"]): row for row in findings}
    if len(by_address) != len(findings):
        raise ValueError("runtime-origin audit returned duplicate strong addresses")
    selected = set(by_address)
    function_rows = rows_by_address(FUNCTIONS)
    origin_rows = rows_by_address(ORIGINS)
    pending: set[str] = set()
    already_applied: set[str] = set()
    for address, finding in by_address.items():
        function = function_rows[address]
        origin_row = origin_rows[address]
        expected_origin = effective_origin(finding)
        expected_subsystem = subsystem(finding)
        if function["status"] == "unclassified" and origin_row["origin"] == "unknown":
            pending.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == expected_subsystem
            and function["owner"] == expected_origin
            and function["is_thunk"] == ("true" if expected_origin == "import_thunk" else "false")
            and origin_row["origin"] == expected_origin
            and origin_row["subsystem"] == expected_subsystem
            and origin_row["disposition"] == "exclude"
            and origin_row["evidence_id"] == audit_module.AUDIT_EVIDENCE_ID
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected runtime-origin ledger state at {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        finding = by_address[row["address"]]
        row["origin"] = effective_origin(finding)
        row["subsystem"] = subsystem(finding)
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = audit_module.AUDIT_EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        finding = by_address[row["address"]]
        origin = effective_origin(finding)
        row["module"] = subsystem(finding)
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "true" if origin == "import_thunk" else "false"
        row["evidence"] = (
            f"Pinned runtime-origin audit: {finding['evidence']}; "
            f"boundary {finding['boundary_review']}"
        )
        row["owner"] = origin
        if origin == "import_thunk":
            row["notes"] = (
                f"Reviewed six-byte PE import thunk to {finding['import']} "
                f"through IAT {finding['iat']}; excluded from authored denominator"
            )
        elif origin == "compiler_generated":
            row["notes"] = (
                "Reviewed compiler-generated associative COFF contribution; "
                "excluded from authored denominator"
            )
        else:
            row["notes"] = (
                "Reviewed complete pinned-library function extent or exact COFF "
                "contribution; excluded from authored denominator"
            )

    updated_origins = rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    updated_functions = rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if updated_origins != len(pending) or updated_functions != len(pending):
        raise ValueError(
            f"incomplete ledger update: origins={updated_origins}, "
            f"functions={updated_functions}, pending={len(pending)}"
        )
    return {
        "write": write,
        "updated_origins": updated_origins,
        "updated_functions": updated_functions,
        "already_applied": len(already_applied),
        "origins": {
            origin: sum(effective_origin(row) == origin for row in findings)
            for origin in ("library", "compiler_generated", "import_thunk")
        },
        "subsystems": {
            name: sum(subsystem(row) == name for row in findings)
            for name in ("D3DX8", "CRT", "Library", "Compiler", "Imports")
        },
        "audit_counts": report["counts"],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="atomically update ledgers")
    parser.add_argument(
        "--subsystem",
        action="append",
        choices=("D3DX8", "CRT", "Library", "Compiler", "Imports"),
        help="limit updates to one or more audited subsystems",
    )
    args = parser.parse_args()
    selected = set(args.subsystem) if args.subsystem else None
    print(json.dumps(apply(args.apply, selected), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError) as exc:
        print(f"error: runtime-origin review apply failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
