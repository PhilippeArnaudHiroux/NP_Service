#ifndef SHOPS_H
#define SHOPS_H

#include <iostream>
#include <string>

using namespace std;

class ShopS
{
    public:
        ShopS();
    protected:
        string recvString = "";
        string sendString = "";

        string shopID = "";
        string command = "";
        string product = "";
};

#endif // SHOPS_H
