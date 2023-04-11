#ifndef CATS_H
#define CATS_H

#include "animals.h"
#include <iostream>

class Cats : public Animals
{
public:
    Cats(std::string name);

    void sound(int volume);
};

#endif // CATS_H
