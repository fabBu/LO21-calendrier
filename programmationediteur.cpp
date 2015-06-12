#include "programmationediteur.h"

ProgrammationEditeur::ProgrammationEditeur(Programmation* pr, QWidget *p): programmationmanager(ProgrammationManager::getInstance()) {

    groupBoxEvenement = new QGroupBox("Evenement");
    groupBoxProgrammation = new QGroupBox("Programmation");

    activite = dynamic_cast<Activite*>(&(pr->getEvenement()));
    tache = dynamic_cast<TacheUnaire*>(&(pr->getEvenement()));
    programmation = pr;

    if (tache) {
        if (tache->isPreemptive()) {
            setWindowTitle("Programmation d'une tache préemptive");
        } else {
            setWindowTitle("Programmation d'une tache non préemptive");
        }
    } else {
        setWindowTitle("Programmation d'une activité");
    }

    main_layout = new QVBoxLayout;
    param_layout = new QVBoxLayout;
    evenement_layout = new QVBoxLayout;
    programmation_layout = new QVBoxLayout;
    calendar_layout = new QVBoxLayout;
    attributs_layout = new QHBoxLayout;

    initTitre();
    evenement_layout->addLayout(l_titre);

    initDesc();
    evenement_layout->addLayout(l_desc);

    if (activite){
        initLieu();
        evenement_layout->addLayout(l_lieu);

        initType();
        evenement_layout->addLayout(l_type);
    } else if (tache) {
        initDates();
        programmation_layout->addLayout(l_dates);

        if (tache->isPreemptive()){
            initDureeTotale();
            programmation_layout->addLayout(l_dureeTotale);
        }
    }

    initDureeRestante();
    programmation_layout->addLayout(l_dureeRestante);

    initProgrammation(pr);
    programmation_layout->addLayout(l_horaires);

    groupBoxEvenement->setLayout(evenement_layout);
    groupBoxProgrammation->setLayout(programmation_layout);

    param_layout->addWidget(groupBoxEvenement);
    param_layout->addWidget(groupBoxProgrammation);

    initCalendar(pr);
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initCancelSave();

    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(modifier()));
    main_layout->addLayout(attributs_layout);

    if (tache && tache->isPreemptive()) {
        initListeProgrammation();
        main_layout->addLayout(programmations);
    }

    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
}

ProgrammationEditeur::ProgrammationEditeur(Evenement* ev, QWidget *p): programmationmanager(ProgrammationManager::getInstance()) {

    activite = dynamic_cast<Activite*>(ev);
    tache = dynamic_cast<TacheUnaire*>(ev);
    programmation = 0;

    if (tache) {
        if (tache->isPreemptive()) {
            setWindowTitle("Programmation d'une tache préemptive");
        } else {
            setWindowTitle("Programmation d'une tache non préemptive");
        }
    } else {
        setWindowTitle("Programmation d'une activité");
    }

    if (tache) {
        if (tache->estTermine())
                throw CalendarException ("Attention: La tache est terminée.");
        if (tache->getDureeRestante().getDureeEnMinutes() == 0)
            throw CalendarException("Attention: l'intégralité de la tache a été programmé.");

    }

    main_layout = new QVBoxLayout;
    param_layout = new QVBoxLayout;
    evenement_layout = new QVBoxLayout;
    programmation_layout = new QVBoxLayout;
    calendar_layout = new QVBoxLayout;
    attributs_layout = new QHBoxLayout;

    groupBoxEvenement = new QGroupBox("Evenement");
    groupBoxProgrammation = new QGroupBox("Programmation");

    initTitre();
    evenement_layout->addLayout(l_titre);

    initDesc();
    evenement_layout->addLayout(l_desc);

    if (ev == 0 || activite){
        initLieu();
        evenement_layout->addLayout(l_lieu);

        initType();
        evenement_layout->addLayout(l_type);
    } else {
        initDates();
        programmation_layout->addLayout(l_dates);

        if (tache->isPreemptive()) {
            initDureeTotale();
            programmation_layout->addLayout(l_dureeTotale);
        }
    }

    initDureeRestante();
    programmation_layout->addLayout(l_dureeRestante);

    initProgrammation();
    programmation_layout->addLayout(l_horaires);

    groupBoxEvenement->setLayout(evenement_layout);
    groupBoxProgrammation->setLayout(programmation_layout);

    param_layout->addWidget(groupBoxEvenement);
    param_layout->addWidget(groupBoxProgrammation);

    initCalendar();
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initCancelSave();
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()) );
    main_layout->addLayout(attributs_layout);

    if (tache && tache->isPreemptive()) {
        qDebug() << "yyololoaloalaol";
        initListeProgrammation();
        main_layout->addLayout(programmations);
    }

    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
}

