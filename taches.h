#ifndef TACHES_H
#define TACHES_H
#include <QString>
#include <QDate>
#include <QDebug>
#include <QTextStream>
#include "xmlparsor.h"
#include "calendar.h"
#include "evenement.h"
#include "tachemanager.h"

using namespace std;


class TacheComposite;

class Tache : public Evenement {
    Q_OBJECT
protected:
    QDate disponibilite; /*!< Date à laquelle la tâche peut commencer */
    QDate echeance; /*!< Date à laquelle la tâche doit être finie */
    bool termine; /*!< Etat de la tâche, terminé ou non */

    list<Tache*> predecesseurs; /*!< Liste des tâches devant être effectuées avant la tâche */
    list<Tache*> successeurs;  /*!< Liste des tâches qui succèdent la tâche */

    TacheComposite* surtache; /*!< Sur-tâche de la tâche. Vaut 0 si la tâche est au premier niveau */

    TacheManager* parent; /*!< Pointeur sur le projet auquel la tâche appartient */

    /*!
     * \brief Constructeur d'une tâche
     * \param p Projet auquel la tâche appartient
     * \param titre Intitulé de la tâche
     * \param desc Description de la tâche
     * \param dispo Date de début à partir de laquelle la date peut commencer
     * \param deadline Date de fin à partir de laquelle la tâche doit être terminée
     * \throw <CalendarException> { Le titre est nul ou la disponibilité est après l'échéance }
     */
    Tache(TacheManager* p, const QString& titre, const QString& desc, const QDate& dispo, const QDate& deadline):
            Evenement(titre, desc),parent(p),disponibilite(dispo),echeance(deadline), surtache(0), termine(false){
        if(titre=="") throw CalendarException("Une tâche doit posséder un titre");
        if(disponibilite>echeance) throw CalendarException("Echeance < Disponibilité");
    }

    friend class TacheManager;

    /*!
     * \brief Création de code XML à partir des informations de la tâche
     * \param doc Document XML dans lequel générer le code
     * \param elem Element sous lequel insérer le code
     * \return Chaîne de caractère généré et introduite dans le document XML
     */
    virtual QString TacheToXML(QDomDocument &doc, QDomElement &elem) =0;
    /*!
     * \brief Création de code XML concernant les contraintes de précédence de la tâche
     * \param doc Document XML dans lequel générer le code
     * \param elem Element sous lequel insérer le code
     * \return Chaîne de caractère généré et introduite dans le document XML
     */
    virtual QString contraintesToXML(QDomDocument &doc, QDomElement &elem) =0;
public:
    QDate getDateDisponibilite() const {  return disponibilite; }  /*!< Getter sur la date de disponibilité de la tâche */
    QDate getDateEcheance() const {  return echeance; }  /*!< Getter sur la date d'échéance de la tâche */
    /*!
     * \brief Setter sur l'intervalle de dates de la tâche
     * \param disp Date de disponibilité
     * \param e Date d'échéance
     * \throw <CalendarException> { La disponibilité est après l'échéance, ou il existe un incohérence avec les dates des prédécesseurs et sur-tâches }
     */
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);
    virtual void affiche()=0;
    bool estTermine() const { return termine; } /*!< Renseigne l'état de la tâche, terminé ou non */
    /*!
     * \brief Setter sur l'état terminé de la tâche
     * \param val Nouvel état de la tâche
     */
    void setTermine(bool val);

    /*!
     * \brief Accès en lecture sur les prédécesseurs de la tâche
     * \return \a predecesseurs
     */
    const list<Tache*> getPred() const { return predecesseurs; }
    /*!
     * \brief Accès aux prédécesseurs de la tâche
     * \return Ensemble des titres des prédécesseurs
     */
    const QString getPredString() const;
    /*!
     * \brief Getter sur le prédécesseur ayant la date d'échéance la plus grande
     * \return Pointeur en lecture sur la tâche
     */
    const Tache *getLastPredecesseur() const;
    /*!
     * \brief Getter sur le successeur ayant la date de disponibilité la plus petite
     * \return Pointeur en lecture sur la tâche
     */
    const Tache *getFirstSuccesseur() const;
    /*!
     * \brief Indique si une tâche précède la tâche courante
     * \param t Tâche pour laquelle vérifier si elle précède la tâche courante
     * \return True si la t précède la tâche
     */
    bool estPredecesseur(const Tache& t);
    /*!
     * \brief Ajout de la tâche t comme étant prédécesseur de la tâche
     * \param t Prédécesseur à ajouter
     * \throw <CalendarException> { La tâche est déjà un prédécesseur ou possède une échéance trop grande }
     */
    void ajouterPredecesseur(Tache& t);
    /*!
     * \brief Retrait du prédécesseur t de la tâche
     * \param t Prédécesseur à retirer
     * \throw <CalendarException> { La tâche n'est pas un prédécesseur }
     */
    void retirerPredecesseur(Tache& t);
    /*!
     * \brief Accès en lecture sur les successeurs de la tâche
     * \return \a successeurs
     */
    const list<Tache*> getSucc() const { return successeurs; }
    /*!
     * \brief Getter sur la sur-tâche (tâche composite)
     * \return Pointeur sur la tâche \a surtache
     */
    TacheComposite* getSurtache() const { return surtache; }
    /*!
     * \brief Setter sur la sur-tâche
     * \param t Nouvelle tâche parent
     */
    void setSurtache(TacheComposite* t) { surtache=t; }
    /*!
     * \brief Getter sur projet auquel appartient la tâche
     * \return \a parent
     */
    const TacheManager* const getParent() const { return parent; }

    /*!
     * \brief Comparaison entre deux tâches
     * \param t Tâche à comparer
     * \return True si les tâches possèdent le même titre
     */
    bool operator==(const Tache& t) { return titre == t.getTitre(); }
    /*!
     * \brief Comparaison entre deux tâches
     * \param t Tâche à comparer
     * \return True si les tâches possèdent un titre différent
     */
    bool operator!=(const Tache& t) { return titre != t.getTitre(); }

