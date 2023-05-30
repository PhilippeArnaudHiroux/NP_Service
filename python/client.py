import zmq
import random
import string

def printGet(input):                                                #The printGet function will print out all the received products
    input = input[5:]                                               #Remove the first 5 elements of the string
    input = ''.join([char for char in input if not char.isupper()]) #Remove all the upperchars of the string
    input = [substring.strip() for substring in input.split(";")]   #Split the string when there is a ;
    for elements in input:                                          #Do this as much times as there are elements in input
        print("   -> ", elements)                                   #Print out the text

def printOutput(input):                                             #The printOutput function will print out the received string
    input = input[5:]                                               #Remove the first 5 elements of the string
    input = ''.join([char for char in input if not char.isupper()]) #Remve all the upperchars of the string
    print(input)                                                    #Print out the text

shopID = string.ascii_uppercase                                     #ShopID will be in uppercase
shopID = ''.join(random.choice(shopID) for _ in range(10))          #Generate a shopID
print(shopID)                                                       #Print out the shopID

context = zmq.Context()                                             #ZeroMQ context object
socketSend = context.socket(zmq.PUSH)                               #SocketSend is to PUSH
socketRecv = context.socket(zmq.SUB)                                #SocketRecv is to SUB

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")           #Send to this adress
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")           #Lissen to this adress

topicRecv = "shop?" + shopID                                        #Create the topicRecv (the client will only receive the messages with this string in front of it)
topicSend = "shop!" + shopID                                        #Create the topicSenf
command = ""                                                        #Create an element to store something in
socketRecv.setsockopt_string(zmq.SUBSCRIBE, topicRecv)              #The client can only receive the strings that start with the value stored in topicRecv

while(1):                                                           #Keep doing this
    messageSend = input("Type in your command: ")                   #Ask the user to input a text
    messageSend = messageSend.lower()                               #Make sure that all character are in lowercase
    command = messageSend[:3]                                       #Take the first three elements of the messageSend and store it in command
    messageSend = topicSend + messageSend                           #Create the string that need to be send
    socketSend.send_string(messageSend)                             #Send the string
    reply = socketRecv.recv_string()                                #Receive the answer
    if(command == "get"):                                           #If the command is equel to get run this code
        printGet(reply)                                             #Run the printGet function
    else:                                                           #If the command is not equel to get run this code
        printOutput(reply)                                          #Run the printOutput function

