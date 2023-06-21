#ifndef SPLIT_H
#define SPLIT_H

#include <iostream>
#include <string>
#include <regex>

#include "shops.h"

class Split : public ShopS
{
public:
    Split();
    ~Split();

    void getShopID();
    void getCommand();
    void getProduct();

private:
    const regex *lowPattern = new const regex ("[a-z");
    const regex *uppPattern = new const regex ("[A-Z]");
};

#endif // SPLIT_H
