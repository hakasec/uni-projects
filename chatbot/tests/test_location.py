# pylint: disable=C0414,C0111,C0103,W0104

import unittest

from chatbot.handlers.location import LocationService


class LocationServiceTestCase(unittest.TestCase):

    def setUp(self):
        self.ls = LocationService()

    def test_get_location_coords(self):
        d = self.ls.get_location_coords("chipping norton")
        self.assertEqual(round(d["latitude"], 1), 51.9)
        self.assertEqual(round(d["longitude"], 1), -1.5)
