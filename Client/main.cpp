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

int main(void)
{
    string data_send = "";

    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH ); //Client push
        zmq::socket_t subscriber( context, ZMQ_SUB ); //Client sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" ); //Client  push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" ); //Client sub
        //subscriber.setsockopt( ZMQ_SUBSCRIBE, "gelukt", strlen("gelukt") ); //Client sub
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "gelukt", strlen("gelukt") ); //Client sub

        zmq::message_t * msg = new zmq::message_t(); //Client sub
        while( ventilator.connected() && subscriber.connected())
        {
            sleep(1000);
            cout << "Geef een tekst in: ";
            cin >> data_send;

            // Client push
            ventilator.send(data_send.c_str(), data_send.size());
            std::cout << "client push : " << "PAH" << std::endl;

            //Client sub
            subscriber.recv( msg );
            std::cout << "Client sub : [" << std::string( (char*) msg->data(), msg->size() ) << "]" << std::endl;
}
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
