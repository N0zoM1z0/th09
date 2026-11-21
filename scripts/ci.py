#!/usr/bin/env python3
"""Fast public checks; --full also runs the target-independent tooling tests."""
from pathlib import Path
import argparse
import json
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
PRIVATE_SUFFIXES = {
    '.exe', '.dll', '.dat', '.obj', '.pdb', '.gpr', '.idb', '.i64',
    '.id0', '.id1', '.id2', '.nam', '.til',
}


def check_file(path: Path) -> None:
    if not path.is_file() and not path.is_symlink():
        return
    if path.suffix.lower() in PRIVATE_SUFFIXES:
        raise ValueError(f'private binary or database is trackable: {path}')
    if path.is_symlink():
        return
    try:
        if path.suffix == '.py':
            compile(path.read_bytes(), str(path), 'exec', dont_inherit=True)
        elif path.suffix == '.json':
            json.loads(path.read_text(encoding='utf-8'))
        elif path.suffix == '.toml':
            tomllib.loads(path.read_text(encoding='utf-8'))
        elif path.suffix == '.sh':
            subprocess.run(['bash', '-n', str(path)], check=True, capture_output=True, text=True)
    except subprocess.CalledProcessError as exc:
        raise ValueError(exc.stderr.strip()) from exc
    except (SyntaxError, ValueError) as exc:
        raise ValueError(f'{path}: {exc}') from exc


# Legacy inline x87 implementation debt. These sites are not new natural-C++
# exactness credit; the budget only prevents fresh assembly from sneaking into
# source while each old owner is reconstructed and tested separately.
LEGACY_ASM_BUDGET = {
    'src/AnmManagerCameraFacing.inl': 1,
    'src/AnmManagerRender.cpp': 4,
    'src/EffectPolarScale.inl': 1,
    'src/ExAttackInitType8.inl': 1,
}


def check_source_purity() -> None:
    for path in (ROOT / 'src').rglob('*'):
        if not path.is_file() or path.suffix not in {'.c', '.cpp', '.h', '.hpp', '.inl'}:
            continue
        relative = path.relative_to(ROOT).as_posix()
        source = path.read_text(encoding='utf-8')
        if re.search(r'\b(?:_emit|__emit)\b|__declspec\s*\(\s*naked\b', source):
            raise ValueError(f'raw byte emission or naked function is forbidden: {relative}')
        asm_sites = len(re.findall(r'\b__asm\b|\basm\s*\{', source))
        allowed = LEGACY_ASM_BUDGET.get(relative, 0)
        # The operator approved restoring the paired x87 primitive here.
        if relative == 'src/EtamaControllerDraw.cpp' and re.search(
                r'\bvoid\s+__fastcall\s+fsincos\s*\([^()]*\)\s*'
                r'\{\s*__asm\s*\{[^{}]*\}\s*\}', source):
            allowed = 1
        if asm_sites > allowed:
            raise ValueError(f'new inline assembly is forbidden: {relative} ({asm_sites}, allowed {allowed})')


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--full', action='store_true',
                        help="also run the target-independent tooling tests (a few seconds)")
    args = parser.parse_args()
    try:
        names = subprocess.check_output([
            'git', 'ls-files', '--cached', '--others', '--exclude-standard', '-z',
        ], cwd=ROOT).decode().split('\0')
        for name in sorted(set(names) - {''}):
            check_file(ROOT / name)
        check_source_purity()
        subprocess.run(['git', 'diff', 'HEAD', '--check'], cwd=ROOT, check=True)
        for script in ('validate-tracking.py', 'validate-docs.py', 'progress.py'):
            command = [sys.executable, '-B', '-S', str(ROOT / 'scripts' / script)]
            if script == 'progress.py':
                command.append('--check')
            subprocess.run(command, cwd=ROOT, check=True)
        print('Basic CI passed (syntax, public files, ledgers, links, progress, whitespace).', flush=True)
        if args.full:
            (ROOT / '.analysis').mkdir(exist_ok=True)
            subprocess.run([sys.executable, '-B', '-S', '-m', 'unittest',
                            'discover', '-s', 'tests'], cwd=ROOT, check=True)
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f'error: CI failed: {exc}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
