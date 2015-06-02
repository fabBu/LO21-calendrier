#include "vueprojets.h"

VueProjets::VueProjets(ProjetsManager *p, QWidget* parent):projets(p), parent(parent)
{

    setWindowTitle("Ensemble des projets");
    setFixedSize(650,450);
    //setMinimumSize(650, 450);
    main_layout = new QGridLayout(this);


    ///    ------      LIGNE CHOIX PROJET     ------- ///
    l_projet = new QHBoxLayout(this);
    //l_projet->SetMaximumSize()

    choix_projet = initListe();

    modifier = new QPushButton("Modifier", this);
    supprimer = new QPushButton("Supprimer", this);
    ajouter = new QPushButton("Créer", this);
    connect(modifier, SIGNAL(clicked(bool)), this, SLOT(modifierProjet()));
    connect(supprimer, SIGNAL(clicked(bool)), this, SLOT(supprimerProjet()));
    connect(ajouter, SIGNAL(clicked(bool)), this, SLOT(ajouterProjet()));

    l_projet->addWidget(choix_projet);
    l_projet->addWidget(modifier);
    l_projet->addWidget(supprimer);
    l_projet->addWidget(ajouter);

    titre = new QGroupBox("Choix projet");
    titre->setLayout(l_projet);
    main_layout->addWidget(titre, 0, 0);


    ///   ----------    PROJET EDITEUR     -------- ///
    chargerProjet(projets->getProjets().front()->getNom());
    main_layout->addWidget(pe, 1,0);
}


QComboBox *VueProjets::initListe()
{
    QComboBox* liste = new QComboBox(this);

    list<TacheManager*> list_proj = projets->getProjets();
    list<TacheManager*>::const_iterator it;
    for( it = list_proj.begin() ; it != list_proj.end() ; ++it )
    {
        liste->addItem( (*it)->getNom() );
    }

    connect( liste, SIGNAL(currentTextChanged(QString)), this, SLOT(chargerProjet(QString)) );
    return liste;
}


void VueProjets::chargerProjet(QString nom)
{
    try
    {
        TacheManager& tm = projets->getProjet(nom);

        ProjetEditeur* tmp = pe;
        pe = new ProjetEditeur(tm, this);
        main_layout->replaceWidget(tmp, pe);
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Chargement projet", "Erreur lors du chargement: "+e.getInfo()); }
}

void VueProjets::modifierProjet()
{
    try
    {
        ProprieteProjetEditeur* pe = new ProprieteProjetEditeur(this, projets, choix_projet->currentText());
        pe->show();
        connect(pe, SIGNAL(fermeture(const QString&)), this, SLOT(refresh(const QString&)));
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
}

void VueProjets::ajouterProjet()
{
    try
    {
        ProprieteProjetEditeur* pe = new ProprieteProjetEditeur(this, projets);
        pe->show();
        connect(pe, SIGNAL(fermeture(const QString&)), this, SLOT(refresh(const QString&)));
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Création de projet", e.getInfo()); }
}



void VueProjets::supprimerProjet()
{
    try
    {
        projets->retirerProjet(choix_projet->currentText());
        refresh(projets->getProjets().front()->getNom());
    }
    catch(CalendarException e)
    { QMessageBox::warning(this, "Suppression de projet", e.getInfo()); }
}

void VueProjets::refresh(const QString &proj)
{
    QComboBox* tmp = choix_projet;
    choix_projet=initListe();
    l_projet->replaceWidget(tmp, choix_projet);

    // TODO : L'un ou l'autre marche, mais pas les deux en même temps.................
    choix_projet->setCurrentText(proj);
    choix_projet->currentTextChanged(proj);
}
