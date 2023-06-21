#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <iostream>
#include <string>
#include <zmq.hpp>

using namespace std;

#include "shops.h"

class zmqServer : public ShopS
{
    public:
        zmqServer();
        ~zmqServer();

        void recvF();

    private:
        zmq::context_t *context = new zmq::context_t(1);
        zmq::socket_t *recvSocket = new zmq::socket_t(*context, ZMQ_SUB);
        zmq::socket_t *sendSocket = new zmq::socket_t(*context, ZMQ_PUSH);
        zmq::message_t *msg = new zmq::message_t();

        string recvTopic = "shop!";
        string sendTopic = "shop?";
};

#endif // ZMQSERVER_H
