# pylint: disable=C0414,C0111,C0103,C0413,C0411,W0104

import sys
import os
sys.path.insert(0, os.path.abspath("..")) # add parent

from datetime import datetime
import unittest

from .weather import WeatherService
from config import Configuration


class WeatherServiceTestCase(unittest.TestCase):

    def setUp(self):
        config = Configuration("config.json")
        self.ws = WeatherService(config.services.weather)

    def test_get_weather_at(self):
        self.ws.get_weather_at(0, 0)

    def test_get_weather_at_time(self):
        self.ws.get_weather_at(0, 0, datetime.now())
        self.ws.get_weather_at(0, 0, 1541168113)

    def test_get_weather_at_type_handling(self):
        with self.assertRaises(TypeError):
            self.ws.get_weather_at("0", "0")

    def test_get_weather_at_time_type_handling(self):
        with self.assertRaises(TypeError):
            self.ws.get_weather_at(0, 0, "hello")
