#!/usr/bin/env python3
"""Probe score construction/storage visibility without changing the Title profile."""
from __future__ import annotations

import argparse
import contextlib
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
CONSTRUCTOR_SYMBOL = '??0TitleScoreRecordView@@QAE@XZ'


def constructor_observation(raw, rows, original):
    """An equal byte string with unresolved fields is not a native body proof."""
    return {
        'candidate_bytes': len(raw), 'original_bytes': len(original),
        'fields': len(rows), 'native_body_equal': raw == original and rows == [],
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'raw_hex': raw.hex(), 'coff': rows,
        'exactness_credit': 'none',
    }


def source_context(source, header, constructor_source, variant):
    if variant not in ('table-storage', 'ctor-visible', 'ctor-table'):
        raise ValueError('unreviewed Title score context')
    if source.count('int TitleScreenView::DrawResult()') != 1:
        raise ValueError('DrawResult definition changed')
    if variant != 'table-storage':
        anchor = '    int InsertIntoTable();'
        constructor_anchor = 'ScoreRecordCtorView::ScoreRecordCtorView()\n'
        if header.count(anchor) != 1 or constructor_source.count(constructor_anchor) != 1:
            raise ValueError('score constructor/declaration changed')
        body = constructor_source.split(constructor_anchor)[1].strip()
        if not re.fullmatch(r'\{\s*memset\(this, 0, sizeof\(\*this\)\);\s*\}', body):
            raise ValueError('review the changed canonical constructor body')
        header = header.replace(anchor, '    TitleScoreRecordView();\n'+anchor)
        source = ('#include "TitleScoreRecordView.hpp"\n#include <string.h>\n'
                  'TitleScoreRecordView::TitleScoreRecordView()\n'+body+'\n'+source)
    if variant in ('table-storage', 'ctor-table'):
        source += '\nTitleScoreRecordView g_TitleScoreTable[16][5][5];\n'
    return source, header


VECTOR_CASES = ('vector-implicit-default', 'vector-value-constructor',
                'vector-initialized-positions')


