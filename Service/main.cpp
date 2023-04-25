#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#include <vector>
#include <regex>
#include <fstream>
#include <Windows.h>
#include <cstdio>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;
vector <string> add(string product, vector <string> bag, string push, string id);
void get(vector <string> bag, string push, string id);
vector <string> del(string product, vector <string> bag, string push, string id);
void cut(string push,string id);
void unknownCommand(string three, string push, string id);
vector <string> readTXTfile(string id);
void writeTXTfile(vector <string> bag,string id);
string delUppChar(string str);
string delLowChar(string str);

int main( void )
{
    string receivedString = ""; //In this string the received messages will be stored
    string firstThree  = "";     //In this string will be the command stored
    string theProduct =  "";     //In this string will be the product stored
    string pushSubject = "shop?";
    string recvSubject = "shop!";
    string shopID = "";
    vector <string> shopBag;    //In this vector the product will be saved
    vector <string> shopIDvector;

    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );                       //Service sub
        zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );        //Service sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
        subscriber.setsockopt( ZMQ_SUBSCRIBE, recvSubject.c_str(), recvSubject.size());    //Service sub

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() && ventilator.connected() )
        {
            subscriber.recv(msg);                                       //Receive the message
            receivedString = string((char*) msg->data(), msg->size());  //Convert the received message to a string
            receivedString.erase(0,5);                                  //Remove the first 5 characters of the string (sub topic)
            shopID = delLowChar(receivedString);
            firstThree = delUppChar(receivedString);                                //Set firstThree equal to receivedString
            theProduct = delUppChar(receivedString);                                //Set theProduct equal to receivedString
            firstThree.erase(3,firstThree.size());                      //Remove everything after the first 3 characters
            theProduct.erase(0, 3);                                     //Remove the first 3 characters

            if(shopIDvector.size()==0)
            {
                shopIDvector.push_back(shopID);
                cout << "new customer by " << shopID << endl;
            }
            else
            {
                for(int k=0; k<shopIDvector.size(); k++)
                {
                    if(shopID == shopIDvector.at(k))
                    {
                        shopBag = readTXTfile(shopID);
                        break;
                    }
                    else
                    {
                        shopIDvector.push_back(shopID);
                        cout << "new customer by " << shopID << endl;
                    }
                }
            }

            if(firstThree == "add")                                                     //If add
            {
                shopBag = add(theProduct, shopBag, pushSubject, shopID);
            }
            else if(firstThree == "get")                                    //If get
            {
                get(shopBag, pushSubject, shopID);
            }
            else if(firstThree == "del")                                //If del
            {
                shopBag = del(theProduct, shopBag, pushSubject, shopID);
            }
            else if(firstThree == "cut")
            {
                cut(pushSubject, shopID);
            }
            else
            {
                unknownCommand(firstThree, pushSubject, shopID);
            }

            writeTXTfile(shopBag, shopID);
            shopBag.clear();
        }
    }
    catch( zmq::error_t & ex ){std::cerr << "Caught an exception : " << ex.what();}

    return 0;
}
vector <string> add(string product, vector <string> bag, string push, string id)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    string sendString = "";

    cout << "add " << product << " by " << id << endl;                                   //Print out the text
    bag.push_back(product);                                          //Add the product to the shopBag
    sendString = push + id + product + " has been added to your basket!";  //Create the string that will be send back
    ventilator.send(sendString.c_str(), sendString.size());                 //Send the string

    return bag;
}

void get(vector <string> bag, string push, string id)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    string products = "";
    string sendString = "";

    cout << "get shopBag by " << id << endl;                              //Print out the text

    for(int j=0; j<bag.size(); j++)
    {
        if(j==0)
        {
            products = bag.at(j);
        }
        else
        {
            products = products + ";" + bag.at(j);
        }
    }
    sendString = push + id + products;                      //Create the string that will be send back
    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
}

vector <string> del(string product, vector <string> bag, string push, string id)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    string sendString = "";

    int i = 0;
    cout << "del " << product << " by " << id << endl;                   //Print out the text
    while(product != bag.at(i)){i++;}                //Look at which positions the element is in the vector
    bag.erase(bag.begin()+i);                       //Remove the element at position i in the vector
    sendString = push + id + product + " is removed";      //Create the string that will be send back
    ventilator.send(sendString.c_str(), sendString.size()); //Send the string

    return bag;
}

void cut(string push, string id)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    string sendString = "";

    std::ofstream txtFile("../txt_files/" + id + ".txt", std::ofstream::out | std::ofstream::trunc);
    txtFile.close();

    sendString = push + id + " bag is removed";      //Create the string that will be send back
    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
}

void unknownCommand(string three, string push, string id)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    string sendString = "";

    cout << three << " unknown command by " << id << endl;      //Print out the text
    sendString = push + id + three + " unknown command"; //Create the string that will be send back
    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
}

vector <string> readTXTfile(string id)
{
    vector <string> bag;
    ifstream txtFile("../txt_files/" + id + ".txt");
    string prouct;
    while(getline(txtFile, prouct)){bag.push_back(prouct);}
    txtFile.close();

    return bag;
}

void writeTXTfile(vector <string> bag,string id)
{
    ofstream txtFile("../txt_files/" + id + ".txt", ios::out);
    for (int j = 0; j<bag.size(); j++){txtFile << bag.at(j) << "\n";} // write each element of the vector to the file
    txtFile.close();
}

string delUppChar(string str)
{
  // Create a regular expression
  const regex pattern("[A-Z]");

  // Replace every matched pattern with the
  // target string using regex_replace() method
  return regex_replace(str, pattern, "");
}

string delLowChar(string str)
{
  // Create a regular expression
  const regex pattern("[a-z]");

  // Replace every matched pattern with the
  // target string using regex_replace() method
  return regex_replace(str, pattern, "");
}
