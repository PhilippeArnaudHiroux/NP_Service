#include "split.h"

Split::Split()
{

}

Split::~Split()
{
    delete lowPattern;
    delete uppPattern;
}

void Split::getShopID()
{
    shopID = regex_replace(recvString, *lowPattern, "");
}

void Split::getCommand()
{
    command = regex_replace(recvString, *uppPattern, "");
    command.erase(3, command.size());
}

void Split::getProduct()
{
    product = regex_replace(recvString, *uppPattern, "");
    product.erase(0,3);
}
