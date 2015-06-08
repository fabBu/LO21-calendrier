#include "programmationediteur.h"

ProgrammationEditeur::ProgrammationEditeur(Programmation* pr, QWidget *p): programmationmanager(ProgrammationManager::getInstance()) {
    setWindowTitle("Programmation d'un événement");
    setFixedSize(650,300);

    activite = dynamic_cast<Activite*>(&(pr->getEvenement()));
    tache = dynamic_cast<TacheUnaire*>(&(pr->getEvenement()));

    main_layout = new QVBoxLayout;
    param_layout = new QVBoxLayout;
    calendar_layout = new QVBoxLayout;
    attributs_layout = new QHBoxLayout;

    initTitre();
    param_layout->addLayout(l_titre);

    initDesc();
    param_layout->addLayout(l_desc);

    if (activite){
        initLieu();
        param_layout->addLayout(l_lieu);

        initType();
        param_layout->addLayout(l_type);
    } else if (tache) {
        initDates();
        param_layout->addLayout(l_dates);

        if (tache->isPreemptive()){
            initDureeTotale();
            param_layout->addLayout(l_dureeTotale);
        }
    }

    initDureeRestante();
    param_layout->addLayout(l_dureeRestante);

    initProgrammation(pr);
    param_layout->addLayout(l_horaires);

    initCalendar(pr);
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initCancelSave();
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(modifier()) );
    main_layout->addLayout(attributs_layout);
    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
}

ProgrammationEditeur::ProgrammationEditeur(Evenement* ev, QWidget *p): programmationmanager(ProgrammationManager::getInstance()) {
    setWindowTitle("Programmation d'un événement");
    setFixedSize(650,300);

    activite = dynamic_cast<Activite*>(ev);
    tache = dynamic_cast<TacheUnaire*>(ev);

    if (tache) {
        if (tache->estTermine())
                throw CalendarException ("Attention: La tache est terminée.");
        if (tache->getDureeRestante().getDureeEnMinutes() == 0)
            throw CalendarException("Attention: l'intégralité de la tache a été programmé.");

    }

    main_layout = new QVBoxLayout;
    param_layout = new QVBoxLayout;
    calendar_layout = new QVBoxLayout;
    attributs_layout = new QHBoxLayout;

    initTitre();
    param_layout->addLayout(l_titre);

    initDesc();
    param_layout->addLayout(l_desc);

    if (ev == 0 || activite){
        initLieu();
        param_layout->addLayout(l_lieu);

        initType();
        param_layout->addLayout(l_type);
    } else {
        initDates();
        param_layout->addLayout(l_dates);

        if (tache->isPreemptive()) {
            initDureeTotale();
            param_layout->addLayout(l_dureeTotale);
        }
    }

    initDureeRestante();
    param_layout->addLayout(l_dureeRestante);

    initProgrammation();
    param_layout->addLayout(l_horaires);

    initCalendar();
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initCancelSave();
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()) );
    main_layout->addLayout(attributs_layout);
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
        dureeRestante_h->setEnabled(false);

        dureeRestante_m->setValue(tache->getDureeRestante().getDureeEnMinutes() - 60*tache->getDureeRestante().getDureeEnHeures());
        dureeRestante_m->setEnabled(false);
    } else {
        dureeRestante_h->setValue(12);
        dureeRestante_m->setValue(0);
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

    duree_m = new QSpinBox(this);
    duree_m->setSuffix("minute(s)");


    if (tache && tache->isPreemptive()) {
        duree_h->setMaximum(tache->getDureeRestante().getDureeEnHeures());
        if (tache->getDureeRestante().getDureeEnMinutes() < 60) {
            duree_m->setMaximum(tache->getDuree().getDureeEnMinutes() - 60*tache->getDuree().getDureeEnHeures());
        }else{
            duree_m->setMaximum(59);
        }
    } else {
        duree_h->setMaximum(12);
        duree_m->setMaximum(59);

        if (tache) {
            duree_h->setValue(tache->getDuree().getDureeEnMinutes()/60);
            duree_m->setValue(tache->getDuree().getDureeEnMinutes()%60);
            duree_h->setEnabled(false);
            duree_m->setEnabled(false);
        }
    }

    if (pr) {
        duree_h->setValue(pr->getDuree().getDureeEnMinutes()/60);
        duree_m->setValue(pr->getDuree().getDureeEnMinutes()%60);
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
//            if (tache->isPreemptive() && tache->getDuree().getDureeEnMinutes() == d.getDureeEnMinutes())
//                throw CalendarException("La tache suivante est préemptive et il n'y a qu'une seule programmation.");
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
    Duree test = Duree(heures, minutes);
    if (tache) {
        if (test.getDureeEnMinutes() > tache->getDureeRestante().getDureeEnMinutes()) {
            dureeRestante_h->setValue(0);
            dureeRestante_m->setValue(0);

            duree_h->setValue(tache->getDureeRestante().getDureeEnMinutes()/60);
            duree_m->setValue(tache->getDureeRestante().getDureeEnMinutes()%60);
        }else{
            int res = tache->getDureeRestante().getDureeEnMinutes() - test.getDureeEnMinutes();
            dureeRestante_h->setValue(res/60);
            dureeRestante_m->setValue(res%60);
        }
    } else {
        Duree max = Duree(12, 00);
        if (test.getDureeEnMinutes() > max.getDureeEnMinutes()) {
            duree_h->setValue(12);
            duree_m->setValue(0);
        }
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
