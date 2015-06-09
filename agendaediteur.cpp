#include "agendaediteur.h"
#include <QDebug>

AgendaEditeur::AgendaEditeur(QWidget *parent) : QWidget(parent), manager(ProgrammationManager::getInstance())
{
    setWindowTitle("Programmation d'un événement");
    setFixedSize(1334,800);

    QShortcut* shortcut = new QShortcut(QKeySequence(QKeySequence::Open),this,SLOT(ajouterProgrammation()));

    semaine_layout = new QGridLayout;

    QScrollArea *scroll = new QScrollArea();
    QWidget *container = new QWidget(scroll);
    container->setMinimumSize(1290,1500);

    horaire_layout = new QGridLayout();
    container->setLayout(horaire_layout);
    scroll->setWidget(container);
    semaine_layout->addWidget(scroll,3,0,1,16);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setHoraireLabel();
    setSizeColumn();
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

    setDateLabel();
    setBouttonSemaine();
    setBouttonProgrammation();
    setLinesSemaine();
    setLinesHoraire();
    setProgrammation();

    setLayout(semaine_layout);
}

void AgendaEditeur::modifDateLabel(){
    lundi_label->setText(lundi.toString("dddd d/M"));
    mardi_label->setText(mardi.toString("dddd d/M"));
    mercredi_label->setText(mercredi.toString("dddd d/M"));
    jeudi_label->setText(jeudi.toString("dddd d/M"));
    vendredi_label->setText(vendredi.toString("dddd d/M"));
    samedi_label->setText(samedi.toString("dddd d/M"));
    dimanche_label->setText(dimanche.toString("dddd d/M"));

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
    refresh();
}

void AgendaEditeur::modifDateLabelPrecedent(){
    semaine--;
    modifDate();
    modifDateLabel();
    refresh();
}

void AgendaEditeur::setLinesSemaine(){
    verticalLinesSemaine = new QFrame*[7];

    for (int i=0;i<7;i++){
        verticalLinesSemaine[i] = new QFrame(this);
        verticalLinesSemaine[i]->setLineWidth(3);
        verticalLinesSemaine[i]->setFixedWidth(3);
//        verticalLinesSemaine[i]->setFrameShape(QFrame::VLine);
        semaine_layout->addWidget(verticalLinesSemaine[i],1,(2*i)+1,1,1);
    }

    horizontalLinesSemaine = new QFrame*[1];

    for (int i=0;i<1;i++){
        horizontalLinesSemaine[i] = new QFrame(this);
        horizontalLinesSemaine[i]->setLineWidth(2);
//        horizontalLinesSemaine[i]->setFrameShape(QFrame::HLine);
//        semaine_layout->addWidget(horizontalLinesSemaine[i],2,0,1,15);
    }

}

void AgendaEditeur::setLinesHoraire(){
    verticalLinesHoraire = new QFrame*[7];

    for (int i=0;i<7;i++){
        verticalLinesHoraire[i] = new QFrame(this);
        verticalLinesHoraire[i]->setLineWidth(3);
        verticalLinesHoraire[i]->setFixedWidth(3);
        verticalLinesHoraire[i]->setFrameShape(QFrame::VLine);
        horaire_layout->addWidget(verticalLinesHoraire[i],0,(2*i)+1,120,1);
    }

    horizontalLinesHoraire = new QFrame*[23];

    for (int i=1;i<24;i++){
        horizontalLinesHoraire[i] = new QFrame(this);
        horizontalLinesHoraire[i]->setLineWidth(1);
        horizontalLinesHoraire[i]->setFrameShape(QFrame::HLine);
        horaire_layout->addWidget(horizontalLinesHoraire[i],(5*i)-1,0,1,15);
    }

}

void AgendaEditeur::setDateLabel() {
    lundi_label = new QLabel(lundi.toString("dddd d/M"),this);
    mardi_label = new QLabel(mardi.toString("dddd d/M"),this);
    mercredi_label = new QLabel(mercredi.toString("dddd d/M"),this);
    jeudi_label = new QLabel(jeudi.toString("dddd d/M"),this);
    vendredi_label = new QLabel(vendredi.toString("dddd d/M"),this);
    samedi_label = new QLabel(samedi.toString("dddd d/M"),this);
    dimanche_label = new QLabel(dimanche.toString("dddd d/M"),this);

    QLabel *tmp = new QLabel();
    tmp->setFixedWidth(50);
    semaine_layout->addWidget(tmp,1,0);
    QLabel *tmp2 = new QLabel();
    tmp2->setFixedWidth(16);
    semaine_layout->addWidget(tmp2,1,15);

    int width_date_label = 150;
    int heigh_date_label = 30;
    lundi_label->setFixedWidth(width_date_label);
    lundi_label->setFixedHeight(heigh_date_label);
    lundi_label->setAlignment(Qt::AlignCenter);
    mardi_label->setFixedWidth(width_date_label);
    mardi_label->setAlignment(Qt::AlignCenter);
    mercredi_label->setFixedWidth(width_date_label);
    mercredi_label->setAlignment(Qt::AlignCenter);
    jeudi_label->setFixedWidth(width_date_label);
    jeudi_label->setAlignment(Qt::AlignCenter);
    vendredi_label->setFixedWidth(width_date_label);
    vendredi_label->setAlignment(Qt::AlignCenter);
    samedi_label->setFixedWidth(width_date_label);
    samedi_label->setAlignment(Qt::AlignCenter);
    dimanche_label->setFixedWidth(width_date_label);
    dimanche_label->setAlignment(Qt::AlignCenter);

    semaine_layout->addWidget(lundi_label,1,2);
    semaine_layout->addWidget(mardi_label,1,4);
    semaine_layout->addWidget(mercredi_label,1,6);
    semaine_layout->addWidget(jeudi_label,1,8);
    semaine_layout->addWidget(vendredi_label,1,10);
    semaine_layout->addWidget(samedi_label,1,12);
    semaine_layout->addWidget(dimanche_label,1,14);

    if (lundi.year() != dimanche.year()) {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM yyyy - ")+dimanche.toString("d MMM yyyy"));
    } else {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM - ")+dimanche.toString("d MMM yyyy"));
    }
    semaine_label->setAlignment(Qt::AlignCenter);
    semaine_layout->addWidget(semaine_label,0,6,1,4);
}

