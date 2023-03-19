#include <iostream>
#include <string>
#include <zmq.hpp>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;

int main( void )
{
    try
    {
        zmq::context_t context(1);

        //Incoming messages come in here
        zmq::socket_t subscriber( context, ZMQ_SUB ); //Service sub
        zmq::socket_t ventilator( context, ZMQ_PUSH ); //Service push
        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" ); //Service sub
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" ); //Service push
        subscriber.setsockopt( ZMQ_SUBSCRIBE, "PAH", strlen("PAH") ); //Service sub

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() && ventilator.connected() )
        {
            sleep(1000);
            //Service sub
            subscriber.recv( msg );
            sleep(1000);
            std::cout << "Service sub : [" << std::string( (char*) msg->data(), msg->size() ) << "]" << std::endl;
            sleep(1000);

            //Service push
            ventilator.send( "gelukt", strlen("gelukt"));
            sleep(1000);
            std::cout << "service Push : " << "gelukt" << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
