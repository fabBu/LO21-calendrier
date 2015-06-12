#include "agendaediteur.h"
#include <QHeaderView>
#include <QScrollBar>

AgendaEditeur::AgendaEditeur()
{
    setWindowTitle("Programmation d'un événement");
//    setMinimumSize(1334,800);

    semaine_layout = new QHBoxLayout;
    calendar_layout = new QVBoxLayout;
    main_layout = new QVBoxLayout;
    calendar = new QTableWidget;

    dateJour = QDate::currentDate();
    jourSemaine = dateJour.dayOfWeek();
    semaine = 0;

    lundi = dateJour.addDays(1-jourSemaine);
    mardi = dateJour.addDays(2-jourSemaine);
    mercredi = dateJour.addDays(3-jourSemaine);
    jeudi = dateJour.addDays(4-jourSemaine);
    vendredi = dateJour.addDays(5-jourSemaine);
    samedi = dateJour.addDays(6-jourSemaine);
    dimanche = dateJour.addDays(7-jourSemaine);

    setSemaine();
    setCalendar();
    setSemaineList();
    setProgrammation();
    setBouttonProgrammation();

    main_layout->addLayout(semaine_layout);
    main_layout->addLayout(calendar_layout);
    setLayout(main_layout);
}

void AgendaEditeur::setSemaine() {
    precedent = new QPushButton("<",this);
    suivant = new QPushButton(">",this);
    connect(precedent, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelPrecedent()));
    connect(suivant, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelSuivant()) );

    if (lundi.year() != dimanche.year()) {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM yyyy - ")+dimanche.toString("d MMM yyyy"));
    } else {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM - ")+dimanche.toString("d MMM yyyy"));
    }

    semaine_label->setAlignment(Qt::AlignCenter);

    semaine_layout->addWidget(precedent);
    semaine_layout->addWidget(semaine_label);
    semaine_layout->addWidget(suivant);
}

void AgendaEditeur::modifDateLabel(){
    if (lundi.year() != dimanche.year()) {
        semaine_label->setText(lundi.toString("Semaine: d MMM yyyy - ")+dimanche.toString("d MMM yyyy"));
    } else {
        semaine_label->setText(lundi.toString("Semaine: d MMM - ")+dimanche.toString("d MMM yyyy"));
    }
}

void AgendaEditeur::modifDate(){
    lundi = dateJour.addDays((7*semaine)+(1-jourSemaine));
    mardi = dateJour.addDays((7*semaine)+(2-jourSemaine));
    mercredi = dateJour.addDays((7*semaine)+(3-jourSemaine));
    jeudi = dateJour.addDays((7*semaine)+(4-jourSemaine));
    vendredi = dateJour.addDays((7*semaine)+(5-jourSemaine));
    samedi = dateJour.addDays((7*semaine)+(6-jourSemaine));
    dimanche = dateJour.addDays((7*semaine)+(7-jourSemaine));
}

void AgendaEditeur::modifDateLabelSuivant(){
    semaine++;
    modifDate();
    modifDateLabel();
    setSemaineList();
    refresh();
}

void AgendaEditeur::modifDateLabelPrecedent(){
    semaine--;
    modifDate();
    modifDateLabel();
    setSemaineList();
    refresh();
}

void AgendaEditeur::setCalendar() {
    calendar->setRowCount(24*4);
    calendar->setColumnCount(7);
    calendar->verticalScrollBar()->setSliderPosition(7*4);
    setSemaineList();
    setHoraireList();
    calendar->setEditTriggers(QAbstractItemView::NoEditTriggers);
    calendar_layout->addWidget(calendar);
    calendar->horizontalHeader()->setStretchLastSection(true);
    calendar->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    calendar->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    calendar->verticalHeader()->setDefaultSectionSize(17);
    connect(calendar, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(editerProgrammation(QTableWidgetItem*)) );
}

void AgendaEditeur::setSemaineList() {
    liste_semaine.clear();

    QString format = "dddd d/M";
    liste_semaine << lundi.toString(format);
    liste_semaine << mardi.toString(format);
    liste_semaine << mercredi.toString(format);
    liste_semaine << jeudi.toString(format);
    liste_semaine << vendredi.toString(format);
    liste_semaine << samedi.toString(format);
    liste_semaine << dimanche.toString(format);

    calendar->setHorizontalHeaderLabels(liste_semaine);
}

void AgendaEditeur::setHoraireList() {
    QStringList liste_horaire;

    for (int i=0; i<24; i++) {
        liste_horaire << QString::number(i)+":00";
        liste_horaire << "";
        liste_horaire << "";
        liste_horaire << "";
    }

    calendar->setVerticalHeaderLabels(liste_horaire);
}

