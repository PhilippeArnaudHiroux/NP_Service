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
        shopC();                        //Constructor
        void createID();                //Fucntion to create your shopID
        string cleanString(string str); //Function to clean up the string
        string delUppChar(string str);  //Function to remove the upperchars from a string
        void splitString();             //Function to split a string
        void printProducts();           //Function to print the products

    private:
        string sendString = "";         //In this string the messages will be stored that will be send
        string receivedString  = "";    //In this string the received messages will be stored
        string shopID = "";             //In this string the shopID will be stored
        string inputString = "";        //In this string the inputString will be stored
        string recvSubject = "shop?";   //In this string the recvSubject is written
        string pushSubject = "shop!";   //In this string the pushSubject is written
        vector <string> products;       //In this vector the products will be stored
        string token;                   //In this string the token will be strored
        int sizeID = (rand() % 5) +5;   //In this integer there will be a random value be stored
};

#endif // SHOPC_H
