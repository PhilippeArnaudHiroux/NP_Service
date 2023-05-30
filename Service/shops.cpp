#include "shops.h"

shopS::shopS()
{
    try
    {
        zmq::context_t context(1);                                                      //ZeroMQ context object
        zmq::socket_t subscriber(context, ZMQ_SUB);                                     //Subscriber is to SUB
        zmq::socket_t ventilator(context, ZMQ_PUSH);                                    //Ventilator is to PUSH
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");                      //Lissen to this adress
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");                      //Send to this adress
        subscriber.setsockopt(ZMQ_SUBSCRIBE, recvSubject.c_str(), recvSubject.size());  //Receive only the topic that you need

        zmq::message_t * msg = new zmq::message_t();                                    //An incoming message will be stored in msg
        while( subscriber.connected() && ventilator.connected() )                       //Keep running this code as long as there is conection
        {
            subscriber.recv(msg);                                                       //Receive the message and save it in msg
            receivedString = string((char*) msg->data(), msg->size());                  //Convert the received message to a string
            receivedString.erase(0,5);                                                  //Remove the first 5 characters of the string (sub topic)
            shopID = delLowChar(receivedString);                                        //Delete all lowerchars of the receivedString
            firstThree = delUppChar(receivedString);                                    //Set firstThree equal to receivedString without all the upperchars in it
            theProduct = delUppChar(receivedString);                                    //Set theProduct equal to receivedString without all the upperchars in it
            firstThree.erase(3,firstThree.size());                                      //Remove everything after the first 3 characters in firstThree string
            theProduct.erase(0, 3);                                                     //Remove the first 3 characters of theProduct string
            int controle = 0;                                                           //An integer to store a controle number in it

            if(firstThree == "***")                                                     //If the firstThree is equel to *** run this code
            {
                heartbeat();                                                            //Run the heartbeat function
            }
            else                                                                        //If the firstThree is not equel to *** run this code
            {
                if(shopIDvector.size() != 0)                                            //When the shopIDvector is not empty run this code
                {
                    for(int k=0; k<shopIDvector.size(); k++)                            //Do this as much times as the size of shopIDvector
                    {
                        if(shopID == shopIDvector.at(k))                                //When the shopID is in the shopIDvector run this code
                        {
                            readTXTfile();                                              //Run the readTXTfile function
                            controle++;                                                 //Do controle + 1
                            break;                                                      //Break out of this code
                        }
                    }
                }
                if(controle == 0)                                                       //If the shopID was not in the shopIDvector the controle is going to be 0
                {
                    shopIDvector.push_back(shopID);                                     //Put the new shopID in the shopIDvector
                    cout << "new customer by " << shopID << endl;                       //Print out this text
                }

                if(firstThree == "add")                                                 //If firstThree is equel to add run this code
                {
                    add();                                                              //Run the add function
                }
                else if(firstThree == "get")                                            //If firstThree is equel to get run this code
                {
                    get();                                                              //Run the get function
                }
                else if(firstThree == "del")                                            //If firstThree is equel to del run this code
                {
                    del();                                                              //Run the del function
                }
                else                                                                    //If the firstThree is not equel to someting above run this code
                {
                    unknownCommand();                                                   //Run the unknownCommand function
                }

                writeTXTfile();                                                         //Run the writeTXTfile function
                shopBag.clear();                                                        //Clear the shopBag vector so its empty and ready to use again
            }
        }
    }
    catch( zmq::error_t & ex ){std::cerr << "Caught an exception : " << ex.what();}     //When there is an error run this code
}

void shopS::add()                                                                       //The add function is to add something to your shopBag
{
    cout << "add " << theProduct << " by " << shopID << endl;                           //Print out the text
    shopBag.push_back(theProduct);                                                      //Add the product to the shopBag
    sendString = pushSubject + shopID + theProduct + " has been added to your basket!"; //Create the string that will be send back
    sendF();                                                                            //Run the sendF function
}

