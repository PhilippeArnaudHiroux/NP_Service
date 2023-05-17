#ifndef SHOPC_H
#define SHOPC_H

#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include <regex>
#include <sstream>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;

class shopC
{
public:
    shopC();
    void createID();
    string cleanString(string str);
    string delUppChar(string str);
    void splitString();
    void printProducts();

private:
    string sendString = "";         //In this string the messages will be stored that will be send
    string receivedString  = "";    //In this string the received messages will be stored
    string shopID = "";
    string inputString = "";
    string recvSubject = "shop?";
    string pushSubject = "shop!";
    vector <string> products;

    string token;
    int sizeID = (rand() % 5) +5;


};

#endif // SHOPC_H