def vector_context(source, header, variant):
    """Probe natural coordinate construction; keep layout and call ABI intact."""
    if variant not in VECTOR_CASES:
        raise ValueError('unreviewed vector context')
    original = '    Float3() {}\n    Float3(float x, float y, float z);\n'
    if header.count(original) != 1:
        raise ValueError('reviewed Float3 constructors changed')
    if source.count('int TitleScreenView::DrawResult()') != 1:
        raise ValueError('DrawResult definition changed')
    if variant == 'vector-implicit-default':
        header = header.replace(original, '')
    else:
        header = header.replace('    Float3(float x, float y, float z);',
            '    Float3(float x, float y, float z) : x(x), y(y), z(z) {}')
    if variant == 'vector-initialized-positions':
        for name in ('position', 'characterPosition'):
            declaration = 'Float3 '+name+';'
            if source.count(declaration) != 1:
                raise ValueError('reviewed coordinate local changed: '+name)
            source = source.replace(declaration, 'Float3 '+name+'(0.0f, 0.0f, 0.0f);')
    return source, header


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--variant', choices=('table-storage', 'ctor-visible', 'ctor-table')+VECTOR_CASES,
                        default='ctor-table')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    output = args.output.resolve() if args.output else None
    if output is not None:
        if not output.is_relative_to((ROOT/'.analysis').resolve()):
            raise ValueError('output must be below this repository .analysis directory')
        if output.exists():
            raise ValueError('refusing to replace an existing evidence file')

    sys.path.insert(0, str(ROOT/'scripts'))

    def module(name, filename):
        spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
        result = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(result)
        return result

    coff = module('title_score_coff', 'compare-coff-function.py')
    inspect = module('title_score_inspect', 'inspect-title-result-draw.py')
    frontier = module('title_score_frontier', 'check-boss-frontiers.py')
    image = coff.verified_target()
    units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
    profile = units['title-screen-setup-thread']['profile']
    native = units['score-record-constructor']
    if (native['target_address'], native['size'], native.get('relocations')) != (0x421C60, 18, []):
        raise ValueError('canonical original score constructor contract changed')
    original_ctor = coff.pe_bytes_at(image, native['target_address'], native['size'])
    inputs = {}

    def collect(name):
        if name in inputs:
            return
        content = (ROOT/'src'/name).read_bytes()
        inputs[name] = content
        for include in re.findall(rb'^\s*#\s*include\s*"([^"]+)"', content, re.M):
            collect((Path(name).parent/include.decode()).as_posix())

    collect('TitleScreenResultDraw.cpp')
    collect('ScoreGlobalConstructors.cpp')
    text = lambda name: inputs[name].decode().replace('\r\n', '\n')
    vector_variant = args.variant in VECTOR_CASES
    ascii_header = text('AsciiManager.hpp')
    if vector_variant:
        source, ascii_header = vector_context(text('TitleScreenResultDraw.cpp'),
                                             ascii_header, args.variant)
        header = text('TitleScoreRecordView.hpp')
    else:
        source, header = source_context(text('TitleScreenResultDraw.cpp'),
                                        text('TitleScoreRecordView.hpp'),
                                        text('ScoreGlobalConstructors.cpp'), args.variant)
    digest = lambda data: hashlib.sha256(data).hexdigest()
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none', 'variant': args.variant,
        'target_sha256': digest(image), 'profile': profile,
        'input_sha256': {name: digest(data) for name, data in inputs.items()},
        'source_sha256': digest(source.encode()), 'header_sha256': digest(header.encode()),
        'ascii_header_sha256': digest(ascii_header.encode()),
        'recipe_sha256': digest(Path(__file__).read_bytes()),
        'original_constructor_reference': native,
        'ownership': 'constructor spelling, original TU and storage owner remain unproven',
    }
    (ROOT/'build/diagnostics').mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='title-score-context-', dir=ROOT/'build/diagnostics') as tmp:
        stage = Path(tmp)
        for name, data in inputs.items():
            (stage/name).parent.mkdir(parents=True, exist_ok=True)
            (stage/name).write_bytes(data)
        (stage/'TitleScreenResultDraw.cpp').write_text(source)
        (stage/'TitleScoreRecordView.hpp').write_text(header)
        (stage/'AsciiManager.hpp').write_text(ascii_header)
        obj = stage/'TitleResultDraw.obj'
        run = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                              str(stage/'TitleScreenResultDraw.cpp'), str(obj), *profile],
                             cwd=ROOT, capture_output=True, text=True)
        result.update(compile_exit=run.returncode, compile_log=run.stdout+run.stderr)
        if run.returncode:
            raise RuntimeError(result['compile_log'])
        raw, rows = coff.object_function(obj, inspect.SYMBOL, include_symbol_locations=True)
        result.update(raw_sha256=digest(raw), raw_hex=raw.hex(), coff=rows)
        if not vector_variant and args.variant != 'table-storage':
            ctor, fields = coff.object_function(obj, CONSTRUCTOR_SYMBOL, include_symbol_locations=True)
            result['constructor'] = constructor_observation(ctor, fields, original_ctor)
        captured = io.StringIO()
        with contextlib.redirect_stdout(captured):
            with contextlib.ExitStack() as stack:
                old = sys.argv
                stack.callback(setattr, sys, 'argv', old)
                sys.argv = ['inspect-title-result-draw.py', str(obj), '--json']
                inspect.main()
        result['caller'] = json.loads(captured.getvalue())
        try:
            frontier.guard_title_draw_result(result['caller'])
            result['caller_frontier_passed'] = True
        except RuntimeError as error:
            result.update(caller_frontier_passed=False, caller_frontier_error=str(error))
    for name, data in inputs.items():
        if (ROOT/'src'/name).read_bytes() != data:
            raise ValueError('source changed during probe: '+name)
    rendered = json.dumps(result, indent=2)+'\n'
    if output:
        output.parent.mkdir(parents=True, exist_ok=True)
        with output.open('x') as stream:
            stream.write(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
