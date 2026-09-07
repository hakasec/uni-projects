""" Location services provided by LocationService class. """

import requests

from .service import Service, register_service


@register_service("location")
class LocationService(Service):
    """ Service that provides location related functions. """

    def __init__(self):
        super().__init__()

        self._api_endpoint = "https://nominatim.openstreetmap.org/"

    def get_location_coords(self, location: str) -> dict:
        """ Queries DarkSky and returns the data as a dictionary. """
        query = "{}/search?q={}&format=json".format(self._api_endpoint,
                                                    location)
        r = requests.get(query)
        data = r.json()
        return {
            "longitude": float(data[0]["lon"]),
            "latitude" : float(data[0]["lat"]),
        }
