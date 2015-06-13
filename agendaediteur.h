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

/*!
 * \class AgendaEditeur
 * \brief Classe de création d'une vue agenda
 */
class AgendaEditeur : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *main_layout, *calendar_layout;
    QHBoxLayout *semaine_layout;
    QPushButton *precedent, *suivant, *ajoutProgrammation;
    QLabel *semaine_label;
    QTableWidget *calendar; /*!< Contient le calendrier*/
    QList<QTableWidgetItem*> programmations; /*!< Contient toutes les programmations*/

    QDate dateJour; /*!< Contient la date du jout*/
    int jourSemaine, semaine;
    QDate lundi, mardi, mercredi, jeudi, vendredi, samedi, dimanche;
    QStringList liste_semaine; /*!< Contient l'ensemble des semaines pour l'en-tête des colonnes de l'agenda*/

    /*!
     * \brief Méthode d'initialisation des bouttons de programmation
     */
    void setProgrammation();
    /*!
     * \brief Méthode d'initialisation du boutton ajouterProgrammation
     */
    void setBouttonProgrammation();
    /*!
     * \brief Méthode d'initialisation du bandeau supérieur de l'agenda concernant les semaines
     */
    void setSemaine();
    /*!
     * \brief Méthode d'initialisation de l'en-tête des colonnes du tableau d'agenda
     */
    void setSemaineList();
    /*!
     * \brief Méthode d'initialisation des lignes horaire de l'agenda
     */
    void setHoraireList();
    /*!
     * \brief Méthode d'initialisation du calendrier
     */
    void setCalendar();

public:
    /*!
     * \brief Constructeur de la classe AgendaEditeur
     */
    AgendaEditeur();

signals:

public slots:
    void resizeEvent(QResizeEvent *event); /*!< Slot appelé lorsque l'on redimenssionne la fenêtre */
    void refresh(); /*!< Slot appelé lorsque l'on veut rafraîchir le tableau agenda */
private slots:
    void editerProgrammation(QTableWidgetItem* button); /*!< Slot appelé lorsque l'on veut éditer une programmation à partir de l'agenda */
    void ajouterProgrammation(); /*!< Slot appelé lorsque l'on veut ajout une programmation à partir de l'agenda */
    void modifDate(); /*!< Slot appelé lorsque l'on change de semaine, les dates en attribut sont modifiées */
    void modifDateLabel(); /*!< Slot appelé lorsque l'on change de semaine, l'en-tête des colonnes du tableau d'agenda */
    void modifDateLabelSuivant(); /*!< Slot appelé lorsque l'on appuie sur le bouton semaine suivante */
    void modifDateLabelPrecedent(); /*!< Slot appelé lorsque l'on appuie sur le bouton semaine précédente */
};

#endif // AGENDAEDITEUR_H
