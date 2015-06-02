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

        initDureeTache();
        param_layout->addLayout(l_durees);
    }

    initProgrammation(pr);
    param_layout->addLayout(l_horaires);

    initCalendar(pr);
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initProgramButtons();
    initCancelSave();
    main_layout->addLayout(attributs_layout);
    main_layout->addLayout(l_programButtons);
    main_layout->addLayout(l_cancelsave);
    this->setLayout(main_layout);
}

ProgrammationEditeur::ProgrammationEditeur(Evenement* ev, QWidget *p): programmationmanager(ProgrammationManager::getInstance()) {
    setWindowTitle("Programmation d'un événement");
    setFixedSize(650,300);

    activite = dynamic_cast<Activite*>(ev);
    tache = dynamic_cast<TacheUnaire*>(ev);

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

        initDureeTache();
        param_layout->addLayout(l_durees);
    }

    initProgrammation();
    param_layout->addLayout(l_horaires);

    initCalendar();
    calendar_layout->addLayout(l_calendar);

    attributs_layout->addLayout(calendar_layout);
    attributs_layout->addLayout(param_layout);

    initCancelSave();
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
    if (tache) titre->setText(tache->getTitre());

    l_titre->addWidget(titre_label);
    l_titre->addWidget(titre);
}

void ProgrammationEditeur::initDesc(){
    l_desc = new QHBoxLayout;
    desc_label = new QLabel("Description",this);
    desc = new QTextEdit(this);
    desc->setFixedSize(250,50);

    if (activite) desc->setText(activite->getDescription());
    if (tache) desc->setText(tache->getDescription());

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
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()) );
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
        type->addItem(m->enumToString(j));
        if (activite){
            if (activite->getType() == j) {
                type->setCurrentIndex(j);
            }
        }
    }
    l_type->addWidget(type_label);
    l_type->addWidget(type);
}

void ProgrammationEditeur::initProgramButtons(){
    l_programButtons = new QHBoxLayout;
    termine = new QCheckBox("Termine",this);
    l_programButtons->addWidget(termine);
}

void ProgrammationEditeur::initDureeTache(){
    l_durees = new QHBoxLayout;
    dureeTotal_label = new QLabel("Duree de la tache",this);
    dureeTotal = new QTimeEdit(this);
    l_durees->addWidget(dureeTotal_label);
    l_durees->addWidget(dureeTotal);

    if (tache->isPreemptive()) {
        dureeRestante_label = new QLabel("Duree restante",this);
        dureeRestante = new QTimeEdit(this);
        l_durees->addWidget(dureeRestante_label);
        l_durees->addWidget(dureeRestante);
    }
}

void ProgrammationEditeur::initProgrammation(Programmation *pr){
    l_horaires = new QHBoxLayout;
    horaire_label = new QLabel("Horaire",this);
    horaire = new QTimeEdit(this);
    if (pr) horaire->setTime(pr->getDate().time());

    duree = new QTimeEdit(this);
    duree_label = new QLabel("Duree",this);
    if (pr) duree->setTime(pr->getDuree().getTime());

    l_horaires->addWidget(horaire_label);
    l_horaires->addWidget(horaire);
    l_horaires->addWidget(duree_label);
    l_horaires->addWidget(duree);
}

void ProgrammationEditeur::sauvegarder(){
    try {
        programmationmanager.addProgrammation(QDateTime(calendar->selectedDate(),horaire->time()),Duree(duree->time().hour(),duree->time().minute()),new Activite(titre->text(),desc->toPlainText(), static_cast<MetaEnum::Type>(type->currentIndex()),lieu->text()));
        QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
        QMessageBox::warning(this,"Ajout programmation", message);
    } catch(CalendarException e)
    {   QMessageBox::warning(this,"Ajout programmation", e.getInfo());  }
}

void ProgrammationEditeur::initDates(){
    l_dates = new QHBoxLayout;
    disponibilite_label = new QLabel("Disponibilite",this);
    disponibilite = new QDateEdit(this);
    disponibilite->setDate(tache->getDateDisponibilite());

    echeance_label = new QLabel("Echeance",this);
    echeance = new QDateEdit(this);
    echeance->setDate(tache->getDateEcheance());

    l_dates->addWidget(disponibilite_label);
    l_dates->addWidget(disponibilite);
    l_dates->addWidget(echeance_label);
    l_dates->addWidget(echeance);
}


ProgrammationEditeur::~ProgrammationEditeur(){
    delete titre_label, desc_label, type_label, lieu_label, horaire_label, duree_label, disponibilite_label, echeance_label;
    delete titre, lieu;
    delete desc;
    delete calendar;
    delete termine;
    delete disponibilite, echeance;
    delete horaire, duree;
    delete btn_cancel, btn_save;
    delete l_main, l_titre, l_desc, l_type, l_lieu, l_horaires, l_cancelsave, attributs_layout, l_calendar, l_dates, l_programButtons;
    delete calendar_layout, main_layout, param_layout;
}
