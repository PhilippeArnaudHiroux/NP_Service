import zmq
import time
from datetime import datetime

context = zmq.Context()                                                         #ZeroMQ context
socketSend = context.socket(zmq.PUSH)                                           #socketSend is to PUSH
socketRecv = context.socket(zmq.SUB)                                            #SocketRecv is to SUB

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")                       #Send to this adress
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")                       #Lissen to this adress

checkMessage = "shop!***"                                                       #Create a string with the name checkMessage and make it equel to shop!***
recvTopic = "shop?***"                                                          #Create a string with the name recvTopic and make it equel to shop?***
timeOutMessage = "I want to let the world know that my service just died at "   #Create a string with the naem timeOutMassage and make it equel to I want to let the world know that my service just died at 
socketRecv.setsockopt_string(zmq.SUBSCRIBE, recvTopic)                          #The code can only receive the strings that start with the value stored in topicRecv
check = 1                                                                       #Create a veriable and make it equel to 1

while(check == 1):                                                              #Do this as long as check is equel to 1
    socketSend.send_string(checkMessage)                                        #Send the checkMessage

    if socketRecv.poll(timeout=10000) & zmq.POLLIN:                             #If there is a repply in 10 seconds run this code
        now = datetime.now()                                                    #Take the time
        date_now = now.strftime("%Y-%m-%d %H:%M:%S")                            #Take the year - month - day - hour - minute - seconds
        reply = socketRecv.recv_string()                                        #Receive the string
        print("Received", reply, "at ", date_now)                               #Print out the text
    else:                                                                       #If there is no reply in 10 seconds run this code
        print("No reply received. Ending the program.")                         #Print the text
        now = datetime.now()                                                    #Take the time
        date_now = now.strftime("%Y-%m-%d %H:%M:%S")                            #Take the year - month - day - hour - minute - seconds
        socketSend.send_string(timeOutMessage+date_now)                         #Send the timeOutMessage and the time
        check = 0                                                               #Make check equel to 0 (by doing this you will go out of the will loop)
        break                                                                   #Break out of the loop

    time.sleep(10)                                                              #The code can take a break and sleep for 10 seconds

socketRecv.close()                                                              #Close the socketRecv
socketSend.close()                                                              #Close the socketSend
context.term()                                                                  #Close the context
