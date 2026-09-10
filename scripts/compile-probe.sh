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

expected_sha256=$(python3 - "$repo_root" <<'PY'
from pathlib import Path
import sys
import tomllib
root = Path(sys.argv[1])
with (root / "config" / "tools.lock.toml").open("rb") as stream:
    print(tomllib.load(stream)["msvc71"]["compiler_sha256"])
PY
)
actual_sha256=$(sha256sum "$compiler" | awk '{print $1}')
if [[ "$actual_sha256" != "$expected_sha256" ]]; then
  echo "pinned VC7.1 compiler hash mismatch: $actual_sha256" >&2
  exit 1
fi

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
INCLUDE="$vc_include;$sdk_include" \
LIB="$vc_lib;$sdk_lib" \
wine "$compiler" /nologo /c "$@" "$source_win" "/Fo$output_win" "/Fd$pdb_win"
