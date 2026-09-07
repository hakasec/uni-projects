from io import BytesIO
from tempfile import NamedTemporaryFile

from PIL import Image
import requests

from .service import Service, register_service


@register_service("pickle")
class PickleService(Service):
    
    def pickle_user(self, avatar_url: str):
        res = requests.get(avatar_url)
        print(avatar_url)
        image = Image.open(BytesIO(res.content))
        image = image.rotate(180)
        temp = NamedTemporaryFile()
        image.save(temp, format="jpeg")
        return temp
