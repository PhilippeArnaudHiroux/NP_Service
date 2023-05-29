#include "shops.h"

shopS::shopS()
{
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
            int controle = 0;

            if(shopIDvector.size() != 0)
            {
                for(int k=0; k<shopIDvector.size(); k++)
                {
                    if(shopID == shopIDvector.at(k))
                    {
                        readTXTfile();
                        controle++;
                        break;
                    } 
                }
            }
            if(controle == 0)
            {
                shopIDvector.push_back(shopID);
                cout << "new customer by " << shopID << endl;
            }

            if(firstThree == "add")                                                     //If add
            {
                add();
            }
            else if(firstThree == "get")                                    //If get
            {
                get();
            }
            else if(firstThree == "del")                                //If del
            {
                del();
            }
            else if(firstThree == "cut")
            {
                cut();
            }
            else
            {
                unknownCommand();
            }

            writeTXTfile();
            shopBag.clear();
        }
    }
    catch( zmq::error_t & ex ){std::cerr << "Caught an exception : " << ex.what();}
}

void shopS::add()
{
    cout << "add " << theProduct << " by " << shopID << endl;                                   //Print out the text
    shopBag.push_back(theProduct);                                          //Add the product to the shopBag
    sendString = pushSubject + shopID + theProduct + " has been added to your basket!";  //Create the string that will be send back
    sendF();
}

void shopS::get()
{
    string products = "";
    cout << "get shopBag by " << shopID << endl;                              //Print out the text
    for(int j=0; j<shopBag.size(); j++)
    {
        if(j==0)
        {
            products = shopBag.at(j);
        }
        else
        {
            products = products + ";" + shopBag.at(j);
        }
    }
    sendString = pushSubject + shopID + products;                      //Create the string that will be send back
    sendF();
}

void shopS::del()
{
    int i = 0;
    cout << "del " << theProduct << " by " << shopID << endl;                   //Print out the text
    sendString = "";
    for(int j=0; j<shopBag.size(); j++)
    {
        if(theProduct == shopBag.at(j))
        {
            shopBag.erase(shopBag.begin()+j);                       //Remove the element at position i in the vector
            sendString = pushSubject + shopID + theProduct + " is removed";      //Create the string that will be send
            i++;
            break;
        }
        if(i==0)
        {
            sendString = pushSubject + shopID + theProduct + " is not in bag";
        }
    }
    sendF();
}

void shopS::cut()
{
    /*
    std::ofstream txtFile("../txt_files/" + shopID + ".txt", std::ofstream::out | std::ofstream::trunc);
    txtFile.close();
    sendString = pushSubject + shopID + " bag is removed";      //Create the string that will be send back
    sendF();
    */

    string test = "../txt_files/" + shopID + ".txt";
    std::filesystem::remove(test);
    sendString = pushSubject + shopID + " bag is removed";      //Create the string that will be send back
    sendF();
}

void shopS::unknownCommand()
{
    cout << firstThree << " unknown command by " << shopID << endl;      //Print out the text
    sendString = pushSubject + shopID + firstThree + " unknown command"; //Create the string that will be send back
    sendF();
}

void shopS::readTXTfile()
{
    ifstream txtFile("../txt_files/" + shopID + ".txt");
    string prouct;
    while(getline(txtFile, prouct))
    {
        shopBag.push_back(prouct);
    }
    txtFile.close();
}

void shopS::writeTXTfile()
{
    ofstream txtFile("../txt_files/" + shopID + ".txt", ios::out);
    for (int j = 0; j<shopBag.size(); j++){txtFile << shopBag.at(j) << "\n";} // write each element of the vector to the file
    txtFile.close();
}

string shopS::delUppChar(string str)
{
    // Create a regular expression
    const regex pattern("[A-Z]");

    // Replace every matched pattern with the
    // target string using regex_replace() method
    return regex_replace(str, pattern, "");
}

string shopS::delLowChar(string str)
{
    // Create a regular expression
    const regex pattern("[a-z]");

    // Replace every matched pattern with the
    // target string using regex_replace() method
    return regex_replace(str, pattern, "");
}

void shopS::sendF()
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    ventilator.send(sendString.c_str(), sendString.size()); //Send the string
}
