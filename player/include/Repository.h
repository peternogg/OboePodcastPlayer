#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Persistable.h"

class Repository {
public:
    bool store(Persistable* object);
private:
    bool insert(Persistable* object);
    bool update(Persistable* object);
};

#endif // REPOSITORY_H
