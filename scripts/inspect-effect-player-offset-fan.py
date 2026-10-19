#!/usr/bin/env python3
"""Complete original Effect25/shared fan diagnostics; no exactness credit."""
import argparse
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OWNERS = {
    'initialize': (0x00447C40, 155, '?EffectInitializeCallbackRadialFan@@YIHPAUEffect@@@Z',
        '3452a8267be4360ec76080a6e7b90b01c386cd57394acd58c8676ee6b835870f'),
    'update': (0x00445E00, 96, '?EffectUpdateCallbackPlayerOffsetFan@@YIHPAUEffect@@@Z',
        '08a67945a622c9d03c7b511ae9ce35d4f374a26c983dcb35863104ac241a3d46'),
    'draw': (0x00447480, 314, '?EffectDrawCallbackPlayerOffsetFan@@YIHPAUEffect@@@Z',
        'a323e9a45a707fa428a4991deeff9a06e9c31f0ee3c80ed4d55428abf4995b67'),
}


def complete_indexed_update_fields(report, indexed):
    """Add the independently reviewed sides[index].player04 static base.

    The generic fan scanner handles absolute operands. Original +0x34 instead
    reads [EDX+0x004A7D94] after the native 0x38 side-stride multiplication.
    Keep the original field inventory separate from candidate bindings.
    """
    expected = [{'offset': 0x36, 'type': 'DIR32', 'destination': 0x004A7D94}]
    if indexed != expected:
        raise ValueError('original Effect25 indexed GameManager field changed')
    fields = sorted(report['original_fields'] + indexed, key=lambda row: row['offset'])
    if len({row['offset'] for row in fields}) != len(fields):
        raise ValueError('duplicate original Effect25 field')
    actual = [{'offset': row['offset'], 'type': row['type'],
               'destination': row['effective_target']} for row in report['bindings']]
    return dict(report, original_fields=fields,
                all_original_fields_agree=actual == fields,
                field_referent_order_agrees=[(r['type'], r['destination']) for r in actual] ==
                    [(r['type'], r['destination']) for r in fields],
                indexed_global_review='Original +0x34 uses side stride0x38 and player04 at global root0x004A7D90+4')


def inspect(obj, role):
    import capstone
    spec = importlib.util.spec_from_file_location(
        'player_offset_fan_audit', ROOT/'scripts/inspect-effect-type34.py')
    audit = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(audit)
    report, original, _ = audit.inspect(obj, role, owners=OWNERS)
    indexed = []
    for ins in original:
        for op in ins.operands:
            if (op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4
                    and (op.mem.base or op.mem.index) and 0x400000 <= op.mem.disp < 0x4E7000):
                indexed.append({'offset': ins.address-OWNERS[role][0]+ins.disp_offset,
                                'type': 'DIR32', 'destination': op.mem.disp})
    if role == 'update':
        prefix = {ins.address-OWNERS[role][0]: (ins.mnemonic, ins.op_str) for ins in original}
        if (prefix.get(0x31) != ('imul', 'edx, edx, 0x38') or
                prefix.get(0x34) != ('mov', 'eax, dword ptr [edx + 0x4a7d94]') or
                prefix.get(0x3A) != ('add', 'eax, 0x1b88')):
            raise ValueError('original Effect25 side/player address sequence changed')
        report = complete_indexed_update_fields(report, indexed)
    elif indexed:
        raise ValueError('unreviewed original indexed field in Effect fan callback')
    return report


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