void AgendaEditeur::setHoraireLabel(){
    horaires = new QLabel*[24];

    for (int i=0;i<24;i++){
        QString tmp = QString("%1h00").arg(i);
        horaires[i] = new QLabel(tmp,this);
        horaires[i]->setFixedWidth(40);
        horaires[i]->setFixedHeight(15);
        horaire_layout->addWidget(horaires[i],5*i,0);
    }
}

void AgendaEditeur::setSizeColumn(){
    QLabel** fixeSizeColumn = new QLabel*[7];

    for (int i=1;i<8;i++){
        fixeSizeColumn[i] = new QLabel("",this);
        fixeSizeColumn[i]->setFixedWidth(176);
        horaire_layout->addWidget(fixeSizeColumn[i],0,2*i);
    }

    QLabel** fixeSizeLine = new QLabel*[72];

    for (int i=0;i<24;i++){
        for (int c=0;c<3;c++){
            fixeSizeLine[i] = new QLabel("",this);
            fixeSizeLine[i]->setFixedHeight(15);
            horaire_layout->addWidget(fixeSizeLine[i],(5*i)+1+c,2);
        }
    }
}

void AgendaEditeur::setBouttonSemaine(){
    precedent = new QPushButton("<",this);
    suivant = new QPushButton(">",this);
    connect(precedent, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelPrecedent()));
    connect(suivant, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelSuivant()) );
    semaine_layout->addWidget(precedent,0,4);
    semaine_layout->addWidget(suivant,0,10);
}

void AgendaEditeur::setBouttonProgrammation(){
    ajoutProgrammation = new QPushButton("Ajouter une programmation",this);
    connect(ajoutProgrammation, SIGNAL(clicked(bool)), this, SLOT(ajouterProgrammation()));
    semaine_layout->addWidget(ajoutProgrammation, 4,0,1,15);
}

