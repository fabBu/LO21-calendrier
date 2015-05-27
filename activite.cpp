#include "activite.h"

void Activite::setType(const Type& t){
    this->type = t;
}

void Activite::affiche(){
    Evenement::affiche();
    std::cout<< "type: " << type << std::endl;
    std::cout << "lieu: " << lieu.toStdString() << std::endl;
}
