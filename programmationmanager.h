#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "programmation.h"
#include "calendar.h"

using namespace std;

class ProgrammationManager
{
private:
    list<Programmation*> programmations;
    void addItem(Programmation* p);
    Programmation* findProgrammation(const QDateTime& d) const;

public:
    ProgrammationManager(){}

    int getSize() {return programmations.size();}
    bool isFree(const QDateTime& d, const QTime& h) const;

    void addProgrammation(const QDateTime& da, const QTime& du, Evenement& e);
    void removeProgrammation(const QDateTime& d);

    Programmation& getProgrammation(const QDateTime& d);
    const Programmation& getProgrammation(const QDateTime& d)const;
};

#endif // PROGRMMATIONMANAGER_H
