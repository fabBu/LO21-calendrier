#include "programmationediteur.h"

ProgrammationEditeur::ProgrammationEditeur(ProgrammationManager& pm, QWidget *p): programmationmanager(pm), parent(p){
    setWindowTitle("Programmation d'un événement");
    setFixedSize(650,300);

    main_layout = new QVBoxLayout;
    param_layout = new QVBoxLayout;
    calendar_layout = new QVBoxLayout;
    attributs_layout = new QHBoxLayout;

    initTitre();
    param_layout->addLayout(l_titre);

    initDesc();
    param_layout->addLayout(l_desc);

    initLieu();
    param_layout->addLayout(l_lieu);

    initType();
    param_layout->addLayout(l_type);

    initHoraire();
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

//ProgrammationEditeur::ProgrammationEditeur(ProgrammationManager& pm, Programmation& pr, QWidget *p = 0): programmationmanager(pm), programmation(p), parent(p){

//}

void ProgrammationEditeur::initTitre(){
    l_titre = new QHBoxLayout;
    titre_label = new QLabel("Titre",this);
    titre = new QLineEdit(this);
    titre->setFixedWidth(250);

    l_titre->addWidget(titre_label);
    l_titre->addWidget(titre);
}

void ProgrammationEditeur::initDesc(){
    l_desc = new QHBoxLayout;
    desc_label = new QLabel("Description",this);
    desc = new QTextEdit(this);
    desc->setFixedSize(250,50);
    l_desc->addWidget(desc_label);
    l_desc->addWidget(desc);
}

void ProgrammationEditeur::initCalendar(){
    l_calendar = new QHBoxLayout;
    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(2000, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
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
    }
    l_type->addWidget(type_label);
    l_type->addWidget(type);
}

void ProgrammationEditeur::initHoraire(){
    l_horaires = new QHBoxLayout;
    horaire_label = new QLabel("Horaire",this);
    horaire = new QTimeEdit(this);
    duree_label = new QLabel("Duree",this);
    duree = new QTimeEdit(this);
    duree->setMaximumTime(QTime(12,0));
    l_horaires->addWidget(horaire_label);
    l_horaires->addWidget(horaire);
    l_horaires->addWidget(duree_label);
    l_horaires->addWidget(duree);
}

void ProgrammationEditeur::sauvegarder(){
    try {
        programmationmanager.addProgrammation(QDateTime(calendar->selectedDate(),horaire->time()),Duree(duree->time().hour(),duree->time().minute()),Activite(titre->text(),desc->toPlainText(), static_cast<MetaEnum::Type>(type->currentIndex()),lieu->text()));
        QString message = "La programmation suivante a été ajouté :" + programmationmanager.getProgrammation(QDateTime(calendar->selectedDate(),horaire->time())).getEvenement().getTitre();
        QMessageBox::warning(this,"Ajout programmation", message);
    } catch(CalendarException e)
    {   QMessageBox::warning(this,"Ajout progrmmation", e.getInfo());  }
}

//void ProgrammationEditeur::fermeture(){

//}


ProgrammationEditeur::~ProgrammationEditeur(){
    delete titre_label, desc_label, type_label, lieu_label, horaire_label, duree_label;
    delete titre, lieu;
    delete desc;
    delete calendar;
    delete horaire, duree;
    delete btn_cancel, btn_save;
    delete l_main, l_titre, l_desc, l_type, l_lieu, l_horaires, l_cancelsave, attributs_layout, l_calendar;
    delete calendar_layout, main_layout, param_layout;
}
