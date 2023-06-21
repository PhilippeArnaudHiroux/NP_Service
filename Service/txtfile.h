#ifndef TXTFILE_H
#define TXTFILE_H

#include <iostream>
#include <fstream>

#include "shops.h"

using namespace std;

class txtFile : public ShopS
{
public:
    txtFile();
    void readFile();
    void writeFile();
private:
    string readProduct = "";
    string location = ".../txt_files/";
    ofstream writeTXT;
};

#endif // TXTFILE_H
