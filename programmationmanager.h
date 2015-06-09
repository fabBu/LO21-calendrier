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

    void addProgrammation(const QDateTime& da, const Duree& du, Evenement *e);
    void removeProgrammation(const QDateTime& d);

    const list<Programmation*> getProgrammation(const QDate& debut, const QDate& fin)const;
    Programmation& getProgrammation(const QDateTime& d);
    const Programmation& getProgrammation(const QDateTime& d)const;
private:
    static Handler<ProgrammationManager> handler;
    friend class Handler<ProgrammationManager>;

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
