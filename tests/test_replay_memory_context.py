import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('memory_context', ROOT/'scripts/probe-replay-memory-context.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class MemoryContextTests(unittest.TestCase):
    def test_actual_definitions_only(self):
        memory = (ROOT/'src/ZunMemoryRelease.cpp').read_text()
        base = (ROOT/'src/ReplayInput.cpp').read_text()
        self.assertEqual(probe.source(base, memory, 'baseline'), base)
        before, after = (probe.source(base, memory, c) for c in ('before', 'after'))
        self.assertTrue(before.endswith(base))
        self.assertTrue(after.startswith(base))
        extra = before[:-len(base)]
        self.assertEqual(extra, after[len(base):][1:]+'\n')
        self.assertIn('return malloc(size);', extra)
        self.assertIn('free(ptr);', extra)
        self.assertNotIn('AddToRegistry', extra)
        self.assertNotIn('~ZunMemory', extra)

    def test_missing_or_duplicate_boundaries_rejected(self):
        memory = (ROOT/'src/ZunMemoryRelease.cpp').read_text()
        for bad in (memory+memory, memory.replace('void *ZunMemory::Alloc(', 'void *Other('),
                    memory.replace('void ZunMemory::Free(', 'void Other(')):
            with self.assertRaises(ValueError):
                probe.source('caller', bad, 'before')
        with self.assertRaises(ValueError):
            probe.source('caller', memory, 'unknown')
