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
string takeFirstThree(string removeFrom);

int main( void )
{
    string gekregenString = "";
    string firstThree  ="";
    int i = 0;

    vector <string> winkelManje;

    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB );                   //Service sub
        zmq::socket_t ventilator( context, ZMQ_PUSH );                  //Service push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );    //Service sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );    //Service push
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "pah", strlen("pah"));    //Service sub

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() && ventilator.connected() )
        {
            subscriber.recv( msg );                                     //Ontvang het bericht
            gekregenString = string((char*) msg->data(), msg->size());  //Vorm ontvangen data om naar een string
            gekregenString = remove(gekregenString);                    //Verwijder de eerste 3 karakters van de string (sub topic)
            firstThree = takeFirstThree(gekregenString);

            if(firstThree == "add")                                                    //Als er add moet gebeuren
            {
                gekregenString = remove(gekregenString);                                    //Verwijder 3 eerste karakters
                cout << "add " << gekregenString << endl;                                   //Print het commando uit
                winkelManje.push_back(gekregenString);                                      //Voeg het product toe aan het winkelmandje
                gekregenString = "hpa" + gekregenString + " is toegevoegd aan uw mandje!";  //Stel de string samen die terugestuurd wordt
                ventilator.send(gekregenString.c_str(), gekregenString.size());             //Stuur de string terug
                //ventilator.send("hpaend", strlen("hpaend"));                                //Stuur door dat client mag stoppen met luisteren
            }
            else if(firstThree == "get")                                       //Als er get moet gebeuren
            {
                cout << "get winkelmanje" << endl;                                  //Print het commando uit
                for(int j=0; j<winkelManje.size(); j++)                             //Doe dit evenvaak als de vector groot is
                {
                    gekregenString = winkelManje.at(j);                             //Neem het element uit de vector kopieer het in gekregenString
                    gekregenString = "hpa" + gekregenString;                        //Stel de string samen die tergestuurd wordt
                    ventilator.send(gekregenString.c_str(), gekregenString.size()); //Stuur se string door
                }
                //ventilator.send("hpaend", strlen("hpaend"));                        //Stuur door dat client mag stoppen met luisteren
            }
            else if(firstThree == "del")                                   //Als er del moet gebeuren
            {
                gekregenString = remove(gekregenString);                        //Verwijder 3 eerste karakters
                cout << "del " << gekregenString << endl;                       //Print het commando uit
                while(gekregenString != winkelManje.at(i)){i++;}                //Kijk op welke posities het element staat in de vector
                winkelManje.erase(winkelManje.begin()+i);                       //Verwijder het element op positie i in de vector
                gekregenString = "hpa" + gekregenString + " is verwijderd";     //Stel de string samen
                ventilator.send(gekregenString.c_str(), gekregenString.size()); //Stuur se string door
                //ventilator.send("hpaend", strlen("hpaend"));                    //Stuur door dat client mag stoppen met luisteren
            }
            else
            {
                gekregenString = takeFirstThree(gekregenString);
                cout << gekregenString << " is geen commando" << endl;
                gekregenString = "hpa" + gekregenString + " onbekend comando";     //Stel de string samen
                ventilator.send(gekregenString.c_str(), gekregenString.size()); //Stuur se string door
                //ventilator.send("hpaend", strlen("hpaend"));                    //Stuur door dat client mag stoppen met luisteren
            }
            ventilator.send("hpaend", strlen("hpaend"));                    //Stuur door dat client mag stoppen met luisteren
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}

string remove(string removeFrom) //Een fuctie waarmee steeds de eerste 3 karakters van een string worden verwijderd
{
    removeFrom.erase(0,3);
    return removeFrom;
}

string takeFirstThree(string removeFrom) //Een fuctie waarmee de eerste 3 karakters van een string worden behouden
{
    removeFrom.erase(3,removeFrom.size());
    return removeFrom;
}
