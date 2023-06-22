#include "zmqservice.h"

ZmqService::ZmqService()
{
    recvSocket->connect("tcp://benternet.pxl-ea-ict.be:24042");
    sendSocket->connect("tcp://benternet.pxl-ea-ict.be:24041");
    recvSocket->setsockopt(ZMQ_SUBSCRIBE, recvTopic.c_str(), recvTopic.size());
}

ZmqService::~ZmqService()
{
    delete context;
    delete recvSocket;
    delete sendSocket;
    delete msg;
}

void ZmqService::recvF()
{
    recvSocket->recv(msg);
    recvString = string((char*) msg->data(), msg->size());
    recvString.erase(0,5);
}

string ZmqService::getRecvString() const
{
    return recvString;
}
