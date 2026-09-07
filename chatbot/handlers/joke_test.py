# pylint: disable=C0414,C0111,C0103,W0104,W0212

import unittest

from .joke import JokeService


class JokeServiceTestCase(unittest.TestCase):

    def setUp(self):
        self.js = JokeService()

    def test_random_joke(self):
        self.assertTrue(self.js.random_joke() in self.js._jokes)
