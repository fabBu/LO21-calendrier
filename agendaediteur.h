#ifndef AGENDAEDITEUR_H
#define AGENDAEDITEUR_H

#include <QObject>
#include <QShortcut>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDate>
#include <QPushButton>
#include <QScrollArea>
#include <QList>
#include "programmationediteur.h"
#include "programmationmanager.h"

class AgendaEditeur : public QWidget
{
    Q_OBJECT
private:
    ProgrammationManager& manager;
    int jourSemaine, semaine;
    QGridLayout *semaine_layout, *horaire_layout;
    QDate dateJour;
    QFrame **verticalLinesSemaine, **horizontalLinesSemaine, **verticalLinesHoraire, **horizontalLinesHoraire;
    QLabel *lundi_label, *mardi_label, *mercredi_label, *jeudi_label, *vendredi_label, *samedi_label, *dimanche_label, *semaine_label, **horaires;
    QDate lundi, mardi, mercredi, jeudi, vendredi, samedi, dimanche;
    QPushButton *precedent, *suivant, *ajoutProgrammation;
    QList<QPushButton*> programmations;

    void setProgrammation();
    void setBouttonSemaine();
    void setBouttonProgrammation();
    void setLinesSemaine();
    void setSizeColumn();
    void setLinesHoraire();
    void setDateLabel();
    void setHoraireLabel();
public:
    AgendaEditeur(QWidget *parent = 0);

signals:

private slots:
    void refresh();
    void editerProgrammation();
    void ajouterProgrammation();
    void modifDate();
    void modifDateLabel();
    void modifDateLabelSuivant();
    void modifDateLabelPrecedent();
};

#endif // AGENDAEDITEUR_H
