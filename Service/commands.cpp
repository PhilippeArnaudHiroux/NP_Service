#include "commands.h"

Commands::Commands()
{

}

void Commands::addF()
{
    cout << product << " add by " << shopID << endl;
    shopBag.push_back(product);
    sendStringF(addText, product);
}

void Commands::getF()
{
    cout << "Get by " << shopID << endl;
    for(int i=0; i<shopBag.size(); i++)
    {
        if(i==0)
        {
            getText = shopBag.at(i);
        }
        else
        {
            getText = getText + ";" + shopBag.at(i);
        }
    }
    sendString = getText;
}

void Commands::delF()
{
    int j = 0;
    cout << product << " del by " << shopID << endl;
    for(int i=0; i<shopBag.size(); i++)
    {
        if(product == shopBag.at(i))
        {
            shopBag.erase(shopBag.begin()+i);
            sendStringF(delText, product);
            j++;
            break;
        }
    }
    if(j==0)
    {
        sendString = product + " is not in bag";
    }
}

void Commands::unkownCommand()
{
    cout << command << " is an unkown command by " << shopID << endl;
    sendString = command + " is an unkown command";
}

void Commands::sendStringF(string text, string theProduct)
{
    sendString = theProduct + text;
}
