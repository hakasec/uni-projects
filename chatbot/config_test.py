# pylint: disable=C0414,C0111,W0104

import unittest
import os.path as path

from frozen import FrozenException
from config import Configuration


class ConfigurationTestCase(unittest.TestCase):

    def setUp(self):
        dirname = path.dirname(__file__)
        self.config_file = path.join(dirname, "config.example.json")
        self.config = Configuration(self.config_file)

    def test_mutation(self):
        with self.assertRaises(FrozenException):
            self.config.discord.token = "hello"

    def test_missing_attr(self):
        with self.assertRaises(AttributeError):
            self.config.not_here

    def test_working_attr(self):
        self.assertTrue(self.config.discord.token, "TOKEN_HERE")

    def test_working_subscript(self):
        self.assertTrue(self.config["discord"]["token"], "TOKEN_HERE")
