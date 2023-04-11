#ifndef DOGS_H
#define DOGS_H

#include "animals.h"
#include <iostream>

class Dogs : public Animals
{
public:
    Dogs(std::string name);

    void sound(int volume);
};

#endif // DOGS_H
