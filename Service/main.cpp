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

string remove(string removeFrom);

int main( void )
{
    string subTopic = "pah";
    string gekregenString = "";
    string sendString = "";
    string addTekst = " is toegevoegd aan uw mandje!";
    int i = 0;

    vector <string> winkelManje;

    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB ); //Service sub
        zmq::socket_t ventilator( context, ZMQ_PUSH ); //Service push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" ); //Service sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" ); //Service push
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "pah", strlen("pah")); //Service sub

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() && ventilator.connected() )
        {
            //Service sub
            subscriber.recv( msg );
            gekregenString = string((char*) msg->data(), msg->size()); //Vorm ontvangen data om naar een string
            gekregenString = remove(gekregenString); //Verwijder de eerste 3 karakters van de string (sub topic)

            //Kijken naar add of get
            if(gekregenString[0] == 'a')
            {
                gekregenString = remove(gekregenString); //Verwijder 3 eerste karakters
                cout << "product: " << gekregenString << endl;
                winkelManje.push_back(gekregenString); //Voeg het product toe aan het winkelmandje
                sendString = "hpa" + gekregenString + addTekst; //Stel de string samen die terugestuurd wordt
                ventilator.send(sendString.c_str(), sendString.size()); //Stuur de string terug
                ventilator.send("hpaend", strlen("hpaend"));
                i++; //i++ voor het vecotr aantal te weten
            }
            else if(gekregenString[0] == 'g')
            {
                for(int j=0; j<winkelManje.size(); j++)
                {
                    string mandjeTerug = winkelManje.at(j);
                    mandjeTerug = "hpa" + mandjeTerug;
                    ventilator.send(mandjeTerug.c_str(), mandjeTerug.size());
                }
                ventilator.send("hpaend", strlen("hpaend"));
            }

            //Service push
            /*
            ventilator.send( "gelukt", strlen("gelukt"));
            std::cout << "service Push : " << "gelukt" << std::endl;
            */
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}


string remove(string removeFrom)
{
    removeFrom.erase(0,3);
    return removeFrom;
}
