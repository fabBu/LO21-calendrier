#ifndef PROPRIETEPROJETEDITEUR_H
#define PROPRIETEPROJETEDITEUR_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include "projetsmanager.h"

class ProprieteProjetEditeur : public QWidget
{
    Q_OBJECT

    QWidget *parent;
    QGridLayout *main_layout;
    QLabel *nom_label, *debut_label, *fin_label;
    QLineEdit *nom_edit;
    QDateEdit *debut_date, *fin_date;
    QPushButton *sauvegarder, *annuler;

    ProjetsManager& projets; /**< Ensemble des projets chargés lors de l'exécution */
    TacheManager* projet; /**< Projet courant. Est nul lors de l'ajout d'un projet */
public:
    /*!
     * \brief Editeur des propriétés d'un projet
     *
     * ProprieteProjetEditeur permet la modification des propriétés d'un projet (nom et dates de début/fin).
     * Si aucun nom n'est renseigné, les champs seront vides et permettront la création d'un nouveau projet.
     *
     * \param pa QWidget parent
     * \param proj Nom du projet à éditer. Nouveau projet si pas renseigné
     */
    ProprieteProjetEditeur(QWidget* pa, const QString& proj="");
private slots:
    /*!
     * \brief Demande de création du projet
     */
    void creerProjet();
    /*!
     * \brief Modification des propriétés du projet
     */
    void modifierProjet();
    /*!
     * \brief Réinitialise les champs avec leurs valeurs de départ
     */
    void refresh();
signals:
    /*!
     * \brief Signal de fermeture de l'éditeur
     * \param proj Nom du projet
     */
    void fermeture(const QString& proj);
};

#endif // PROPRIETEPROJETEDITEUR_H
