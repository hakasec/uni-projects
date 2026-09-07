# pylint: disable=C0414,C0111,W0104
import unittest

from chatbot.datastore import global_datastore


class DataStoreTestCase(unittest.TestCase):

    def test_set(self):
        self.assertTrue(global_datastore.set("hello", "world"))
        self.assertEqual(global_datastore.get("hello"), "world")
        self.assertTrue(global_datastore.delete("hello"))
