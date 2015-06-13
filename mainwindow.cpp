#include "mainwindow.h"

MainWindow::MainWindow()
    :projets(ProjetsManager::getInstance()), agenda(ProgrammationManager::getInstance()), agenda_ouvert(0)
{
    setWindowTitle("Gestionnaire de projets avec agenda");
    setMinimumHeight(700);
    setMinimumWidth(850);

    // Initialisation de la barre des menus
    initMenuBar();

    // Initialisation du QTabWidget onglet: propriétés et connection signal/slot
    onglets = new QTabWidget(this);
    onglets->setTabsClosable(true);
    onglets->setMovable(true);
    connect( onglets,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)) );

    // Mettre le QTabWidget comme étant l'élément principal de la fenêtre
    setCentralWidget(onglets);

    QShortcut* closeTab = new QShortcut(QKeySequence("Ctrl+W"),this, SLOT(closeCurrentTab()));
    QShortcut* refreshAg = new QShortcut(QKeySequence(QKeySequence::Refresh),this, SLOT(refreshAgenda()));

    try{
        chargerProjets("Projets");
        chargerAgenda("Agenda");
    }catch(CalendarException e)
    {QMessageBox::warning(this, "Chargement des données", e.getInfo().toStdString().c_str());}
}

void MainWindow::initMenuBar()
{
    menubar = new QMenuBar(this);

    QMenu* menu_fichier = menubar->addMenu("Fichier");
    QAction *menu_fichier_save = menu_fichier->addAction("&Sauvegarder");
    menu_fichier_save->setShortcut(QKeySequence::Save);
    connect(menu_fichier_save, SIGNAL(triggered(bool)), this, SLOT(save()));

    QMenu* menu_agenda = menubar->addMenu("Agenda");
    QAction *menu_agenda_ouvrir = menu_agenda->addAction("Ouvrir l'agenda");
    menu_agenda_ouvrir->setShortcut(QKeySequence::AddTab);
    connect(menu_agenda_ouvrir, SIGNAL(triggered(bool)), this, SLOT(ouvrirAgenda()));

    QMenu* menu_projets = menubar->addMenu("Projets");
    QAction *menu_projets_creer = menu_projets->addAction("Créer projet");
    menu_projets_creer->setShortcut(QKeySequence::New);
    connect(menu_projets_creer, SIGNAL(triggered(bool)), this, SLOT(creerProjet()));

    QAction *menu_projets_ouvrir = menu_projets->addAction("Ouvrir projet");
    menu_projets_ouvrir->setShortcut(QKeySequence::Open);
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

    if(agenda_ouvert)               // Passer l'agenda comme onglet courant s'il a déjà été ouvert
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
    try
    {
        ProprieteProjetEditeur *ppe = new ProprieteProjetEditeur(this);
        ppe->show();
    }
    catch(CalendarException e)
    { QMessageBox::information(this, "Création projet", e.getInfo().toStdString().c_str()); }

}

