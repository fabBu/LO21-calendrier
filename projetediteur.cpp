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

    modifier = new QPushButton(this);
    modifier->setText("Modifier");
    modifier->setEnabled(false);
    main_layout->addWidget(modifier, 0, 1);

    supprimer = new QPushButton(this);
    supprimer->setText("Supprimer");
    supprimer->setEnabled(false);
    main_layout->addWidget(supprimer, 1, 1);

    ajouter_unaire = new QPushButton(this);
    ajouter_unaire->setText("Ajout tâche unaire");
    main_layout->addWidget(ajouter_unaire, 2, 1);

    ajouter_composite = new QPushButton(this);
    ajouter_composite->setText("Ajout tâche composite");
    main_layout->addWidget(ajouter_composite, 3, 1);

    connect(modifier, SIGNAL(clicked(bool)), this, SLOT(modifierTache()));
    connect(supprimer, SIGNAL(clicked(bool)), this, SLOT(supprimerTache()));
    connect(ajouter_unaire, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));
    connect(ajouter_composite, SIGNAL(clicked(bool)), this, SLOT(ajouterTache()));


    setLayout(main_layout);
}

void ProjetEditeur::chargerTaches()
{
    taches->clear();

    const list<Tache*> l = tm.getTaches();
    for( list<Tache*>::const_iterator it = l.begin() ; it != l.end() ; ++it )
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0, (*it)->getTitre());
        item->setText(1, (*it)->getPredString());
        item->setText(2, (*it)->getDescription().mid(0, 250));
        taches->addTopLevelItem(item);

        TacheComposite* tc = dynamic_cast<TacheComposite*>( (*it) );
        if( tc )
        {
            const list<Tache*> soust = (tc)->getSousTaches();
            for( list<Tache*>::const_iterator it_soust = soust.begin() ; it_soust != soust.end() ; ++it_soust )
            {
                QTreeWidgetItem* sous_item = new QTreeWidgetItem;
                sous_item->setText(0, (*it_soust)->getTitre());
                sous_item->setText(1, (*it_soust)->getPredString());
                sous_item->setText(2, (*it_soust)->getDescription().mid(0, 250));
                item->addChild(sous_item);

                // Retirer la sous-tâche si elle a déjà été ajoutée dans l'arbre
                QList<QTreeWidgetItem*> list = taches->findItems((*it_soust)->getTitre(), 0); //taches->find
                if( list.size() != 0 )
                    delete taches->takeTopLevelItem( taches->indexOfTopLevelItem(list.at(0)) );
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
    modifier->setEnabled(true);
    supprimer->setEnabled(true);
    tache_courante = item->text(c);
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

}


