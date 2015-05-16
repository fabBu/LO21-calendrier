#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "programmation.h"

using namespace std;

class ProgrammationManager
{
private:
    list<Programmation*> programmations;
    void addItem(Programmation* p);
    Programmation* findProgrammation(const QDate& d, const Horaire& h) const;
public:
    ProgrammationManager(){}

    int getSize() {return programmations.size();}

    Programmation& getProgrammation(const QDate& d, const Horaire& h) const;

    bool isProgrammationExistante(const QDate& d, const Horaire& h)const;
    void addProgrammation(const QDate& da, const Horaire& h, const Duree& du, const Evenement& e);
    void removeProgrammation();
};

#endif // PROGRMMATIONMANAGER_H
