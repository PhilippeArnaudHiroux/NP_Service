#include <iostream>
#include "zmqservice.h"
#include "commands.h"

using namespace std;

int main()
{
    Commands commandObj;

    commandObj.setInput();
    commandObj.setShopID();
    commandObj.setPorduct();
    commandObj.setCommand();




    return 0;
}
