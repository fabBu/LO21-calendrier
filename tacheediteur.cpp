#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include<iostream>
#include <typeinfo>

#include "tacheediteur.h"


TacheEditeur::TacheEditeur(TacheManager &tm1, bool unaire, QWidget* p):tm(tm1),parent(p), unaire(unaire)
{
    t=0;

    if( unaire )
        setWindowTitle(tm1.getNom()+" - ajout tâche UNAIRE");
    else
        setWindowTitle(tm1.getNom()+" - ajout tâche COMPOSITE");

    setFixedSize(500,230);
    main_layout = new QVBoxLayout;
    initTitre(unaire);
    main_layout->addLayout(l_titre);


    ///    ------      LIGNE DESCRIPTION      ------- ///
    initDesc();
    main_layout->addLayout(l_desc);


    ///   ----------    LIGNE DATES / DUREE     -------- ///
    initDates(unaire);
    main_layout->addLayout(l_dates);

    initCancelSave();
    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
    //save->setEnabled(false);

}


TacheEditeur::TacheEditeur(TacheManager& tm1, Tache* t1, QWidget *p) :tm(tm1), t(t1), parent(p){

    // tu pointera sur t si celle-ci est une TacheUnaire et tc sera nul
    //  si t est une TacheComposite, ce sera l'inverse
    TacheUnaire* tu = dynamic_cast<TacheUnaire*>(t);
    TacheComposite* tc = dynamic_cast<TacheComposite*>(t);

    if( tu )
        setWindowTitle(tm1.getNom()+" - "+t->getTitre()+" (UNAIRE)");
    else
        setWindowTitle(tm1.getNom()+" - "+t->getTitre()+" (COMPOSITE)");

    setFixedSize(500,230);
    main_layout = new QVBoxLayout;
    initTitre(tu);
    main_layout->addLayout(l_titre);

    ///    ------      LIGNE DESCRIPTION      ------- ///
    initDesc();
    main_layout->addLayout(l_desc);

    ///   ----------    LIGNE DATES / DUREE     -------- ///
    initDates(tu);
    main_layout->addLayout(l_dates);

    ///     -------   LIGNE PRECEDENCE     ------   ///
    initPrecedence();
    main_layout->addLayout(l_pred);

    /// ----------     LIGNE SOUS-TACHES        ----------- ///
    // Si la tache est composite et possede des sous-taches
    if( tc )
    {
        initSousTaches();
        main_layout->addLayout(l_soust);
    }

    initCancelSave();
    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
    //save->setEnabled(false);
}


void TacheEditeur::initTitre(bool unaire)
{
    l_titre = new QHBoxLayout;
    titre_label = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    titre->setFixedWidth(250);
    if(t != 0 ) titre->setText(t->getTitre());
    l_titre->addWidget(titre_label);
    l_titre->addWidget(titre);

    if( unaire )
    {
        preemp = new QCheckBox("Préemptive",this);
        if(t!=0) preemp->setChecked(dynamic_cast<TacheUnaire*>(t)->isPreemptive());
        l_titre->addWidget(preemp);
    }
}

void TacheEditeur::initDesc()
{
    l_desc = new QHBoxLayout;
    desc_label = new QLabel("Description",this);
    desc = new QTextEdit(this);
    if(t != 0 ) desc->setText(t->getDescription());
    desc->setFixedSize(400,50);
    l_desc->addWidget(desc_label);
    l_desc->addWidget(desc);
}