void ProgrammationEditeur::initTitre(){
    l_titre = new QHBoxLayout;
    titre_label = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    titre->setFixedWidth(250);

    if (activite) titre->setText(activite->getTitre());
    if (tache) {
        titre->setText(tache->getTitre());
        titre->setEnabled(false);
        QPalette palette = titre->palette();
        palette.setColor(QPalette::Base, QColor(255,255,255));
        titre->setPalette(palette);
        titre->setStyleSheet("color: black;");
    }

    l_titre->addWidget(titre_label);
    l_titre->addWidget(titre);
}

void ProgrammationEditeur::initDesc(){
    l_desc = new QHBoxLayout;
    desc_label = new QLabel("Description",this);
    desc = new QTextEdit(this);
    desc->setFixedSize(250,50);

    if (activite) desc->setText(activite->getDescription());
    if (tache){
        desc->setText(tache->getDescription());
        desc->setEnabled(false);
        QPalette palette = titre->palette();
        palette.setColor(QPalette::Base, QColor(255,255,255));
        desc->setPalette(palette);
        desc->setStyleSheet("color: black;");
    }

    l_desc->addWidget(desc_label);
    l_desc->addWidget(desc);
}

void ProgrammationEditeur::initCalendar(Programmation* pr){
    l_calendar = new QHBoxLayout;
    calendar = new QCalendarWidget;
    if (tache){
        calendar->setMinimumDate(tache->getDateDisponibilite());
        calendar->setMaximumDate(tache->getDateEcheance());
    } else {
        calendar->setMinimumDate(QDate(2000,1,1));
        calendar->setMaximumDate(QDate(3000,1,1));
    }

    if (pr) {calendar->setSelectedDate(pr->getDate().date());}

    l_calendar->addWidget(calendar);
}

void ProgrammationEditeur::initCancelSave(){
    l_cancelsave = new QHBoxLayout;
    btn_cancel = new QPushButton("Annuler",this);
    btn_save = new QPushButton("Sauver",this);
    l_cancelsave->addWidget(btn_cancel);

    if (programmation) {
        btn_supprimer = new QPushButton("Suppression",this);
        connect(btn_supprimer, SIGNAL(clicked(bool)), this, SLOT(supprimer()));
        l_cancelsave->addWidget(btn_supprimer);
    }

    l_cancelsave->addWidget(btn_save);

    connect(btn_cancel, SIGNAL(clicked(bool)), this, SLOT(close()) );
}

void ProgrammationEditeur::initLieu(){
    l_lieu = new QHBoxLayout;
    lieu_label = new QLabel("Lieu",this);
    lieu = new QLineEdit(this);
    lieu->setFixedWidth(250);

    if (activite) lieu->setText(activite->getLieu());

    l_lieu->addWidget(lieu_label);
    l_lieu->addWidget(lieu);
}

void ProgrammationEditeur::initType(){
    l_type = new QHBoxLayout;
    type = new QComboBox(this);
    type_label = new QLabel("Type",this);
    MetaEnum *m = new MetaEnum();
    int i = m->getNbEnum();
    for (int j=0;j<i;j++){
        type->addItem(m->typeToString(j));
        if (activite){
            if (activite->getType() == j) {
                type->setCurrentIndex(j);
            }
        }
    }
    l_type->addWidget(type_label);
    l_type->addWidget(type);
}

