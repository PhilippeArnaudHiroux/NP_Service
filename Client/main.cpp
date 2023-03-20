#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;

string remove(string removeFrom);

int main(void)
{
    string data_send = "";
    string data_ontvangen = "";

    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH ); //Client push
        zmq::socket_t subscriber( context, ZMQ_SUB ); //Client sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" ); //Client  push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" ); //Client sub
        //subscriber.setsockopt( ZMQ_SUBSCRIBE, "gelukt", strlen("gelukt") ); //Client sub
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "hpa", strlen("hpa") ); //Client sub

        zmq::message_t * msg = new zmq::message_t(); //Client sub
        while( ventilator.connected() && subscriber.connected())
        {
            cout << "Geef het commando in: ";
            cin >> data_send;

            // Client push
            ventilator.send(data_send.c_str(), data_send.size());

            //Client sub
            do{
                subscriber.recv( msg );
                data_ontvangen = string( (char*) msg->data(), msg->size() );
                data_ontvangen = remove(data_ontvangen);
                if(data_ontvangen != "end")
                {
                    cout << "    -> " << data_ontvangen << endl;
                }
            }while(data_ontvangen != "end");
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
