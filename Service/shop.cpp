#include "shop.h"

shopC::shopC()
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
}

vector<string> shopC::add(string product, vector<string> bag, string push, string id)
{
    cout << "add " << product << " by " << id << endl;                                   //Print out the text
    bag.push_back(product);                                          //Add the product to the shopBag
    string sendString = push + id + product + " has been added to your basket!";  //Create the string that will be send back
    sendF(sendString);

    return bag;
}

void shopC::get(vector<string> bag, string push, string id)
{
    string products = "";
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
    string sendString = push + id + products;                      //Create the string that will be send back
    sendF(sendString);
}

vector<string> shopC::del(string product, vector<string> bag, string push, string id)
{
    int i = 0;
    cout << "del " << product << " by " << id << endl;                   //Print out the text
    string sendString = "";
    for(int j=0; j<bag.size(); j++)
    {
        if(product == bag.at(j))
        {
            bag.erase(bag.begin()+j);                       //Remove the element at position i in the vector
            sendString = push + id + product + " is removed";      //Create the string that will be send
            i++;
            break;
        }
        if(i==0)
        {
            sendString = push + id + product + " is not in bag";
        }
    }
    sendF(sendString);

    return bag;
}

void shopC::cut(string push, string id)
{
    std::ofstream txtFile("../txt_files/" + id + ".txt", std::ofstream::out | std::ofstream::trunc);
    txtFile.close();
    string sendString = push + id + " bag is removed";      //Create the string that will be send back
    sendF(sendString);
}

void shopC::unknownCommand(string three, string push, string id)
{
    cout << three << " unknown command by " << id << endl;      //Print out the text
    string sendString = push + id + three + " unknown command"; //Create the string that will be send back
    sendF(sendString);
}

vector<string> shopC::readTXTfile(string id)
{
    vector <string> bag;
    ifstream txtFile("../txt_files/" + id + ".txt");
    string prouct;
    while(getline(txtFile, prouct)){bag.push_back(prouct);}
    txtFile.close();

    return bag;
}

void shopC::writeTXTfile(vector<string> bag, string id)
{
    ofstream txtFile("../txt_files/" + id + ".txt", ios::out);
    for (int j = 0; j<bag.size(); j++){txtFile << bag.at(j) << "\n";} // write each element of the vector to the file
    txtFile.close();
}

string shopC::delUppChar(string str)
{
    // Create a regular expression
    const regex pattern("[A-Z]");

    // Replace every matched pattern with the
    // target string using regex_replace() method
    return regex_replace(str, pattern, "");
}

string shopC::delLowChar(string str)
{
    // Create a regular expression
    const regex pattern("[a-z]");

    // Replace every matched pattern with the
    // target string using regex_replace() method
    return regex_replace(str, pattern, "");
}

void shopC::sendF(string sendText)
{
    zmq::context_t context(1);
    zmq::socket_t ventilator( context, ZMQ_PUSH );                      //Service push
    ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );        //Service push
    ventilator.send(sendText.c_str(), sendText.size()); //Send the string
}