void ProgrammationEditeur::initDureeTotale(){
    l_dureeTotale = new QHBoxLayout;
    dureeTotal_label = new QLabel("Duree de la tache",this);

    dureeTotal_h = new QSpinBox(this);
    dureeTotal_h->setSuffix("heure(s)");
    dureeTotal_h->setValue(tache->getDuree().getDureeEnHeures());
    dureeTotal_h->setEnabled(false);

    dureeTotal_m = new QSpinBox(this);
    dureeTotal_m->setSuffix("minute(s)");
    dureeTotal_m->setValue(tache->getDuree().getDureeEnMinutes() - 60*tache->getDuree().getDureeEnHeures());
    dureeTotal_m->setEnabled(false);

    l_dureeTotale->addWidget(dureeTotal_label);
    l_dureeTotale->addWidget(dureeTotal_h);
    l_dureeTotale->addWidget(dureeTotal_m);
}

void ProgrammationEditeur::initDureeRestante(){
    l_dureeRestante = new QHBoxLayout;
    dureeRestante_h = new QSpinBox(this);
    dureeRestante_h->setSuffix("heure(s)");
    dureeRestante_m = new QSpinBox(this);
    dureeRestante_m->setSuffix("minute(s)");


    if (tache && tache->isPreemptive()) {
        dureeRestante_label = new QLabel("Duree restante",this);
        l_dureeRestante->addWidget(dureeRestante_label);
        dureeRestante_h->setValue(tache->getDureeRestante().getDureeEnHeures());
        dureeRestante_m->setValue(tache->getDureeRestante().getDureeEnMinutes()%60);
        dureeRestante_h->setEnabled(false);
        dureeRestante_m->setEnabled(false);
    } else {
        dureeRestante_h->hide();
        dureeRestante_m->hide();
    }

    l_dureeRestante->addWidget(dureeRestante_h);
    l_dureeRestante->addWidget(dureeRestante_m);
}

void ProgrammationEditeur::initProgrammation(Programmation *pr){
    l_horaires = new QHBoxLayout;
    horaire_label = new QLabel("Horaire",this);
    horaire = new QTimeEdit(this);
    if (pr) horaire->setTime(pr->getDate().time());
    l_horaires->addWidget(horaire_label);
    l_horaires->addWidget(horaire);

    duree_label = new QLabel("Duree",this);

    duree_h = new QSpinBox(this);
    duree_h->setSuffix("heure(s)");
    duree_h->setMaximum(12);

    duree_m = new QSpinBox(this);
    duree_m->setSuffix("minute(s)");
    duree_m->setMaximum(45);
    duree_m->setSingleStep(15);


    if (tache && tache->isPreemptive()) {
        int dureeMax = tache->getDureeRestante().getDureeEnMinutes();
        if (programmation) {
            dureeMax += programmation->getDuree().getDureeEnMinutes();
        }
        if (dureeMax < (12*60)) {
            duree_h->setMaximum(dureeMax/60);
            duree_m->setMaximum(dureeMax%60);

        }
    }

    if (tache && !(tache->isPreemptive())) {
        duree_h->setValue(tache->getDuree().getDureeEnMinutes()/60);
        duree_m->setValue(tache->getDuree().getDureeEnMinutes()%60);
        duree_h->setEnabled(false);
        duree_m->setEnabled(false);
    }

    if (programmation) {
        duree_h->setValue(programmation->getDuree().getDureeEnMinutes()/60);
        duree_m->setValue(programmation->getDuree().getDureeEnMinutes()%60);
    }

    connect(duree_h, SIGNAL(valueChanged(int)), this, SLOT(modifDureeRestante()) );
    connect(duree_m, SIGNAL(valueChanged(int)), this, SLOT(modifDureeRestante()) );

    l_horaires->addWidget(duree_label);
    l_horaires->addWidget(duree_h);
    l_horaires->addWidget(duree_m);

}

