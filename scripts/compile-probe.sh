#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
msvc_root=${TH09_MSVC71_ROOT:-}
if [[ -z "$msvc_root" ]]; then
  for candidate in \
    "$repo_root/.tools/msvc710" \
    "/home/pentester/coding/codex_ida/th095-reconstruction/th095/.tools/msvc710"
  do
    if [[ -f "$candidate/Vc7/bin/cl.exe" ]]; then
      msvc_root=$candidate
      break
    fi
  done
fi

if [[ $# -lt 3 ]]; then
  echo "usage: $0 SOURCE OUTPUT.obj MSVC_FLAG..." >&2
  echo "flags are mandatory because the original compile profile is not yet proven" >&2
  exit 2
fi

source_path=$1
output_path=$2
shift 2
compiler="$msvc_root/Vc7/bin/cl.exe"

if [[ -z "$msvc_root" || ! -f "$compiler" ]]; then
  echo "missing pinned VC7.1 compiler; set TH09_MSVC71_ROOT to the msvc710 root" >&2
  exit 1
fi
if [[ ! -f "$source_path" ]]; then
  echo "missing probe source: $source_path" >&2
  exit 1
fi

python3 - "$repo_root" "$msvc_root" <<'PY'
import hashlib
from pathlib import Path
import re
import sys
import tomllib
root = Path(sys.argv[1])
tool_root = Path(sys.argv[2])
with (root / "config" / "tools.lock.toml").open("rb") as stream:
    pins = tomllib.load(stream)["msvc71"]
for name, key in (
    ("cl.exe", "compiler_sha256"),
    ("c1.dll", "c_frontend_sha256"),
    ("c1xx.dll", "cpp_frontend_sha256"),
    ("c2.dll", "backend_sha256"),
):
    expected = pins.get(key, "")
    if not isinstance(expected, str) or not re.fullmatch(r"[0-9a-fA-F]{64}", expected):
        raise SystemExit(f"missing or invalid pinned VC7.1 component hash: {name}")
    path = tool_root / "Vc7" / "bin" / name
    if not path.is_file():
        raise SystemExit(f"missing pinned VC7.1 component: {name}")
    actual = hashlib.sha256(path.read_bytes()).hexdigest()
    if actual != expected.lower():
        raise SystemExit(f"pinned VC7.1 component hash mismatch: {name} ({actual})")
PY

mkdir -p "$(dirname -- "$output_path")"
source_win=$(WINEDEBUG=-all winepath -w "$(realpath "$source_path")")
output_win=$(WINEDEBUG=-all winepath -w "$(realpath -m "$output_path")")
pdb_path="${output_path%.*}.pdb"
pdb_win=$(WINEDEBUG=-all winepath -w "$(realpath -m "$pdb_path")")
vc_include=$(WINEDEBUG=-all winepath -w "$msvc_root/Vc7/include")
sdk_include=$(WINEDEBUG=-all winepath -w "$msvc_root/Vc7/PlatformSDK/Include")
vc_lib=$(WINEDEBUG=-all winepath -w "$msvc_root/Vc7/lib")
sdk_lib=$(WINEDEBUG=-all winepath -w "$msvc_root/Vc7/PlatformSDK/Lib")

WINEDEBUG=-all \
CL= \
_CL_= \
INCLUDE="$vc_include;$sdk_include" \
LIB="$vc_lib;$sdk_lib" \
wine "$compiler" /nologo /c "$@" "$source_win" "/Fo$output_win" "/Fd$pdb_win"
