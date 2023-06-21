#include "txtfile.h"

txtFile::txtFile()
{

}

void txtFile::readFile()
{
    location = location + shopID + ".txt";
    ifstream readTXT(location);
    while(getline(readTXT, readProduct))
    {
        shopBag.push_back(readProduct);
    }
    readTXT.close();
}
