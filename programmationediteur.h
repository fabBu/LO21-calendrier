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
#include <QDateEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QListWidgetItem>
#include "activite.h"
#include "calendar.h"
#include "evenement.h"
#include "programmation.h"
#include "programmationmanager.h"
#include "taches.h"

/*!
 * \class ProgrammationEditeur
 * \brief Classe représentant une vue d'édition de programmation
 */
class ProgrammationEditeur : public QWidget
{
private:
    Q_OBJECT
    QLabel *titre_label, *desc_label, *type_label, *lieu_label, *horaire_label, *duree_label, *disponibilite_label, *echeance_label, *dureeTotal_label, *dureeRestante_label;
    QLineEdit *titre, *lieu;
    QTextEdit *desc;
    QComboBox *type;
    QCheckBox *termine;
    QDateEdit *disponibilite, *echeance;
    QCalendarWidget *calendar;
    QSpinBox *dureeTotal_h, *dureeTotal_m, *dureeRestante_h, *dureeRestante_m, *duree_h, *duree_m;
    QTimeEdit *horaire;
    QPushButton *btn_cancel, *btn_save, *btn_supprimer;
    QGroupBox *groupBoxEvenement, *groupBoxProgrammation;
    QListWidget *listesProgrammation;

    QHBoxLayout *l_main, *l_titre, *l_desc, *l_type, *l_lieu, *l_horaires, *l_cancelsave, *attributs_layout, *l_calendar, *l_dates, *l_dureeTotale, *l_dureeRestante, *programmations;
    QVBoxLayout *calendar_layout, *main_layout, *param_layout, *evenement_layout, *programmation_layout;

    QWidget* parent;
    ProgrammationManager& programmationmanager;
    Programmation* programmation;
    Activite* activite;
    TacheUnaire* tache;

    /*!
     * \brief Initialisation du titre de l'événement
     */
    void initTitre();
    /*!
     * \brief Initialisation du titre de l'événement
     */
    void initDates();
    /*!
     * \brief Initialisation de la description de l'événement
     */
    void initDesc();
    /*!
     * \brief Initialisation du lieu de l'événement
     */
    void initLieu();
    /*!
     * \brief Initialisation du type d'événement
     */
    void initType();
    /*!
     * \brief Initialisation des bouttons d'annulation, de suppression et de sauvegarde
     */
    void initCancelSave();
    /*!
     * \brief Initialisation de la partie durée totale d'une tache
     */
    void initDureeTotale();
    /*!
     * \brief Initialisation la partie durée restante d'une programmation
     */
    void initDureeRestante();
    /*!
     * \brief Initialisation de la liste des programmations pour une tache préemptive
     */
    void initListeProgrammation();
    /*!
     * \brief Initialisation de la partie calendrier pour choisir la date de la programmation
     */
    void initCalendar(Programmation* pr = 0);
    /*!
     * \brief Initialisation de la partie programmation pour choisir l'horaire et la durée de la programmation
     */
    void initProgrammation(Programmation* pr = 0);

public:
    /*!
     * \brief Constructeur de la classe ProgrammationEditeur pour l'édition d'une programmation
     * \param pr : programmation
     * \param p : widget parent
     */
    ProgrammationEditeur(Programmation* pr, QWidget *p = 0);
    /*!
     * \brief Constructeur de la classe ProgrammationEditeur pour l'ajout d'une programmation
     * \param pr : programmation
     * \param p : widget parent
     */
    ProgrammationEditeur(Evenement* ev = 0, QWidget *p = 0);
    /*!
     * \brief Destructeur de la classe ProgrammationEditeur
     */
    ~ProgrammationEditeur();

signals:
    void fermeture(); /*!< Signal de fermeture*/
private slots:
    void sauvegarder(); /*!< Slot appelé pour sauvegarder l'ajout d'une programmation */
    void supprimer(); /*!< Slot appelé pour supprimer une programmation*/
    void modifier(); /*!< Slot appelé pour sauvegarder la modification d'une programmation */
    void modifDureeRestante(); /*!< Slot appelé lorsque l'on modifie la durée de la tache */
};

#endif // ACTIVITEEDITEUR_H
