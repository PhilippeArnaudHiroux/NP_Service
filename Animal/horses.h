#ifndef HORSES_H
#define HORSES_H

#include "animals.h"
#include <iostream>

class Horses : public Animals
{
public:
    Horses(std::string name);

    void sound(int volume);
};

#endif // HORSES_H
