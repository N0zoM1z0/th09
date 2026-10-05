#!/usr/bin/env python3
"""Complete Type11 position diagnostic; no exactness or partial byte credit.

Bindings come from TH09 direct calls and independently maintained callees.
The constructor callback at 0x4343D0 is a physical identity only: its frozen
shared/folded source ownership is deliberately not resolved here.
"""
import importlib.util
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x449D50, 732
SYMBOL = '?PlayerPositionCallback30404Type11@@YIHPAUPlayerPositionCallbackType11View@@PBUPlayerType11Point@@@Z'
DESTINATIONS = {
    '??GPlayerType11Point@@QBE?AU0@ABU0@@Z': 0x401140,
    '??HPlayerType11Point@@QBE?AU0@ABU0@@Z': 0x401100,
    '??KPlayerType11Point@@QBE?AU0@M@Z': 0x40F5A0,
    '??0PlayerType11Point@@QAE@XZ': 0x4343D0,
    '??_H@YGXPAXIHP6EPAX0@Z@Z': 0x401470,
    '?FromAngleMagnitude@PlayerType11Point@@QAEXMM@Z': 0x441890,
    '?AddNormalizeAngle@@YIMMM@Z': 0x42AED0,
    '?PlayerType11OrientationSign@@YIHPBUPlayerType11Point@@00@Z': 0x449B50,
}


if __name__ == '__main__':
    spec = importlib.util.spec_from_file_location(
        'complete_diagnostic', ROOT / 'scripts/inspect-exattack-type01.py')
    diagnostic = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(diagnostic)
    diagnostic.__doc__ = __doc__
    diagnostic.BASE, diagnostic.SIZE = BASE, SIZE
    diagnostic.SYMBOL = SYMBOL
    diagnostic.DESTINATIONS = DESTINATIONS
    diagnostic.main()
