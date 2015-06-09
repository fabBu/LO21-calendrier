#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QInputDialog>
#include <QWidget>
#include <QTabWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>
#include <QScrollArea>
#include "projetediteur.h"
#include "programmationmanager.h"
#include "agendaediteur.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTabWidget *onglets; /**< Contient tous les onglets ouvert */

    QMenuBar *menubar;
    QMenu *menu_fichier, *menu_projets;
    QAction *sauvegarder, *editerProjet;

    ProjetsManager& projets; /**< Référence sur le manager de projets */
    ProgrammationManager& agenda; /**< Référence sur l'agenda */

    std::list<ProjetEditeur*> projets_ouverts; /**< Liste des projets dont un onglet est déjà ouvert */
    AgendaEditeur* agenda_ouvert; /**< Pointeur vers l'Agenda si celui-ci est ouvert, 0 sinon */

    void initMenuBar(); /**< Initialise la barre de menu de la fenêtre */
    /*!
     * \brief Chargement des projets depuis fichiers d'exportation
     * \param dossier Dossier dans lequel se trouvent les fichiers XML des projets
     */
    void chargerProjets(const QString &dossier);
    /*!
     * \brief Chargement des programmations de l'agenda depuis fichiers d'exportation
     * \param dossier Dossier dans lequel se trouve le fichier XML contenant les programmations
     */
    void chargerAgenda(const QString& dossier);

    /*!
     * \brief Récupère un ProjetEditeur depuis la liste des projets ouverts
     *
     * \param nom Nom du projet
     * \return Pointeur sur ProjetEditeur du projet nom
     */
    ProjetEditeur* getProjetEdit(const QString nom);
    bool estOuvert(const QString nom) { return getProjetEdit(nom)!=0;  }
public:
    /*!
     * \brief Constructeur de la classe, initialisation de la fenêtre et tous ses composants
     */
    MainWindow();

private slots:
    /*!
     * \brief Ouvre l'onglet Agenda contenant l'ensemble des programmations. Focus si déjà ouvert
     */
    void ouvrirAgenda();
    /*!
     * \brief Création d'un nouveau projet
     *
     * Ouvre une fenêtre ProprieteProjetEditeur vide permettant l'ajout d'un nouveau projet.
     */
    void creerProjet();
    /*!
     * \brief Ouvrir un projet existant
     *
     * Ouvre une fenêtre proposant une liste déroulante de l'ensemble des projets pour choisir celui que l'on souhaite gérer.
     * Si aucun projet n'existe, une popup propose alors la création d'un projet.
     */
    void ouvrirProjet();
    /*!
     * \brief Ferme l'onglet de l'éditeur du projet nom si celui-ci est ouvert
     * \param nom Nom du projet
     */
    void fermerProjet(const QString nom);
    /*!
     * \brief Ferme un onglet selon son index
     * \param index Index de l'onglet à fermer
     */
    void closeTab(int index);
    /*!
     * \brief Ferme l'onglet courant
     */
    void closeCurrentTab();
    /*!
     * \brief Sauvegarde des données avant fermeture de la fenêtre
     */
    void save();

protected:
     void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
