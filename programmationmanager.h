#ifndef PROGRMMATIONMANAGER_H
#define PROGRMMATIONMANAGER_H

#include <iostream>
#include <list>
#include "singleton.h"
#include "taches.h"
#include "programmation.h"
#include "calendar.h"

using namespace std;

class ProgrammationManager
{
public:

    int getSize() {return programmations.size();}
    bool isFree(const QDateTime& d, const Duree& h) const;
    bool isFree(const Programmation* pr, const QDateTime& d, const Duree& h) const;

    void addProgrammation(const QDateTime& da, const Duree& du, Evenement *e);
    void removeProgrammation(const QDateTime& d);
    void removeProgrammation(Programmation *pr);

    const list<Programmation*> getProgrammation(const QDate& debut, const QDate& fin)const;
    Programmation& getProgrammation(const QDateTime& d);
    const Programmation& getProgrammation(const QDateTime& d)const;
private:
    static Handler<ProgrammationManager> handler;
    friend Handler<ProgrammationManager>::~Handler();

    list<Programmation*> programmations;
    void addItem(Programmation* p);
    Programmation* findProgrammation(const QDateTime& d) const;

    ProgrammationManager(){}
    ~ProgrammationManager() {}
    ProgrammationManager(const ProgrammationManager& p);
    ProgrammationManager& operator=(const ProgrammationManager& p);

public:
    static ProgrammationManager& getInstance();
    static void libererInstance();

    void writeXML(const QString &dossier);
    void readXML(QFile &file);
};

#endif // PROGRMMATIONMANAGER_H
