#!/usr/bin/env python3
"""Direct Ghidra headless import/reuse for the original TH09 target.

Keeps a local project and exports below .analysis. Every use compares all
file-backed headers/sections against the original PE. Decompiler output and
function boundaries are provisional and confer no source exactness credit.
"""
from __future__ import annotations

import argparse
from datetime import datetime, timezone
import fcntl
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

from target_identity import ROOT, parse_pe, verify_target


TARGET_SHA256 = '10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822'
SCRIPT = ROOT / 'scripts/ghidra/Th09Export.java'
DEFAULT_FUNCTIONS = ('0x004086c0', '0x00410730', '0x00442750')


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def file_ranges(image: bytes) -> list[dict]:
    pe = parse_pe(image)
    return [dict(name='Headers', address=hex(pe['image_base']), file_offset=0,
                 length=pe['headers_raw_size'])] + [
        dict(name=section['name'], address=hex(pe['image_base'] + section['rva']),
             file_offset=section['raw_offset'], length=section['raw_size'])
        for section in pe['sections'] if section['raw_size']
    ]


def tool_identity(root: Path) -> dict:
    components = (
        'Ghidra/application.properties', 'support/analyzeHeadless',
        'Ghidra/Features/Base/lib/Base.jar',
        'Ghidra/Framework/SoftwareModeling/lib/SoftwareModeling.jar',
        'Ghidra/Features/Decompiler/lib/Decompiler.jar',
        'Ghidra/Features/Decompiler/os/linux_x86_64/decompile',
    )
    return dict(root=str(root), components={name: digest(root / name) for name in components})


