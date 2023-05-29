import zmq
import time
from datetime import datetime

context = zmq.Context()
socketSend = context.socket(zmq.PUSH)
socketRecv = context.socket(zmq.SUB)

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")

checkMessage = "shop!***"
timeOutMessage = "I want to let the world know that my service just died at "
recvTopic = "shop?***"
socketRecv.setsockopt_string(zmq.SUBSCRIBE, recvTopic)
check = 1

while(check == 1):
    socketSend.send_string(checkMessage)

    if socketRecv.poll(timeout=10000) & zmq.POLLIN:
        now = datetime.now()
        date_now = now.strftime("%Y-%m-%d %H:%M:%S")
        reply = socketRecv.recv_string()
        print("Received", reply, "at ", date_now)
    else:
        print("No reply received. Ending the program.")
        now = datetime.now()
        date_now = now.strftime("%Y-%m-%d %H:%M:%S")
        socketSend.send_string(timeOutMessage+date_now)
        check = 0
        break

    time.sleep(10)

socketRecv.close()
socketSend.close()
context.term()
