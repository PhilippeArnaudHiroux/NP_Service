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
                cout << "add " << gekregenString << endl;
                winkelManje.push_back(gekregenString); //Voeg het product toe aan het winkelmandje
                gekregenString = "hpa" + gekregenString + " is toegevoegd aan uw mandje!"; //Stel de string samen die terugestuurd wordt
                ventilator.send(gekregenString.c_str(), gekregenString.size()); //Stuur de string terug
                ventilator.send("hpaend", strlen("hpaend"));
            }
            else if(gekregenString[0] == 'g')
            {
                cout << "get winkelmanje" << endl;
                for(int j=0; j<winkelManje.size(); j++)
                {
                    gekregenString = winkelManje.at(j);
                    gekregenString = "hpa" + gekregenString;
                    ventilator.send(gekregenString.c_str(), gekregenString.size());
                }
                ventilator.send("hpaend", strlen("hpaend"));
            }
            else if(gekregenString[0] == 'd')
            {
                gekregenString = remove(gekregenString); //Verwijder 3 eerste karakters
                cout << "del " << gekregenString << endl;
                while(gekregenString != winkelManje.at(i))
                {
                    i++;
                }
                winkelManje.erase(winkelManje.begin()+i);
                gekregenString = "hpa" + gekregenString + " is verwijderd";
                ventilator.send(gekregenString.c_str(), gekregenString.size());
                ventilator.send("hpaend", strlen("hpaend"));
            }
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
