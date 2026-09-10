#!/usr/bin/env python3
"""Provide the pinned Python runtime used by the IDA MCP clients."""

from __future__ import annotations

from importlib.metadata import PackageNotFoundError, version
import os
import shutil
import sys


MCP_REQUIREMENT = "mcp==1.26.0"
BOOTSTRAP_MARKER = "TH09_IDA_MCP_UV_BOOTSTRAPPED"


def installed_mcp_version() -> str | None:
    try:
        return version("mcp")
    except PackageNotFoundError:
        return None


def ensure_mcp_runtime() -> None:
    current = installed_mcp_version()
    if current == MCP_REQUIREMENT.removeprefix("mcp=="):
        return
    if os.environ.get(BOOTSTRAP_MARKER) == "1":
        raise RuntimeError(
            f"uv bootstrap did not provide {MCP_REQUIREMENT}; found {current or 'nothing'}"
        )
    uv = shutil.which("uv")
    if uv is None:
        raise RuntimeError(
            f"Python requires {MCP_REQUIREMENT}; install uv or use a compatible environment"
        )
    environment = os.environ.copy()
    environment[BOOTSTRAP_MARKER] = "1"
    command = [
        uv,
        "run",
        "--quiet",
        "--no-project",
        "--with",
        MCP_REQUIREMENT,
        "python3",
        *sys.argv,
    ]
    print(f"[th09] re-running with {MCP_REQUIREMENT}", file=sys.stderr, flush=True)
    os.execvpe(uv, command, environment)
