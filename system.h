#ifndef SYSTEM_H
#define SYSTEM_H

#include "tachemanager.h"
#include "programmationmanager.h"

class System
{
    std::list<TacheManager*> projets;
public:
    System();
    void addProject(TacheManager* p) { projets.push_back(p); }
    void removeProject(TacheManager* p) { projets.remove(p); }
};

#endif // SYSTEM_H
