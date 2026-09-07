# pylint: disable=C0103
""" All things redis related. """
import redis

from config import global_config



_default_config = {
    "host": "localhost",
    "port": 6379,
}


def create_datastore(**kwargs):
    """ create_datastore creates a Redis datastore with a config. """
    kwargs["decode_responses"] = True
    return redis.StrictRedis(**kwargs)


# create global datastore
if "redis" in global_config:
    global_datastore = create_datastore(**global_config.redis.to_dict())
else:
    global_datastore = create_datastore(**_default_config)
