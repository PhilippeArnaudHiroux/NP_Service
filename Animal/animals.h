#ifndef ANIMALS_H
#define ANIMALS_H

#include <string>

class Animals
{
    public:
        Animals(std::string name);

        virtual void sound(int volume) = 0;

    protected:
        std::string name;
};

#endif // ANIMALS_H