void ProgrammationEditeur::sauvegarder(){
    try {
        if (titre->text().isEmpty()) throw CalendarException("Attention: Il faut un titre pour pouvoir enregistrer l'activité");
        if (duree_h->value() == 0 && duree_m->value() == 0) throw CalendarException("Attention: Il faut une durée supérieur à 0!");
        if (tache){
            Duree d = Duree(duree_h->value(),duree_m->value());
            programmationmanager.addProgrammation(QDateTime(calendar->selectedDate(),horaire->time()),d,tache);
            QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
            QMessageBox::warning(this,"Ajout programmation", message);
        } else {
            programmationmanager.addProgrammation(QDateTime(calendar->selectedDate(),horaire->time()),Duree(duree_h->value(),duree_m->value()),new Activite(titre->text(),desc->toPlainText(), static_cast<MetaEnum::Type>(type->currentIndex()),lieu->text()));
            QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
            QMessageBox::warning(this,"Ajout programmation", message);
        }
        emit fermeture();
        close();
    } catch(CalendarException e)
    {
        QMessageBox::warning(this,"Ajout programmation", e.getInfo());
    }
}

void ProgrammationEditeur::modifier(){
    try {
        if (titre->text().isEmpty()) throw CalendarException("Attention: Vous avez modifié le titre de la tache.");
        if (duree_h->value() == 0 && duree_m->value() == 0) throw CalendarException("Attention: Il faut une durée supérieur à 0!");
        if (tache){
            Duree d = Duree(duree_h->value(),duree_m->value());
            Duree dRestante = Duree(dureeRestante_h->value(),dureeRestante_m->value());
            if (programmationmanager.isFree(programmation, QDateTime(calendar->selectedDate(),horaire->time()), Duree(duree_h->value(),duree_m->value()))) {
                programmation->setDate(QDateTime(calendar->selectedDate(),horaire->time()));
                programmation->setDuree(Duree(duree_h->value(),duree_m->value()));
                if (dRestante.getDureeEnMinutes() != tache->getDureeRestante().getDureeEnMinutes()) {
                    tache->setDureeRestante(dRestante);
                    if (dRestante.getDureeEnMinutes() == 0) {
                        tache->setTermine(true);
                    } else {
                        tache->setTermine(false);
                    }
                }
            } else {
                throw CalendarException("ERREUR: Une tâche est déjà programmé à cette date");
            }
            QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
            QMessageBox::warning(this,"Ajout programmation", message);
        } else {
            if (programmationmanager.isFree(programmation, QDateTime(calendar->selectedDate(),horaire->time()), Duree(duree_h->value(),duree_m->value()))) {
                programmation->setDate(QDateTime(calendar->selectedDate(),horaire->time()));
                programmation->setDuree(Duree(duree_h->value(),duree_m->value()));
            } else {
                throw CalendarException("ERREUR: Une tâche est déjà programmé à cette date");
            }
            if (titre->text() != activite->getTitre()) {
                activite->setTitre(titre->text());
            }
            if (desc->toPlainText() != activite->getDescription()){
                activite->setDescription(desc->toPlainText());
            }
            if (lieu->text() != activite->getLieu()) {
                activite->setLieu(lieu->text());
            }
            if (static_cast<MetaEnum::Type>(type->currentIndex()) != activite->getType()) {
                activite->setType(static_cast<MetaEnum::Type>(type->currentIndex()));
            }
            QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
            QMessageBox::warning(this,"Ajout programmation", message);
        }
        emit fermeture();
        close();
    } catch(CalendarException e)
    {
        QMessageBox::warning(this,"Ajout programmation", e.getInfo());
    }
}

