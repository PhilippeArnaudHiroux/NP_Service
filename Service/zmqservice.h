#ifndef ZMQSERVICE_H
#define ZMQSERVICE_H

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

class ZmqService
{
    public:
        //Constructor and destructor
        ZmqService();
        ~ZmqService();

        //Functions
        void recvF();
        string getRecvString() const;

    private:
        //Zmq
        zmq::context_t *context = new zmq::context_t(1);
        zmq::socket_t *recvSocket = new zmq::socket_t(*context, ZMQ_SUB);
        zmq::socket_t *sendSocket = new zmq::socket_t(*context, ZMQ_PUSH);
        zmq::message_t *msg = new zmq::message_t();

        //Variabels
        string recvTopic = "shop!";
        string sendTopic = "shop?";
        string recvString = "";
};

#endif // ZMQSERVICE_H
