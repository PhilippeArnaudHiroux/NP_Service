import zmq
import random
import string

def printGet(input):
    input = input[5:]
    input = ''.join([char for char in input if not char.isupper()])
    input = [substring.strip() for substring in input.split(";")]
    for elements in input:
        print("   -> ", elements)

def printOutput(input):
    input = input[5:]
    input = ''.join([char for char in input if not char.isupper()])
    print(input)

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
command = ""
socketRecv.setsockopt_string(zmq.SUBSCRIBE, topicRecv)

while(1):
    messageSend = input("Type in your command: ")
    messageSend = messageSend.lower()
    command = messageSend[:3]
    messageSend = topicSend + messageSend
    socketSend.send_string(messageSend)
    reply = socketRecv.recv_string()
    if(command == "get"):
        printGet(reply)
    else:
        printOutput(reply)

