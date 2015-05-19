#include <iostream>
#include "xmlparsor.h"
#include "taches.h"
#include "tachemanager.h"

int main()
{
    TacheManager* tm = XMLparsor::loadProjet("Projets/projet1_chargement.xml");


    std::cout<<"MAIN projet="<<tm->getNom().toStdString()<<"\n";

    /*
    list<TacheManager*> l = list<TacheManager*>();
    XMLparsor::loadProjets(l);

    if( l.empty() )
        std::cout<<"DEG\n";

        */
}
