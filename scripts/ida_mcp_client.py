#!/usr/bin/env python3
"""Protocol-native helpers for the operator-managed TH09 IDA MCP."""

from __future__ import annotations

from contextlib import AsyncExitStack, asynccontextmanager
import json
import os
import subprocess
from typing import Any, AsyncIterator, TYPE_CHECKING

from mcp_runtime import ensure_mcp_runtime
from target_identity import load_manifest


if TYPE_CHECKING:
    from mcp import ClientSession, StdioServerParameters


DEFAULT_SERVER = "ida-pro-mcp"
REQUIRED_READ_TOOLS = (
    "check_connection",
    "get_metadata",
    "get_entry_points",
    "get_function_by_address",
    "disassemble_function",
    "decompile_function",
    "get_xrefs_to",
    "get_callers",
    "get_callees",
    "list_functions",
    "read_memory_bytes",
)


class IdaMcpError(RuntimeError):
    def __init__(self, reason: str, message: str):
        super().__init__(message)
        self.reason = reason


def load_server(name: str = DEFAULT_SERVER) -> StdioServerParameters:
    from mcp import StdioServerParameters

    try:
        completed = subprocess.run(
            ["codex", "mcp", "get", name, "--json"],
            check=True,
            capture_output=True,
            text=True,
        )
        config = json.loads(completed.stdout)
    except (OSError, subprocess.CalledProcessError, json.JSONDecodeError) as exc:
        raise IdaMcpError("unavailable", f"cannot load Codex MCP {name!r}: {exc}") from exc
    transport = config.get("transport") or {}
    if not config.get("enabled") or transport.get("type") != "stdio":
        raise IdaMcpError("unavailable", f"Codex MCP {name!r} is not enabled stdio")
    child_env = os.environ.copy()
    configured_env = transport.get("env")
    if isinstance(configured_env, dict):
        child_env.update({str(key): str(value) for key, value in configured_env.items()})
    return StdioServerParameters(
        command=str(transport["command"]),
        args=[str(arg) for arg in transport.get("args") or []],
        cwd=transport.get("cwd"),
        env=child_env,
    )


@asynccontextmanager
async def open_session(
    name: str = DEFAULT_SERVER,
) -> AsyncIterator[tuple[ClientSession, Any]]:
    ensure_mcp_runtime()
    from mcp import ClientSession
    from mcp.client.stdio import stdio_client

    stack = AsyncExitStack()
    try:
        read_stream, write_stream = await stack.enter_async_context(stdio_client(load_server(name)))
        session = await stack.enter_async_context(ClientSession(read_stream, write_stream))
        initialized = await session.initialize()
    except Exception as exc:
        await stack.aclose()
        if isinstance(exc, IdaMcpError):
            raise
        raise IdaMcpError("unavailable", f"IDA MCP session failed: {exc}") from exc
    try:
        yield session, initialized
    finally:
        await stack.aclose()


def decode_tool_result(result: Any, tool: str) -> Any:
    texts = [item.text for item in result.content if item.type == "text"]
    if result.isError:
        raise IdaMcpError("tool_error", f"IDA MCP tool failed: {tool}: {'; '.join(texts)}")
    if not texts:
        return None
    if len(texts) == 1:
        try:
            return json.loads(texts[0])
        except json.JSONDecodeError:
            return texts[0]
    return texts


async def call_json(session: ClientSession, tool: str, arguments: dict[str, Any]) -> Any:
    try:
        return decode_tool_result(await session.call_tool(tool, arguments), tool)
    except IdaMcpError:
        raise
    except Exception as exc:
        raise IdaMcpError("tool_error", f"IDA MCP call {tool} failed: {exc}") from exc


def parse_int(value: Any) -> int:
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        return int(value, 0)
    raise ValueError(f"not an integer: {value!r}")


def validate_metadata(metadata: dict[str, Any]) -> None:
    manifest = load_manifest()
    target = manifest["target"]
    pe = manifest["pe"]
    for field in ("sha256", "md5"):
        actual = str(metadata.get(field, "")).lower()
        wanted = str(target[field]).lower()
        if actual != wanted:
            raise IdaMcpError(
                "target_mismatch", f"IDA {field} mismatch: got {actual!r}, expected {wanted!r}"
            )
    numeric_checks = {
        "base": (metadata.get("base"), pe["image_base"]),
        "filesize": (metadata.get("filesize"), target["size"]),
        "size": (metadata.get("size"), pe["size_of_image"]),
    }
    for field, (actual_raw, wanted_raw) in numeric_checks.items():
        try:
            actual, wanted = parse_int(actual_raw), parse_int(wanted_raw)
        except (TypeError, ValueError) as exc:
            raise IdaMcpError("target_mismatch", f"invalid IDA {field}: {actual_raw!r}") from exc
        if actual != wanted:
            raise IdaMcpError(
                "target_mismatch", f"IDA {field} mismatch: got {actual:#x}, expected {wanted:#x}"
            )


async def require_target(session: ClientSession) -> dict[str, Any]:
    metadata = await call_json(session, "get_metadata", {})
    if not isinstance(metadata, dict):
        raise IdaMcpError("target_mismatch", f"unexpected IDA metadata: {metadata!r}")
    validate_metadata(metadata)
    return metadata
