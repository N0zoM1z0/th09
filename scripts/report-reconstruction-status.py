#!/usr/bin/env python3
"""Report TH09 state from ledgers rather than prose."""

from __future__ import annotations

import argparse
import json

from progress import measures


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    values = measures()
    report = {
        **values,
        "target": "th09-v1.50a-original-japanese",
        "current_stage": "exact-reconstruction",
        "whole_build_closed": False,
        "windows_i386_runtime_validated": False,
        "semantic_reconstruction_started": False,
        "port_started": False,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target:                    {report['target']}")
        print(f"stage:                     {report['current_stage']}")
        print(f"function candidates:       {values['functions']}")
        print(f"origin/boundary unreviewed:{values['pending']:>5}")
        print(f"reviewed but unresolved:  {values['unresolved']:>5}")
        print(f"authored / excluded:       {values['authored']} / {values['excluded']}")
        print(f"source-present / exact:    {values['implemented']} / {values['matches']}")
        print(
            "source-present non-exact: "
            f"{values['source_nonexact']} functions / {values['source_nonexact_bytes']} bytes"
        )
        print(
            "authored without source:  "
            f"{values['without_source']} functions / {values['without_source_bytes']} bytes"
        )
        print("whole Windows i386 build:  open")
        print("semantic and port stages:  not started")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
