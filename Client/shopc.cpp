#include "shopc.h"
#include <locale>

shopC::shopC()
{
    createID();                                                                             //Run the function shopID
    try
    {
        zmq::context_t context(1);                                                          //ZeroMQ context object
        zmq::socket_t ventilator( context, ZMQ_PUSH );                                      //Ventilator is to PUSH
        zmq::socket_t subscriber( context, ZMQ_SUB );                                       //Subscriber is to SUB
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );                        //Send to this adress
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );                        //Lissen to this adress
        recvSubject = recvSubject + shopID;                                                 //Paste the shopID behind the recvSubject (When you have more then 1 client running, this will make sure you only get your messages)
        subscriber.setsockopt( ZMQ_SUBSCRIBE, recvSubject.c_str(), recvSubject.size() );    //Receive only the topic that you need

        zmq::message_t * msg = new zmq::message_t();                                        //An incoming message will be stored in msg
        while( ventilator.connected() && subscriber.connected())                            //Keep running this code as long as there is conection
        {
            cout << "Enter the command: ";                                                  //Print out the text
            cin >> inputString;                                                             //Let the user type in a command
            sendString = cleanString(inputString);                                          //Run the cleanString function and store the new string in to sendString
            sendString = pushSubject + shopID + sendString;                                 //Create the string that will be send
            ventilator.send(sendString.c_str(), sendString.size());                         //Send the string in to the world

            subscriber.recv(msg);                                                           //Receive the message and save it in msg
            receivedString  = string( (char*) msg->data(), msg->size() );                   //Convert the received message to a string
            receivedString.erase(0, 5);                                                     //Remove the first 5 characters of the string (sub topic)
            receivedString = delUppChar(receivedString);                                    //Set the receivedString string equal to receivedString without the upperchars
            inputString.erase(3,inputString.size());                                        //Remove everything after the first 3 characters in firstThree string

            if(inputString == "get")                                                        //If inputString is equel to get run this code
            {
                splitString();                                                              //Run the function splitString
                printProducts();                                                            //Run the function printProducts
            }
            else                                                                            //If inputString is not equel to get run this code
            {
                cout << "    -> " << receivedString  << endl;                               //Print out the text
            }
            products.clear();                                                               //Clear the products vector so its empty and ready to use again
        }
    }
    catch( zmq::error_t & ex ){std::cerr << "Caught an exception : " << ex.what();}         //When there is an error run this code
}

void shopC::createID()                                                                      //The createID function will create a random ID in uppercharacters
{
    srand(time(0));                                                                         //Put time on 0
    for(int i=0; i<sizeID; i++)                                                             //Do this as much as the value in sizeID
    {
        shopID = shopID + char('A' + rand() % 26);                                          //Take a random char and paste it behind shopID
    }

    cout << shopID << endl;                                                                 //Print out shopID
}

string shopC::cleanString(string str)                                                       //The cleanString will replace all the upperchars by there lowwerchars
{
    for (int i=0; i<str.size(); ++i)                                                        //Do this as much as the size of str
    {
        if(str[i] >= 'A' and str[i] <= 'Z')                                                 //If the char is between A and Z
        {
            str[i] = str[i] + 32;                                                           //A will be a, B will be b, ..., Y will be y and Z will be z
        }
    }
    return str;                                                                             //Return str
}

string shopC::delUppChar(string str)                                                        //The dellUppChar function will remove all the upperchars of a string
{
    const regex pattern("[A-Z]");                                                           //Create a regular expression from A to Z
    return regex_replace(str, pattern, "");                                                 //Return the string without the upperchars in it
}

void shopC::splitString()                                                                   //The split string fucntion will split the received string when there is a ; in the string
{
    istringstream iss(receivedString);                                                      //Make iss equel to receivedString
    while(getline(iss, token, ';'))                                                         //Split the string when there is a ; and save it in token
    {
        products.push_back(token);                                                          //Push_back token in to the products vector
    }
}

void shopC::printProducts()                                                                 //The printProducts fucntion will print out every element of the products vector
{
    for(int i=0; i<products.size(); i++)                                                    //Do this as much times as the size of the products vector
    {
        cout << "   -> " << products.at(i) << endl;                                         //Print out the text
    }
}
