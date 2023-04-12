#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#include <vector>
#include <regex>
#include <fstream>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;
string delUppChar(string str);
string delLowChar(string str);

int main( void )
{
    string receivedString = ""; //In this string the received messages will be stored
    string sendString =  "";     //In this string the messages will be stored that will be send
    string firstThree  = "";     //In this string will be the command stored
    string theProduct =  "";     //In this string will be the product stored
    string pushSubject = "shop?";
    string recvSubject = "shop!";
    string shopID = "";
    vector <string> shopBag;    //In this vector the product will be saved
    vector <string> shopIDvector;
    int i = 0;

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
            //vector <string> shopBag;    //In this vector the product will be saved
            //shopBag.clear();
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
            }
            else
            {
                for(int k=0; k<shopIDvector.size(); k++)
                {
                    if(shopID == shopIDvector.at(k))
                    {
                        ifstream txtFile("../txt_files/" + shopID + ".txt");
                        string prouct;
                        while(getline(txtFile, prouct))
                        {
                            shopBag.push_back(prouct);
                        } // add each number to the vector
                        txtFile.close();
                    }
                }
            }

            if(firstThree == "add")                                                     //If add
            {
                cout << "add " << theProduct << endl;                                   //Print out the text
                shopBag.push_back(theProduct);                                          //Add the product to the shopBag
                sendString = pushSubject + shopID + theProduct + " has been added to your basket!";  //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size());                 //Send the string
            }
            else if(firstThree == "get")                                    //If get
            {
                cout << "get shopBag" << endl;                              //Print out the text
                for(int j=0; j<shopBag.size(); j++)                         //Do this as many times as the vector is large
                {
                    theProduct = shopBag.at(j);                             //Take the element from the vector and copy it into theProduct
                    sendString = pushSubject + shopID + theProduct;                      //Create the string that will be send back
                    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
                }
            }
            else if(firstThree == "del")                                //If del
            {
                cout << "del " << theProduct << endl;                   //Print out the text
                while(theProduct != shopBag.at(i)){i++;}                //Look at which positions the element is in the vector
                shopBag.erase(shopBag.begin()+i);                       //Remove the element at position i in the vector
                sendString = pushSubject + shopID + theProduct + " is removed";      //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size()); //Send the string
            }
            else
            {
                cout << firstThree << " is not a command" << endl;      //Print out the text
                sendString = pushSubject + shopID + firstThree + " unknown command"; //Create the string that will be send back
                ventilator.send(sendString.c_str(), sendString.size()); //Send the string
            }
            sendString = pushSubject + shopID + "end";
            ventilator.send(sendString.c_str(), sendString.size());    //Forward that client may stop listening

            ofstream txtFile("../txt_files/" + shopID + ".txt", ios::out);
            for (int j = 0; j<shopBag.size(); j++) {
                    txtFile << shopBag.at(j) << "\n"; // write each element of the vector to the file
                }
            //shopBag.clear();
            txtFile.close();
            shopBag.clear();
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
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
