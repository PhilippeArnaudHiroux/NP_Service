import zmq
import random
import string

shopID = string.ascii_uppercase
shopID = ''.join(random.choice(shopID) for _ in range(10))
print(shopID)

context = zmq.Context()
socketSend = context.socket(zmq.PUSH)
socketRecv = context.socket(zmq.SUB)

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")

topicRecv = "shop?" + shopID
topicSend = "shop!" + shopID
socketRecv.setsockopt_string(zmq.SUBSCRIBE, topicRecv)

while(1):
    messageSend = input("Type in your command: ")
    messageSend = topicSend + messageSend
    print(messageSend)
    socketSend.send_string(messageSend)
    reply = socketRecv.recv_string()
    print(reply)

