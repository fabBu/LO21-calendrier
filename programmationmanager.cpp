#include "programmationmanager.h"

ProgrammationManager::Handler ProgrammationManager::handler=ProgrammationManager::Handler();

void ProgrammationManager::addItem(Programmation* p){
    if (!isFree(p->getDate(),p->getDuree()))
        throw CalendarException("ERREUR: Un événement est déjà programmé à cette date.");
    programmations.push_back(p);
}


Programmation* ProgrammationManager::findProgrammation(const QDateTime& d) const{
    for (list<Programmation*>::const_iterator it = programmations.begin(); it != programmations.end(); it++){
        if ((*it)->getDate() == d){
            return *it;
        }
    }
    return 0;
}

bool ProgrammationManager::isFree(const QDateTime& d, const QTime& h) const{
    QDateTime fin = d + h;
    for (list<Programmation*>::const_iterator it = programmations.begin(); it != programmations.end(); it++){
        if ((*it)->getDate() > d && (*it)->getDate() < fin ) return false;
    }
    return true;
}

Programmation& ProgrammationManager::getProgrammation(const QDateTime& d){
    Programmation* p = findProgrammation(d);
    if (!p) throw CalendarException("ERREUR: Activité inexistante");
    return *p;
}

const Programmation& ProgrammationManager::getProgrammation(const QDateTime& d) const{
    return const_cast<ProgrammationManager*>(this)->getProgrammation(d);
}

void ProgrammationManager::addProgrammation(const QDateTime& da, const QTime& du, Evenement& e){
    // TODO try {
        Programmation* p = new Programmation(da,du,e);
        addItem(p);
//    } catch (exception e) {}
}

void ProgrammationManager::removeProgrammation(const QDateTime& d){
    Programmation* p = findProgrammation(d);
    if (!p) throw CalendarException("ERREUR: retrait d'un événement inexistant impossible.");
    programmations.remove(p);
}

ProgrammationManager& ProgrammationManager::getInstance()
{
    if (!handler.instance)
        handler.instance = new ProgrammationManager();

    return *handler.instance;
}

void ProgrammationManager::libererInstance()
{
    if (handler.instance)
        delete handler.instance;

    handler.instance = 0;
}
