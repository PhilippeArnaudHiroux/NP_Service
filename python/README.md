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
There must be created a socket to send and a socket to receive. And give the sockets an adress to sent to or receive from.
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

# heartbeatCheck
The [heartbeatCheck](https://github.com/PhilippeArnaudHiroux/NP_Service/blob/main/python/heartbeatCheck.py) code will look if the service is still running.

## Import
The imports needed for this code are.
```py
import zmq
import time
from datetime import datetime
import requests
```

## Discord
Its also possible that this code send a message to your discord when it get not response from the service. If you want that you need to fill in the next two lines of code.
```py
url = ""                                                                        #Fill in the url to your Discord chanel
auth = {'Authorization' : '' }                                                  #Fill in Authorization between the ''
```

## Create sockets
There must be created a socket to send and a socket to receive. And give the sockets an adress to sent to or receive from.
```py
context = zmq.Context()                                                         #ZeroMQ context
socketSend = context.socket(zmq.PUSH)                                           #socketSend is to PUSH
socketRecv = context.socket(zmq.SUB)                                            #SocketRecv is to SUB

socketSend.connect("tcp://benternet.pxl-ea-ict.be:24041")                       #Send to this adress
socketRecv.connect("tcp://benternet.pxl-ea-ict.be:24042")                       #Lissen to this adress
```

## Create topics
Create the topics the heartbeatCheck need to lissen to and to send with.<br>
You can alse change the text that need to be send to your discord.
```py
checkMessage = "shop!***"                                                       #Create a string with the name checkMessage and make it equel to shop!***
recvTopic = "shop?***"                                                          #Create a string with the name recvTopic and make it equel to shop?***
timeOutMessage = "I want to let the world know that my service just died at "   #Create a string with the naem timeOutMassage and make it equel to I want to let the world know that my service just died at 
socketRecv.setsockopt_string(zmq.SUBSCRIBE, recvTopic)                          #The code can only receive the strings that start with the value stored in topicRecv
check = 1                                                                       #Create a veriable and make it equel to 1
```

## while loop
First the code will send a message to the service. Afhter that it will wait for 10 seconds to get a reply from the service.<br>
if there is a reply in the 10 seconds, the Python code will look what time it is and will print out on what time he received the reply from the service.<br>
When there is no reply in the 10 seconds, the Python code will also look what time it is and print it out on what time the service did not reply anymore. It will also send a message to your discord if you have filled in the discord part.
```py
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
        msg = {'content':'The server died at ' + date_now}                      #Create string that need to be send to Discord
        requests.post(url, headers=auth, data=msg)                              #Send to Discord
        check = 0                                                               #Make check equel to 0 (by doing this you will go out of the will loop)
        break                                                                   #Break out of the loop

    time.sleep(10)                                                              #The code can take a break and sleep for 10 seconds
```

## Close sockets
At the end of the code you have to close your sockets.
```py
socketRecv.close()                                                              #Close the socketRecv
socketSend.close()                                                              #Close the socketSend
context.term()                                                                  #Close the context
```

# zmqScanner
The [zmqScanne](https://github.com/PhilippeArnaudHiroux/NP_Service/blob/main/python/zmqScanner.py) will scan for everything that will be send on the given adress

## Import
The imports needed for this code are.
```py
import zmq 
from datetime import datetime
```

## Create socket
There must be created a socket to receive. And give the socket an adress to receive from.<br>
If you fill in **topic_filter** you can filter on a specific topic. If you leave it empty, it will show you everything.
```py
context = zmq.Context()                                     #ZeroMQ context object
subscriber = context.socket(zmq.SUB)                        #subscriber is to SUB
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")   #Lissen to this adress
topic_filter = ""                                           #Create a string (by pasting a text between the "" you can filter on topics)
subscriber.setsockopt_string(zmq.SUBSCRIBE, topic_filter)   #The code can only receive the strings that start with the value stored in topicRecv
```

## while loop
First the code will look what time it is. Afhter that is will receive all the message that are send on the adress. Or only the topic that you gave if you have given a topic. At the end it will print out the time and the received message.
```py
while True:                                                 #Keep running this code
    now = datetime.now()                                    #Take the time
    date_now = now.strftime("%Y-%m-%d %H:%M:%S")            #Take the year - month - day - hour - minute - seconds
    message = subscriber.recv_string()                      #Receive a string
    print(date_now, " - ", message)                         #Print out the text
```