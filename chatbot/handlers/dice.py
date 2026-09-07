""" DiceService and related. """

import random
import math

from .service import Service


class DiceService(Service):
    """ Service that provides dice related functions. """

    def roll_dice(self, sides=6):
        """ Returns the result of a dice roll. """
        accepted = (4, 6, 8, 10, 12, 20, 100, math.inf)
        if not isinstance(sides, (int, float)):
            raise TypeError("sides must be numeric.")
        if sides not in accepted:
            raise ValueError(
                ("{} is not a valid number of sides. "
                 + "Valid sides are {}").format(sides, accepted))

        # if sides is math.inf, return it
        if sides == math.inf:
            return math.inf
        return random.randrange(1, sides)
