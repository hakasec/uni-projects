# pylint: disable=C0103
""" Configuration classes and global_config. """

import json
from os import path

from frozen import FrozenClass


class ConfigDict(FrozenClass):
    """ ConfigDict is a dict with attributes as it's contents. """

    def __init__(self, parent=None, **kwargs):
        """ Takes a dictionary and stores them as attributes. """
        # get all items from kwargs and assigned them as attributes
        for k, v in kwargs.items():
            # if the item is a dict, embed another ConfigDict
            if type(v) is dict:
                setattr(self, k, ConfigDict(parent=self, **v))
            else:
                setattr(self, k, v)

        self._parent = parent
        # freeze object
        self._freeze()

    def __getitem__(self, attr):
        """ Makes ConfigDict subscriptable. """
        return getattr(self, attr)

    def __contains__(self, attr):
        return True if attr in self.__dict__.keys() else False

    def to_dict(self):
        """ Converts ConfigDict to dict. """
        return {
            k: v
            for k, v in self.__dict__.items()
            if not k.startswith("_")}


class Configuration(ConfigDict):
    """ Configuration is a ConfigDict with ability to read json files. """

    def __init__(self, fp):
        """ Takes a file-like or filename fp and stores it as attributes. """
        close = False
        # if fp is str convert to file-like
        if isinstance(fp, str):
            fp = open(fp, "r")
            close = True

        # call super
        super().__init__(parent=self, **json.load(fp))

        if close:
            fp.close()


# global_config is the global configuration for the file
global_config = Configuration(
    path.join(path.dirname(__file__), "config.json"))
