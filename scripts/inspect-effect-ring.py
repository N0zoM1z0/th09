#!/usr/bin/env python3
"""Complete original Effect ring callback diagnostics; no match credit."""
import argparse
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OWNERS = {
    'initialize': (0x0040DBB0, 184,
        '?EffectInitializeCallbackRing@@YIHPAUEffect@@@Z',
        '0d89ac586172704744f9fdaf7e56b6d0f91a12774eb2f5fbef2a8a90e05052e1'),
    'draw': (0x0040DC70, 1028,
        '?EffectDrawCallbackRing@@YIHPAUEffect@@@Z',
        'f90abe928b89f047763b77c06f881787877b116f4e38e893dafa5a50a94f411e'),
}


def inspect(obj, role):
    spec = importlib.util.spec_from_file_location(
        'effect_ring_field_audit', ROOT/'scripts/inspect-effect-type34.py')
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
