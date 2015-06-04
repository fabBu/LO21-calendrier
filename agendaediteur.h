#ifndef AGENDAEDITEUR_H
#define AGENDAEDITEUR_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDate>
#include <QPushButton>
#include "programmationmanager.h"

class AgendaEditeur : public QWidget
{
    Q_OBJECT
private:
//    ProgrammationManager manager;
    int jourSemaine, semaine;
    QGridLayout *layout;
    QDate dateJour;
    QFrame **verticalLines, **horizontalLines;
    QLabel *lundi_label, *mardi_label, *mercredi_label, *jeudi_label, *vendredi_label, *samedi_label, *dimanche_label, *semaine_label;
    QDate lundi, mardi, mercredi, jeudi, vendredi, samedi, dimanche;
    QPushButton *precedent, *suivant;
    void setBouttonSemaine();
    void setLines();
    void setDateLabel();
public:
    AgendaEditeur(QWidget *parent = 0);

signals:

private slots:
    void modifDate();
    void modifDateLabel();
    void modifDateLabelSuivant();
    void modifDateLabelPrecedent();
};

#endif // AGENDAEDITEUR_H
