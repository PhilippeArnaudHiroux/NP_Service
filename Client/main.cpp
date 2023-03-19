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

int main(void)
{
    try
    {
        zmq::context_t context(1);

        //Outgoing message go out through here
        zmq::socket_t ventilator( context, ZMQ_PUSH );
        ventilator.connect( "tcp://benternet.pxl-ea-ict.be:24041" );
        //ventilator.connect( "tcp://localhost:24041" );

        while( ventilator.connected() )
        {
            sleep( 1000 );
            //ventilator.send( &datatest, lengte_datatest);
            ventilator.send( "PAH", strlen("PAH"));
            std::cout << "Pushed : " << "PAH_test" << std::endl;
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}
