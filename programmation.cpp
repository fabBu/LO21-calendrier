#include <typeinfo>
#include "programmation.h"

void Programmation::setDate(const QDate& d){
    this->date = d;
}

void Programmation::setHoraire(const Horaire& h){
    this->horaire = h;
}

void Programmation::setDuree(const Duree& d){
    this->duree = d;
}

void Programmation::setEvenement(const Evenement& e){
    this->evenement = &e;
}

Programmation::~Programmation(){
    if (typeid(*evenement).name() == "Activite"){
        delete evenement;
    }
}