void MainWindow::ouvrirProjet()
{
    const std::list<TacheManager*> liste_projets = projets.getProjets();
    // S'il n'existe aucun projet, proposer d'en créer un
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
        // Cas où tous les projets sont ouverts
        if( liste_projets.size() == projets_ouverts.size() )
        {
            QMessageBox::warning(this, "Ouverture projet", "Tous les projets sont ouverts");
            return;
        }

        // Ajout de tous les projets qui ne sont pas encore ouverts dans la liste de choix
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
        //projets_ouverts.erase( std::remove(projets_ouverts.begin(), projets_ouverts.end(), pe), projets_ouverts.end() );
        projets_ouverts.remove(pe);
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

void MainWindow::closeCurrentTab()
{
    closeTab(onglets->currentIndex());
}

void MainWindow::refreshAgenda()
{
    if( agenda_ouvert ) agenda_ouvert->refresh();
}

void MainWindow::save()
{
    projets.writeXML("Projets");
    agenda.writeXML("Agenda");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    try
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Fermeture",
                                                                 "Voulez-vous sauvegarder les dernières modifications ?",
                                                                 QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
                save();

                projets.libererInstance();
                agenda.libererInstance();
        }
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Suppression tâche", e.getInfo()); }
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

void MainWindow::chargerAgenda(const QString& dossier)
{
    QFile file(dossier+"\\programmations.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement agendaElement = doc.namedItem("agenda").toElement();
    if ( agendaElement.isNull() ) {
      qWarning() << "No <agenda> element found at the top-level "
                 << "of the XML file!";
      return;
    }

    Evenement* event;
    QString titre;
    QDateTime date;
    Duree dur = Duree();

    // Parcourir les programmations pour les charger
    QDomNode nd = agendaElement.firstChild();
    for ( ; !nd.isNull(); nd = nd.nextSibling() )
    {
        QDomNode prog = nd.firstChild();
        for ( ; !prog.isNull(); prog = prog.nextSibling() )
        {

            if ( prog.isElement() && prog.toElement().tagName() == "activite" )
            {
                QString description;
                QString lieu;
                MetaEnum::Type type;
                MetaEnum *me = new MetaEnum();

                QDomNode prop = prog.firstChild();
                for ( ; !prop.isNull(); prop = prop.nextSibling() )
                {
                  if ( prop.isElement() && prop.toElement().tagName() == "titre" )
                      titre= prop.toElement().text();
                  if ( prop.isElement() && prop.toElement().tagName() == "description" )
                      description= prop.toElement().text();
                  if ( prop.isElement() && prop.toElement().tagName() == "lieu" )
                      lieu= prop.toElement().text();
                  if ( prop.isElement() && prop.toElement().tagName() == "type" )
                      type= me->stringToType( prop.toElement().text() );

                  if ( prop.isElement() && prop.toElement().tagName() == "date" )
                      date= QDateTime::fromString(prop.toElement().text());

                  if ( prop.isElement() && prop.toElement().tagName() == "duree" )
                  {
                      if(!prop.toElement().hasAttribute("jours")
                              || !prop.toElement().hasAttribute("heures")
                              || !prop.toElement().hasAttribute("minutes"))
                          break;

                      dur.setNbJour( prop.toElement().attribute("jours").toInt() );
                      dur.setHeure( prop.toElement().attribute("heures").toInt() );
                      dur.setMinute( prop.toElement().attribute("minutes").toInt() );
                  }
                }

                try
                {
                      event= new Activite(titre, description, type, lieu);
                      agenda.addProgrammation(date, dur, event );
                }
                catch(CalendarException e)
                { QMessageBox::warning(this, "Chargement programmation activité", e.getInfo()); }
            }

          if ( prog.isElement() && prog.toElement().tagName() == "tache" )
          {
              QString projet;
              QDomNode prop = prog.firstChild();
              for ( ; !prop.isNull(); prop = prop.nextSibling() )
              {
                if ( prop.isElement() && prop.toElement().tagName() == "projet" )
                    projet= prop.toElement().text();
                if ( prop.isElement() && prop.toElement().tagName() == "titre" )
                    titre= prop.toElement().text();

                if ( prop.isElement() && prop.toElement().tagName() == "date" )
                    date= QDateTime::fromString(prop.toElement().text());

                if ( prop.isElement() && prop.toElement().tagName() == "duree" )
                {
                    if(!prop.toElement().hasAttribute("jours")
                            || !prop.toElement().hasAttribute("heures")
                            || !prop.toElement().hasAttribute("minutes"))
                        break;

                    dur.setNbJour( prop.toElement().attribute("jours").toInt() );
                    dur.setHeure( prop.toElement().attribute("heures").toInt() );
                    dur.setMinute( prop.toElement().attribute("minutes").toInt() );
                }
              }

              try
              {
                    event= &projets.getProjet(projet).getTache(titre);
                    agenda.addProgrammation(date, dur, event );
              }
              catch(CalendarException e)
              { QMessageBox::warning(this, "Chargement programmation tâche", e.getInfo()); }
          }
        }
     }
}
