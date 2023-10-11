# Client
There is also a [client](https://github.com/PhilippeArnaudHiroux/NP_Service/blob/main/python/client.py) in Python to test the service.

## Imports
For the client in Python you need to import three libraries.
```py
import zmq
import random
import string
```

## Create shopID
The Python code wil generate a random shopID of 10 characters
```py
shopID = string.ascii_uppercase                                     #ShopID will be in uppercase
shopID = ''.join(random.choice(shopID) for _ in range(10))          #Generate a shopID
print(shopID)                                                       #Print out the shopID
```

## Create sockets
There must be created a socket to send and a sochet to receive. And give the sockets an adress ro sent to or receive from.
```py
context = zmq.Context()                                             #ZeroMQ context object
socketSend = context.socket(zmq.PUSH)                               #SocketSend is to PUSH
socketRecv = context.socket(zmq.SUB)                                #SocketRecv is to SUB

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")           #Send to this adress
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")           #Lissen to this adress
```

## Create topics
Create the topics the client need to lissen to and to send with.
```py
topicRecv = "shop?" + shopID                                        #Create the topicRecv (the client will only receive the messages with this string in front of it)
topicSend = "shop!" + shopID                                        #Create the topicSend
command = ""                                                        #Create an element to store something in
socketRecv.setsockopt_string(zmq.SUBSCRIBE, topicRecv)              #The client can only receive the strings that start with the value stored in topicRecv
```

## While loop
In the infinite while loop the client code will be rund. First the code will ask you to enter a **messageSend**. This are the same commands as you would do in the C++ code. Next the code will put the command you typed to lower case characters. It will also store the first three characters you type in **command**.<br>
Afther this it will combine the **topicSend** and the **messageSend**, and send it to the service. Now the client will wait to receive a message from the service.<br>
```py
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
```

### printGet function
When the **command** is equal to get, this function will be runned.
```py
def printGet(input):                                                #The printGet function will print out all the received products
    input = input[5:]                                               #Remove the first 5 elements of the string
    input = ''.join([char for char in input if not char.isupper()]) #Remove all the upperchars of the string
    input = [substring.strip() for substring in input.split(";")]   #Split the string when there is a ;
    for elements in input:                                          #Do this as much times as there are elements in input
        print("   -> ", elements)                                   #Print out the text
```

### printOutput
If the **command** is not equal to get, this function will be runned.
```py
def printOutput(input):                                             #The printOutput function will print out the received string
    input = input[5:]                                               #Remove the first 5 elements of the string
    input = ''.join([char for char in input if not char.isupper()]) #Remve all the upperchars of the string
    print(input)                                                    #Print out the text
```