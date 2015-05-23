#include <QApplication>

#include <iostream>
#include "tacheediteur.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    TacheManager tm("Projet1");

    try
    {
        tm.ajouterTacheUnaire("Tache0",
                              "Preparer l\'ensemble des tests",
                              QDate(2014,12,15),
                              QDate(2014, 12,31),
                              Duree(100,0));

        tm.ajouterTacheUnaire("Tache1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15),
                              Duree(48,0));

        tm.ajouterTacheUnaire("Tache2",
                              "Proceder au test d\'une deuxieme tache unaire",
                              QDate(2015,02,01),
                              QDate(2015, 02,15),
                              Duree(456,0),
                              true);

        tm.getTache("Tache2").ajouterPredecesseur( tm.getTache("Tache1") );

        tm.ajouterTacheComposite("Phase1",
                                 "Phase de test",
                                 QDate(2015,01,01),
                                 QDate(2015, 02,15));

        TacheComposite& p = dynamic_cast<TacheComposite&>(tm.getTache("Phase1"));
        p.ajouterSousTache( tm.getTache("Tache1") );
        p.ajouterSousTache( tm.getTache("Tache2") );
        // p.retirerSousTache( tm.getTache("Tache2") );      // OK : fonctionne
        p.ajouterPredecesseur( tm.getTache("Tache0"));

        tm.ajouterTacheUnaire("Tache3",
                              "Valider les tests !",
                              QDate(2015,02,16),
                              QDate(2015, 02,18),
                              Duree(48,0));

        tm.getTache("Tache3").ajouterPredecesseur( tm.getTache("Phase1") );
        tm.getTache("Tache3").ajouterPredecesseur( tm.getTache("Tache0") );


        Tache& t1 = tm.getTache("Tache2");
        TacheEditeur* fenetre1 = new TacheEditeur(tm, &t1);
        fenetre1->show();

        Tache& t2 = tm.getTache("Phase1");
        TacheEditeur* fenetre2 = new TacheEditeur(tm, &t2);
        fenetre2->show();

        Tache& t3 = tm.getTache("Tache3");
        TacheEditeur* fenetre3 = new TacheEditeur(tm, &t3);
        fenetre3->show();

        Tache& t4 = tm.getTache("Tache0");
        TacheEditeur* fenetre4 = new TacheEditeur(tm, &t4);
        fenetre4->show();

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
