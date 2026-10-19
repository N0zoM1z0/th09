#!/usr/bin/env python3
"""Complete original Effect 3/4 callback diagnostics; no match credit."""
import argparse
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OWNERS = {
    'initialize': (0x0040D910, 200,
        '?EffectInitializeCallbackType3Type4@@YIHPAUEffect@@@Z',
        '6d70de53f14a34898ace9b7e3c648c093605f3a5befcd8d9d025afb94ffe0788'),
    'update': (0x0040D9E0, 464,
        '?EffectUpdateCallbackType3Type4@@YIHPAUEffect@@@Z',
        '3e3c7ddbb573b061ddc4f6f0ffc85e9da4f83e4e6268002c8b9f43877105b083'),
}


def inspect(obj, role):
    spec = importlib.util.spec_from_file_location(
        'effect_trail_field_audit', ROOT/'scripts/inspect-effect-type34.py')
    audit = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(audit)
    return audit.inspect(obj, role, owners=OWNERS)[0]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--role', choices=tuple(OWNERS), required=True)
    args = parser.parse_args()
    try:
        print(json.dumps(inspect(args.object, args.role), indent=2))
    except (OSError, ValueError, AssertionError, KeyError, ImportError) as exc:
        print(json.dumps({'result': 'error', 'message': str(exc)}))
        return 2
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
