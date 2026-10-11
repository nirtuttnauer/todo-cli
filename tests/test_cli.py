"""Exercise the built CLI using a fresh task store for every test."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

BINARY = str(Path(sys.argv.pop(1)).resolve())


class TaskCliTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.environment = dict(os.environ, HOME=self.directory.name)
        self.store = Path(self.directory.name) / 'Documents/.todo-cli/tasks.json'

    def run_cli(self, *arguments, expected=0):
        result = subprocess.run([BINARY, *arguments], env=self.environment,
                                capture_output=True, text=True, encoding="utf-8", timeout=10)
        self.assertEqual(result.returncode, expected, result.stderr)
        return result

    def tasks(self):
        return json.loads(self.store.read_text())

    def test_creates_missing_documents_directory(self):
        self.run_cli('add', 'First task')
        self.assertEqual(self.tasks()[0]['description'], 'First task')

    def test_ids_remain_unique_after_deletion_and_restart(self):
        for description in ['one', 'two', 'three']:
            self.run_cli('add', description)
        self.run_cli('delete', '2')
        self.run_cli('add', 'four')
        self.assertEqual([task['id'] for task in self.tasks()], [1, 3, 4])
        self.run_cli('edit', '3', 'edited third')
        self.assertEqual([task['description'] for task in self.tasks()],
                         ['one', 'edited third', 'four'])

    def test_strings_round_trip_without_loss(self):
        description = 'Quotes " braces { } slash \\ newline\n tab\t שלום 😀\x01'
        self.run_cli('add', description)
        self.run_cli('add', 'another task')
        self.assertEqual(self.tasks()[0]['description'], description)
        self.run_cli('mark-done', '1')
        self.assertEqual(self.tasks()[0]['description'], description)
        self.assertEqual(self.tasks()[0]['status'], 'done')
        self.assertIn(description, self.run_cli('list', 'done').stdout)

    def test_read_commands_do_not_rewrite_existing_data(self):
        self.run_cli('add', 'first')
        original = json.dumps(self.tasks(), indent=2).encode()
        self.store.write_bytes(original)
        timestamp = self.store.stat().st_mtime_ns
        for command in ['list', 'help']:
            self.run_cli(command)
            self.assertEqual(self.store.read_bytes(), original)
            self.assertEqual(self.store.stat().st_mtime_ns, timestamp)

    def test_malformed_files_are_rejected_and_preserved(self):
        self.run_cli('add', 'first')
        valid = self.tasks()[0]
        bad_id = dict(valid, id=2147483648)
        for source in ['[{', '{}', '[{"id":1}]', '{broken}',
                       json.dumps([valid, valid]), json.dumps([bad_id]),
                       json.dumps([dict(valid, id=1.5)]),
                       json.dumps([dict(valid, status='unknown')]),
                       json.dumps([dict(valid, description=None)])]:
            with self.subTest(source=source):
                self.store.write_text(source)
                result = self.run_cli('add', 'must not be saved', expected=1)
                self.assertIn('Error:', result.stderr)
                self.assertEqual(self.store.read_text(), source)

    def test_failed_save_preserves_previous_file(self):
        self.run_cli('add', 'first')
        original = self.store.read_bytes()
        Path(str(self.store) + '.tmp').mkdir()
        self.run_cli('add', 'cannot save', expected=1)
        self.assertEqual(self.store.read_bytes(), original)

    def test_id_exhaustion_is_rejected_without_overflow(self):
        self.run_cli('add', 'first')
        self.store.write_text(json.dumps([dict(self.tasks()[0], id=2147483647)]))
        original = self.store.read_bytes()
        self.run_cli('add', 'cannot allocate', expected=1)
        self.assertEqual(self.store.read_bytes(), original)


if __name__ == '__main__':
    unittest.main()