private:
    Tache(const Tache& t);
    Tache& operator=(const Tache&);
};

QTextStream& operator<<(QTextStream& f, const Tache& t);

/*! \class TacheUnaire
   * \brief Classe représentant une tache réelle
   *
   *  Spécification de la classe tâche.
   *  Elle est une tâche concrète et possède une durée
   */
class TacheUnaire : public Tache
{Q_OBJECT

    friend class TacheManager;
    Duree duree; /*!< Duree totale de la tâche */
    Duree duree_restante; /*!< Duree restante de la tâche */
    bool preemptive; /*!< Définit si la tâche peut être effectuée en plusieurs fois */

    /*!
     * \brief Constructeur d'une tâche unaire
     * \param p Projet auquel la tâche appartient
     * \param titre Intitulé de la tâche
     * \param desc Description de la tâche
     * \param dispo Date de début à partir de laquelle la date peut commencer
     * \param deadline Date de fin à partir de laquelle la tâche doit être terminée
     * \param dur Durée totale de la tâche
     * \param pree Tâche préemptive ou non
     * \throw <CalendarException> { La tâche est non préemptive avec une durée de plus de 12h ou incohérence des dates et durée }
     */
    TacheUnaire(TacheManager* p,const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool pree=false):
            Tache(p, id, desc, dispo, deadline),duree(dur), preemptive(pree), duree_restante(dur) {
        if(duree.getDureeEnHeures()>12 && !preemptive)
            throw CalendarException("Une tâche non préemptive ne peut durer plus de 12H");
        if(!verifCoherence(dispo, deadline, dur))
            throw CalendarException("La tâche dure trop longtemps par rapport à sa date d'échéance");
    }
public:
    /*!
     * \brief Vérification de la cohérence entre un intervalle de dates et une durée
     * \param dispo Date de disponibilité
     * \param deadline Date de fin
     * \param dur Durée
     * \return False si dispo+dur dépasse deadline
     */
    bool verifCoherence(const QDate& dispo, const QDate& deadline, const Duree& dur);
    Duree getDuree() const { return duree; } /*!< Getter sur \a duree */
    /*!
     * \brief Setter sur durée totale de la tâche
     * \param dur Nouvelle durée
     * \throw <CalendarException> { La tâche est non préemptive avec une durée de plus de 12h ou incohérence de la durée avec les dates }
     */
    void setDuree(const Duree& dur);
    /*!
     * \brief Setter sur la durée restante de la tâche
     * \param dur Nouvelee durée
     * \throw <CalendarException> { La durée dépasse la durée totale de la tâche }
     */
    void setDureeRestante(const Duree& dur);
    Duree getDureeRestante() const { return duree_restante; } /*!< Getter sur \a duree_restante */
    /*!
     * \brief Indique si la tâche est préemptive
     * \return \a preemptive, true si tâche préemptive
     */
    bool isPreemptive() const { return preemptive; }
    /*!
     * \brief Setter sur \a preemptive
     * \param Nouveau statut de la tâche
     * \throw <CalendarException> { Si set à true avec durée totale inférieure à 12h }
     */
    void setPreemptive(const bool value);
    virtual void affiche();

private:
    TacheUnaire(const TacheUnaire& t);
    TacheUnaire& operator=(const TacheUnaire&);

