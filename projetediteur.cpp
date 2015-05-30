#include "projetediteur.h"

#include <QDebug>

ProjetEditeur::ProjetEditeur(TacheManager &tm1, QWidget *p):tm(tm1),parent(p)
{
    setWindowTitle(tm.getNom());
    setFixedSize(650,350);

    main_layout = new QGridLayout(this);

    taches = new QTreeWidget(this);
    taches->setFixedSize(450, 300);
    taches->setHeaderLabels(QStringList()<<"Tâches"<<"Prédécesseurs"<<"Description");

    chargerTaches();
    main_layout->addWidget(taches, 0, 0, 4, 1);
    connect(taches, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(getTacheCourante(QTreeWidgetItem*,int)));

    /// ---- Ajout des boutons de gestion d'une tâche ---- ///
    grp_existante = new QGroupBox("Tâche courante");
    l_existante = new QVBoxLayout();

    modifier = new QPushButton(this);
    modifier->setText("Modifier");
    modifier->setEnabled(false);
    l_existante->addWidget(modifier);

    supprimer = new QPushButton(this);
    supprimer->setText("Supprimer");
    supprimer->setEnabled(false);
    l_existante->addWidget(supprimer);

    programmer = new QPushButton(this);
    programmer->setText("Programmer");
    programmer->setEnabled(false);
    l_existante->addWidget(programmer);

    grp_existante->setLayout(l_existante);
    main_layout->addWidget(grp_existante,0, 1);

    connect(modifier, SIGNAL(clicked(bool)), this, SLOT(modifierTache()));
    connect(supprimer, SIGNAL(clicked(bool)), this, SLOT(supprimerTache()));
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
    main_layout->addWidget(grp_nouvelle, 2, 1);

    connect(ajouter_unaire, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));
    connect(ajouter_composite, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));


    setLayout(main_layout);
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
        item->setText(2, (*it)->getDescription().mid(0, 250)+"...");

        TacheComposite* tc = dynamic_cast<TacheComposite*>( (*it) );
        chargerSousTaches(item, tc);

        taches->addTopLevelItem(item);
    }
}

void ProjetEditeur::chargerSousTaches(QTreeWidgetItem* item, TacheComposite* tc )
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
            chargerSousTaches(sous_item, tc2);

            // Retirer la sous-tâche si elle a déjà été ajoutée dans l'arbre
            QList<QTreeWidgetItem*> list = taches->findItems((*it_soust)->getTitre(), 0); //taches->find
            if( list.size() != 0 )
            {
                delete taches->takeTopLevelItem( taches->indexOfTopLevelItem(list.at(0)) );
                //l->remove((*it_soust));
            }
        }
    }
}

void ProjetEditeur::refresh()
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

    te = new TacheEditeur(tm, unaire);
    connect(te, SIGNAL(fermeture()), this, SLOT(refresh()));

    te->show();
}


void ProjetEditeur::getTacheCourante(QTreeWidgetItem* item,int c)
{
    tache_courante = item->text(0);
    modifier->setEnabled(true);
    supprimer->setEnabled(true);
    programmer->setEnabled(true);
}

void ProjetEditeur::modifierTache()
{
    try
    {
        Tache& t =tm.getTache(tache_courante);
        te = new TacheEditeur(tm, &t);
        connect(te, SIGNAL(fermeture()), this, SLOT(refresh()));
        te->show();

        modifier->setEnabled(false);
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Edition de tâche", e.getInfo()); }
}

void ProjetEditeur::supprimerTache()
{
    try
    {
        tm.retirerTache(tache_courante);
        refresh();
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Suppression tâche", e.getInfo()); }
}

void ProjetEditeur::programmerTache()
{

}

