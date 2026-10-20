"""Producer failure and compile scope, independent of the machine-code Oracle."""
import importlib.util
from pathlib import Path
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    'type3_word_producer', ROOT / 'scripts/probe-exattack-type3-word-order.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


class Type3WordProducerTests(unittest.TestCase):
    def modules(self):
        diagnostic = {
            'difference_offsets': ['0x1b2', '0x1b7', '0x247', '0x249'],
            'candidate_bytes': 780, 'ordinary_differences': 4,
        }

        def rejected(_):
            raise RuntimeError('candidate changed previously equal bytes')

        modules = {
            'compare-coff-function.py': SimpleNamespace(
                verified_target=lambda: b'attested test fixture',
                object_function=lambda *a, **k: (bytearray(780), [])),
            'inspect-exattack-type3-schedule.py': SimpleNamespace(
                SYMBOL='real-source-symbol-fixture', analyze=lambda _: diagnostic),
            'check-boss-frontiers.py': SimpleNamespace(
                TYPE3_UPDATE_ORIGINAL_RESIDUAL_OFFSETS=frozenset(),
                guard_exattack_type3_update=rejected),
        }
        return lambda _, filename: modules[filename]

    def test_failed_compile_invalidates_prior_report_and_cleans_temporary_files(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            report = output / 'type3-word-order.json.gz'
            report.write_bytes(b'previous completed result')
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n'),
                  patch.object(probe.subprocess, 'run', return_value=SimpleNamespace(
                      returncode=1, stdout='', stderr='intentional compiler failure'))):
                with self.assertRaisesRegex(RuntimeError, 'compiler failed'):
                    probe.run(output, 'forwarded-collision')
            self.assertFalse(report.exists())
            self.assertEqual(list(output.iterdir()), [])

    def test_selected_candidate_compiles_once_and_keeps_guard_rejection(self):
        commands = []

        def compile_one(command, **kwargs):
            commands.append(command)
            Path(command[5]).write_bytes(b'cold object fixture')
            return SimpleNamespace(returncode=0, stdout='', stderr='')

        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            with (patch.object(probe, 'module', side_effect=self.modules()),
                  patch.object(probe.subprocess, 'check_output', return_value='fixture\n'),
                  patch.object(probe.subprocess, 'run', side_effect=compile_one)):
                report = probe.run(output, 'forwarded-collision')
            self.assertEqual(len(commands), 1)
            self.assertEqual(commands[0][:3], ['nice', '-n', '10'])
            self.assertEqual(commands[0][6:], probe.PROFILE)
            self.assertEqual(report['requested_case'], 'forwarded-collision')
            self.assertEqual([r['name'] for r in report['rows']], ['forwarded-collision'])
            self.assertFalse(report['rows'][0]['production_frontier']['passed'])
            self.assertEqual(len(report['rows'][0]['previously_equal_bytes_changed']), 4)
            self.assertEqual(report['exactness_credit'], 'none')
            self.assertEqual(list(output.iterdir()), [])

    def test_wrong_target_preserves_existing_evidence_without_compiling(self):
        def wrong_target():
            raise ValueError('target identity differs')

        factory = self.modules()

        def modules(name, filename):
            result = factory(name, filename)
            if filename == 'compare-coff-function.py':
                result.verified_target = wrong_target
            return result

        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary)
            report = output / 'type3-word-order.json.gz'
            report.write_bytes(b'evidence from earlier attested target')
            with (patch.object(probe, 'module', side_effect=modules),
                  patch.object(probe.subprocess, 'run') as compiler):
                with self.assertRaisesRegex(ValueError, 'target identity'):
                    probe.run(output, 'forwarded-collision')
            compiler.assert_not_called()
            self.assertEqual(report.read_bytes(), b'evidence from earlier attested target')


if __name__ == '__main__':
    unittest.main()