void ProgrammationEditeur::initDates(){
    l_dates = new QHBoxLayout;
    disponibilite_label = new QLabel("Disponibilite",this);
    disponibilite = new QDateEdit(this);
    disponibilite->setDate(tache->getDateDisponibilite());
    disponibilite->setEnabled(false);

    echeance_label = new QLabel("Echeance",this);
    echeance = new QDateEdit(this);
    echeance->setDate(tache->getDateEcheance());
    echeance->setEnabled(false);

    l_dates->addWidget(disponibilite_label);
    l_dates->addWidget(disponibilite);
    l_dates->addWidget(echeance_label);
    l_dates->addWidget(echeance);
}

void ProgrammationEditeur::modifDureeRestante(){
    int heures = duree_h->value();
    int minutes = duree_m->value();
    int test = Duree(heures, minutes).getDureeEnMinutes();

    if (test>(12*60)) {
        duree_h->setValue(12);
        duree_m->setValue(0);
        test = Duree(12, 0).getDureeEnMinutes();
    }
    if (tache && tache->isPreemptive()) {
        int dureeMax = tache->getDureeRestante().getDureeEnMinutes();
        if (programmation) {
            dureeMax += programmation->getDuree().getDureeEnMinutes();
        }
        if (test > dureeMax) {
            duree_h->setValue(dureeMax/60);
            duree_m->setValue(dureeMax%60);
        }
        int res = dureeMax - test;
        dureeRestante_h->setValue(res/60);
        dureeRestante_m->setValue(res%60);
    }
}

void ProgrammationEditeur::supprimer(){
    QMessageBox::StandardButton reply;
    QString question = "Voulez-vous vraiment supprimer la programmation du "+programmation->getDate().date().toString("dddd d MMMM yyyy")+" à "
            + programmation->getDate().time().toString("hh:mm")+ " ?";
    if (tache) {
        question+="\n\nNB: Toutes les tâches successeurs déjà programmées seront également supprimées.";
    }
    reply = QMessageBox::question(this, "Suppression",question,QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        try {
            programmationmanager.removeProgrammation(programmation);
            delete programmation;
            emit fermeture();
            close();
        }  catch(CalendarException e)
        {
            QMessageBox::warning(this,"Suppression programmation", e.getInfo());
        }
    }
}

void ProgrammationEditeur::initListeProgrammation(){
    programmations = new QHBoxLayout;
    listesProgrammation = new QListWidget();

    list<Programmation*> listes = ProgrammationManager::getInstance().getProgrammation(tache);
    int i=0;
    qDebug() << "je suis là";
    for (list<Programmation*>::const_iterator it = listes.begin(); it != listes.end(); it++){
        if ((*it) != programmation) {
            QString texte = "Programmé le "+(*it)->getDate().date().toString("dddd d/M/yyyy") + " à " +
                    (*it)->getDate().time().toString("hh:mm") + " pour une durée de " + QString::number((*it)->getDuree().getDureeEnMinutes()/60) + "h";
            int minutes = (*it)->getDuree().getDureeEnMinutes()%60;
            if (minutes != 0) {
                texte+= " et " + QString::number(minutes) + " min";
            }
            texte+=".";
            qDebug() << texte;
            listesProgrammation->insertItem(i,new QListWidgetItem(texte));
            i++;
        }
    }

    if (programmation == 0 && listes.size()!=0) {
        programmations->addWidget(listesProgrammation);
    } else if (listes.size()-1 != 0) {
        programmations->addWidget(listesProgrammation);
    }
}

ProgrammationEditeur::~ProgrammationEditeur(){
    delete titre_label, desc_label, type_label, lieu_label, horaire_label, duree_label, disponibilite_label, echeance_label;
    delete titre, lieu;
    delete desc;
    delete calendar;
    delete termine;
    delete disponibilite, echeance;
    delete horaire;
    delete btn_cancel, btn_save;
    delete l_main, l_titre, l_desc, l_type, l_lieu, l_horaires, l_cancelsave, attributs_layout, l_calendar, l_dates, l_dureeTotale, l_dureeRestante;
    delete calendar_layout, main_layout, param_layout;
}
