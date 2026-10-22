"""Stored-value source models cannot silently substitute different mutators."""
import importlib.util
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]
def module(name,file):
    spec=importlib.util.spec_from_file_location(name,ROOT/'scripts'/file)
    result=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result
probe=module('timer_value','probe-timer-value-contract.py')
helper=module('timer_setter','probe-runecl-setter-context.py')

class TimerValueContractTests(unittest.TestCase):
    def test_value_is_the_actual_stored_current_and_forwarded_setter_result(self):
        source,header=probe.value_source((ROOT/'src/ZunTimer.cpp').read_text(),
                                        (ROOT/'src/ZunTimer.hpp').read_text(),helper)
        self.assertEqual(source.count('return this->current;'),1)
        self.assertEqual(source.count('return this->SetCurrent(value);'),1)
        self.assertIn('this->current = value;',source)
        self.assertIn('this->subFrame = (float)value;',source)
        self.assertIn('int operator=(int value);',header)

    def test_ambiguous_definition_and_missing_state_write_are_rejected(self):
        source=(ROOT/'src/ZunTimer.cpp').read_text()
        header=(ROOT/'src/ZunTimer.hpp').read_text()
        for changed in (source+'\nvoid ZunTimer::SetCurrent(int value)',
                        source.replace('this->previous = -999999;', 'changed_write;')):
            with self.assertRaises(ValueError):
                probe.value_source(changed,header,helper)
        with self.assertRaises(ValueError):
            probe.value_source(source,header.replace('void SetCurrent(int value);',''),helper)
