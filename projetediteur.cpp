#include "projetediteur.h"
#include "programmationediteur.h"

#include <QDebug>

ProjetEditeur::ProjetEditeur(TacheManager &tm1, QWidget *p):tm(tm1),parent(p)
{
    setWindowTitle(tm.getNom());
    setFixedSize(650,350);

    main_layout = new QGridLayout(this);

    /// ---- Ligne indiquant les bornes (dates) du projet ---- ///
    grp_proprietes = initProprietes();
    main_layout->addWidget(grp_proprietes,0,0,1,2);

    /// ---- Arbre des tâches ---- ///
    taches = new QTreeWidget(this);
    //taches->setFixedSize(450, 300);
    taches->setHeaderLabels(QStringList()<<"Tâches"<<"Prédécesseurs"<<"Description");
    chargerTaches();
    main_layout->addWidget(taches, 1, 0, 4, 1);
    connect(taches, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(getTacheCourante(QTreeWidgetItem*,int)));


    /// ---- Ajout des boutons de gestion d'une tâche ---- ///
    grp_modification = new QGroupBox("Tâche courante");
    l_existante = new QVBoxLayout();

    modifier_tache = new QPushButton(this);
    modifier_tache->setText("Modifier");
    modifier_tache->setEnabled(false);
    l_existante->addWidget(modifier_tache);

    supprimer_tache = new QPushButton(this);
    supprimer_tache->setText("Supprimer");
    supprimer_tache->setEnabled(false);
    l_existante->addWidget(supprimer_tache);

    programmer = new QPushButton(this);
    programmer->setText("Programmer");
    programmer->setHidden(true);
    l_existante->addWidget(programmer);

    termine = new QCheckBox(this);
    termine->setText("Terminée");
    termine->setChecked(false);
    termine->setEnabled(false);
    l_existante->addWidget(termine);

    grp_modification->setLayout(l_existante);
    main_layout->addWidget(grp_modification,1, 1);

    connect(modifier_tache, SIGNAL(clicked(bool)), this, SLOT(modifierTache()));
    connect(supprimer_tache, SIGNAL(clicked(bool)), this, SLOT(supprimerTache()));
    connect(programmer, SIGNAL(clicked(bool)), this, SLOT(programmerTache()));

    /// ---- Ajout des boutons d'ajout de tâche ---- ///
    grp_nouvelle = new QGroupBox("Nouvelle tâche");
    l_nouvelle = new QVBoxLayout();

    ajouter_unaire = new QPushButton(this);
    ajouter_unaire->setText("Ajout tâche unaire");
    l_nouvelle->addWidget(ajouter_unaire);

    ajouter_composite = new QPushButton(this);
    ajouter_composite->setText("Ajout tâche composite");   
    l_nouvelle->addWidget(ajouter_composite);

    grp_nouvelle->setLayout(l_nouvelle);
    main_layout->addWidget(grp_nouvelle, 3, 1);

    connect(ajouter_unaire, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));
    connect(ajouter_composite, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));


    setLayout(main_layout);
}

QGroupBox* ProjetEditeur::initProprietes()
{
    QGroupBox* tmp = new QGroupBox("Propriétés de "+tm.getNom());
    l_proprietes = new QHBoxLayout(this);

    debut_label = new QLabel("Début: ");
    debut_date = new QLabel(tm.getDebut().toString("dd-MM-yyyy"));
    l_proprietes->addWidget(debut_label);
    l_proprietes->addWidget(debut_date);

    fin_label = new QLabel("Fin: ");
    fin_date = new QLabel(tm.getFin().toString("dd-MM-yyyy"));
    l_proprietes->addWidget(fin_label);
    l_proprietes->addWidget(fin_date);

    modifier_projet = new QPushButton("Modifier");
    l_proprietes->addWidget(modifier_projet);
    connect(modifier_projet, SIGNAL(clicked(bool)), this, SLOT(modifierProjet()));

    supprimer_projet = new QPushButton("Supprimer");
    l_proprietes->addWidget(supprimer_projet);
    connect(supprimer_projet, SIGNAL(clicked(bool)), this, SLOT(supprimerProjet()));

    tmp->setLayout(l_proprietes);
    return tmp;
}

void ProjetEditeur::chargerTaches()
{
    taches->clear();

    list<Tache*> l = tm.getTaches();
    for( list<Tache*>::const_iterator it = l.begin() ; it != l.end() ; ++it )
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, (*it)->getTitre());
        item->setText(1, (*it)->getPredString());
        item->setText(2, (*it)->getDescription().mid(0, 100)+"...");

        TacheComposite* tc = dynamic_cast<TacheComposite*>( (*it) );
        chargerSousTaches(item, &l, tc);

        taches->addTopLevelItem(item);
    }
}

