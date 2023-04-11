#include "horses.h"

Horses::Horses(std::string name) : Animals(name)
{

}

void Horses::sound(int volume)
{
    if(volume < 5)
    {
        std::cout << name << ": hihihi" << std::endl;
    }
    else
    {
        std::cout << name << ": HIHIHI" << std::endl;
    }
}
