#ifndef TACHEEDITEUR_H
#define TACHEEDITEUR_H

#include<QTextEdit>
#include<QLineEdit>
#include<QApplication>
#include<QPushButton>
#include<QLabel>
#include<QCheckBox>
#include<QDateEdit>
#include<QSpinBox>
#include<QHBoxLayout>
#include <QComboBox>
#include <QMessageBox>


#include "taches.h"
#include "tachemanager.h"
#include "programmationmanager.h"


class TacheEditeur : public QWidget {
private:
    Q_OBJECT
    QLabel *titre_label, *desc_label, *dispo_label, *duree_label, *echeance_label, *pred_label, *soust_label;
    QLineEdit *titre;
    QTextEdit *desc;
    QCheckBox *preemp;
    QDateEdit *dispo, *echeance;
    QSpinBox *duree_h, *duree_m;
    QPushButton *btn_cancel, *btn_save, *btn_ajouterpred, *btn_retirerpred, *btn_ajoutersoust, *btn_retirersoust;
    QComboBox *pred_list, *soust_list, *nonpred_list, *nonsoust_list;

    QHBoxLayout *l_titre, *l_desc, *l_dates, *l_pred, *l_soust, *l_cancelsave;
    QVBoxLayout *main_layout;

    QWidget* parent;
    TacheManager& tm; /*!< Référence sur le projet auquel appartient la tâche à éditer */
    Tache* t; /*!< Pointeur sur la tâche à éditer. Vaut 0 dans le cas d'une nouvelle tâche */
    bool unaire; /*!< Permet de déterminer si la tâche à éditer/créer est unaire ou non */

    /*!
     * \brief Initialisation de la ligne de titre
     *
     * Initialise \a l_titre avec un QLineEdit et un QCheckBox si \a unaire est à true
     *
     * \param unaire Indicateur tâche unaire ou non
     */
    void initTitre(bool unaire=true);
    void initDesc(); /*!< Initialisation de la ligne de description de la tâche */
    /*!
     * \brief Initialisation de la partie dates de la fenêtre
     *
     * Insère des champs pour le choix des dates de dispo/échéance de la tâche.
     * Le choix possible est limité par son dernier prédécesseur et son premier successeur, si la tâche en possède.
     * Sinon limité par les dates du projets.
     * Si la tâche est unaire, Une ligne pour définir la durée est ajoutée.
     *
     * \param unaire Indicateur tâche unaire ou non
     */
    void initDates(bool unaire=true);
    void initCancelSave(); /*!< Initialisation des boutons Sauvegarder et Annuler */
    /*!
     * \brief Initialisation de la ligne de choix des prédécesseurs
     *
     * Crée une liste composée des prédécesseurs de la tâche puis une liste composée des autres tâches du projet.
     * Deux boutons permettent d'ajouter ou retirer une tâche comme étant un prédécesseur.
     *
     * Ces listes s'affichent ou se cachent dynamiquement selon si elles possèdent au moins un élément ou non.
     */
    void initPrecedence();
    /*!
     * \brief Initialisation de la ligne de choix des sous-tâches, seulement si la tâche est composite
     *
     * Crée une liste composée des sous-tâches de la tâche puis une liste composée des autres tâches du projet.
     * Deux boutons permettent d'ajouter ou retirer une tâche comme étant une sous-tâche.
     *
     * Ces listes s'affichent ou se cachent dynamiquement selon si elles possèdent au moins un élément ou non.
     */
    void initSousTaches();
public:
    TacheEditeur(TacheManager& tm, Tache* t1, QWidget* p=0);
    TacheEditeur(TacheManager &tm, bool unaire=true, QWidget* p=0);
    ~TacheEditeur();

private slots:
    void retirerPredecesseur(); /*!< Slot appelé lorsque l'on souhaite retirer une tâche depuis la liste des prédécesseurs */
    void ajouterPredecesseur(); /*!< Slot appelé lorsque l'on souhaite ajouter un prédécesseur depuis la liste des tâches */
    void modifListePred(); /*!< Slot appelé lorsqu'une des liste de précédence est modifiée. Affichage dynamique des liste */

    void retirerSousTache(); /*!< Slot appelé lorsque l'on souhaite retirer une tâche depuis la liste des sous-tâches */
    void ajouterSousTache(); /*!< Slot appelé lorsque l'on souhaite ajouter une sous-tâche depuis la liste des tâches */
    void modifierListeSoust(); /*!< Slot appelé lorsqu'une des liste de sous-tâches est modifiée. Affichage dynamique des liste */

    void sauvegarder(); /*!< Permet de sauvegarder les modification ou créer la tâche, selon si \a t vaut 0 ou non */

signals:
    void fermeture(); /*!< Signal emit avant la fermeture de la fenêtre */
};


#endif // TACHEEDITEUR_H
