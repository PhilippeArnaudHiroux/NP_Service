#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#include <vector>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;

int main( void )
{
    string receivedString = ""; //In this string the received messages will be stored
    string sendString = "";     //In this string the messages will be stored that will be send
    string firstThree  ="";     //In this string will be the command stored
    string theProduct = "";     //In this string will be the product stored
    vector <string> shopBag;    //In this vector the product will be saved
    int i = 0;

    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );                       //Service sub
        zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );        //Service sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "shop!", strlen("shop!"));    //Service sub

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() && ventilator.connected() )
        {
            subscriber.recv(msg);                                       //Receive the message
            receivedString = string((char*) msg->data(), msg->size());  //Convert the received message to a string
            receivedString.erase(0,5);                                  //Remove the first 5 characters of the string (sub topic)
            firstThree = receivedString;                                //Set firstThree equal to receivedString
            theProduct = receivedString;                                //Set theProduct equal to receivedString
            firstThree.erase(3,firstThree.size());                      //Remove everything after the first 3 characters
            theProduct.erase(0, 3);                                     //Remove the first 3 characters

            if(firstThree == "add")                                                 //If add
            {
                cout << "add " << theProduct << endl;                               //Print out the text
                shopBag.push_back(theProduct);                                      //Add the product to the shopBag
                sendString = "hpa" + theProduct + " is toegevoegd aan uw mandje!";  //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size());             //Send the string
            }
            else if(firstThree == "get")                                    //If get
            {
                cout << "get winkelmanje" << endl;                          //Print out the text
                for(int j=0; j<shopBag.size(); j++)                         //Do this as many times as the vector is large
                {
                    theProduct = shopBag.at(j);                             //Take the element from the vector and copy it into theProduct
                    sendString = "hpa" + theProduct;                        //Create the string that will be send back
                    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
                }
            }
            else if(firstThree == "del")                                //If del
            {
                cout << "del " << theProduct << endl;                   //Print out the text
                while(theProduct != shopBag.at(i)){i++;}                //Look at which positions the element is in the vector
                shopBag.erase(shopBag.begin()+i);                       //Remove the element at position i in the vector
                sendString = "hpa" + theProduct + " is verwijderd";     //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size()); //Send the string
            }
            else
            {
                cout << firstThree << " is geen commando" << endl;      //Print out the text
                sendString = "hpa" + firstThree + " onbekend comando";  //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size()); //Send the string
            }

            ventilator.send("hpaend", strlen("hpaend"));    //Forward that client may stop listening
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
