#include "mainwindow.h"

MainWindow::MainWindow()
    :projets(ProjetsManager::getInstance()), agenda(ProgrammationManager::getInstance())//, agenda_ouvert(0)
{
    setWindowTitle("Le super logiciel");
    setMinimumHeight(700);
    setMinimumWidth(850);

    initMenuBar();

    onglets = new QTabWidget(this);
    onglets->setTabsClosable(true);
    onglets->setMovable(true);
    connect( onglets,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)) );
    setCentralWidget(onglets);

    chargerProjets();
}

void MainWindow::initMenuBar()
{
    menubar = new QMenuBar(this);

    QMenu* menu_agenda = menubar->addMenu("Ouvrir Agenda");

    QMenu* menu_projets = menubar->addMenu("Projets");
    QAction *menu_projets_creer = menu_projets->addAction("Créer projet");
    QAction *menu_projets_ouvrir = menu_projets->addAction("Ouvrir projet");
    connect(menu_projets_creer, SIGNAL(triggered(bool)), this, SLOT(creerProjet()));
    connect(menu_projets_ouvrir, SIGNAL(triggered(bool)), this, SLOT(ouvrirProjet()));

    setMenuBar(menubar);
}

ProjetEditeur* MainWindow::getProjetEdit(const QString nom)
{
    for(std::list<ProjetEditeur*>::const_iterator it=projets_ouverts.begin() ; it!=projets_ouverts.end() ; ++it)
    {
        if( (*it)->getNom() == nom )
            return *it;
    }

    return 0;
}

void MainWindow::ouvrirAgenda()
{
    /*
    if(agenda_ouvert)
        onglets->addTab(new AgendaEditeur(), "AGENDA");
    else
        onglets->setCurrentWidget( agenda_ouvert );
    */
}

void MainWindow::creerProjet()
{
    ProprieteProjetEditeur *ppe = new ProprieteProjetEditeur(this);
    ppe->show();
}

void MainWindow::ouvrirProjet()
{
    const std::list<TacheManager*> liste_projets = projets.getProjets();
    if( liste_projets.size() == 0 )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Ouverture projet",
                                      "Il n'existe aucun projet actuellement.\nVoulez-vous en créer un ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            try
            {
                ProprieteProjetEditeur *ppe = new ProprieteProjetEditeur(this);
                ppe->show();
            }
            catch(CalendarException e)
            { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
        }
    }
    else
    {
        if( liste_projets.size() == projets_ouverts.size() )
        {
            QMessageBox::warning(this, "Ouverture projet", "Tous les projets sont ouverts");
            return;
        }
        QStringList l_projets;
        for(std::list<TacheManager*>::const_iterator it=liste_projets.begin() ; it!=liste_projets.end() ; ++it)
        {
            if( !estOuvert((*it)->getNom()) )
                l_projets<<(*it)->getNom();
        }

        bool ok;
        QString projet = QInputDialog::getItem(this, "Choisir projet à consulter", "Ouvrir:", l_projets,0, false, &ok);

        try
        {
            if( ok )
            {
                ProjetEditeur* pe = new ProjetEditeur(projets.getProjet(projet));
                connect(pe, SIGNAL(fermeture(QString)), this,  SLOT(fermerProjet(QString)));
                projets_ouverts.push_back(pe);
                onglets->addTab( pe,projet);
            }
        }
        catch(CalendarException e)
        { QMessageBox::warning(this, "Ouverture projet", e.getInfo()); }
    }
}


void MainWindow::fermerProjet(const QString nom)
{
    ProjetEditeur* pe = getProjetEdit(nom);
    if( pe )
    {
        // Retirer le projet de la liste des ProjetEditeur ouverts puis des onglets
        projets_ouverts.erase( std::remove(projets_ouverts.begin(), projets_ouverts.end(), pe), projets_ouverts.end() );
        onglets->removeTab( onglets->indexOf(pe) );
    }
}

void MainWindow::closeTab(int index)
{
    // Si l'onglet est un onglet projet, le retirer de la liste des ProjetEditeur ouverts
    ProjetEditeur* pe = dynamic_cast<ProjetEditeur*>( onglets->widget(index) );
    if( pe )
        projets_ouverts.erase( std::remove(projets_ouverts.begin(), projets_ouverts.end(), pe), projets_ouverts.end() );

    // Si l'onglet est l'agenda, passer l'attribut agenda_ouvert à faux
//    AgendaEditeur* ae = dynamic_cast<AgendaEditeur*>( onglets->widget(index) );
//    if( ae )
//        agenda_ouvert=false;

    onglets->removeTab(index);
}

void MainWindow::chargerProjets()
{
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
                              Duree(4,0),
                              Duree(4,0));

        tm.ajouterTacheComposite("Phase1",
                                 "Phase de test",
                                 QDate(2015,01,01),
                                 QDate(2015, 02,15));

        tm.ajouterTacheUnaire("Tache1",
                              "Proceder au test d\'une tache unaire",
                              QDate(2015,01,01),
                              QDate(2015, 01,15),
                              Duree(8,0),
                              Duree(8,0));

        tm.ajouterTacheUnaire("Tache2",
                              "Proceder au test d\'une deuxieme tache unaire",
                              QDate(2015,02,01),
                              QDate(2015, 02,15),
                              Duree(48,0),
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
                              Duree(3,0),
                              Duree(3,0));

        tm.getTache("Tache3").ajouterPredecesseur( tm.getTache("Phase1") );
        p1.ajouterPredecesseur( tm.getTache("Tache0"));

 qDebug()<<"Projet1 : OK";

    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }
    try
        {
            TacheManager& tm2 = projets.getProjet("Projet2");

            tm2.ajouterTacheUnaire("Tache-0",
                                  "Preparer l\'ensemble des tests",
                                  QDate(2014,12,15),
                                  QDate(2014, 12,31),
                                  Duree(4,0),
                                  Duree(4,0));


            tm2.ajouterTacheUnaire("Tache-1",
                                  "Proceder au test d\'une tache unaire",
                                  QDate(2015,01,01),
                                  QDate(2015, 01,15),
                                  Duree(8,0),
                                  Duree(8,0));

            tm2.ajouterTacheUnaire("Tache-2",
                                  "Proceder au test d\'une deuxieme tache unaire",
                                  QDate(2015,02,01),
                                  QDate(2015, 02,15),
                                  Duree(48,0),
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
                                  Duree(3,0),
                                  Duree(3,0));

            tm2.getTache("Tache-3").ajouterPredecesseur( tm2.getTache("Phase-1") );
            p1.ajouterPredecesseur( tm2.getTache("Tache-0"));
    qDebug()<<"Projet2 : OK";
        }
        catch(CalendarException e)
        {
            qDebug()<<(e.getInfo().toStdString().c_str());
        }

    projets.writeXML();
}
