#ifndef ACTIVITEEDITEUR_H
#define ACTIVITEEDITEUR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include "activite.h"
#include "programmation.h"
#include "programmationmanager.h"

class ProgrammationEditeur : public QWidget
{
private:
//    Q_ENUMS(MetaEnum::Type)

    Q_OBJECT
    QLabel *titre_label, *desc_label, *type_label, *lieu_label, *horaire_label, *duree_label;
    QLineEdit *titre, *lieu;
    QTextEdit *desc;
    QComboBox *type;
    QCalendarWidget *calendar;
    QTimeEdit *horaire, *duree;
    QPushButton *btn_cancel, *btn_save;

    QHBoxLayout *l_main, *l_titre, *l_desc, *l_type, *l_lieu, *l_horaires, *l_cancelsave, *attributs_layout, *l_calendar;
    QVBoxLayout *calendar_layout, *main_layout, *param_layout;

    QWidget* parent;
    ProgrammationManager& programmationmanager;
    /*Activite& activite;
    Programmation& programmation;*/

    void initTitre();
    void initDesc();
    void initLieu();
    void initType();
    void initCancelSave();
    void initCalendar();
    void initHoraire();

public:
    ProgrammationEditeur(ProgrammationManager& pm, QWidget *p = 0);
//    ActiviteEditeur(ProgrammationManager& pm, Programmation& pr, QWidget *p = 0);
    ~ProgrammationEditeur();

signals:
//    void fermeture();
private slots:
    void sauvegarder();
};

#endif // ACTIVITEEDITEUR_H
