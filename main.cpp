#include <iostream>
#include "activite.h"

int main()
{
    Activite* e = new Activite("Reunion", "Chez moi", "Réunion avec Jean-Paul",Type::rendez_vous);

    e->affiche();

    return 1;
}
