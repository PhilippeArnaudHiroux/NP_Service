#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include <regex>

#include "zmqservice.h"

using namespace std;

class Commands
{
    public:
        //Constructor and destructor
        Commands();
        ~Commands();

        //Functions
        void setInput();
        void setShopID();
        void setPorduct();
        void setCommand();

private:
        //Objects
        ZmqService zmqObj;

        //Variabels
        string input = "";
        string shopID = "";
        string product = "";
        string command = "";

        //Pointers
        const regex *lowChar = new const regex ("[a-z]");
        const regex *uppChar = new const regex ("[A-Z]");
};

#endif // COMMANDS_H
