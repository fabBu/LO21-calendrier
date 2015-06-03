#ifndef SYSTEM_H
#define SYSTEM_H

#include "projetsmanager.h"
#include "programmationmanager.h"

class System
{
    ProjetsManager* projets;
public:
    System();
    ProjetsManager* getProjets() { return projets; }
};

#endif // SYSTEM_H
