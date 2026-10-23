"""Source/identity guards; actual PE/COFF replay remains separately required."""
import csv
from pathlib import Path
import re
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = '?InitializeSubroutine@EnemyEclManagerView@@QAEHPAUTh09EclContextView@@F@Z'
NAME = 'EnemyEclManagerView::InitializeSubroutine'
LEGACY = ('?InitializeSubroutine@ManagerStateView@Th09EclRunState@@',
          '?InitializeSubroutine@EnemySpawnEclManagerBoundary@@',
          '?InitializeSubroutine@EnemySpawnRecordEclManagerBoundary@@')


def compact(text):
    text = re.sub(r'/\*.*?\*/|//[^\n]*', '', text, flags=re.S)
    return re.sub(r'\s+', '', text)


def block(text, marker):
    """Extract one balanced source block, not a whole-file golden string."""
    start = text.index('{', text.index(marker))
    depth = 1
    for end in range(start + 1, len(text)):
        depth += (text[end] == '{') - (text[end] == '}')
        if not depth:
            return text[start + 1:end]
    raise ValueError('unbalanced source block')


class SelectedEclManagerOwner(unittest.TestCase):
    def source(self, name):
        return (ROOT/'src'/name).read_text()

    def test_selected_pointer_and_existing_layout_assertions(self):
        header = self.source('EclOpcodes.hpp')
        context = compact(block(header, 'struct Th09EclContextView\n'))
        self.assertTrue(context.startswith('EnemyEclManagerView*selectedManager000;'))
        self.assertIn('Th09EclRawInstructionHeaderView*currentInstruction004;', context)
        for assertion in ('sizeof(Th09EclContextView)==0x22C',
                          'offsetof(Th09EclContextView,selectedManager000)==0',
                          'offsetof(Th09EclContextView,currentInstruction004)==0x04'):
            self.assertIn(assertion, compact(header))
        manager = compact(self.source('EnemyManager.hpp'))
        self.assertIn('sizeof(EnemyEclManagerView)==0x188', manager)
        self.assertIn('offsetof(EnemyManagerView,opposingEclManager188)==0x188', manager)
        state = compact(block(self.source('EclRunState.inl'), 'struct ManagerStateView\n'))
        self.assertTrue(state.startswith('EnemyEclManagerViewprimaryEcl000;EnemyEclManagerViewopposingEcl188;'))
        self.assertIn('unsignedcharunknown310[0x0320-0x0310];SideStateView*sideState320;', state)
        self.assertNotIn('InitializeSubroutine', state)

    def test_negative_id_cannot_publish_or_reset_context(self):
        source = self.source('EclInitializeSubroutine.cpp')
        body = block(source, 'int '+NAME)
        guarded = block(body, 'if (subroutineId >= 0)')
        before, after = body.split(guarded)
        outside = compact(before + after)
        self.assertNotIn('context->', outside)
        self.assertIn('return0;', outside)
        steps = ('subroutineTable04', 'context->currentInstruction004=',
                 'context->time008=0;', 'context->secondaryTime094=0;',
                 'context->subroutineId228=subroutineId;',
                 'context->selectedManager000=this;')
        normalized = compact(guarded)
        positions = [normalized.index(step) for step in steps]
        self.assertEqual(positions, sorted(positions))
        self.assertEqual(compact(body).count('context->selectedManager000=this;'), 1)
        self.assertIn('intInitializeSubroutine(Th09EclContextView*context,shortsubroutineId);',
                      compact(self.source('EnemyManager.hpp')))

    def test_call_subroutine_reloads_selected_owner_after_initializer(self):
        body = compact(block(self.source('EclManager.cpp'), 'static void CallSubroutine('))
        call = 'view->activeContext2CE0->selectedManager000->InitializeSubroutine('
        self.assertEqual(body.count(call), 1)
        call_end = body.index(';', body.index(call))
        after = body[call_end + 1:]
        self.assertIn('memcpy(view->activeContext2CE0->unknown01C+(0x74-0x1C),reinterpret_cast<unsignedchar*>(view->activeContext2CE0->selectedManager000)+0x168,0x20);', after)
        self.assertNotIn('manager0000', body)
        self.assertEqual(body.count('selectedManager000'), 2)

    def test_primary_callbacks_do_not_follow_selected_context(self):
        expectations = {
            'EclManager.cpp': 'enemyState->manager0000->primaryEcl000.InitializeSubroutine(',
            'EclRunState.inl': 'Th09EclRunState::View(enemy)->manager0000->primaryEcl000.InitializeSubroutine(',
            'EnemyManagerCore.inl': 'enemyManager->primaryEclManager000.InitializeSubroutine(',
            'EnemyManagerClear.cpp': 'reinterpret_cast<Th09EclRunState::ManagerStateView*>(this)->primaryEcl000.InitializeSubroutine(',
        }
        for file, receiver in expectations.items():
            with self.subTest(file=file):
                source = compact(self.source(file))
                self.assertEqual(source.count(receiver), 1)
                if file != 'EclManager.cpp':
                    self.assertNotIn('selectedManager000->InitializeSubroutine', source)
        core = compact(self.source('EnemyManagerCore.inl'))
        self.assertIn('enemyManager->primaryEclManager000.RunEcl(', core)
        self.assertIn('enemyManager->primaryEclManager000.GetSubroutine(', core)

    def test_spawn_branches_preserve_both_physical_receivers(self):
        for file in ('EnemyManagerSpawnEnemy.cpp', 'EnemyManagerSpawnRecord.cpp'):
            source = self.source(file)
            body = compact(block(source, 'if (useOpposingEclManager == 0)'))
            tail = source[source.index('if (useOpposingEclManager == 0)'):]
            other = compact(block(tail, 'else'))
            for branch, owner, excluded in ((body, 'primaryEcl000', 'opposingEcl188'),
                                             (other, 'opposingEcl188', 'primaryEcl000')):
                with self.subTest(file=file, owner=owner):
                    self.assertIn('manager->'+owner+'.InitializeSubroutine(reinterpret_cast<Th09EclContextView*>(enemy->mainContext7F4),', branch)
                    self.assertIn('manager->'+owner+'.RunEcl(', branch)
                    self.assertNotIn(excluded, branch)
                    self.assertLess(branch.index('InitializeSubroutine'), branch.index('RunEcl'))
                    if file == 'EnemyManagerSpawnEnemy.cpp':
                        self.assertLess(branch.index('InitializeSubroutine'), branch.index('memcpy('))
                        self.assertLess(branch.index('memcpy('), branch.index('RunEcl'))
                        self.assertIn('enemy->mainContext7F4+0x1C,initialVariables,0x78', branch)
            self.assertNotIn('EclManagerBoundary', source)

    def test_canonical_initializer_and_every_original_call_use_one_identity(self):
        units = tomllib.loads((ROOT/'config/match-units.toml').read_text())['units']
        owner = units['ecl-initialize-subroutine']
        self.assertEqual((owner['symbol'], owner['functions'], owner['source']),
                         (SYMBOL, [NAME], 'src/EclInitializeSubroutine.cpp'))
        self.assertEqual((owner['target_address'], owner['size']), (0x406850, 0x49))
        self.assertEqual([(r['offset'], r['target']) for r in owner['relocations']],
                         [(0x26, 0x401500), (0x33, 0x401500)])
        calls = []
        for name, unit in units.items():
            for row in unit.get('relocations', []):
                if row['type'] == 'REL32' and row['target'] == 0x406850:
                    self.assertEqual(row['symbol'], SYMBOL, name)
                    calls.append(name)
            for old in LEGACY:
                self.assertNotIn(old, str(unit))
        self.assertGreaterEqual(len(calls), 6)

    def test_all_identity_ledgers_remove_legacy_owner(self):
        for file, field in (('functions.csv', 'proposed_name'),
                            ('matches.csv', 'name'), ('reccmp-functions.csv', 'name')):
            with (ROOT/'config'/file).open(newline='') as stream:
                rows = [r for r in csv.DictReader(stream) if int(r['address'], 0) == 0x406850]
            self.assertEqual(len(rows), 1, file)
            self.assertEqual(rows[0][field], NAME, file)
        implemented = (ROOT/'config/implemented.csv').read_text().splitlines()
        self.assertIn(NAME, implemented)
        self.assertNotIn('Th09EclRunState::ManagerStateView::InitializeSubroutine', implemented)


if __name__ == '__main__':
    unittest.main()
