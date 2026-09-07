# pylint: disable=C0103
""" Discord stuff. """

from wrappers import DiscordBot
from config import global_config


def main():
    """ Run Discord bot. """
    b = DiscordBot(global_config.discord)
    b.start()


if __name__ == "__main__":
    main()
