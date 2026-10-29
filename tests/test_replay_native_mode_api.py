"""Reject wrong native Replay mode callees and GameManager receivers."""

import copy
import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[1]


def load(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class ReplayNativeModeApiTests(unittest.TestCase):
    def test_save_requires_actual_mode_callee_and_receiver(self):
        audit = load('native_mode_save', 'inspect-replay-save.py')
        manager = '?g_GameManager@@3UGameManagerModeView@@A'
        mode = '?IsGameMode2@GameManagerModeView@@QAEHXZ'
        rows = []
        for offset, kind, destination in audit.TARGET_FIELDS:
            choices = [(symbol, 0) for symbol, address in audit.REFERENTS.items()
                       if address == destination]
            symbol, addend = choices[0] if choices else (manager, destination - 0x4A7D90)
            rows.append(dict(offset=offset, type=kind, symbol=symbol, addend=addend))
        audit.bind_candidate(bytes(audit.SIZE), rows)
        receiver = next(i for i, row in enumerate(rows) if row['symbol'] == manager)
        call = next(i for i, row in enumerate(rows) if row['symbol'] == mode)
        mutations = (
            (receiver, {'symbol': '?g_GameManager@@3UGameManagerReplayView@@A'}),
            (receiver, {'addend': 0x58 if rows[receiver]['addend'] != 0x58 else 0x20}),
            (receiver, {'type': 'REL32'}),
            (call, {'symbol': '?IsReplayStateTwo@GameManagerReplayView@@QAEHXZ'}),
            (call, {'symbol': '?IsGameMode0@GameManagerModeView@@QAEHXZ'}),
        )
        for index, mutation in mutations:
            changed = copy.deepcopy(rows)
            changed[index].update(mutation)
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                audit.bind_candidate(bytes(audit.SIZE), changed)

    def test_playback_requires_actual_mode_callees_and_receiver(self):
        audit = load('native_mode_playback', 'inspect-replay-begin-playback.py')
        known = {symbol: {target} for symbol, _, target, _ in audit.SOURCE_FIELDS}
        rows = [dict(offset=0x10 + i * 7, symbol=symbol, type=kind, addend=addend)
                for i, (symbol, kind, _, addend) in enumerate(audit.SOURCE_FIELDS)]
        audit.link_candidate(bytes(538), rows, known)
        obsolete = {
            '?g_GameManager@@3UGameManagerModeView@@A':
                '?g_GameManager@@3UGameManagerReplayView@@A',
            '?IsGameMode0@GameManagerModeView@@QAEHXZ':
                '?IsPlaybackStateZero@GameManagerReplayView@@QAEHXZ',
            '?IsGameMode1@GameManagerModeView@@QAEHXZ':
                '?IsPlaybackStateOne@GameManagerReplayView@@QAEHXZ',
        }
        for actual, old in obsolete.items():
            changed = copy.deepcopy(rows)
            next(row for row in changed if row['symbol'] == actual)['symbol'] = old
            with self.subTest(symbol=old), self.assertRaises(ValueError):
                audit.link_candidate(bytes(538), changed, known)
        wrong_receiver = copy.deepcopy(known)
        wrong_receiver['?g_GameManager@@3UGameManagerModeView@@A'] = {0x4A7D94}
        with self.assertRaises(ValueError):
            audit.link_candidate(bytes(538), rows, wrong_receiver)


if __name__ == '__main__':
    unittest.main()
