#include "Evenement.h"

void Evenement::setDescription(const QString& d) {
    this->description = d;
}

void Evenement::setTitre(const QString &t){
    this->titre = t;
}

void Evenement::affiche(){
    std::cout << "EVENEMENT: " << titre.toStdString() << std::endl;
    std::cout << description.toStdString() << std::endl;
}