void TacheEditeur::initDates(bool unaire)
{
    l_dates = new QHBoxLayout;
    dispo_label = new QLabel("Dispo",this);
    echeance_label = new QLabel("Echeance",this);

    dispo = new QDateEdit(QDate::currentDate());
    echeance = new QDateEdit(QDate::currentDate());
    dispo->setCalendarPopup(true);
    echeance->setCalendarPopup(true);

    dispo->setMinimumDate(tm.getDebut());
    dispo->setMaximumDate(tm.getFin());
    echeance->setMinimumDate(tm.getDebut());
    echeance->setMaximumDate(tm.getFin());

    if( t!=0 )
    {
        dispo->setDate(t->getDateDisponibilite());
        echeance->setDate(t->getDateEcheance());

        const Tache* pred = t->getLastPredecesseur();
        if( pred !=0 )
        {
            dispo->setMinimumDate(pred->getDateEcheance());
            echeance->setMinimumDate(pred->getDateEcheance());
        }

        const Tache* succ = t->getFirstSuccesseur();
        if( succ !=0 )
        {
            dispo->setMaximumDate(succ->getDateDisponibilite());
            echeance->setMaximumDate(succ->getDateDisponibilite());
        }
    }

    l_dates->addWidget(dispo_label);
    l_dates->addWidget(dispo);
    l_dates->addWidget(echeance_label);
    l_dates->addWidget(echeance);

    if( unaire )
    {
        duree_label = new QLabel("Duree",this);
        duree_h = new QSpinBox(this);
        duree_m = new QSpinBox(this);
        duree_h->setSuffix("heure(s)");
        duree_m->setSuffix("minute(s)");
        duree_h->setMinimum(0);
        duree_m->setMinimum(0);
        duree_h->setMaximum(1000);
        duree_m->setMaximum(45);
        duree_m->setSingleStep(15);

        unsigned int heures;
        unsigned int minutes;
        if(t!=0)
        {
            heures = dynamic_cast<TacheUnaire*>(t)->getDuree().getDureeEnHeures();
            minutes = dynamic_cast<TacheUnaire*>(t)->getDuree().getDureeEnMinutes() - 60*heures;
        }
        else
            heures =  minutes = 0;

        duree_h->setValue(heures);
        duree_m->setValue(minutes);

        l_dates->addWidget(duree_label);
        l_dates->addWidget(duree_h);
        l_dates->addWidget(duree_m);
    }
}

void TacheEditeur::initCancelSave()
{
    l_cancelsave = new QHBoxLayout;
    btn_cancel = new QPushButton("Annuler",this);
    btn_save = new QPushButton("Sauver",this);
    l_cancelsave->addWidget(btn_cancel);
    l_cancelsave->addWidget(btn_save);

    connect(btn_cancel, SIGNAL(clicked(bool)), this, SLOT(close()) );
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()) );
}

void TacheEditeur::initPrecedence()
{
    l_pred = new QHBoxLayout;
    pred_label = new QLabel("Predecesseurs", this);
    pred_list = new QComboBox(this);

    // Liste déroulante des prédécesseurs
    const list<Tache*> l = t->getPred();
    list<Tache*>::const_iterator it;
    for( it = l.begin() ; it != l.end() ; ++it )
        pred_list->addItem( (*it)->getTitre() );

    btn_retirerpred = new QPushButton("Retirer");


    // Liste déroulante des autres tâches
    nonpred_list = new QComboBox(this);
    const list<Tache*> l2 = tm.getTaches();
    for( it = l2.begin() ; it != l2.end() ; ++it )
    {
        if( t != (*it) && !t->estPredecesseur((**it)) )
            nonpred_list->addItem( (*it)->getTitre() );
    }

    btn_ajouterpred = new QPushButton("Ajouter");

    l_pred->addWidget(pred_label);
    l_pred->addWidget(pred_list);
    l_pred->addWidget(btn_retirerpred);
    l_pred->addWidget(nonpred_list);
    l_pred->addWidget(btn_ajouterpred);

    connect( pred_list, SIGNAL(currentTextChanged(QString)), this, SLOT(modifListePred()) );
    connect( nonpred_list, SIGNAL(currentTextChanged(QString)), this, SLOT(modifListePred()) );
    connect( btn_retirerpred, SIGNAL(clicked(bool)), this, SLOT(retirerPredecesseur()) );
    connect( btn_ajouterpred, SIGNAL(clicked(bool)), this, SLOT(ajouterPredecesseur()) );

    // Modifier l'interface si la tache ne possède aucun prédécesseur
    if( pred_list->count() == 0 )
    {
        pred_list->setHidden(true);
        btn_retirerpred->setHidden(true);
    }
    // Traiter le cas où aucune tâche ne peut être ajoutée
    if( nonpred_list->count() == 0 )
    {
        nonpred_list->setHidden(true);
        //laucun->setHidden(false);
        btn_ajouterpred->setHidden(true);
    }
    // Si aucune des deux listes n'est présente (tâche seule dans le projet)
    if( pred_list->count() == 0 && nonpred_list->count() == 0 )
        pred_label->setHidden(true);
}

