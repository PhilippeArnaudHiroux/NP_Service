#include "shopc.h"
#include <locale>
shopC::shopC()
{
    createID();
    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Client push
        zmq::socket_t subscriber( context, ZMQ_SUB );                       //Client sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Client push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );        //Client sub
        recvSubject = recvSubject + shopID;
        subscriber.setsockopt( ZMQ_SUBSCRIBE, recvSubject.c_str(), recvSubject.size() );   //Client sub

        zmq::message_t * msg = new zmq::message_t();

        while( ventilator.connected() && subscriber.connected())
        {
            cout << "Enter the command: ";                          //Print out the text
            cin >> inputString;                                      //Let the user type in a command

            sendString = cleanString(inputString);

            sendString = pushSubject + shopID + sendString;
            ventilator.send(sendString.c_str(), sendString.size()); //Send the string

            subscriber.recv(msg);                                           //Receive the message
            receivedString  = string( (char*) msg->data(), msg->size() );   //Convert the received message to a string
            receivedString.erase(0, 5);                                     //Remove the first 5 characters of the string (sub topic)
            receivedString = delUppChar(receivedString);
            inputString.erase(3,inputString.size());

            if(inputString == "get")
            {
                splitString();
                printProducts();
            }
            else
            {
                cout << "    -> " << receivedString  << endl;               //Print out the text                                 //As long as the end command is not received

            }
            products.clear();
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}

void shopC::createID()
{
    srand(time(0));
    for(int i=0; i<sizeID; i++)
    {
        shopID = shopID + char('A' + rand() % 26);
    }

    cout << shopID << endl;
}

string shopC::cleanString(string str)
{
    for (int i=0; i<str.size(); ++i)
    {
        if(str[i] >= 'A' and str[i] <= 'Z')
        {
            str[i] = str[i] + 32;
        }
    }

    return str;
}

string shopC::delUppChar(string str)
{
    // Create a regular expression
    const regex pattern("[A-Z]");

    // Replace every matched pattern with the
    // target string using regex_replace() method
    return regex_replace(str, pattern, "");
}

void shopC::splitString()
{
    istringstream iss(receivedString);
    while(getline(iss, token, ';'))
    {
        products.push_back(token);
    }
}

void shopC::printProducts()
{
    for(int i=0; i<products.size(); i++)
    {
        cout << "   -> " << products.at(i) << endl;
    }
}
