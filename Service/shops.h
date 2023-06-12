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
        shopS();                        //Constructor

        void writeTXTfile();            //Function for writing in the txt file
        string delUppChar(string str);  //Function for removing the upperchars of a string
        string delLowChar(string str);  //Function for removing the lowerchars of a string
        void sendF();                   //Function to send the sendString
        void add();                     //Function to add someting to the shopBag vector
        void get();                     //Function to get every element of the shopBag vector
        void del();                     //Function to remove an element out of the vector
        void unknownCommand();          //Function for if there is an unkown command
        void readTXTfile();             //Function to read the txt file with the right shopID
        void heartbeat();               //FUnction for the heartbeat test (heartbeatCheck.py needed for this)
        void getNumber();

    private:
        string receivedString = "";     //In this string the received messages will be stored
        string firstThree  = "";        //In this string will be the command stored
        string theProduct =  "";        //In this string will be the product stored
        string pushSubject = "shop?";   //In this string the pushSubject is written
        string recvSubject = "shop!";   //In this string the recvSubject is written
        string shopID = "";             //In this string the shopID will be stored
        string sendString = "";         //In this string the sendString will be stored
        vector <string> shopBag;        //In this vector the product will be saved
        vector <string> shopIDvector;   //In this vector all the shopID will be stored
        string productAmount = "";

};

#endif // SHOPS_H
