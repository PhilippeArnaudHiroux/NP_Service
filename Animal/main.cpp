#include <iostream>
#include <vector>

#include <animals.h>
#include <dogs.h>
#include <horses.h>
#include <cats.h>

int main()
{
    std::vector <Animals*> dieren;

    dieren.push_back(new Dogs("Bart"));
    dieren.push_back(new Cats("Dieter"));
    dieren.push_back(new Horses("Vincent"));
    dieren.push_back(new Dogs("Wout"));

    for(int i=0; i<4; i++)
    {
        dieren.at(i)->sound(5);
    }

    return 0;
}
