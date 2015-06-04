#include "agendaediteur.h"
#include <QDebug>

AgendaEditeur::AgendaEditeur(QWidget *parent) : QWidget(parent)
{
//    manager = ProgrammationManager::getInstance();
    layout = new QGridLayout;

    QLabel* clock = new QLabel("0h00",this);
    layout->addWidget(clock,3,0);

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

    setLines();


    setLayout(layout);
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
}

void AgendaEditeur::modifDateLabelPrecedent(){
    semaine--;
    modifDate();
    modifDateLabel();
}

void AgendaEditeur::setLines(){
    verticalLines = new QFrame*[7];

    for (int i=0;i<7;i++){
        verticalLines[i] = new QFrame(this);
        verticalLines[i]->setLineWidth(2);
        verticalLines[i]->setFrameShape(QFrame::VLine);
        layout->addWidget(verticalLines[i],1,(2*i)+1,10,1);
    }

    horizontalLines = new QFrame*[1];

    for (int i=0;i<1;i++){
        horizontalLines[i] = new QFrame(this);
        horizontalLines[i]->setLineWidth(2);
        horizontalLines[i]->setFrameShape(QFrame::HLine);
        layout->addWidget(horizontalLines[i],2,0,1,15);
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
    int width_date_label = 180;
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

    layout->addWidget(lundi_label,1,2);
    layout->addWidget(mardi_label,1,4);
    layout->addWidget(mercredi_label,1,6);
    layout->addWidget(jeudi_label,1,8);
    layout->addWidget(vendredi_label,1,10);
    layout->addWidget(samedi_label,1,12);
    layout->addWidget(dimanche_label,1,14);

    if (lundi.year() != dimanche.year()) {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM yyyy - ")+dimanche.toString("d MMM yyyy"));
    } else {
        semaine_label = new QLabel(lundi.toString("Semaine: d MMM - ")+dimanche.toString("d MMM yyyy"));
    }
    semaine_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(semaine_label,0,6,1,4);
}

void AgendaEditeur::setBouttonSemaine(){
    precedent = new QPushButton("<",this);
    suivant = new QPushButton(">",this);
    connect(precedent, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelPrecedent()));
    connect(suivant, SIGNAL(clicked(bool)), this, SLOT(modifDateLabelSuivant()) );
    layout->addWidget(precedent,0,4);
    layout->addWidget(suivant,0,10);
}
