#!/usr/bin/env python3
"""Cold, serial ScreenEffect context controls; no exactness credit.

Compare actual Timer/RNG definitions and a repeated-field source hypothesis
against the whole original 336-byte owner. The default builds only baseline.
--prepare-only reports reproducible source hashes without compiling or deleting
previous outputs. Generated C++/COFF/receipts belong below .analysis/.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tomllib

ROOT = Path(__file__).resolve().parents[1]
PROFILE = ('/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF',
           '/Oi', '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src')
INPUTS = ('ScreenEffect.cpp', 'ZunTimer.cpp', 'BackgroundStageScript.cpp',
          'RngRuntimeLeaves.cpp')


def module(name: str, file: str):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/file)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def function_body(source: str, signature: str) -> str:
    marker = signature + '\n{'
    if source.count(marker) != 1:
        raise ValueError('review actual callee definition: ' + signature)
    start = source.index(marker)
    return source[start:source.index('\n}', start) + 2] + '\n'


def sources() -> tuple[dict[str, str], dict[str, str]]:
    raw = {name: (ROOT/'src'/name).read_bytes() for name in INPUTS}
    text = {name: data.decode().replace('\r\n', '\n') for name, data in raw.items()}
    base = text['ScreenEffect.cpp']
    timer = text['ZunTimer.cpp']
    less = function_body(text['BackgroundStageScript.cpp'],
                         'unsigned int ZunTimer::operator<(int value)')
    pre = '#include "ZunTimer.hpp"\n'
    variants = {'baseline': base}
    for name, body in (
        ('rng', text['RngRuntimeLeaves.cpp']),
        ('timer-float', pre + function_body(timer, 'ZunTimer::operator float()')),
        ('timer-increment', pre + function_body(timer, 'void ZunTimer::operator++(int)')),
        ('timer-less', pre + less),
        ('timer-all', timer),
    ):
        variants[name] = body + '\n' + base

    # Adjacent TH095 suggests repeated field expressions. This is solely a
    # source-shape hypothesis; every address/layout/byte claim remains TH09-local.
    start = base.index('int ScreenEffect::CalcShakeEnvelope(')
    end = base.index('int ScreenEffect::InitializeTimer(', start)
    owner = base[start:end]
    a = owner.index('    int endFrame =')
    b = owner.index('    shakeAmount = (float)screenEffect->duration', a)
    endpoint = ('screenEffect->rawParameter0 + screenEffect->rawParameter1 + '
                'screenEffect->rawParameter2')
    phases = '''    if (screenEffect->timer < screenEffect->rawParameter0)
    {
        shakeAmount = (float)screenEffect->timer / screenEffect->rawParameter0;
    }
    else if (screenEffect->timer < screenEffect->rawParameter0 + screenEffect->rawParameter1)
    {
        shakeAmount = 1.0f;
    }
    else if (screenEffect->timer < ENDPOINT)
    {
        shakeAmount = ((float)(unsigned int)(ENDPOINT) - (float)screenEffect->timer) /
            (unsigned int)screenEffect->rawParameter2;
    }
    else
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB;
    }

'''.replace('ENDPOINT', endpoint)
    fields = base[:start] + owner[:a] + phases + owner[b:] + base[end:]
    variants['direct-fields'] = fields
    variants['direct-fields-timer-less'] = pre + less + '\n' + fields
    return variants, {name: hashlib.sha256(data).hexdigest() for name, data in raw.items()}


def profile() -> list[str]:
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        unit = tomllib.load(stream)['units']['screen-effect-calc-shake']
    if (unit['source'] != 'src/ScreenEffect.cpp' or
            unit['symbol'] != '?CalcShake@ScreenEffect@@SIHPAU1@@Z' or
            unit['target_address'] != 0x00422660 or unit['size'] != 316 or
            unit['profile'] != list(PROFILE)):
        raise ValueError('ScreenEffect source/profile reference changed; review required')
    return list(PROFILE)


def run_variant(destination: Path, source: str, flags: list[str]) -> dict:
    # Verify identity before touching old outputs or invoking the compiler.
    module('screen_context_target', 'compare-coff-function.py').verified_target()
    destination = Path(destination)
    destination.resolve().relative_to((ROOT/'.analysis').resolve())
    paths = [destination/name for name in
             ('ScreenEffect.cpp', 'ScreenEffect.obj', 'result.json', 'compiler.log')]
    if destination.is_symlink() or any(p.is_symlink() for p in paths):
        raise ValueError('refusing symlinked ScreenEffect probe outputs')
    destination.mkdir(parents=True, exist_ok=True)
    src, obj, report, log = paths
    for path in (obj, report, log):
        path.unlink(missing_ok=True)
    src.write_text(source, encoding='utf-8')
    row = {'diagnostic_only': True, 'exactness_credit': 'none',
           'source_sha256': hashlib.sha256(src.read_bytes()).hexdigest(),
           'profile': flags}
    command = [str(ROOT/'scripts/compile-probe.sh'), str(src), str(obj), *flags]
    compiled = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    log.write_text(compiled.stdout + compiled.stderr)
    row['compile_exit'] = compiled.returncode
    if compiled.returncode or not obj.is_file():
        row['error'] = 'cold compiler failed or produced no object; inspect compiler.log'
    else:
        row['object_sha256'] = hashlib.sha256(obj.read_bytes()).hexdigest()
        try:
            row['diagnostic'] = module('screen_context_audit',
                                      'inspect-screen-shake-envelope.py').diagnose(obj)
        except (ValueError, RuntimeError) as error:
            row['error'] = str(error)
    report.write_text(json.dumps(row, indent=2) + '\n')
    return row


def main() -> int:
    variants, inputs = sources()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--variant', choices=tuple(variants), default='baseline')
    parser.add_argument('--all', action='store_true')
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--output', type=Path, default=ROOT/'.analysis/screen-shake-context')
    args = parser.parse_args()
    selected = tuple(variants) if args.all else (args.variant,)
    if args.prepare_only:
        print(json.dumps({'codegen_attempted': False, 'input_sha256': inputs,
                          'source_sha256': {name: hashlib.sha256(variants[name].encode()).hexdigest()
                                            for name in selected}}, indent=2))
        return 0
    flags = profile()
    os.nice(10)
    rows = {name: run_variant(args.output/name, variants[name], flags) for name in selected}
    print(json.dumps({'diagnostic_only': True, 'exactness_credit': 'none',
                      'input_sha256': inputs, 'rows': rows}, indent=2))
    return 2 if any('error' in row for row in rows.values()) else 0


if __name__ == '__main__':
    raise SystemExit(main())
