""" FrozenClass and related. """

class FrozenException(Exception):
    """ Is raised when a FrozenClass object is mutated. """


class FrozenClass(object):
    """ Prevents mutation after initialisation.
        Designed to be inherited from. """

    _frozen = False

    def __setattr__(self, name, value):
        if self._frozen:
            raise FrozenException("This object is immutable")

        super().__setattr__(name, value)

    def _freeze(self):
        self._frozen = True