void TacheEditeur::initSousTaches()
{
    l_soust = new QHBoxLayout;
    soust_label = new QLabel("Sous-tâches", this);
    soust_list = new QComboBox(this);

    // Liste déroulante des sous-tâches
    const list<Tache*> l = dynamic_cast<TacheComposite*>(t)->getSousTaches();
    list<Tache*>::const_iterator it;
    for( it = l.begin() ; it != l.end() ; ++it )
        soust_list->addItem( (*it)->getTitre() );

    btn_retirersoust = new QPushButton("Retirer");

    // Liste déroulante des autres tâches
    nonsoust_list = new QComboBox(this);
    const list<Tache*> l2 = tm.getTaches();
    for( it = l2.begin() ; it != l2.end() ; ++it )
    {
        if( t != (*it) && !dynamic_cast<TacheComposite*>(t)->estSousTache((**it)) )
            nonsoust_list->addItem( (*it)->getTitre() );
    }

    btn_ajoutersoust = new QPushButton("Ajouter");

    l_soust->addWidget(soust_label);
    l_soust->addWidget(soust_list);
    l_soust->addWidget(btn_retirersoust);
    l_soust->addWidget(nonsoust_list);
    l_soust->addWidget(btn_ajoutersoust);

    connect( soust_list, SIGNAL(currentTextChanged(QString)), this, SLOT(modifierListeSoust()) );
    connect( nonsoust_list, SIGNAL(currentTextChanged(QString)), this, SLOT(modifierListeSoust()) );
    connect( btn_retirersoust, SIGNAL(clicked(bool)), this, SLOT(retirerSousTache()) );
    connect( btn_ajoutersoust, SIGNAL(clicked(bool)), this, SLOT(ajouterSousTache()) );

    // Modifier l'interface si la tache ne possède aucune sous-tâche
    if( soust_list->count() == 0 )
    {
        soust_list->setHidden(true);
        btn_retirersoust->setHidden(true);
    }

    // Traiter le cas où aucune tâche ne peut être ajoutée
    if( nonsoust_list->count() == 0 )
    {
        nonsoust_list->setHidden(true);
        btn_ajoutersoust->setHidden(true);
    }

    // Si aucune des deux listes n'est présente (tâche seule dans le projet)
    if( soust_list->count() == 0 && nonsoust_list->count() == 0 )
        soust_label->setHidden(true);

}

void TacheEditeur::retirerPredecesseur()
{
    const QString& title = pred_list->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    try
    {
        Tache* p = tm.trouverTache(title);
        t->retirerPredecesseur(*p);
        pred_list->removeItem( pred_list->currentIndex() );

        if( pred_list->count() == 0)
            pred_list->setCurrentText("");
        nonpred_list->addItem( title );
    }
    catch(CalendarException e)
    {   QMessageBox::warning(this,"Ajout precedence", e.getInfo()); }
}

