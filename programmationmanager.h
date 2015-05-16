#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "programmation.h"

using namespace std;

class ProgrammationManager
{
private:
    int nombre;
    list<Programmation> programmations;

public:
    ProgrammationManager();

    void addProgrammation();
    Programmation* findProgrammation();
    void removeProgrammation();
};

#endif // PROGRMMATIONMANAGER_H
