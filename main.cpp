#include <QApplication>
#include <QDebug>

#include <iostream>
#include "projetediteur.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    TacheManager* tm = new TacheManager("Projet1");

    try
    {
        tm->ajouterTacheUnaire("Tache0",
                              "Preparer l\'ensemble des tests",
                              QDate(2014,12,15),
                              QDate(2014, 12,31),
                              Duree(100,0));

       tm->ajouterTacheComposite("Tache1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15));

//       TacheEditeur* te = new TacheEditeur(*tm, &tm->getTache("Tache0"));
//       te->show();
//       TacheEditeur* te2 = new TacheEditeur(*tm, &tm->getTache("Tache1"));
//       te2->show();


       ProjetEditeur *pe = new ProjetEditeur(*tm);
       pe->show();

    }
    catch(CalendarException e)
    {
        std::cout<<e.getInfo().toStdString();
    }
    catch(...)
    {
        std::cerr<<"Je sais pas";
    }

    return app.exec();

}
