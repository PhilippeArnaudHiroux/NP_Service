import zmq 
from datetime import datetime

context = zmq.Context()                                     #ZeroMQ context object
subscriber = context.socket(zmq.SUB)                        #subscriber is to SUB
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")   #Lissen to this adress
topic_filter = ""                                           #Create a string (by pasting a text between the "" you can filter on topics)
subscriber.setsockopt_string(zmq.SUBSCRIBE, topic_filter)   #The code can only receive the strings that start with the value stored in topicRecv

while True:                                                 #Keep running this code
    now = datetime.now()                                    #Take the time
    date_now = now.strftime("%Y-%m-%d %H:%M:%S")            #Take the year - month - day - hour - minute - seconds
    message = subscriber.recv_string()                      #Receive a string
    print(date_now, " - ", message)                         #Print out the text