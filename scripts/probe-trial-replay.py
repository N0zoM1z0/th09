#!/usr/bin/env python3
"""Read a bounded TH09 replay trace at the priority-27 playback callback."""

import argparse
import atexit
import csv
import fcntl
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import runpy
import shutil
import signal
import struct
import subprocess
import sys
import threading
import time

from target_identity import load_manifest


ROOT = Path(__file__).resolve().parents[1]
DEMO_SHA256 = '1d946b2d55776a5c910b6fd9cdb96a456c02d41c5d2cbcb4bf12aff50740c2ce'
DEMO_INPUT_COUNT = 7996
# The reviewed CheckVersion failure path sets EAX to -1 and returns.
VERSION_REJECT_OFFSET = 0xB2
VERSION_REJECT_CODE = bytes.fromhex('83c8ff5b8be55dc20c00')
FIELDS = ('stage', 'frame', 'input_left', 'input_right', 'input_shared',
          'rng_seed', 'rng_draws', 'left_score', 'right_score',
          'left_x_bits', 'left_y_bits', 'right_x_bits', 'right_y_bits')


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def choose_replay(output):
    try:
        for _ in range(12):
            windows = subprocess.run(['xdotool', 'search', '--onlyvisible', '--name',
                                      'Phantasmagoria of Flower View'],
                                     stdout=subprocess.PIPE, text=True, timeout=3).stdout.split()
            for window in windows:
                geometry = subprocess.run(['xwininfo', '-id', window], capture_output=True,
                                          text=True, timeout=3).stdout
                if all(item in geometry for item in ('Width: 640', 'Height: 480', 'IsViewable')):
                    subprocess.run(['xdotool', 'windowfocus', '--sync', window], check=True, timeout=3)
                    for key in ('Down', 'Down', 'z', 'z', 'z'):
                        subprocess.run(['xdotool', 'keydown', key], check=True, timeout=3)
                        time.sleep(0.12)
                        subprocess.run(['xdotool', 'keyup', key], check=True, timeout=3)
                        time.sleep(0.80)
                    return
            time.sleep(0.5)
        raise RuntimeError('could not find the TH09 title window')
    except (OSError, RuntimeError, subprocess.SubprocessError) as exc:
        (output / 'menu-error.txt').write_text(str(exc) + '\n')


