""" JokeService and related. """

import random

from .service import Service


class JokeService(Service):
    """ Services that provides jokes and other comedic things. """

    _jokes = (
        "Why did the chicken cross the road?\nTo get to the other side.",
        "Why do keyboards work 24/7?\nBecause they have two shifts.",
        "What days do fish dislike the most?\nFry-Days!.",
        "I don't know. You tell me.",
        "What kind of music is a balloon scared of?\nPop music.",
        "What do you call the security outside of a Samsung Store?\nGuardians of the Galaxy.",
        "Want a bad joke?\nLook in the mirror.",
        "What do you call a bee that was born is the United States?\nA USB.",
        "What do you call a cow in an earthquake?\nA milkshake.",
        "Where do animals go when their tails fall off?\nThe retail store.",
    )

    def random_joke(self):
        """ Returns a random joke. """
        return random.choice(self._jokes)
