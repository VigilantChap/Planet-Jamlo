#ifndef DOG_H
#define DOG_H
#include "Entity.h"


class Dog : public Entity
{
public:
	Dog(std::string ID);
	~Dog();
};

#endif // !DOG_H