def verify_report(report: dict, image: bytes, functions: list[str]) -> None:
    """Check an export against the independently verified input and selection."""
    binding = report['binding']
    if (binding.get('target_sha256') != TARGET_SHA256 or
            binding.get('program_sha256', '').lower() != TARGET_SHA256 or
            int(binding.get('image_base', '0'), 16) != 0x400000 or
            int(binding.get('entry_point', '0'), 16) != 0x47d45f or
            not binding.get('language', '').startswith('x86:LE:32:') or
            report.get('analysis_complete') is not True or
            report.get('requested_functions_complete') is not True or
            report.get('exactness_credit') != 'none'):
        raise ValueError('Ghidra target, architecture, entry or analysis binding failed')
    exported = report.get('functions', [])
    if ([int(item['address'], 16) for item in exported] !=
            [int(address, 16) for address in functions] or
            any(item.get('decompile_complete') is not True for item in exported)):
        raise ValueError('Ghidra export does not cover the requested functions')
    expected = file_ranges(image)
    observed = binding.get('file_ranges', [])
    if len(observed) != len(expected):
        raise ValueError('incomplete Ghidra mapped-file comparison')
    for actual, wanted in zip(observed, expected):
        offset, length = wanted['file_offset'], wanted['length']
        wanted = {**wanted, 'sha256': hashlib.sha256(image[offset:offset + length]).hexdigest()}
        if actual != wanted:
            raise ValueError('Ghidra mapped-file identity changed: ' + wanted['name'])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ghidra-dir', type=Path, default=ROOT / '.tools/ghidra')
    parser.add_argument('--function', action='append', dest='functions',
                        help='exact function entry address; repeat to select more')
    parser.add_argument('--output', type=Path, help='new output directory below .analysis')
    args = parser.parse_args()
    target = ROOT / 'resources/th09.exe'
    observation, problems = verify_target(target)
    if problems or observation['sha256'] != TARGET_SHA256:
        raise ValueError('original TH09 target verification failed: ' + '; '.join(problems))
    image = target.read_bytes()
    functions = [hex(int(value, 0)) for value in (args.functions or DEFAULT_FUNCTIONS)]
    pe = parse_pe(image)
    text = next(section for section in pe['sections'] if section['name'] == '.text')
    if len(set(functions)) != len(functions) or any(
            not pe['image_base'] + text['rva'] <= int(value, 16) <
            pe['image_base'] + text['rva'] + text['virtual_size'] for value in functions):
        raise ValueError('function selectors must be distinct original .text addresses')
    ghidra = args.ghidra_dir.expanduser().resolve()
    identity = dict(target_sha256=TARGET_SHA256, tool=tool_identity(ghidra))
    key = hashlib.sha256(json.dumps(identity, sort_keys=True).encode()).hexdigest()[:16]
    base = ROOT / '.analysis/ghidra-direct'
    cache = base / key
    output = (args.output or base / ('export-' + datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S')
                                    + '-' + str(os.getpid()))).resolve()
    if not output.is_relative_to((ROOT / '.analysis').resolve()):
        raise ValueError('Ghidra exports must be below .analysis')
    output.mkdir(parents=True, exist_ok=False)
    cache.mkdir(parents=True, exist_ok=True)
    for path in (base / 'home', base / 'tmp'):
        path.mkdir(exist_ok=True)
    with (cache / 'session.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        state_path = cache / 'state.json'
        reuse = state_path.exists()
        if reuse:
            if json.loads(state_path.read_text()) != identity or not (cache / 'th09.gpr').is_file():
                raise ValueError('Ghidra project identity or saved database changed')
        elif (cache / 'th09.gpr').exists() or (cache / 'th09.rep').exists():
            raise ValueError('unverified Ghidra project exists; preserve and inspect it before reuse')
        request = dict(input=str(target), output=str(output), functions=functions,
                       file_ranges=file_ranges(image))
        request_path = output / 'request.json'
        request_path.write_text(json.dumps(request, indent=2) + '\n')
        script_sha256 = digest(SCRIPT)
        runner_sha256 = digest(Path(__file__))
        command = ['nice', '-n', '10', 'timeout', '--signal=TERM', '--kill-after=10s', '600s',
                   str(ghidra / 'support/analyzeHeadless'), str(cache), 'th09',
                   *(['-process', 'th09.exe', '-noanalysis', '-readOnly'] if reuse else
                     ['-import', str(target), '-analysisTimeoutPerFile', '300']),
                   '-max-cpu', '2', '-scriptPath', str(SCRIPT.parent),
                   '-preScript', SCRIPT.name, str(request_path), 'verify',
                   '-postScript', SCRIPT.name, str(request_path), 'export',
                   '-log', str(output / 'ghidra.log'),
                   '-scriptlog', str(output / 'script.log')]
        env = dict(os.environ, GHIDRA_HEADLESS_MAXMEM='2G',
                   GHIDRA_HEADLESS_JAVA_OPTIONS=(
                       f'-Duser.home={base / "home"} -Djava.io.tmpdir={base / "tmp"} '
                       '-XX:ActiveProcessorCount=2'))
        # Ghidra rejects hidden path components in project URLs. A temporary
        # visible alias keeps the actual database below .analysis.
        with tempfile.TemporaryDirectory(prefix='th09-ghidra-') as temporary:
            alias = Path(temporary) / 'project'
            alias.symlink_to(cache, target_is_directory=True)
            command[8] = str(alias)
            with (output / 'launcher.log').open('w') as log:
                run = subprocess.run(command, cwd=ROOT, env=env, stdout=log, stderr=subprocess.STDOUT)
        receipt = dict(command=command, exit=run.returncode, reused_project=reuse,
                       identity=identity, script_sha256=script_sha256,
                       runner_sha256=runner_sha256)
        (output / 'run.json').write_text(json.dumps(receipt, indent=2) + '\n')
        if run.returncode or (output / 'failure.json').exists() or not (output / 'report.json').exists():
            raise RuntimeError('direct Ghidra analysis failed; inspect ' + str(output / 'launcher.log'))
        report = json.loads((output / 'report.json').read_text())
        verify_report(report, image, functions)
        for function in report['functions']:
            filename = function['pseudocode_file']
            if Path(filename).name != filename or digest(output / filename) != function['pseudocode_sha256']:
                raise ValueError('Ghidra pseudocode file changed or escaped the output directory')
        if digest(target) != TARGET_SHA256:
            raise ValueError('original target changed during Ghidra analysis')
        if (digest(SCRIPT) != script_sha256 or digest(Path(__file__)) != runner_sha256 or
                tool_identity(ghidra) != identity['tool']):
            raise ValueError('Ghidra scripts or tool inputs changed during analysis')
        if not reuse:
            state_path.write_text(json.dumps(identity, indent=2) + '\n')
    print(json.dumps(dict(output=str(output), project=str(cache / 'th09.gpr'),
                          reused_project=reuse, ghidra_version=report['binding']['ghidra_version'],
                          mapped_file_bytes=sum(item['length'] for item in report['binding']['file_ranges']),
                          functions=len(report['functions']), exactness_credit='none'), indent=2))
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError, KeyError) as error:
        print('Ghidra analysis rejected:', error, file=sys.stderr)
        raise SystemExit(1)
