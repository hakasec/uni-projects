# pylint: disable=C0413,C0111
""" ChatBot base class. """

import sys
import os
# so we can access ../config.py
sys.path.insert(0, os.path.abspath('..'))

from config import ConfigDict
from datastore import global_datastore
from handlers import global_services


class ChatBot(object):
    """ ChatBot is an interface for chat bots. """

    def __init__(self, config=ConfigDict(), services=None, datastore=None):
        if isinstance(config, dict):
            config = ConfigDict(**config)
        elif not isinstance(config, ConfigDict):
            raise TypeError("config must be dict or ConfigDict")

        self.services = services
        self.config = config

        if datastore:
            self.datastore = datastore
        else:
            self.datastore = global_datastore

        # get service configs
        if "services" in self.config and services is None:
            self.services = {
                k: v 
                for k, v in global_services.items() 
                if k in self.config.services}

    def start(self):
        raise NotImplementedError("start is not implemented!")

    def stop(self):
        raise NotImplementedError("stop is not implemented!")

