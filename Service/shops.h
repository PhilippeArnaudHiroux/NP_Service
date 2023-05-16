#ifndef SHOPS_H
#define SHOPS_H

#include <iostream>
#include <string>
#include <zmq.hpp>
#include <cstring>
#include <vector>
#include <regex>
#include <fstream>
#include <Windows.h>
#include <cstdio>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

using namespace std;


class shopS
{
public:
    shopS();
    vector <string> add(string product, vector <string> bag, string push, string id);
    void get(vector <string> bag, string push, string id);
    vector <string> del(string product, vector <string> bag, string push, string id);
    void cut(string push,string id);
    void unknownCommand(string three, string push, string id);
    vector <string> readTXTfile(string id);
    void writeTXTfile(vector <string> bag,string id);
    string delUppChar(string str);
    string delLowChar(string str);
    void sendF(string sendText);

protected:
    string receivedString = ""; //In this string the received messages will be stored
    string firstThree  = "";     //In this string will be the command stored
    string theProduct =  "";     //In this string will be the product stored
    string pushSubject = "shop?";
    string recvSubject = "shop!";
    string shopID = "";
    vector <string> shopBag;    //In this vector the product will be saved
    vector <string> shopIDvector;

};

#endif // SHOPS_H