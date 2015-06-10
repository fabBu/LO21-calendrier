#ifndef PROJETEDITEUR_H
#define PROJETEDITEUR_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QStringListModel>
#include <QStandardItem>
#include "projetsmanager.h"
#include "proprieteprojetediteur.h"
#include "tacheediteur.h"
#include "programmationediteur.h"

class ProjetEditeur : public QWidget {
private:
    Q_OBJECT

    QGroupBox *grp_proprietes, *grp_modification,*grp_recherche, *grp_nouvelle;
    QPushButton *modifier_projet, *supprimer_projet, *modifier_tache, *supprimer_tache,*a_programmer, *toutes_taches, *ajouter_unaire, *ajouter_composite, *programmer;
    QCheckBox *termine;
    QTreeWidget *taches;
    QGridLayout *main_layout;
    QVBoxLayout *l_existante, *l_recherche, *l_nouvelle;
    QHBoxLayout *l_proprietes;
    QLabel *debut_label, *debut_date, *fin_label, *fin_date;

    QWidget* parent; /**< QWidget parent de l'éditeur */
    TacheManager& tm; /**< Référence sur le projet en cours d'édition */
    QString tache_courante; /**< Titre de la tâche courante dans l'arbre des tâches du projet */


    QGroupBox* initProprietes(); /**< Initialise la vue des propriétés du projet */

    /*!
     * \brief Chargement de l'arbre des tâches du projet
     *
     * Remplit le QTreeWidget \a taches avec l'ensemble des tâches du projet.
     * A chacune des tâches sont rattachés l'ensemble de ses prédécesseurs et le début de sa description.
     */
    void chargerTaches(list<Tache *> l);
    /*!
     * \brief Permet l'affichage des sous-tâches dans des sous-niveaux
     *
     * Est utilisée par chargerTaches()
     *
     * \param item Item de l'arbre dans lequel ajouter des sous-éléments
     * \param l Liste des tâches du projet. Permet d'éviter les doublons dans l'arbre
     * \param tc Tâche pour laquelle charger les sous-tâches
     */
    void chargerSousTaches(QTreeWidgetItem* item, list<Tache *> *l, TacheComposite* tc );
public:
    /*!
     * \brief Editeur de projet premettant la gestion d'un projet ainsi que ses tâches
     *
     *  ProjetEditeur permet l'affichage et la gestion d'un projet et de ses tâches.
     * Une ligne est dédiée aux informations du projet (nom et intervalle de dates) avec possibilité de modifier ou supprimer le projet.
     * Vue en arbre via QTreeWidget pour la visualisation de l'ensemble des tâches et boutons d'ajout/modification/suppression d'une tâche.
     *
     * \param tm1 projet à éditer
     * \param p QWidget parent
     */
    ProjetEditeur(TacheManager& tm1, QWidget* p=0);
    /*!
     * \brief Getter nom du projet
     * \return nom du projet
     */
    const QString& getNom() const { return tm.getNom(); }

private slots:
    /*!
     * \brief Permet la modification des propriétés du projet
     *
     * Ouvre un ProprieteProjetEditeur avec le TacheManager courant.
     */
    void modifierProjet();
    /*!
     * \brief Permet la suppression du projet
     */
    void supprimerProjet();
    /*!
     * \brief Récupère la tâche courante selon le clic sur le QTreeWidget
     *
     * Permet de récupérer le titre de la tâche choisie dans le QTreeWidget afin de modifier l'interface en conséquence.
     *
     * \param item Element cliqué sur le QTreeWidget
     */
    void getTacheCourante(QTreeWidgetItem *item);
    /*!
     * \brief Demande d'ajout d'une nouvelle tâche dans le projet
     *
     * Ouvre un TacheEditeur vide afin de créer une tâche pour le projet.
     */
    void ajouterTache();
    /*!
     * \brief Demande de modification d'une tâche du projet
     *
     * Ouvre un TacheEditeur avec la tâche courante afin de modifier ses propriétés.
     */
    void modifierTache();
    /*!
     * \brief Suppression de la tâche courante
     */
    void supprimerTache();
    /*!
     * \brief Demande de programmation de la tâche courante
     *
     * Ouvre un ProgrammationEditeur avec la tâche courante afin de la programmer dans l'agenda.
     */
    void programmerTache();
    /*!
     * \brief Refresh de la vue des tâches
     *
     * Appelle chargerTaches() avec toutes les tâches
     */
    void refresh_taches();
    /*!
     * \brief L'arbre des tâches n'affiche maintenant que les tâches à programmer
     *
     * Appelle chargerTaches() avec les tâches qui ne sont pas encore programmées
     */
    void taches_a_programmer();
    /*!
     * \brief Refresh des propriétés du projet
     *
     * Utilise initProprietes()
     */
    void refresh_projet();

signals:
    /*!
     * \brief Signal de fermeture de l'éditeur
     * \param nom Nom du projet
     */
    void fermeture(const QString nom);
};


#endif // PROJETEDITEUR_H
