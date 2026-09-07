FROM python:3.5

WORKDIR /opt/chatbot
COPY . .

VOLUME [ "/opt/chatbot" ]

RUN [ "pip", "install", "-r", "requirements.txt" ]

CMD [ "python", "run_discord.py" ]