def observe():
    import gdb

    output = Path.cwd().parent
    settings = json.loads((output / 'observer.json').read_text())
    read = gdb.selected_inferior().read_memory
    trace = (output / 'trace.csv').open('w', newline='')
    atexit.register(trace.close)
    writer = csv.writer(trace)
    writer.writerow(FIELDS)

    def word(data, offset):
        return struct.unpack_from('<I', data, offset)[0]

    class Playback(gdb.Breakpoint):
        def __init__(self):
            super().__init__(f"*0x{settings['attestAddress']:08X}", gdb.BP_HARDWARE_BREAKPOINT)
            self.count = 0
            self.initial_draws = None

        def finish(self, errors, **details):
            trace.close()
            (output / 'complete.json').write_text(json.dumps(
                dict(rows=self.count, diagnosticComplete=not errors, errors=errors, **details)) + '\n')
            self.enabled = False

        def stop(self):
            try:
                manager = int(gdb.parse_and_eval('$ecx'))
                replay = read(manager, 0x11C)
                game = read(settings['gameManager'], 0x138)
                frame, flags = word(replay, 0), word(game, 0x134)
                if not frame or not flags & 4 or flags & 0x200:
                    return False
                if self.count == 0:
                    path = b'./replay/th9_01.rpy\x00'
                    if read(word(replay, 0x118), len(path)).lower() != path:
                        raise ValueError('unexpected replay path for playback')
                if frame == self.count:
                    return False
                stage = word(game, 0xFC)
                if (stage, frame) != (9, self.count + 1):
                    raise ValueError(f'expected stage 9 frame {self.count + 1}, got {stage}:{frame}')

                inputs = read(settings['inputs'], 3 * 0x8E)
                histories = [struct.unpack_from('<H', inputs, side * 0x8E + 0x2C)[0]
                             for side in range(3)]
                seed, draws = struct.unpack('<H2xI', read(settings['rng'], 8))
                if self.initial_draws is None:
                    self.initial_draws = draws
                scores = []
                coordinates = []
                for side in (0, 0x38):
                    runtime, player = word(game, side + 0x1C), word(game, side + 4)
                    scores.append(word(read(runtime + 8, 4), 0))
                    coordinates.extend(struct.unpack('<II', read(player + 0x1B88, 8)))
                writer.writerow((stage, frame, *histories, seed,
                                 (draws - self.initial_draws) & 0xFFFFFFFF, *scores, *coordinates))
                self.count += 1
                if self.count == settings['limit']:
                    self.finish([])
            except (OSError, ValueError, struct.error) as exc:
                self.finish([str(exc)])
            return False

    class StartMenu(gdb.Breakpoint):
        def stop(self):
            title = int(gdb.parse_and_eval('$ecx'))
            state, timer = struct.unpack('<II', read(title + 0x28, 8))
            if state == 1 and timer >= 10:
                self.enabled = False
                threading.Thread(target=choose_replay, args=(output,), daemon=True).start()
            return False

    class VersionRejected(gdb.Breakpoint):
        def stop(self):
            frame = int(gdb.parse_and_eval('$ebp'))
            caller, name, size, checksum = struct.unpack('<4I', read(frame + 4, 16))
            if caller == settings['periodicReturn']:
                calculation = word(read(settings['supervisor'] + 0x454, 4), 0)
                identifier = bytes(read(name, 6)).rstrip(b'\0').decode('ascii')
                capture.finish(['periodic executable version check rejected the running image'],
                               runtimeVersion=dict(identifier=identifier, size=size, checksum=checksum,
                                                   calculation=calculation, result=-1))
            return False

    capture = Playback()
    for name, breakpoint, expected in (
            ('startMenu', StartMenu, bytes.fromhex(settings['startMenuBytes'])),
            ('versionReject', VersionRejected, VERSION_REJECT_CODE)):
        address = settings[name]
        if read(address, len(expected)) != expected:
            raise ValueError(f'{name} probe does not match live instructions')
        breakpoint(f'*0x{address:08X}', gdb.BP_HARDWARE_BREAKPOINT)
    return capture


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('layout', choices=('retail', 'trial', 'trial-reversed'))
    parser.add_argument('replay', type=Path, help='demorpy0.rpy extracted from the TH09 archive')
    parser.add_argument('output', type=Path, help='new private run directory below .analysis')
    parser.add_argument('--ptrace-runner', required=True, type=Path,
                        help="Factory TH08 scripts/replay-ptrace.py hardware-breakpoint driver")
    parser.add_argument('--frames', type=int, default=600)
    parser.add_argument('--base', type=lambda value: int(value, 0), metavar='{0x400000,0x600000}',
                        help='select a separate trial PE linked at this image base')
    args = parser.parse_args()
    output = args.output.resolve()
    if sys.platform != 'linux' or os.uname().machine != 'x86_64' or not os.environ.get('DISPLAY'):
        parser.error('use Linux x86-64 with an isolated X display such as xvfb-run')
    if not output.is_relative_to(ROOT / '.analysis') or output == ROOT / '.analysis':
        parser.error('output must be a new directory below .analysis')
    if not 1 <= args.frames <= DEMO_INPUT_COUNT:
        parser.error(f'frames must be in 1..{DEMO_INPUT_COUNT}')
    replay = args.replay.read_bytes()
    if hashlib.sha256(replay).hexdigest() != DEMO_SHA256:
        parser.error('replay fixture differs from the Japanese TH09 1.50a demorpy0.rpy')
    game_data = load_manifest()['game_data']
    archive = ROOT / 'resources' / game_data['filename']
    with archive.open('rb') as stream:
        if hashlib.file_digest(stream, 'sha256').hexdigest() != game_data['sha256']:
            parser.error('th09.dat differs from the fixture archive')
    driver = args.ptrace_runner.resolve(strict=True)

    layout = load('trial_layout', ROOT / 'scripts/verify-trial-layouts.py')
    trial, coff = layout.trial, layout.coff
    if args.base is not None and (args.layout == 'retail' or args.base not in trial.IMAGE_BASES):
        parser.error('--base requires a trial layout and one of its two supported image bases')
    original = coff.verified_target()
    units = trial.load_units()
    boundary = units['replay-control-frame-advance']
    start_menu = units['title-screen-start-menu']
    version = units['supervisor-check-version']
    update = units['supervisor-on-update']
    periodic_call, = [field for field in update['relocations'] if field['symbol'] == version['symbol']]
    game_manager = '?g_GameManager@@3UAsciiGameManagerView@@A'
    inputs = '?g_ReplayInputStates@@3PAUReplayInputState@@A'
    rng = '?g_ReplayRng@@3URngRuntimeView@@A'
    supervisor = '?g_Supervisor@@3VSupervisor@@A'
    image = original
    addresses = layout.original_symbol_addresses(units)
    if args.layout != 'retail':
        reverse = args.layout == 'trial-reversed'
        stem = trial.link_stem(reverse, args.base)
        index = json.loads((trial.OUT / 'index.json').read_text())
        objects = {entry['source']: trial.OUT / entry['object'] for entry in trial.selected_carriers(index)}
        data_name = 'ProductDataReversed.obj' if reverse else 'ProductData.obj'
        layout.verify_layout(stem, data_name, original, units, objects)
        source = trial.OUT / f'{stem}.exe'
        image = source.read_bytes()
        addresses = layout.read_map(source.with_suffix('.map'))
        if args.base is not None:
            pe = layout.parse_pe(image)
            if pe['image_base'] != args.base or pe['relocations_stripped']:
                raise ValueError(f'{stem} is not a relocatable PE at the requested base')
    address = addresses[boundary['symbol']]
    menu_address = addresses[start_menu['symbol']]
    version_reject = addresses[version['symbol']] + VERSION_REJECT_OFFSET
    if coff.pe_bytes_at(image, version_reject, len(VERSION_REJECT_CODE)) != VERSION_REJECT_CODE:
        raise ValueError('the linked CheckVersion reject path differs from the reviewed instructions')
    settings = dict(attestAddress=address,
                    attestBytes=coff.pe_bytes_at(image, address, boundary['size']).hex(),
                    startMenu=menu_address,
                    startMenuBytes=coff.pe_bytes_at(image, menu_address, start_menu['size']).hex(),
                    versionReject=version_reject,
                    periodicReturn=addresses[update['symbol']] + periodic_call['offset'] + 4,
                    supervisor=addresses[supervisor],
                    gameManager=addresses[game_manager], inputs=addresses[inputs], rng=addresses[rng],
                    limit=args.frames)

    output.mkdir(parents=True)
    runtime = output / 'runtime'
    (runtime / 'replay').mkdir(parents=True)
    (runtime / 'replay' / 'th9_01.rpy').write_bytes(replay)
    executable = runtime / 'th09.exe'
    executable.write_bytes(image)
    (runtime / 'th09.dat').symlink_to(archive)
    config = runtime / 'th09.cfg'
    shutil.copyfile(ROOT / 'resources/th09.cfg', config)
    settings.update(executableSha256=hashlib.sha256(image).hexdigest(), replaySha256=DEMO_SHA256,
                    gameDataSha256=game_data['sha256'],
                    configSha256=hashlib.sha256(config.read_bytes()).hexdigest())
    (output / 'observer.json').write_text(json.dumps(settings, indent=2) + '\n')
    runner = output / 'replay-ptrace.py'
    runner.symlink_to(driver)
    (output / 'replay-external-observer.py').symlink_to(Path(__file__).resolve())
    factory = load('factory_runtime', ROOT / '.tools/factory-runtime.py')
    prefix = ROOT / '.analysis/game-prefix'
    wine, environment, _ = factory.prepare(ROOT, 'wine', prefix)
    os.environ.update(environment)
    os.environ.update(LIBGL_ALWAYS_SOFTWARE='1', LP_NUM_THREADS='1', WINEDEBUG='-all')
    os.chdir(runtime)
    sys.argv = [str(runner), '--wine', str(wine), '--executable', str(executable),
                '--timeout', str(45 + args.frames // 25)]

    def terminate(signum, frame):
        raise SystemExit(128 + signum)

    signal.signal(signal.SIGTERM, terminate)
    with (ROOT / '.analysis/factory-runtime/wine.lock').open('a') as gate:
        fcntl.flock(gate, fcntl.LOCK_EX)
        try:
            runpy.run_path(str(runner), run_name='__main__')
        finally:
            subprocess.run([str(ROOT / 'scripts/shared_runtime.py'), '--prefix', str(prefix),
                            'wineserver', '--', '-k'], check=False, timeout=10)


if __name__ == 'observer':
    observer = observe()
elif __name__ == '__main__':
    try:
        main()
    except (OSError, RuntimeError, ValueError, KeyError) as exc:
        sys.exit(f'replay capture failed: {exc}')
