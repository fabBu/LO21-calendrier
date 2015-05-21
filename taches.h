#ifndef TACHES_H
#define TACHES_H
#include <QString>
#include <QDate>
#include <QTextStream>
#include "calendar.h"
#include "evenement.h"
using namespace std;


enum Statut { enCours, pause, fini };

class Tache : public Evenement {
protected:
    QDate disponibilite; /*!< Date à laquelle la tâche peut commencer */
    QDate echeance; /*!< Date à laquelle la tâche doit être finie */
    Statut statut; /*!< Etat de la tâche */

    list<Tache*> predecesseurs; /*!< Liste des tâches devant être² effectuées avant la tâche */

    Tache(const QString& id, const QString& li, const QString& desc, const QDate& dispo, const QDate& deadline):
            Evenement(id, li, desc),disponibilite(dispo),echeance(deadline){}
    Tache(const Tache& t);
    Tache& operator=(const Tache&);
    friend class TacheManager;
public:
    QDate getDateDisponibilite() const {  return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tâche : date echéance < date disponibilité");
        disponibilite=disp; echeance=e;
    }
    virtual void affiche();

    const list<Tache*> getPred() const { return predecesseurs; }
    bool estPredecesseur(Tache& t);
    void ajouterPredecesseur(Tache& t);
    void retirerPredecesseur(Tache& t);
};

QTextStream& operator<<(QTextStream& f, const Tache& t);

/*! \class TacheUnaire
   * \brief Classe représentant une tache réelle
   *
   *  Spécification de la classe tâche.
   *  Elle est une tâche concrète et possède une durée
   */
class TacheUnaire : public Tache
{friend class TacheManager;
    Duree duree; /*!< Duree de la tâche */
    bool preemptive; /*!< Définit si la tâche peut être effectuée en plusieurs fois */

    TacheUnaire(const QString& id, const QString& li, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool pree=false):
            Tache(id, li, desc, dispo, deadline),duree(dur), preemptive(pree) {}
public:

    Duree getDuree() const { return duree; }
    void setDuree(Duree& dur) { duree = dur; }
    Statut getStatut() const { return statut; }
    void setStatut(Statut stat) { statut = stat; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive() { preemptive=true; }
    void setNonPreemptive() { preemptive=false; }
    virtual void affiche();
};

/*! \class TacheComposite
   * \brief Classe représentant une tache composite
   *
   *  Spécification de la classe tâche.
   *  Elle est une sur-tâche : elle est un ensemble de tâches (unaires ou composites)
   */
class TacheComposite : public Tache
{friend class TacheManager;
    list<Tache*> soustaches; /*!< Ensemble des sous-tâches */

    TacheComposite(const QString& id, const QString& li, const QString& desc, const QDate& dispo, const QDate& deadline):
            Tache(id, li, desc, dispo, deadline) {}
public:
    const list<Tache*> getSousTaches() const { return soustaches; }
    void ajouterSousTache(Tache& t);
    void retirerSousTache(Tache& t);
    virtual void affiche();
};

#endif // TACHES_H
