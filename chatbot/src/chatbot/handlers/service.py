# pylint: disable=C0413,C0103
""" Service base clase and global_services. """

from ..config import ConfigDict, global_config


global_services = {}

def register_service(name):
    """ Registers a service globally. """
    def wrap(c):
        if name in global_config.services:
            # create service using global_config
            s = c(global_config.services[name])
        else:
            # create service without config
            try:
                s = c()
            except TypeError:
                return c
        global_services[name] = s
        return c
    return wrap


class Service(object):
    """ Service base class. """

    def __init__(self, config=ConfigDict()):
        if isinstance(config, dict):
            config = ConfigDict(**config)
        elif not isinstance(config, ConfigDict):
            raise TypeError("config must be dict or ConfigDict")

        self.config = config