void AgendaEditeur::setProgrammation() {
    std::list<Programmation*> listes = ProgrammationManager::getInstance().getProgrammation(lundi,dimanche);
    programmations =  QList<QTableWidgetItem*>();
    for(std::list<Programmation*>::const_iterator it = listes.begin(); it != listes.end(); ++it) {
        qDebug() << (*it)->getEvenement().getTitre();
        int nbJours = (*it)->getDateFin().date().dayOfWeek() - (*it)->getDate().date().dayOfWeek();
        if (((*it)->getDateFin().date().dayOfWeek() == 7 || (*it)->getDateFin().date().dayOfWeek() == 1) && nbJours != 0){
            nbJours = 1;
        }

        if (nbJours == 0) {
            QString texte = (*it)->getDate().time().toString("hh:mm") + " - " + (*it)->getDateFin().time().toString("hh:mm") + ": " + (*it)->getEvenement().getTitre();
            QTableWidgetItem* temp = new QTableWidgetItem(texte);
            QFont font;
            font.setBold(true);
            temp->setFont(font);
            temp->setTextAlignment(Qt::AlignTop);
            TacheUnaire* test = dynamic_cast<TacheUnaire*>(&((*it)->getEvenement()));
            if (test) {
                temp->setData(Qt::BackgroundRole, QVariant(test->getCouleur()));
            } else {
                temp->setData(Qt::BackgroundRole, QVariant(QColor(Qt::lightGray)));
            }

            int nbLignes = (*it)->getDuree().getDureeEnMinutes()/15;
            int debutLigne = ((*it)->getDate().time().minute() + ((*it)->getDate().time().hour()*60))/15;
            calendar->setItem(debutLigne, ((*it)->getDate().date().dayOfWeek())-1, temp);
            calendar->setSpan(debutLigne, (*it)->getDate().date().dayOfWeek()-1, nbLignes, 1);
            programmations.push_back(temp);
        } else {
            for (int i=0; i<nbJours+1;i++){
               QDate date = (*it)->getDate().date().addDays(i);
               if (!(date<lundi || date>dimanche)){
                   QString texte = (*it)->getDate().time().toString("hh:mm") + " - " + (*it)->getDateFin().time().toString("hh:mm") + ": " + (*it)->getEvenement().getTitre();
                   QTableWidgetItem* temp = new QTableWidgetItem(texte);
                   QFont font;
                   font.setBold(true);
                   temp->setFont(font);
                   temp->setTextAlignment(Qt::AlignTop);

                   TacheUnaire* test = dynamic_cast<TacheUnaire*>(&((*it)->getEvenement()));
                   if (test) {
                       temp->setData(Qt::BackgroundRole, QVariant(test->getCouleur()));
                   } else {
                       temp->setData(Qt::BackgroundRole, QVariant(QColor(Qt::lightGray)));
                   }


                   int nbLignes;
                   int debutLigne;
                   if (date != (*it)->getDate().date()) {
                       nbLignes = ((*it)->getDateFin().time().minute() + ((*it)->getDateFin().time().hour()*60))/15;
                       debutLigne = 0;
                   } else {
                       nbLignes = (*it)->getDuree().getDureeEnMinutes()/15;
                       debutLigne = ((*it)->getDate().time().minute() + ((*it)->getDate().time().hour()*60))/15;
                   }

                   calendar->setItem(debutLigne, (date.dayOfWeek())-1, temp);
                   calendar->setSpan(debutLigne, (date.dayOfWeek())-1, nbLignes, 1);
                   programmations.push_back(temp);
               }
           }
        }
    }

}

void AgendaEditeur::editerProgrammation(QTableWidgetItem* button) {
    try{
        QDate date;
        switch (button->column()) {
            case 0:
                date=lundi;
                break;
            case 1:
                date=mardi;
                break;
            case 2:
                date=mercredi;
                break;
            case 3:
                date=jeudi;
                break;
            case 4:
                date=vendredi;
                break;
            case 5:
                date=samedi;
                break;
            case 6:
                date=dimanche;
                break;
        }

        QDateTime dateTime = QDateTime(date,QTime((button->row()*15)/60,((button->row()*15))%60));
        Programmation* pr = &(ProgrammationManager::getInstance().getProgrammation(dateTime));
        ProgrammationEditeur* pe = new ProgrammationEditeur(pr);
        pe->show();
        connect(pe, SIGNAL(fermeture()), this, SLOT(refresh()));

    }catch(CalendarException e)
    { QMessageBox::warning(this, "Ajout de programmation", e.getInfo()); }
}

void AgendaEditeur::refresh() {
    calendar->clearContents();
    calendar->clearSpans();
    setProgrammation();
}

void AgendaEditeur::setBouttonProgrammation() {
    ajoutProgrammation = new QPushButton("Ajouter une programmation",this);
    connect(ajoutProgrammation, SIGNAL(clicked(bool)), this, SLOT(ajouterProgrammation()));
    calendar_layout->addWidget(ajoutProgrammation);
}

void AgendaEditeur::ajouterProgrammation(){
    try{
       ProgrammationEditeur* pe = new ProgrammationEditeur();
       pe->show();
       connect(pe, SIGNAL(fermeture()), this, SLOT(refresh()));

   }catch(CalendarException e)
   { QMessageBox::warning(this, "Ajout de programmation", e.getInfo()); }
}

void AgendaEditeur::resizeEvent(QResizeEvent *event) {
    calendar->setColumnWidth(0, (this->width()-80)/7);
    calendar->setColumnWidth(1, (this->width()-80)/7);
    calendar->setColumnWidth(2, (this->width()-80)/7);
    calendar->setColumnWidth(3, (this->width()-80)/7);
    calendar->setColumnWidth(4, (this->width()-80)/7);
    calendar->setColumnWidth(5, (this->width()-80)/7);
    calendar->setColumnWidth(6, (this->width()-80)/7);
}
