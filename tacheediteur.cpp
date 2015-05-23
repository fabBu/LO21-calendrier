#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include<iostream>
#include <typeinfo>

#include "tacheediteur.h"


TacheEditeur::TacheEditeur(TacheManager &tm, bool unaire)
{

}


TacheEditeur::TacheEditeur(TacheManager& tm1, Tache& t1, QWidget *p) :tm(tm1), t(t1), QWidget(p){

    // tu pointera sur t si celle-ci est une TacheUnaire
    //      vaudra nul sinon
    TacheUnaire* tu = dynamic_cast<TacheUnaire*>(&t);
    TacheComposite* tc = dynamic_cast<TacheComposite*>(&t);

    if( tu )
        setWindowTitle(tm1.getNom()+" - "+t.getTitre()+" (UNAIRE)");
    if( tc )
        setWindowTitle(tm1.getNom()+" - "+t.getTitre()+" (COMPOSITE)");

    setFixedSize(500,230);
    lv = new QVBoxLayout;

    lh1 = new QHBoxLayout;
    lid = new QLabel("Titre",this);
    id = new QLineEdit(this);
    id->setFixedWidth(250);
    id->setText(t.getTitre());
    lh1->addWidget(lid);
    lh1->addWidget(id);

    if( tu )
    {
        preemp = new QCheckBox("Préemptive",this);
        preemp->setChecked(tu->isPreemptive());
        lh1->addWidget(preemp);
    }
    lv->addLayout(lh1);


    ///    ------      LIGNE DESCRIPTION      ------- ///
    lh2 = new QHBoxLayout;
    ltitre = new QLabel("Description",this);
    titre = new QTextEdit(this);
    titre->setText(t.getDescription());
    titre->setFixedSize(400,50);
    lh2->addWidget(ltitre);
    lh2->addWidget(titre);

    lv->addLayout(lh2);


    ///   ----------    LIGNE DATES / DUREE     -------- ///
    lh3 = new QHBoxLayout;
    ldispo = new QLabel("Dispo",this);
    dispo = new QDateEdit(t.getDateDisponibilite(),this);
    lecheance = new QLabel("Echeance",this);
    echeance = new QDateEdit(t.getDateEcheance(),this);

    lh3->addWidget(ldispo);
    lh3->addWidget(dispo);
    lh3->addWidget(lecheance);
    lh3->addWidget(echeance);

    if( tu )
    {
        lduree = new QLabel("Duree",this);
        h = new QSpinBox(this);
        m = new QSpinBox(this);
        h->setSuffix("heure(s)");
        m->setSuffix("minute(s)");
        h->setMinimum(0);
        m->setMinimum(0);
        h->setMaximum(1000);
        m->setMaximum(59);

        unsigned int heures = tu->getDuree().getDureeEnHeures();
        unsigned int minutes = tu->getDuree().getDureeEnMinutes() - 60*heures;
        h->setValue(heures);
        m->setValue(minutes);

        lh3->addWidget(lduree);
        lh3->addWidget(h);
        lh3->addWidget(m);
    }
    lv->addLayout(lh3);

    ///     -------   LIGNE PRECEDENCE     ------   ///
    lh4 = new QHBoxLayout;
    lpred = new QLabel("Predecesseurs", this);
//    laucun = new QLabel("< Aucun >", this);
//    laucun->setHidden(true);
    pred = new QComboBox(this);

    // Liste déroulante des prédécesseurs
    const list<Tache*> l = t.getPred();
    list<Tache*>::const_iterator it;
    for( it = l.begin() ; it != l.end() ; ++it )
        pred->addItem( (*it)->getTitre() );

    retirerpred = new QPushButton("Retirer");


//    laucune = new QLabel("< Aucune à ajouter >", this);
//    laucune->setHidden(true);

    // Liste déroulante des autres tâches
    tachespred = new QComboBox(this);
    const list<Tache*> l2 = tm.getTaches();
    for( it = l2.begin() ; it != l2.end() ; ++it )
    {
        if( &t != (*it) && !t.estPredecesseur((**it)) )
            tachespred->addItem( (*it)->getTitre() );
    }

    ajouterpred = new QPushButton("Ajouter");


    lh4->addWidget(lpred);
    lh4->addWidget(pred);
    //lh4->addWidget(laucun);
    lh4->addWidget(retirerpred);
    lh4->addWidget(tachespred);
    lh4->addWidget(ajouterpred);
    lv->addLayout(lh4);

    connect( pred, SIGNAL(currentTextChanged(QString)), this, SLOT(modifListePred()) );
    connect( tachespred, SIGNAL(currentTextChanged(QString)), this, SLOT(modifListePred()) );
    connect( retirerpred, SIGNAL(clicked(bool)), this, SLOT(retirerPredecesseur()) );
    connect( ajouterpred, SIGNAL(clicked(bool)), this, SLOT(ajouterPredecesseur()) );

    // Modifier l'interface si la tache ne possède aucun prédécesseur
    if( l.empty() )
    {
        pred->setHidden(true);
        //laucun->setHidden(false);
        retirerpred->setHidden(true);
    }



    /// ----------     LIGNE SOUS-TACHES        ----------- ///
    // Si la tache est composite et possede des sous-taches
    if( tc )
    {
        lh5 = new QHBoxLayout;
        lsoust = new QLabel("Sous-tâches", this);
        soust = new QComboBox(this);

        // Liste déroulante des sous-tâches
        const list<Tache*> l = tc->getSousTaches();
        for( list<Tache*>::const_iterator it = l.begin() ; it != l.end() ; ++it )
            soust->addItem( (*it)->getTitre() );

        retirersoust = new QPushButton("Retirer");

        // Liste déroulante des autres tâches
        tachessoust = new QComboBox(this);
        const list<Tache*> l2 = tm.getTaches();
        for( it = l2.begin() ; it != l2.end() ; ++it )
        {
            if( &t != (*it) && !tc->estSousTache((**it)) )
                tachessoust->addItem( (*it)->getTitre() );
        }

        ajoutersoust = new QPushButton("Ajouter");

        lh5->addWidget(lsoust);
        lh5->addWidget(soust);
        lh5->addWidget(retirersoust);
        lh5->addWidget(tachessoust);
        lh5->addWidget(ajoutersoust);
        lv->addLayout(lh5);

        connect( soust, SIGNAL(currentTextChanged(QString)), this, SLOT(modifierListeSoust()) );
        connect( tachessoust, SIGNAL(currentTextChanged(QString)), this, SLOT(modifierListeSoust()) );
        connect( retirersoust, SIGNAL(clicked(bool)), this, SLOT(retirerSousTache()) );
        connect( ajoutersoust, SIGNAL(clicked(bool)), this, SLOT(ajouterSousTache()) );

        // Modifier l'interface si la tache ne possède aucun prédécesseur
        if( l.empty() )
        {
            pred->setHidden(true);
            //laucun->setHidden(false);
            retirerpred->setHidden(true);
        }
    }

    lh6 = new QHBoxLayout;
    cancel = new QPushButton("Annuler",this);
    save = new QPushButton("Sauver",this);
    lh6->addWidget(cancel);
    lh6->addWidget(save);

    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(close()) );
    connect(save, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()) );

    lv->addLayout(lh6);
    this->setLayout(lv);
    //save->setEnabled(false);
}


