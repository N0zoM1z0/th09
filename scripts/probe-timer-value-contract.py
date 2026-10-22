#!/usr/bin/env python3
"""Prove a real stored-value Timer return model before any caller hypothesis.

Complete native equality is machine compatibility, not original C++ return-type
recovery. Maintained declarations and canonical ledgers are never changed.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def value_source(timer, header, helper):
    for signature in ('void ZunTimer::SetCurrent(int value)', 'void ZunTimer::operator=(int value)'):
        if timer.count(signature) != 1:
            raise ValueError('reviewed Timer definition is not unique')
    setter = helper.definition(timer, 'void ZunTimer::SetCurrent(int value)')
    assignment = helper.definition(timer, 'void ZunTimer::operator=(int value)')
    previous = '    this->previous = -999999;'
    call = '    this->SetCurrent(value);'
    if setter.count(previous) != 1 or assignment.count(call) != 1:
        raise ValueError('reviewed Timer mutator body changed')
    for declaration in ('void SetCurrent(int value);', 'void operator=(int value);'):
        if header.count(declaration) != 1:
            raise ValueError('reviewed Timer declaration changed')
        header = header.replace(declaration, declaration.replace('void ', 'int ', 1))
    setter = setter.replace('void ZunTimer::SetCurrent', 'int ZunTimer::SetCurrent', 1)
    setter = setter.replace(previous, previous+'\n    return this->current;')
    assignment = assignment.replace('void ZunTimer::operator=', 'int ZunTimer::operator=', 1)
    assignment = assignment.replace(call, '    return this->SetCurrent(value);')
    return '#include "ZunTimer.hpp"\n'+setter+assignment, header


def run():
    sys.path.insert(0, str(ROOT/'scripts'))
    spec = importlib.util.spec_from_file_location('value_setter', ROOT/'scripts/probe-runecl-setter-context.py')
    helper = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(helper)
    coff = helper.load('compare-coff-function.py')
    image = coff.verified_target()
    sources, pins = helper.capture_inputs()
    pins['scripts/probe-timer-value-contract.py'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    profile = units['zun-timer-set-current']['profile']
    helper.require_native_contract(units, profile)
    source, header = value_source((ROOT/'src/ZunTimer.cpp').read_text(),
                                  sources['ZunTimer.hpp'].decode(), helper)
    private_units = copy.deepcopy(units)
    for name in helper.CALLEES:
        unit = private_units[name]
        unit['symbol'] = unit['symbol'].replace('QAEXH', 'QAEHH')
        unit['relocations'] = [dict(row, symbol=row['symbol'].replace('QAEXH', 'QAEHH'))
                               for row in unit.get('relocations', [])]
    staged = {**sources, 'ZunTimer.hpp':header.encode()}
    build = ROOT/'build/diagnostics'
    build.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='timer-value-', dir=build) as temporary:
        root = Path(temporary)
        for name, data in staged.items():
            path = root/name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
        src, obj = root/'native.cpp', root/'native.obj'
        src.write_text(source)
        compiled = subprocess.run(['nice','-n','10',str(ROOT/'scripts/compile-probe.sh'),
                                   str(src),str(obj),*profile],cwd=ROOT,capture_output=True,text=True)
        if compiled.returncode:
            raise ValueError('native compiler failed: '+(compiled.stdout+compiled.stderr)[-1500:])
        proofs = helper.compare_callees(coff,image,obj,private_units,'pair-before')
        for proof in proofs:
            name = proof['unit']
            proof['hypothesis_symbol'] = private_units[name]['symbol']
            proof['canonical_reference_symbol'] = units[name]['symbol']
        helper.require_staged(root, staged)
        if src.read_text() != source:
            raise ValueError('native source changed during compilation')
    helper.require_unchanged(pins)
    digest = lambda data: hashlib.sha256(data).hexdigest()
    return dict(diagnostic_only=True,exactness_credit='none',original_return_declaration='unknown',
                target_sha256=digest(image),profile=profile,source_inputs_sha256=pins,
                recipe_sha256=pins['scripts/probe-timer-value-contract.py'],
                source_sha256=digest(source.encode()),header_sha256=digest(header.encode()),
                native_callees=proofs,compiler_artifacts_retained=False)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    output=args.output.resolve()
    if not output.is_relative_to((ROOT/'.analysis').resolve()) or output.exists():
        parser.error('output must be a new file below .analysis')
    report=run()
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(report,indent=2)+'\n')
    print('native Timer pair: 25 + 9 original bytes; one same-object field; return declaration unknown')

if __name__=='__main__':
    main()