void AgendaEditeur::setProgrammation(){
    std::list<Programmation*> listes = manager.getProgrammation(lundi,dimanche);
    programmations =  QList<QPushButton*>();
    int i=0;
    for(std::list<Programmation*>::const_iterator it = listes.begin(); it != listes.end(); ++it) {
        int nbJours = (*it)->getDateFin().date().dayOfWeek() - (*it)->getDate().date().dayOfWeek();
        if (((*it)->getDateFin().date().dayOfWeek() == 7 || (*it)->getDateFin().date().dayOfWeek() == 1) && nbJours != 0){
            nbJours = 1;
        }

//        qDebug() << "nbJour :" << nbJours;
        if (nbJours == 0) {
            QPushButton* temp = new QPushButton((*it)->getEvenement().getTitre());
            temp->setStyleSheet("text-align: top left");

            int nbLignesHeure = (((*it)->getDateFin().time().minute() + ((*it)->getDateFin().time().hour()*60)) -
                    ((*it)->getDate().time().minute() + ((*it)->getDate().time().hour()*60)))/15;
//            qDebug() << nbLignesHeure;
            int nbLignesVerticales = 0;
            if (nbLignesHeure > 4) {
                nbLignesVerticales = nbLignesHeure/4;
            }
//            qDebug() << nbLignesVerticales;
            temp->setMinimumHeight(nbLignesHeure*15+nbLignesVerticales);

            int nb = ((*it)->getDate().time().minute() + ((*it)->getDate().time().hour()*60))/15;
//            qDebug() << nb;
            int nb2 = 0;
            if (nb > 4) {
                nb2 = nb/4;
            }
//            qDebug() << nb2;
            horaire_layout->addWidget(temp,nb+nb2,(*it)->getDate().date().dayOfWeek()*2,nbLignesHeure+nbLignesVerticales,1);
            int row, col, index, col_span, row_span;
            index = horaire_layout->indexOf(temp);
            horaire_layout->getItemPosition(index, &row, &col, &col_span, &row_span);
            qDebug() << "index:"<<index<<",col:"<<col<<",row:"<<row;
            programmations.push_back(temp);
            connect(temp, SIGNAL(clicked(bool)), this, SLOT(editerProgrammation()) );
        } else {

            for (int i=0; i<nbJours+1;i++){
                QDate t = (*it)->getDate().date().addDays(i);
//                qDebug() << "yolo" << t.toString("yyyy.MM.dd");
                if (!(t<lundi || t>dimanche)){
                    QPushButton* temp = new QPushButton((*it)->getEvenement().getTitre());
                    temp->setStyleSheet("text-align: top left");
                    if (i==0){
                        QDateTime ra = QDateTime((*it)->getDate());
                        int nbLignesHeure = (ra.time().hour()*60 + ra.time().minute())/15;

//                        qDebug() << "yolo" << nbLignesHeure;
                        int nbLignesVerticales = 0;
                        if (nbLignesHeure > 4) {
                            nbLignesVerticales = nbLignesHeure/4;
                        }
//                        qDebug() << "yolo" << nbLignesVerticales;
                        temp->setMinimumHeight(nbLignesHeure*15+nbLignesVerticales);

                        int nb = ((24*60)-(ra.time().hour()*60 + ra.time().minute()))/15;
//                        qDebug() << "yo" << nb;
                        int nb2 = 0;
                        if (nb > 4) {
                            nb2 = nb/4;
                        }
//                        qDebug() << nb2;
                        horaire_layout->addWidget(temp,nb+nb2,(*it)->getDate().date().dayOfWeek()*2,nbLignesHeure+nbLignesVerticales,1);
                        int row, col, index, col_span, row_span;
                        index = horaire_layout->indexOf(temp);
                        horaire_layout->getItemPosition(index, &row, &col, &col_span, &row_span);
                        qDebug() << "index:"<<index<<",col:"<<col<<",row:"<<row;
                    } else if (i==1){
                        QDateTime ra = QDateTime((*it)->getDateFin());

                        int nbLignesHeure = (ra.time().hour()*60 + ra.time().minute())/15;
//                        qDebug() << nbLignesHeure;
                        int nbLignesVerticales = 0;
                        if (nbLignesHeure > 4) {
                            nbLignesVerticales = nbLignesHeure/4;
                        }
//                        qDebug() << nbLignesVerticales;
                        temp->setMinimumHeight(nbLignesHeure*15+nbLignesVerticales);

                        horaire_layout->addWidget(temp,0,(*it)->getDateFin().date().dayOfWeek()*2,nbLignesHeure+nbLignesVerticales,1);
                        int row, col, index, col_span, row_span;
                        index = horaire_layout->indexOf(temp);
                        horaire_layout->getItemPosition(index, &row, &col, &col_span, &row_span);
//                        qDebug() << "index:"<<index<<",col:"<<col<<",row:"<<row;

                    }
                    programmations.push_back(temp);
                    connect(temp, SIGNAL(clicked(bool)), this, SLOT(editerProgrammation()) );
                }
            }
        }
        i++;
    }

}

void AgendaEditeur::refresh(){
    qDeleteAll(programmations);
//    qDebug() << "yolo";
    setProgrammation();
}

void AgendaEditeur::ajouterProgrammation() {
    try{
        ProgrammationEditeur* pe = new ProgrammationEditeur();
        pe->show();
        connect(pe, SIGNAL(fermeture()), this, SLOT(refresh()));

    }catch(CalendarException e)
    { QMessageBox::warning(this, "Ajout de programmation", e.getInfo()); }
}

void AgendaEditeur::editerProgrammation() {
    try{
        QPushButton* button = dynamic_cast<QPushButton*>(QObject::sender());
        int row, col, index, col_span, row_span;
        index = horaire_layout->indexOf(button);
        horaire_layout->getItemPosition(index, &row, &col, &col_span, &row_span);
//        qDebug() << "yololololo";
//        qDebug() << "index:"<<index<<",col:"<<col<<",row:"<<row;

        QDate date;
        switch (col) {
            case 2:
                date=lundi;
                break;
            case 4:
                date=mardi;
                break;
            case 6:
                date=mercredi;
                break;
            case 8:
                date=jeudi;
                break;
            case 10:
                date=vendredi;
                break;
            case 12:
                date=samedi;
                break;
            case 14:
                date=dimanche;
                break;
        }

        int nbLignes = 0;
        if (row > 4) {
            nbLignes = row/5;
        }

        QDateTime dateTime = QDateTime(date,QTime(((row-nbLignes)*15)/60,((row-nbLignes)*15)%60));
        qDebug() << "yolo date:" << dateTime.date().toString("dd.MM.yyyy") << " , yolo time:" <<dateTime.time().toString("hh:mm");
        Programmation* pr = &(manager.getProgrammation(dateTime));
        ProgrammationEditeur* pe = new ProgrammationEditeur(pr);
        pe->show();
        connect(pe, SIGNAL(fermeture()), this, SLOT(refresh()));

    }catch(CalendarException e)
    { QMessageBox::warning(this, "Ajout de programmation", e.getInfo()); }
}
