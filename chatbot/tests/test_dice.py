# pylint: disable=C0414,C0111,C0103,W0104

import unittest
import math

from chatbot.handlers.dice import DiceService


class DiceServiceTestCase(unittest.TestCase):

    def setUp(self):
        self.ds = DiceService()

    def test_valid_dx_roll(self):
        valid = (4, 6, 8, 10, 12, 20, 100)
        for d in valid:
            roll = self.ds.roll_dice(sides=d)
            self.assertTrue(roll <= d)

    def test_inf_roll(self):
        roll = self.ds.roll_dice(math.inf)
        self.assertEqual(roll, math.inf)

    def test_invalid_dx_roll(self):
        with self.assertRaises(ValueError):
            self.ds.roll_dice(9)
            self.ds.roll_dice(5)
            self.ds.roll_dice(6.0)

    def test_type_roll(self):
        with self.assertRaises(TypeError):
            self.ds.roll_dice("hello world")
