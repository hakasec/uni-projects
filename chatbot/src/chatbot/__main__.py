# pylint: disable=C0103
""" Discord stuff. """

from .config import global_config
from .wrappers import DiscordBot


def main():
    """ Run Discord bot. """
    b = DiscordBot(global_config.discord)
    b.start()


if __name__ == "__main__":
    main()
