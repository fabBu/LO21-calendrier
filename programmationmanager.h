#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "taches.h"
#include "programmation.h"
#include "calendar.h"

using namespace std;

class ProgrammationManager
{
public:

    int getSize() {return programmations.size();}
    bool isFree(const QDateTime& d, const Duree& h) const;

    void addProgrammation(const QDateTime& da, const Duree& du, Evenement *e);
    void removeProgrammation(const QDateTime& d);

    const list<Programmation*> getProgrammation(const QDate& debut, const QDate& fin)const;
    Programmation& getProgrammation(const QDateTime& d);
    const Programmation& getProgrammation(const QDateTime& d)const;
private:
    list<Programmation*> programmations;
    void addItem(Programmation* p);
    Programmation* findProgrammation(const QDateTime& d) const;

    ProgrammationManager(){}
    ~ProgrammationManager() {}
    ProgrammationManager(const ProgrammationManager& p);
    ProgrammationManager& operator=(const ProgrammationManager& p);

    struct Handler
    {
        ProgrammationManager *instance;

        Handler(): instance(0) {}
        ~Handler() {if (instance) delete instance;}
    };

    static Handler handler;

public:
    static ProgrammationManager& getInstance();
    static void libererInstance();
};

#endif // PROGRMMATIONMANAGER_H
