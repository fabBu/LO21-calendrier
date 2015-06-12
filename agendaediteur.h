#ifndef AGENDAEDITEUR_H
#define AGENDAEDITEUR_H

#include <QObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateTime>
#include <QTableWidget>
#include "programmationmanager.h"
#include "programmationediteur.h"


class AgendaEditeur : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *main_layout, *calendar_layout;
    QHBoxLayout *semaine_layout;
    QPushButton *precedent, *suivant, *ajoutProgrammation;
    QLabel *semaine_label;
    QTableWidget *calendar;
    QList<QTableWidgetItem*> programmations;


    QDate dateJour;
    int jourSemaine, semaine;
    QDate lundi, mardi, mercredi, jeudi, vendredi, samedi, dimanche;
    QStringList liste_semaine;

    void setProgrammation();
    void setBouttonProgrammation();
    void setSemaine();
    void setSemaineList();
    void setHoraireList();
    void setCalendar();


public:
    AgendaEditeur();

signals:

public slots:
    void resizeEvent(QResizeEvent *event);
    void refresh();
private slots:
    void editerProgrammation(QTableWidgetItem* button);
    void ajouterProgrammation();
    void modifDate();
    void modifDateLabel();
    void modifDateLabelSuivant();
    void modifDateLabelPrecedent();
};

#endif // AGENDAEDITEUR_H
