#include "mainwindow.h"

MainWindow::MainWindow()
    :projets(ProjetsManager::getInstance()), agenda(ProgrammationManager::getInstance()), agenda_ouvert(0)
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

    chargerProjets("Projets");
}

void MainWindow::initMenuBar()
{
    menubar = new QMenuBar(this);

    QMenu* menu_agenda = menubar->addMenu("Agenda");
    QAction *menu_agenda_ouvrir = menu_agenda->addAction("Ouvrir l'agenda");
    connect(menu_agenda_ouvrir, SIGNAL(triggered(bool)), this, SLOT(ouvrirAgenda()));

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

    if(agenda_ouvert)
        onglets->setCurrentWidget( agenda_ouvert );
    else
    {
        AgendaEditeur *ae = new AgendaEditeur();
        agenda_ouvert = ae;
        onglets->addTab(ae, "AGENDA");
        onglets->setCurrentWidget(ae);
    }

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
                onglets->setCurrentWidget(pe);
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
    AgendaEditeur* ae = dynamic_cast<AgendaEditeur*>( onglets->widget(index) );
    if( ae )
        agenda_ouvert=0;

    onglets->removeTab(index);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
      projets.writeXML("Projets");
}

void MainWindow::chargerProjets(const QString& dossier)
{
    QStringList filters;
    filters << "*.xml";
    QStringList files = QDir( dossier ).entryList( filters );

    for( QStringList::iterator it=files.begin() ; it!=files.end() ; ++it )
    {
        QFile file(dossier+"\\"+(*it));
        projets.readXML(file);
    }
}
