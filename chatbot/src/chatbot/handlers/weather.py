""" Weather services """

from concurrent.futures import ThreadPoolExecutor, wait as future_wait
from datetime import datetime, timedelta
from functools import partial

import requests

from .service import Service, register_service


@register_service("weather")
class WeatherService(Service):
    """ Service that provides data and summaries of weather
        at certain times and locations. """

    def __init__(self, config):
        super().__init__(config)

        self._api_endpoint = "https://api.darksky.net/forecast/{}".format(
            self.config.token)

    def get_weather_at(self, long, lat, time=None) -> dict:
        """ Queries DarkSky and returns weather data as a dictionary. """
        epoch = 0 # epoch is the unix timestamp
        if time is not None:
            if isinstance(time, datetime):
                epoch = time.timestamp()
            elif isinstance(time, (int, float)):
                epoch = time
            else:
                raise TypeError(
                    "time should be datetime.datetime, int, or float")

        if not isinstance(long, (float, int)):
            raise TypeError("long must be numeric")
        elif not isinstance(lat, (float, int)):
            raise TypeError("lat must be numeric")

        # if we have a valid epoch, pass it to the DarkSky API
        if epoch > 0:
            res = requests.get("{}/{},{},{:.0f}?units=uk2".format(
                self._api_endpoint, long, lat, epoch))
        # else, ignore
        else:
            res = requests.get("{}/{},{}?units=uk2".format(
                self._api_endpoint, long, lat))

        return res.json()

    def get_hourly_summary(self, long, lat, time=None) -> str:
        """ Queries DarkSky and returns an hourly summary. """
        return self.get_weather_at(long, lat, time)["hourly"]["summary"]

    def get_weekly_summary(self, long, lat, time=datetime.now()) -> str:
        """ Queries DarkSky and returns a weekly summary. """
        day_pairs = []
        # nested function that gets the daily summary for a given time
        def _get_day(t):
            w_info = self.get_weather_at(long, lat, t)
            return (t, w_info["hourly"]["summary"])

        # nested function that acts as a callback to _get_day future 
        def _update_days(i, f):
            print(f, i)
            # insert the result to day_pairs at i
            day_pairs.insert(i, f.result())

        futures = []
        with ThreadPoolExecutor(max_workers=7) as executor:
            for i in range(0, 7):
                # get the days summary in a separate thread
                f = executor.submit(_get_day, time + timedelta(i))
                # pass the index to the callback so it can be ordered
                f.add_done_callback(partial(_update_days, i))
                futures.append(f)

        future_wait(futures) # wait for all futures to complete
        summary = ""
        for pair in day_pairs:
            summary += "{}: {}\n".format(pair[0].strftime("%A"),
                                         pair[1])
        return summary[:-1] # remove last newline