void shopS::get()                                                                       //The get function is the get all elements of your shopBag
{
    string products = "";                                                               //Create a string to save product in
    cout << "get shopBag by " << shopID << endl;                                        //Print out the text
    for(int j=0; j<shopBag.size(); j++)                                                 //Do this as much times as the size of shopBag
    {
        if(j==0)                                                                        //If j is equel to 0 run this code
        {
            products = shopBag.at(j);                                                   //Put the element of position 0 in products
        }
        else                                                                            //If j in not equel to 0 run this code
        {
            products = products + ";" + shopBag.at(j);                                  //Paste a new element of the shopBag behind product with a ; between them
        }
    }
    sendString = pushSubject + shopID + products;                                       //Create the string that will be send back
    sendF();                                                                            //Run the sendF function
}

void shopS::del()                                                                       //The del function is for removing an element out of your shopBag
{
    int i = 0;                                                                          //Create an integer and make it equal to 0
    cout << "del " << theProduct << " by " << shopID << endl;                           //Print out the text
    for(int j=0; j<shopBag.size(); j++)                                                 //Do this as much times as the size of shopBag
    {
        if(theProduct == shopBag.at(j))                                                 //If the product is in the shopBag run this code
        {
            shopBag.erase(shopBag.begin()+j);                                           //Remove the element at position j in the vector
            sendString = pushSubject + shopID + theProduct + " is removed";             //Create the string that will be send
            i++;                                                                        //Do i + 1
            break;                                                                      //Break out of this code
        }
    }
    if(i==0)                                                                            //If i is equel to 0 run this code (i will be equel to 0 if the product is not find in the shopBag)
    {
        sendString = pushSubject + shopID + theProduct + " is not in bag";              //Create the string that will be send
    }
    sendF();                                                                            //Run the sendF function
}

void shopS::unknownCommand()                                                            //The unknownCommand function is for when you send a command that is not correct
{
    cout << firstThree << " unknown command by " << shopID << endl;                     //Print out the text
    sendString = pushSubject + shopID + firstThree + " unknown command";                //Create the string that will be send back
    sendF();                                                                            //Run the sendF function
}

void shopS::readTXTfile()                                                               //The unknownCommand function is to read the txt file with the right shopID
{
    ifstream txtFile("../txt_files/" + shopID + ".txt");                                //Open the txt file
    string prouct;                                                                      //Create a string to store the product in from the txt file
    while(getline(txtFile, prouct))                                                     //Read every line of the txt file and right save the element in product
    {
        shopBag.push_back(prouct);                                                      //Push_back the product in the shopBag vector
    }
    txtFile.close();                                                                    //Close the txt file
}

void shopS::heartbeat()                                                                 //The heartbeat function is the check if the service is still running (this function works together with the heartbeatCheck.py script)
{
    sendString = "shop?***";                                                            //Create the string that will be send back
    sendF();                                                                            //Run the sendF function
}

void shopS::writeTXTfile()                                                              //The writeTXTfile function re-write the txt file
{
    ofstream txtFile("../txt_files/" + shopID + ".txt", ios::out);                      //Open the txt file
    for (int j = 0; j<shopBag.size(); j++){txtFile << shopBag.at(j) << "\n";}           //Write each element of the vector to the file
    txtFile.close();                                                                    //Close the txt file
}

string shopS::delUppChar(string str)                                                    //The dellUppChar function will remove all the upperchars of a string
{
    const regex pattern("[A-Z]");                                                       //Create a regular expression from A to Z
    return regex_replace(str, pattern, "");                                             //Return the string without the upperchars in it
}

string shopS::delLowChar(string str)                                                    //The delLowChar function will remove all the lowwerchars of a string
{
    const regex pattern("[a-z]");                                                       //Create a regular expression from a to z
    return regex_replace(str, pattern, "");                                             //Return the string without the lowwerchars in it
}

void shopS::sendF()                                                                     //The sendF function is the function that will be used to send the sendString in to the world
{
    zmq::context_t context(1);                                                          //ZeroMQ context object
    zmq::socket_t ventilator(context, ZMQ_PUSH);                                        //ventilator is to PUSH
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );                        //Send to this adress
    ventilator.send(sendString.c_str(), sendString.size());                             //Send the string in to the world
}
