#include <QApplication>
#include <QDebug>

#include <iostream>
#include "system.h"
#include "projetediteur.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);


    ProjetsManager& projets = ProjetsManager::getInstance();
qDebug()<<"ProjetMan : OK";
    ///  ----   PROJET 1  ----  ///
    try
    {
        projets.ajouterProjet("Projet1",QDate(2012,04,01),QDate(2017,01,01));
        projets.ajouterProjet("Projet2",QDate(2012,01,01),QDate(2017,01,01));
        projets.ajouterProjet("Projet3",QDate(2012,01,01),QDate(2017,01,01));
qDebug()<<"Ajout projets : OK";

        TacheManager& tm = projets.getProjet("Projet1");

        tm.ajouterTacheComposite("Phase0",
                                 "Début projet",
                                 QDate(2013,01,01),
                                 QDate(2016, 02,15));

        tm.ajouterTacheUnaire("Tache0",
                              "Preparer l\'ensemble des tests",
                              QDate(2014,12,15),
                              QDate(2014, 12,31),
                              Duree(4,0));

        tm.ajouterTacheComposite("Phase1",
                                 "Phase de test",
                                 QDate(2015,01,01),
                                 QDate(2015, 02,15));

        tm.ajouterTacheUnaire("Tache1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15),
                              Duree(8,0));

        tm.ajouterTacheUnaire("Tache2",
                              "Proceder au test d\'une deuxieme tache unaire",
                              QDate(2015,02,01),
                              QDate(2015, 02,15),
                              Duree(48,0),
                              true);

        tm.getTache("Tache2").ajouterPredecesseur( tm.getTache("Tache1") );

        TacheComposite& p1 = dynamic_cast<TacheComposite&>(tm.getTache("Phase1"));
        p1.ajouterSousTache( tm.getTache("Tache1") );
        p1.ajouterSousTache( tm.getTache("Tache2") );




        TacheComposite& p0 = dynamic_cast<TacheComposite&>(tm.getTache("Phase0"));
        p0.ajouterSousTache( tm.getTache("Phase1") );
        p0.ajouterSousTache( tm.getTache("Tache0") );

        tm.ajouterTacheUnaire("Tache3",
                              "Valider les tests !",
                              QDate(2015,02,16),
                              QDate(2015, 02,18),
                              Duree(3,0));

        tm.getTache("Tache3").ajouterPredecesseur( tm.getTache("Phase1") );
        p1.ajouterPredecesseur( tm.getTache("Tache0"));
 qDebug()<<"Projet1 : OK";

    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }

    ///  ----  PROJET 2  ----  ///
    try
    {
        TacheManager& tm2 = projets.getProjet("Projet2");

        tm2.ajouterTacheUnaire("Tache-0",
                              "Preparer l\'ensemble des tests",
                              QDate(2014,12,15),
                              QDate(2014, 12,31),
                              Duree(4,0));


        tm2.ajouterTacheUnaire("Tache-1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15),
                              Duree(8,0));

        tm2.ajouterTacheUnaire("Tache-2",
                              "Proceder au test d\'une deuxieme tache unaire",
                              QDate(2015,02,01),
                              QDate(2015, 02,15),
                              Duree(48,0),
                              true);

        tm2.getTache("Tache-2").ajouterPredecesseur( tm2.getTache("Tache-1") );

        tm2.ajouterTacheComposite("Phase-1",
                                 "Phase de test",
                                 QDate(2015,01,01),
                                 QDate(2015, 02,15));

        TacheComposite& p1 = dynamic_cast<TacheComposite&>(tm2.getTache("Phase-1"));
        p1.ajouterSousTache( tm2.getTache("Tache-1") );
        p1.ajouterSousTache( tm2.getTache("Tache-2") );

        tm2.ajouterTacheComposite("Phase-0",
                                 "Début projet",
                                 QDate(2013,01,01),
                                 QDate(2016, 02,15));

        TacheComposite& p0 = dynamic_cast<TacheComposite&>(tm2.getTache("Phase-0"));
        p0.ajouterSousTache( tm2.getTache("Phase-1") );
        p0.ajouterSousTache( tm2.getTache("Tache-0") );

        tm2.ajouterTacheUnaire("Tache-3",
                              "Valider les tests !",
                              QDate(2015,02,16),
                              QDate(2015, 02,18),
                              Duree(3,0));

        tm2.getTache("Tache-3").ajouterPredecesseur( tm2.getTache("Phase-1") );
        p1.ajouterPredecesseur( tm2.getTache("Tache-0"));
qDebug()<<"Projet2 : OK";
    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }


    ProjetEditeur*pe;
    try
    {

        pe = new ProjetEditeur(projets.getProjet("Projet1"));
        pe->show();

    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }


    return app.exec();

}