void ProjetEditeur::chargerSousTaches(QTreeWidgetItem* item, list<Tache*> *l, TacheComposite* tc )
{
    if( tc )
    {
        const list<Tache*> soust = (tc)->getSousTaches();
        for( list<Tache*>::const_iterator it_soust = soust.begin() ; it_soust != soust.end() ; ++it_soust )
        {
            QTreeWidgetItem* sous_item = new QTreeWidgetItem;
            sous_item->setText(0, (*it_soust)->getTitre());
            sous_item->setText(1, (*it_soust)->getPredString());
            sous_item->setText(2, (*it_soust)->getDescription().mid(0, 100)+"...");
            item->addChild(sous_item);

            TacheComposite* tc2 = dynamic_cast<TacheComposite*>( (*it_soust) );
            chargerSousTaches(sous_item, l, tc2);

            // Retirer la sous-tâche si elle a déjà été ajoutée dans l'arbre
            QList<QTreeWidgetItem*> listItems = taches->findItems((*it_soust)->getTitre(), 0); //taches->find
            if( listItems.size() != 0 )
            {
                delete taches->takeTopLevelItem( taches->indexOfTopLevelItem(listItems.at(0)) );
            }
            // Retirer la sous-tâche de la liste des tâches pour ne pas
            //  la prendre en considération une deuxième fois dans le QTree
            l->remove( &tm.getTache((*it_soust)->getTitre()) );
        }
    }
}

void ProjetEditeur::modifierProjet()
{
    try
    {
        ProprieteProjetEditeur* pe = new ProprieteProjetEditeur(this, tm.getNom());
        pe->show();
        connect(pe, SIGNAL(fermeture(const QString&)), this, SLOT(refresh_projet()));
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
}

void ProjetEditeur::supprimerProjet()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Suppression",
                                  "Voulez-vous vraiment supprimer le projet "+tm.getNom()+" ?\n\nToutes ses tâches seront également perdues",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try
        {
            ProjetsManager& pm = ProjetsManager::getInstance();
            emit fermeture(tm.getNom());
            pm.retirerProjet(tm.getNom());
        }
        catch(CalendarException e)
        { QMessageBox::warning(this, "Suppression de projet", e.getInfo()); }
    }
}

void ProjetEditeur::refresh_projet()
{
    QGroupBox* tmp = grp_proprietes;
    delete tmp;
    grp_proprietes=initProprietes();
    main_layout->addWidget(grp_proprietes,0,0,1,2);
}

void ProjetEditeur::refresh_taches()
{
    chargerTaches();
}

void ProjetEditeur::ajouterTache()
{
    bool unaire;
    if( QObject::sender() == ajouter_unaire )
        unaire=true;

    if( QObject::sender() == ajouter_composite )
        unaire=false;

    TacheEditeur* te = new TacheEditeur(tm, unaire);
    connect(te, SIGNAL(fermeture()), this, SLOT(refresh_taches()));

    te->show();
}

void ProjetEditeur::getTacheCourante(QTreeWidgetItem* item,int c)
{
    tache_courante = item->text(0);
    modifier_tache->setEnabled(true);
    supprimer_tache->setEnabled(true);

    Tache& t =tm.getTache(tache_courante);

    bool estTermine = t.estTermine();
    if(dynamic_cast<TacheComposite*>(&t))
    {
        programmer->setHidden(true);
        termine->setEnabled(false);
        termine->setChecked(estTermine);
    }
    if(dynamic_cast<TacheUnaire*>(&t))
    {
        programmer->setHidden(false);
        termine->setEnabled(true);
        termine->setChecked(estTermine);
    }
}

void ProjetEditeur::modifierTache()
{
    try
    {
        Tache& t =tm.getTache(tache_courante);
        TacheEditeur* te = new TacheEditeur(tm, &t);
        connect(te, SIGNAL(fermeture()), this, SLOT(refresh_taches()));
        te->show();

        modifier_tache->setEnabled(false);
        supprimer_tache->setEnabled(false);
        programmer->setEnabled(false);
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Edition de tâche", e.getInfo()); }
}

void ProjetEditeur::supprimerTache()
{
    try
    {
        tm.retirerTache(tache_courante);
        refresh_taches();
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Suppression tâche", e.getInfo()); }
}

void ProjetEditeur::programmerTache()
{
    /*
    try
    {
        Tache& t =tm.getTache(tache_courante);
        pe = new ProgrammationEditeur(&t, this);
        te->show();

        modifier->setEnabled(false);
        supprimer->setEnabled(false);
        programmer->setEnabled(false);
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Programmation tâche", e.getInfo()); }
*/
}

void ProjetEditeur::fermeture(const QString)
{
}

