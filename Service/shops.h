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
#include <filesystem>


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

    void writeTXTfile();
    string delUppChar(string str);
    string delLowChar(string str);
    void sendF();
    void add();
    void get();
    void del();
    void cut();
    void unknownCommand();
    void readTXTfile();
    void heartbeat();

private:
    string receivedString = ""; //In this string the received messages will be stored
    string firstThree  = "";     //In this string will be the command stored
    string theProduct =  "";     //In this string will be the product stored
    string pushSubject = "shop?";
    string recvSubject = "shop!";
    string shopID = "";
    string sendString = "";
    vector <string> shopBag;    //In this vector the product will be saved
    vector <string> shopIDvector;

};

#endif // SHOPS_H
