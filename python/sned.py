import zmq
import random
import string


context = zmq.Context()                                            
socketSend = context.socket(zmq.PUSH)                             
socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")         

topicSend = "shop!"                                                    

while(1):                                                           #Keep doing this
    messageSend = input("Type in your command: ")                   #Ask the user to input a text
    messageSend = topicSend + messageSend                           #Create the string that need to be send
    socketSend.send_string(messageSend)                             #Send the string
 