void TacheEditeur::retirerPredecesseur()
{
    const QString& title = pred->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    t.retirerPredecesseur(*p);
    pred->removeItem( pred->currentIndex() );

    if( pred->count() == 0)
        pred->setCurrentText("");
    tachespred->addItem( title );
}

void TacheEditeur::ajouterPredecesseur()
{
    const QString& title = tachespred->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    try
    {
        t.ajouterPredecesseur(*p);
        tachespred->removeItem( tachespred->currentIndex() );

        if( tachespred->count() == 0)
            tachespred->setCurrentText("");
        pred->addItem( title );
    }
    catch(CalendarException e)
    {
        QMessageBox::warning(this,"Ajout precedence", e.getInfo());
    }

}

void TacheEditeur::modifListePred()
{
    // Le bouton "Retirer" a été solicité
    //  la liste des prédécesseurs a été modifiée et envoie un signal
    if( QObject::sender() == pred )
    {
        if( pred->currentText() == "" )
        {
            pred->setHidden(true);
            //laucun->setHidden(false);
            retirerpred->setHidden(true);
        }
        else
        {
            //laucun->setHidden(true);
            pred->setHidden(false);
            retirerpred->setHidden(false);
        }
    }

    // Le bouton "Ajouter" a été solicité
    //  la liste des tâches non prédécesseurs a été modifiée et envoie un signal
    if( QObject::sender() == tachespred )
    {
        if( tachespred->currentText() == "" )
        {
            tachespred->setHidden(true);
            //laucune->setHidden(false);
            ajouterpred->setHidden(true);
        }
        else
        {
            //laucune->setHidden(true);
            tachespred->setHidden(false);
            ajouterpred->setHidden(false);
        }
    }
}


void TacheEditeur::retirerSousTache()
{
    const QString& title = soust->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    dynamic_cast<TacheComposite*>(&t)->retirerSousTache(*p);
    soust->removeItem( soust->currentIndex() );

    if( soust->count() == 0)
        soust->setCurrentText("");
    tachessoust->addItem( title );
}

void TacheEditeur::ajouterSousTache()
{
    const QString& title = tachessoust->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    try
    {
        dynamic_cast<TacheComposite*>(&t)->ajouterSousTache(*p);
        tachessoust->removeItem( tachessoust->currentIndex() );

        if( tachessoust->count() == 0)
            tachessoust->setCurrentText("");
        soust->addItem( title );
    }
    catch(CalendarException e)
    {
        QMessageBox::warning(this,"Ajout precedence", e.getInfo());
    }
}

void TacheEditeur::modifierListeSoust()
{
    // Le bouton "Retirer" a été solicité
    //  la liste des sous-taches a été modifiée et envoie un signal
    if( QObject::sender() == soust )
    {
        if( soust->currentText() == "" )
        {
            soust->setHidden(true);
            retirersoust->setHidden(true);
        }
        else
        {
            soust->setHidden(false);
            retirersoust->setHidden(false);
        }
    }

    // Le bouton "Ajouter" a été solicité
    //  la liste des tâches qui ne sont pas des sous-tâches a été modifiée et envoie un signal
    if( QObject::sender() == tachessoust )
    {
        if( tachessoust->currentText() == "" )
        {
            tachessoust->setHidden(true);
            ajoutersoust->setHidden(true);
        }
        else
        {
            tachessoust->setHidden(false);
            ajoutersoust->setHidden(false);
        }
    }
}


void TacheEditeur::sauvegarder()
{
    try
    {
        t.setTitre(id->text());
        t.setDescription(titre->toPlainText());
        t.setDatesDisponibiliteEcheance(dispo->date(), echeance->date());

        TacheUnaire* tu = dynamic_cast<TacheUnaire*>(&t);

        if(tu)
        {
            tu->setPreemptive(preemp->isChecked());
            Duree dur(h->text().toInt(), m->text().toInt());
            tu->setDuree(dur);
        }

        close();
    }
    catch( CalendarException e )
    {
        QMessageBox::warning(this,"Sauvegarde tâche", e.getInfo());
    }

}

