#include "vueprojets.h"

VueProjets::VueProjets(std::list<TacheManager*> *p, QWidget* parent):projets(p), parent(parent)
{

    setWindowTitle("Ensemble des projets");
    setFixedSize(650,450);
    main_layout = new QGridLayout(this);


    ///    ------      LIGNE CHOIX PROJET     ------- ///
    l_projet = new QHBoxLayout(this);
    titre = new QGroupBox("Choix projet");

    initListe();
    supprimer = new QPushButton("Supprimer", this);
    ajouter = new QPushButton("Créer", this);
    connect(ajouter, SIGNAL(clicked(bool)), this, SLOT(ajouterProjet()));

    l_projet->addWidget(choix_projet);
    l_projet->addWidget(supprimer);
    l_projet->addWidget(ajouter);
    titre->setLayout(l_projet);

    main_layout->addWidget(titre, 0, 0);


    ///   ----------    PROJET EDITEUR     -------- ///
    chargerProjet(p->front()->getNom());
    main_layout->addWidget(pe, 1,0);
}


void VueProjets::initListe()
{
    choix_projet = new QComboBox(this);

    list<TacheManager*>::const_iterator it;
    for( it = projets->begin() ; it != projets->end() ; ++it )
    {
        choix_projet->addItem( (*it)->getNom() );
    }
    connect( choix_projet, SIGNAL(currentTextChanged(QString)), this, SLOT(chargerProjet(QString)) );
}


void VueProjets::chargerProjet(QString nom)
{
    if( QObject::sender() == choix_projet )
    { nom = choix_projet->currentText(); }

    TacheManager* tm;
    list<TacheManager*>::const_iterator it;
    for( it = projets->begin() ; it != projets->end() ; ++it )
    {
        if( (*it)->getNom() == nom )
        {
            tm = (*it);
            break;
        }
    }

    ProjetEditeur* tmp = pe;
    pe = new ProjetEditeur(*tm, this);
    main_layout->replaceWidget(tmp, pe);
    main_layout->addWidget(pe, 1,0);
}

void VueProjets::ajouterProjet()
{
    QString name = QInputDialog::getText(this, tr("Nouveau projet"),
                                              tr("Nom"), QLineEdit::Normal);

    qDebug()<<name;
    try{
        TacheManager* tm = new TacheManager(name);
        projets->push_back(tm);
        //choix_projet->addItem(name);
        initListe();
    }
    catch(...)
    {
        qDebug()<<"FAIL";
    }

}
