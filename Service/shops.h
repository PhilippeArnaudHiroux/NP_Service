#ifndef SHOPS_H
#define SHOPS_H

#include <iostream>
#include <string>
#include <vector>

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

        vector <string> shopBag;
};

#endif // SHOPS_H
