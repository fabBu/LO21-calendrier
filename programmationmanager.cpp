#include "programmationmanager.h"

void ProgrammationManager::addItem(Programmation* p){
    programmations.push_back(p);
}

Programmation* ProgrammationManager::findProgrammation(const QDate& d, const Horaire& h) const{
    for(){

    }
}

Programmation& ProgrammationManager::getProgrammation(const QDate& d, const Horaire& h) const;

bool ProgrammationManager::isProgrammationExistante(const QDate& d, const Horaire& h)const;
void ProgrammationManager::addProgrammation(const QDate& da, const Horaire& h, const Duree& du, const Evenement& e);
void ProgrammationManager::removeProgrammation();
