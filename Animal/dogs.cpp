#include "dogs.h"

Dogs::Dogs(std::string name) : Animals(name)
{

}

void Dogs::sound(int volume)
{
    if(volume <3)
    {
        std::cout << name << ": woef" << std::endl;
    }
    else if(volume <7)
    {
        std::cout << name << ": waf waf" << std::endl;
    }
    else
    {
        std::cout << name << ": grrrr WAF" << std::endl;
    }
}
