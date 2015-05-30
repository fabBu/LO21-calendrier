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
        tm->ajouterTacheComposite("Phase0",
                                 "Début projet",
                                 QDate(2013,01,01),
                                 QDate(2018, 02,15));

        tm->ajouterTacheUnaire("Tache0",
                              "Preparer l\'ensemble des tests",
                              QDate(2014,12,15),
                              QDate(2014, 12,31),
                              Duree(4,0));
        qDebug()<<"T0";

        tm->ajouterTacheComposite("Phase1",
                                 "Phase de test",
                                 QDate(2015,01,01),
                                 QDate(2015, 02,15));
qDebug()<<"P1";

        tm->ajouterTacheUnaire("Tache1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15),
                              Duree(8,0));
        qDebug()<<"T1";
        tm->ajouterTacheUnaire("Tache2",
                              "Proceder au test d\'une deuxieme tache unaire",
                              QDate(2015,02,01),
                              QDate(2015, 02,15),
                              Duree(48,0),
                              true);
        qDebug()<<"T2";
        tm->getTache("Tache2").ajouterPredecesseur( tm->getTache("Tache1") );

        TacheComposite& p1 = dynamic_cast<TacheComposite&>(tm->getTache("Phase1"));
        p1.ajouterSousTache( tm->getTache("Tache1") );
        p1.ajouterSousTache( tm->getTache("Tache2") );




        TacheComposite& p0 = dynamic_cast<TacheComposite&>(tm->getTache("Phase0"));
        p0.ajouterSousTache( tm->getTache("Phase1") );
        p0.ajouterSousTache( tm->getTache("Tache0") );

        tm->ajouterTacheUnaire("Tache3",
                              "Valider les tests !",
                              QDate(2015,02,16),
                              QDate(2015, 02,18),
                              Duree(3,0));

        tm->getTache("Tache3").ajouterPredecesseur( tm->getTache("Phase1") );
        p1.ajouterPredecesseur( tm->getTache("Tache0"));
qDebug()<<"T3";

       ProjetEditeur *pe = new ProjetEditeur(*tm);
       pe->show();

    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }

    return app.exec();

}