    virtual QString TacheToXML(QDomDocument& doc, QDomElement& elem);
    virtual QString contraintesToXML(QDomDocument &doc, QDomElement &elem);
};

/*! \class TacheComposite
   * \brief Classe représentant une tache composite
   *
   *  Spécification de la classe tâche.
   *  Elle est une sur-tâche : elle est un ensemble de tâches (unaires ou composites)
   */
class TacheComposite : public Tache
{Q_OBJECT
    friend class TacheManager;
    list<Tache*> soustaches; /*!< Ensemble des sous-tâches */

    /*!
     * \brief Constructeur d'une tâche composite
     * \param p Projet auquel la tâche appartient
     * \param titre Intitulé de la tâche
     * \param desc Description de la tâche
     * \param dispo Date de début à partir de laquelle la date peut commencer
     * \param deadline Date de fin à partir de laquelle la tâche doit être terminée
     * \throw <CalendarException> { Le titre est nul ou la disponibilité est après l'échéance }
     */
    TacheComposite(TacheManager* p, const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline):
            Tache(p,id, desc, dispo, deadline) {}
public:
    /*!
     * \brief Vérification de l'état terminé ou non de la tâche composite
     *
     * Parcourt l'ensemble des sous-tâches afin de déterminer son état.
     * Si et seulement si toutes ses sous-tâches sont terminées, alors la tâche l'est aussi.
     */
    void verifTermine();
    /*!
     * \brief Getter sur \a soustaches
     * \return Liste constante des sous-tâches
     */
    const list<Tache*> getSousTaches() const { return soustaches; }
    /*!
     * \brief Indique si t est une sous-tâche
     * \param t Tâche à rechercher
     * \return True si t fait partie des sous-tâches
     */
    bool estSousTache(const Tache& t);
    /*!
     * \brief Ajoute t dans la liste des sous-tâches
     * \param t Tâche à ajouter
     * \throw <CalendarException> { Si t est déjà une sous-tâche ou s'il y a incohérence des dates }
     */
    void ajouterSousTache(Tache& t);
    /*!
     * \brief Retrait de t depuis la liste des sous-tâches
     * \param t Tâche à retirer
     * \throw <CalendarException> { Si t ne fait pas partie des sous-tâches }
     */
    void retirerSousTache(Tache& t);
    virtual void affiche();


private:
    TacheComposite(const TacheComposite& t);
    TacheComposite& operator=(const TacheComposite&);

    virtual QString TacheToXML(QDomDocument& doc, QDomElement& elem);
    virtual QString contraintesToXML(QDomDocument &doc, QDomElement &elem);
};

#endif // TACHES_H
