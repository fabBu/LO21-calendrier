#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include<iostream>
#include <typeinfo>

#include "TacheEditeur.h"

TacheEditeur::TacheEditeur(Tache& t1, QWidget *p) :QWidget(p), t(t1){

    // tu pointera sur t si celle-ci est une TacheUnaire
    //      vaudra nul sinon
    TacheUnaire* tu = dynamic_cast<TacheUnaire*>(&t);
    TacheComposite* tc = dynamic_cast<TacheComposite*>(&t);

    setWindowTitle("Edition de la tache : " + t.getTitre());
    setFixedSize(500,200);
    lv = new QVBoxLayout;

    lh1 = new QHBoxLayout;
    lid = new QLabel("Identificateur",this);
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

    lh2 = new QHBoxLayout;
    ltitre = new QLabel("Titre",this);
    titre = new QTextEdit(this);
    titre->setText(t.getDescription());
    titre->setFixedSize(400,50);
    lh2->addWidget(ltitre);
    lh2->addWidget(titre);

    lv->addLayout(lh2);


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

    // Ajout de la liste des prédécesseurs s'il y en a
    if( !t.getPred().empty() )
    {
        lh4 = new QHBoxLayout;
        lpred = new QLabel("Predecesseurs", this);
        pred = new QComboBox(this);

        const list<Tache*> l = t.getPred();
        for( list<Tache*>::const_iterator it = l.begin() ; it != l.end() ; ++it )
            pred->addItem( (*it)->getTitre() );

        lh4->addWidget(lpred);
        lh4->addWidget(pred);
        lv->addLayout(lh4);
    }

    // Si la tache est composite et possede des sous-taches
    if( tc && !tc->getSousTaches().empty() )
    {
        lh5 = new QHBoxLayout;
        lsoust = new QLabel("Sous-taches", this);
        soust = new QComboBox(this);

        const list<Tache*> l = tc->getSousTaches();
        for( list<Tache*>::const_iterator it = l.begin() ; it != l.end() ; ++it )
            soust->addItem( (*it)->getTitre() );

        lh5->addWidget(lsoust);
        lh5->addWidget(soust);
        lv->addLayout(lh5);
    }

    lh6 = new QHBoxLayout;
    cancel = new QPushButton("Annuler",this);
    save = new QPushButton("Sauver",this);
    lh6->addWidget(cancel);
    lh6->addWidget(save);


    lv->addLayout(lh6);
    this->setLayout(lv);
    save->setEnabled(false);
}


