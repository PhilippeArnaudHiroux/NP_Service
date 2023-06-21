#include "zmqserver.h"

zmqServer::zmqServer()
{
    recvSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    recvSocket->setsockopt(ZMQ_SUBSCRIBE, recvTopic.c_str(), recvTopic.size());
}

zmqServer::~zmqServer()
{
    delete context;
    delete recvSocket;
    delete sendSocket;
    delete msg;
}

void zmqServer::recvF()
{
    recvSocket->recv(msg);
    recvString = string((char*) msg->data(), msg->size());
    recvString = recvString.erase(0,5);
}
