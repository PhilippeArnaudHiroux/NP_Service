#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include <vector>

#include "shops.h"

using namespace std;

class Commands : public ShopS
{
public:
    Commands();
    void addF();
    void getF();
    void delF();
    void unkownCommand();
    void sendStringF(string text, string theProduct);

private:
    string addText = " is add to your bag";
    string getText = "";
    string delText = " is del from your bag";
};

#endif // COMMANDS_H
