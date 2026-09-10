#!/usr/bin/env python3
"""Attest the live IDA database against the exact private TH09 target."""

from __future__ import annotations

import argparse
import asyncio
import json
import tomllib

from ida_mcp_client import (
    DEFAULT_SERVER,
    IdaMcpError,
    REQUIRED_READ_TOOLS,
    call_json,
    open_session,
    parse_int,
    require_target,
)
from target_identity import load_manifest, pe_bytes_at, resolve_target, verify_target


SAMPLE_ADDRESSES = (0x00401000, 0x00420000, 0x00460000, 0x0047D45F, 0x0048D000)


def parse_ida_bytes(value: object) -> bytes:
    if not isinstance(value, str):
        raise IdaMcpError("tool_error", f"unexpected IDA byte payload: {value!r}")
    try:
        return bytes(int(token, 16) for token in value.split())
    except ValueError as exc:
        raise IdaMcpError("tool_error", f"invalid IDA byte payload: {value!r}") from exc


async def check(server_name: str) -> dict[str, object]:
    manifest = load_manifest()
    expected_entry = int(str(manifest["pe"]["entry_point"]), 0)
    target_path = resolve_target()
    observed, problems = verify_target(target_path)
    if problems:
        raise IdaMcpError("target_mismatch", "; ".join(problems))
    image = target_path.read_bytes()
    async with open_session(server_name) as (session, initialized):
        listed = await session.list_tools()
        names = {tool.name for tool in listed.tools}
        missing = sorted(set(REQUIRED_READ_TOOLS) - names)
        if missing:
            raise IdaMcpError("missing_tool", f"IDA MCP missing required tools: {missing}")
        connection = await call_json(session, "check_connection", {})
        if not isinstance(connection, str) or not connection.startswith("Successfully connected"):
            raise IdaMcpError("unavailable", f"IDA plugin is not connected: {connection!r}")
        metadata = await require_target(session)
        raw_entries = await call_json(session, "get_entry_points", {})
        entries = raw_entries if isinstance(raw_entries, list) else [raw_entries]
        if not any(
            isinstance(row, dict) and parse_int(row.get("address")) == expected_entry
            for row in entries
        ):
            raise IdaMcpError("target_mismatch", f"IDA entry point mismatch: {raw_entries!r}")
        samples = []
        for address in SAMPLE_ADDRESSES:
            expected = pe_bytes_at(image, address, 16)
            actual = parse_ida_bytes(
                await call_json(
                    session,
                    "read_memory_bytes",
                    {"memory_address": f"0x{address:08X}", "size": 16},
                )
            )
            if actual != expected:
                raise IdaMcpError("target_mismatch", f"IDA bytes differ at {address:#x}")
            samples.append({"address": f"0x{address:08X}", "bytes": expected.hex()})
        first = await call_json(session, "list_functions", {"offset": 0, "count": 1})
        disassembly = await call_json(
            session, "disassemble_function", {"start_address": "0x00401000"}
        )
        decompiler = await call_json(
            session, "decompile_function", {"address": "0x00401000"}
        )
        if not isinstance(disassembly, dict) or not disassembly.get("lines"):
            raise IdaMcpError("tool_error", "IDA disassembly probe is empty")
        if not isinstance(decompiler, str) or not decompiler.strip():
            raise IdaMcpError("tool_error", "IDA decompiler probe is empty")
        return {
            "ok": True,
            "protocol_version": initialized.protocolVersion,
            "server": initialized.serverInfo.name,
            "target": metadata,
            "entry_points": entries,
            "mapped_byte_attestation": samples,
            "function_inventory_probe": first,
            "semantic_probe": {
                "address": "0x00401000",
                "disassembly_lines": len(disassembly["lines"]),
                "decompiler_characters": len(decompiler),
            },
            "disk_target_sha256": observed["sha256"],
            "boundary_rule": "IDA extents are provisional until target control flow and comparison agree",
        }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--server", default=DEFAULT_SERVER)
    args = parser.parse_args()
    try:
        output = asyncio.run(check(args.server))
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(json.dumps({"ok": False, "reason": "target_mismatch", "error": str(exc)}, indent=2))
        return 1
    except IdaMcpError as exc:
        print(json.dumps({"ok": False, "reason": exc.reason, "error": str(exc)}, indent=2))
        return 1
    print(json.dumps(output, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
