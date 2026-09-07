FROM python:3.5

WORKDIR /opt/chatbot
COPY . .
ENV PYTHONPATH=/opt/chatbot/src

VOLUME [ "/opt/chatbot" ]

RUN [ "pip", "install", "-r", "requirements.txt" ]

CMD [ "python", "-m", "chatbot" ]