void TacheEditeur::ajouterPredecesseur()
{
    const QString& title = nonpred_list->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");
    try
    {
        Tache* p = tm.trouverTache(title);
        t->ajouterPredecesseur(*p);
        nonpred_list->removeItem( nonpred_list->currentIndex() );

        if( nonpred_list->count() == 0)
            nonpred_list->setCurrentText("");
        pred_list->addItem( title );
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
    if( QObject::sender() == pred_list )
    {
        if( pred_list->currentText() == "" )
        {
            pred_list->setHidden(true);
            btn_retirerpred->setHidden(true);
        }
        else
        {
            pred_list->setHidden(false);
            btn_retirerpred->setHidden(false);
        }
    }

    // Le bouton "Ajouter" a été solicité
    //  la liste des tâches non prédécesseurs a été modifiée et envoie un signal
    if( QObject::sender() == nonpred_list )
    {
        if( nonpred_list->currentText() == "" )
        {
            nonpred_list->setHidden(true);
            btn_ajouterpred->setHidden(true);
        }
        else
        {
            nonpred_list->setHidden(false);
            btn_ajouterpred->setHidden(false);
        }
    }
}

void TacheEditeur::retirerSousTache()
{
    const QString& title = soust_list->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    dynamic_cast<TacheComposite*>(t)->retirerSousTache(*p);
    soust_list->removeItem( soust_list->currentIndex() );

    if( soust_list->count() == 0)
        soust_list->setCurrentText("");
    nonsoust_list->addItem( title );
}

void TacheEditeur::ajouterSousTache()
{
    const QString& title = nonsoust_list->currentText();

    if( title == "")
        throw CalendarException("Aucune tache");

    Tache* p = tm.trouverTache(title);
    try
    {
        dynamic_cast<TacheComposite*>(t)->ajouterSousTache(*p);
        nonsoust_list->removeItem( nonsoust_list->currentIndex() );

        if( nonsoust_list->count() == 0)
            nonsoust_list->setCurrentText("");
        soust_list->addItem( title );
    }
    catch(CalendarException e)
    {   QMessageBox::warning(this,"Ajout precedence", e.getInfo());  }
}

void TacheEditeur::modifierListeSoust()
{
    // Le bouton "Retirer" a été solicité
    //  la liste des sous-taches a été modifiée et envoie un signal
    if( QObject::sender() == soust_list )
    {
        if( soust_list->currentText() == "" )
        {
            soust_list->setHidden(true);
            btn_retirersoust->setHidden(true);
        }
        else
        {
            soust_list->setHidden(false);
            btn_retirersoust->setHidden(false);
        }
    }

    // Le bouton "Ajouter" a été solicité
    //  la liste des tâches qui ne sont pas des sous-tâches a été modifiée et envoie un signal
    if( QObject::sender() == nonsoust_list )
    {
        if( nonsoust_list->currentText() == "" )
        {
            nonsoust_list->setHidden(true);
            btn_ajoutersoust->setHidden(true);
        }
        else
        {
            nonsoust_list->setHidden(false);
            btn_ajoutersoust->setHidden(false);
        }
    }
}

void TacheEditeur::sauvegarder()
{
    if(unaire && (duree_h->value()+duree_m->value() == 0) )
    {
        QMessageBox::warning(this,"Sauvegarde tâche unaire", "Une tâche unaire doit avoir une durée non nulle");
        return;
    }
    // Lors de la modification d'une tâche existante
    if(t!=0)
    {
        try
        {
            t->setTitre(titre->text());
            t->setDescription(desc->toPlainText());
            t->setDatesDisponibiliteEcheance(dispo->date(), echeance->date());

            TacheUnaire* tu = dynamic_cast<TacheUnaire*>(t);
            if( tu )
            {
                Duree dur(duree_h->value(), duree_m->value());
                if(preemp->isChecked()!= tu->isPreemptive()
                        || dur.getDureeEnMinutes() != tu->getDuree().getDureeEnMinutes() )
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Modification tâche",
                                                  "La modification des dates/durée engendrera la suppression"
                                                  "des programmations de la tâche.\nVoulez-vous continuer ?",
                                                  QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::Yes)
                    {
                        tu->setPreemptive(preemp->isChecked());
                        tu->setDuree(dur);
                    }
                }
            }

            emit fermeture();
            close();
        }
        catch(CalendarException e)
        {   QMessageBox::warning(this,"Ajout precedence", e.getInfo());  }
    }
    // Lors de l'ajout d'une nouvelle tâche
    else
    {
        try
        {
            if(unaire)
                tm.ajouterTacheUnaire(titre->text(), desc->toPlainText(), dispo->date(), echeance->date(),
                                      Duree(duree_h->value(), duree_m->value()), preemp->isChecked());
            else
                tm.ajouterTacheComposite(titre->text(), desc->toPlainText(), dispo->date(), echeance->date());

            emit fermeture();
            close();
        }
        catch(CalendarException e)
        {   QMessageBox::warning(this,"Ajout precedence", e.getInfo());  }
    }
}



TacheEditeur::~TacheEditeur()
{
    delete titre_label, desc_label, dispo_label, duree_label, echeance_label, pred_label, soust_label;
    delete titre, desc, preemp;
    delete dispo, echeance, duree_h, duree_m;
    delete btn_cancel, btn_save, btn_ajouterpred, btn_retirerpred, btn_ajoutersoust, btn_retirersoust;
    delete pred_list, soust_list, nonpred_list, nonsoust_list;
    delete l_titre, l_desc, l_dates, l_pred, l_soust, l_cancelsave;
    delete main_layout;
}



