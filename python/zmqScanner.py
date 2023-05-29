import zmq 
from datetime import datetime

context = zmq.Context()
subscriber = context.socket(zmq.SUB)
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")
topic_filter = "shop"
subscriber.setsockopt_string(zmq.SUBSCRIBE, topic_filter)

while True:
    now = datetime.now()
    date_now = now.strftime("%Y-%m-%d %H:%M:%S")
    message = subscriber.recv_string()
    print(date_now, " - ", message)