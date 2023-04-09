#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;

int main(void)
{
    string sendString = "";         //In this string the messages will be stored that will be send
    string receivedString  = "";    //In this string the received messages will be stored

    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Client push
        zmq::socket_t subscriber( context, ZMQ_SUB );                       //Client sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Client push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );        //Client sub
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "shop?", strlen("shop?") );   //Client sub

        zmq::message_t * msg = new zmq::message_t();

        while( ventilator.connected() && subscriber.connected())
        {
            cout << "Enter the command: ";                          //Print out the text
            cin >> sendString;                                      //Let the user type in a command
            ventilator.send(sendString.c_str(), sendString.size()); //Send the string

            do{
                subscriber.recv(msg);                                           //Receive the message
                receivedString  = string( (char*) msg->data(), msg->size() );   //Convert the received message to a string
                receivedString.erase(0, 5);                                     //Remove the first 5 characters of the string (sub topic)
                if(receivedString  != "end")                                    //If not end
                {
                    cout << "    -> " << receivedString  << endl;               //Print out the text
                }
            }while(receivedString  != "end");                                   //As long as the end command is not received
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
