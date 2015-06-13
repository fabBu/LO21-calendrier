#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H
#include <QString>
#include <QDate>
#include <QColor>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include "xmlparsor.h"
#include "calendar.h"

class Tache; //Cyclic Dependency
using namespace std;

class TacheManager : public QObject {
    Q_OBJECT
private:
    friend class TacheEditeur;
    QString nom; /*!< Nom du projet */
    QDate debut; /*!< Date de début du projet */
    QDate fin; /*!< Date de fin du projet */
    QColor couleur; /*!< Couleur du projet */

    list<Tache*> taches; /*!< Ensemble des tâches du projet */

    /*!
     * \brief Permet de chercher une tâche de nom id parimis la liste \a taches
     * \param id Nom de la tâche à rechercher
     * \return Pointeur sur la tâche si existante, 0 sinon
     */
    Tache* trouverTache(const QString& id) const;
    /*!
     * \brief Ajout d'une tâche dans \a taches
     * \param t Pointeur sur la tâche à ajouter
     * \throw <CalendarException> { Une tâche portant le même nom que t existe déjà parmis \a taches }
     */
    void addItem(Tache* t);

public:
    /*!
     * \brief Constructeur d'un projet
     * \param name Nom du projet à créer
     * \param dispo Date de début du projet
     * \param deadline Date de fin du projet
     * \param coul Couleur associée au projet
     */
    TacheManager(const QString& name, const QDate& dispo, const QDate& deadline, const QColor& coul=Qt::white);

    /*!
     * \brief Accès en lecture sur le nom du projet
     * \return \a nom
     */
    const QString& getNom() const { return nom; }
    /*!
     * \brief Setter sur le nom du projet
     * \param name Nouveau nom à attribuer au projet
     */
    void setNom(const QString& name) { nom=name; }
    /*!
     * \brief Accès en lecture sur la date de début du projet
     * \return \a debut
     */
    QDate getDebut() const {  return debut; }
    /*!
     * \brief Accès en lecture sur la date de fin du projet
     * \return \a fin
     */
    QDate getFin() const {  return fin; }
    /*!
     * \brief Setter sur \a debut
     * \param d Nouvelle date de début à associer
     */
    void setDebut(const QDate& d);
    /*!
     * \brief Setter sur \a fin
     * \param f Nouvelle deadline à associer
     */
    void setFin(const QDate& f);
    /*!
     * \brief Accès en lecture sur la couleur associée au projet
     * \return \a couleur
     */
    QColor getCouleur() { return couleur;}
    /*!
     * \brief Setter sur \a couleur
     * \param c Nouvelle couleur à associer
     */
    void setCouleur(QColor c) { couleur=c; emit colorHasChanged(); }
    list<Tache*> getTaches() { return taches; } /*!< Getter sur \a taches */
    const list<Tache*> getTaches() const { return taches; } /*!< Getter constant sur \a taches */
    /*!
     * \brief Donne une référence sur la tâche souhaitée
     * \param titre Titre de la tâche à retourner
     * \return Référence sur la tâche
     * \throw <CalendarException> { La tâche n'existe pas au sein du projet }
     */
    Tache& getTache(const QString& titre);
    /*!
     * \brief Donne une référence constante sur la tâche souhaitée
     * \param titre Titre de la tâche à retourner
     * \return Référence constante sur la tâche
     * \throw <CalendarException> { La tâche n'existe pas au sein du projet }
     */
    const Tache& getTache(const QString& titre) const;
    /*!
     * \brief Création d'une tâche unaire au sein du projet
     * \param t Titre de la tâche
     * \param desc Description de la tâche
     * \param dispo Date de disponibilité de la tâche (avant laquelle elle ne peut commencer)
     * \param deadline Date d'échéance de la tâche (après laquelle elle doit être terminée)
     * \param dur Durée de la tâche
     * \param preemp Tâche préemptive ou non
     * \return Référence sur la tâche créée
     * \throw <CalendarException> { Une tâche portant le même nom existe déjà ou il y a incohérence dans les dates }
     */
    Tache& ajouterTacheUnaire(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree &dur, bool preempt=false);
    /*!
     * \brief Création d'une tâche composite au sein du projet
     * \param t Titre de la tâche
     * \param desc Description de la tâche
     * \param dispo Date de disponibilité de la tâche (avant laquelle elle ne peut commencer)
     * \param deadline Date d'échéance de la tâche (après laquelle elle doit être terminée)
     * \return Référence sur la tâche créée
     * \throw <CalendarException> { Une tâche portant le même nom existe déjà ou il y a incohérence dans les dates }
     */
    Tache& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline);
    /*!
     * \brief Ajout d'une sous-tâche à une tâche composite
     * \param t Nom de la tâche composite
     * \param soust Nom de la sous-tâche
     * \throw <CalendarException> { Aucune tâche de nom t/soust n'existe dans le projet ou soust n'est pas composite }
     */
    void ajouterSousTache(const QString& t, const QString& soust);
    /*!
     * \brief Ajout d'une contrainte de précédence entre deux tâches
     * \param t Nom de la tâche à laquelle ajouter un prédécesseur
     * \param pred Nom du nouveau prédécesseur
     * \throw <CalendarException> { Aucune tâche de nom t/pred n'existe dans le projet }
     */
    void ajouterPred(const QString& t, const QString& pred);
    /*!
     * \brief Suppression d'une tâche
     *
     * Retire une tâche du projet, délie toutes les contraintes qui la liait à d'autres tâches
     * (précédence, et soust-tâches si tâche composite) puis la supprime
     *
     * \param id Nom de la tâche à supprimer
     * \throw <CalendarException> { Aucune tâche de nom id n'existe dans le projet }
     */
    void retirerTache(const QString& id);
    /*!
     * \brief Vérification de l'existance d'un tâche au sein du projet
     * \param id Nom de la tâche à rechercher
     * \return True si une tâche de nom id existe, false sinon
     */
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    /*!
     * \brief Modifie les dates de disponibilité et échéance d'une tâche
     * \param t Tâche pour laquelle modifier les dates
     * \param disp Date de disponibilité
     * \param e Date d'échéance
     * \throw <CalendarException> { Incohérence des dates disp et e ou avec les tâches liées (prédécesseurs/sous-tâches) }
     */
    void setDatesDisponibiliteEcheance(Tache& t, const QDate& disp, const QDate& e);

    /*!
     * \brief Crée un document de l'API Dom contenant le code XML du projet (description des tâches puis leurs contraintes)
     * \return Document contenant le XML
     */
    QDomDocument projetToXML();
private:
    TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);

signals:
    void colorHasChanged();
};
#endif // TACHEMANAGER_H
