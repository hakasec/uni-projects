# pylint: disable=W0201
""" Discord bot implementation. """

import json
import os
from datetime import datetime

import discord
import redis
from rasa_nlu.model import Interpreter

from .bot import ChatBot


class DiscordBot(ChatBot):
    """ Discord bot. """

    def start(self):
        self.client = discord.Client()
        # nested event handlers because self.client
        # doesn't exist at class definition scope
        @self.client.event
        async def on_ready():
            await self._ready()

        @self.client.event
        async def on_message(message):
            await self._handle_message(message)

        model_path = os.path.join(
            os.getcwd(), "training", "models", "current", "nlu")
        self._interpreter = Interpreter.load(model_path)
        self.client.run(self.config.token)

    async def _ready(self):
        print("DiscordBot running!")

    async def _handle_message(self, message):
        client = self.client

        if message.author.bot:
            return

        if client.user in message.mentions:
            # remove the first word
            message_str = " ".join(message.content.split(" ")[1:])
            interpreted = self._interpreter.parse(message_str)

            print(json.dumps(interpreted, indent=2))

            intent = interpreted["intent"]["name"]
            entities = interpreted["entities"]
            if intent == "greet":
                await client.send_message(message.channel, "Hello World!")
            elif intent == "get_weather":
                await client.send_message(
                    message.channel,
                    self.get_weather_summary(message.author, entities))
            elif intent == "remember_location":
                self.remember_user_location(message.author,
                                            entities[0]["value"])
                await client.send_message(message.channel,
                                          "Ok. I'll remember that.")
            elif intent == "recite_location":
                l = self.get_user_location(message.author)
                if l:
                    await client.send_message(
                        message.channel,
                        "You told me you live in {} ({}, {})".format(
                            l["location_name"],
                            l["location_lat"],
                            l["location_long"]))
                else:
                    await client.send_message(
                        message.channel,
                        "You haven't told me where you live.")

    def get_weather_summary(self, user, entities: list) -> str:
        """ Parses the users request for weather forecast and sends it. """
        ws = self.services["weather"]
        if not entities:
            # if no entities supplied, return minutely summary
            w_info = ws.get_weather_at(51.5, 0)
            return w_info["minutely"]["summary"]

        get_entities = lambda name: [
            x for x in entities if x["entity"] == name]
        periods = get_entities("period")
        days = get_entities("day")
        locations = get_entities("location")
        times = get_entities("time")

        # get user's stored location
        user_location = self.get_user_location(user)
        location = [51.5, 0] # default to London
        if locations:
            l = locations[0]["value"]
            ls = self.services["location"]
            data = ls.get_location_coords(l)
            location = [data["longitude"], data["latitude"]]
        elif user_location:
            location = [
                float(user_location["location_long"]),
                float(user_location["location_lat"]),
            ]

        # test for time periods
        if periods:
            # if it's a week, return weekly
            p = periods[0]["value"].lower()
            if p == "week":
                return ws.get_weekly_summary(*location)
        elif days:
            d = days[0]["value"]
            if d == "today":
                return ws.get_hourly_summary(*location, datetime.today())

        return "I don't understand that request"

    def remember_user_location(self, user, location):
        """ remember_user_location sets the user's location in the
            datastore.
        """
        key = "user:{}".format(user.id)
        ls = self.services["location"]
        data = ls.get_location_coords(location)
        mapping = {
            "location_name": location,
            "location_long": data["longitude"],
            "location_lat" : data["latitude"],
        }
        return self.datastore.hmset(key, mapping)

    def get_user_location(self, user):
        """ get_user_location returns the stored user location. """
        key = "user:{}".format(user.id)
        return self.datastore.hgetall(key)
