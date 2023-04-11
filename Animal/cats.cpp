#include "cats.h"

Cats::Cats(std::string name) : Animals(name)
{

}

void Cats::sound(int volume)
{
    if(volume <4)
    {
        std::cout << name << ": miauw" << std::endl;
    }
    else
    {
        std::cout << name << ": MIAUW" << std::endl;
    }
}
