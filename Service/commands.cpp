#include "commands.h"

Commands::Commands()
{
    zmqObj.recvF();
}

Commands::~Commands()
{
    delete lowChar;
    delete uppChar;
}

void Commands::setInput()
{
    input = zmqObj.getRecvString();
    cout << input << endl;
}

void Commands::setShopID()
{
    shopID = regex_replace(input, *lowChar, "");
    cout << "DEBUG 1 " << shopID << endl;
}

void Commands::setPorduct()
{
    product = regex_replace(input, *uppChar, "");
    product.erase(0,3);
    cout << "DEBUG 2 " << product << endl;
}

void Commands::setCommand()
{
    command = regex_replace(input, *uppChar, "");
    command.erase(3, command.size());
    cout << "DEBUG 3 " << command << endl;
}

