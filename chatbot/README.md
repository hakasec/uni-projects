# Discord Chatbot

Does some stuff. It's cool. Believe me. :^)

## Requirements

Because this bot uses async/await keywords, please use Python 3.5 or higher. 
To install this for Codio, create a new Python 3 project and install the `python3.5-dev` package using `sudo apt-get update && sudo apt-get install python3.5-dev`.

You can install all requirements by running `sudo pip3 install -r requirements.txt`

**It is recommended to use virtualenv and create your own python3.5 environment**

To use datastore features, you may need to install it locally; this can be done using the following command `sudo apt-get install redis-server`.
This should default to `localhost` with the port being `6379`, this can be configured in `config.json` under `redis`.


## Configuration

To configure the bots, rename `config.example.json` to `config.json`.
Each bot has its own configuration section, usually with a token and a list of services it can use.

Configuration for services are in the `services` section.

