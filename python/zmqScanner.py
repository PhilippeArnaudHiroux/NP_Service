import zmq
from datetime import datetime

# Create a ZMQ context
context = zmq.Context()

# Create a subscriber socket
subscriber = context.socket(zmq.SUB)

# Connect the subscriber socket to the publisher's address
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")

# Subscribe to a specific topic
topic_filter = ""
subscriber.setsockopt_string(zmq.SUBSCRIBE, topic_filter)

# Receive and process messages
while True:
    now = datetime.now()
    date_now = now.strftime("%Y-%m-%d %H:%M:%S")
    message = subscriber.recv_string()
    print(date_now, " - ", message)
