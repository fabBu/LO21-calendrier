#include "programmation.h"

void Programmation::setDate(const QDateTime& d){
    this->date = d;
}

void Programmation::setDuree(const QTime& d){
    this->duree = d;
}

void Programmation::setEvenement(Evenement& e){
    this->evenement = &e;
}

Programmation::~Programmation(){
    Activite* ac = dynamic_cast<Activite*>(evenement);

    if (ac){
        delete evenement;
    }
}
