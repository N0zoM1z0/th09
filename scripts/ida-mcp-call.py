#!/usr/bin/env python3
"""Call an IDA MCP operation after fresh TH09 target attestation."""

from __future__ import annotations

import argparse
import asyncio
import json
import re

from ida_mcp_client import DEFAULT_SERVER, IdaMcpError, call_json, open_session, require_target


WRITE_PREFIXES = ("create_", "declare_", "delete_", "patch_", "rename_", "set_")
NEVER_ALLOWED = {"patch_address_assembles"}


async def run(
    server_name: str,
    allow_write: bool,
    schemas: list[str],
    searches: list[str],
    calls: list[tuple[str, dict[str, object]]],
) -> None:
    async with open_session(server_name) as (session, _initialized):
        tools = {tool.name: tool for tool in (await session.list_tools()).tools}
        print(json.dumps({"target": await require_target(session)}, indent=2))
        for pattern in searches:
            print(json.dumps(sorted(name for name in tools if re.search(pattern, name))))
        for name in schemas:
            if name not in tools:
                raise IdaMcpError("missing_tool", f"unknown IDA MCP tool: {name}")
            print(json.dumps({"name": name, "inputSchema": tools[name].inputSchema}, indent=2))
        for name, arguments in calls:
            if name not in tools:
                raise IdaMcpError("missing_tool", f"unknown IDA MCP tool: {name}")
            if name in NEVER_ALLOWED:
                raise IdaMcpError("unsafe_tool", f"target patching is forbidden: {name}")
            if name.startswith(WRITE_PREFIXES) and not allow_write:
                raise IdaMcpError(
                    "write_not_allowed", f"IDA metadata write requires --allow-write: {name}"
                )
            output = await call_json(session, name, arguments)
            print(json.dumps({"tool": name, "content": output}, indent=2))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--server", default=DEFAULT_SERVER)
    parser.add_argument("--allow-write", action="store_true")
    parser.add_argument("--schema", action="append", default=[], metavar="TOOL")
    parser.add_argument("--search", action="append", default=[], metavar="REGEX")
    parser.add_argument("--call", action="append", nargs=2, default=[], metavar=("TOOL", "JSON"))
    args = parser.parse_args()
    try:
        calls = [(name, json.loads(arguments)) for name, arguments in args.call]
        asyncio.run(run(args.server, args.allow_write, args.schema, args.search, calls))
    except (IdaMcpError, json.JSONDecodeError) as exc:
        reason = exc.reason if isinstance(exc, IdaMcpError) else "invalid_arguments"
        print(json.dumps({"ok": False, "reason": reason, "error": str(exc)}, indent=2))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
