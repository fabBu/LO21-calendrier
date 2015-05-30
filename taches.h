#ifndef TACHES_H
#define TACHES_H
#include <QString>
#include <QDate>
#include <QDebug>
#include <QTextStream>
#include "calendar.h"
#include "evenement.h"
using namespace std;


enum Statut { enCours, pause, fini };

class Tache : public Evenement {
protected:
    QDate disponibilite; /*!< Date à laquelle la tâche peut commencer */
    QDate echeance; /*!< Date à laquelle la tâche doit être finie */
    bool termine;
    Statut statut; /*!< Etat de la tâche */

    list<Tache*> predecesseurs; /*!< Liste des tâches devant être² effectuées avant la tâche */
    list<Tache*> successeurs;

    Tache* surtache; ///    TODO : il faudrait un TacheComposite*

    Tache(const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline):
            Evenement(id, desc),disponibilite(dispo),echeance(deadline), surtache(0){ if(disponibilite>echeance) throw CalendarException("Echeance < Disponibilité"); }

    friend class TacheManager;
public:
    QDate getDateDisponibilite() const {  return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);
    virtual void affiche();
    bool estTermine() const { return termine; }
    void setTermine(bool val) { termine=val; }

    const list<Tache*> getPred() const { return predecesseurs; }
    const QString getPredString() const;
    bool estPredecesseur(const Tache& t);
    void ajouterPredecesseur(Tache& t);
    void retirerPredecesseur(Tache& t);
    const list<Tache*> getSucc() const { return successeurs; }
    Tache* getSurtache() const { return surtache; }
    void setSurtache(Tache* t) { surtache=t; }   // TODO : Doit être TacheComposite*

    bool operator==(const Tache& t) { return titre == t.getTitre(); }
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
{friend class TacheManager;
    Duree duree; /*!< Duree de la tâche */
    Duree duree_restante;
    bool preemptive; /*!< Définit si la tâche peut être effectuée en plusieurs fois */

    TacheUnaire(const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool pree=false):
            Tache(id, desc, dispo, deadline),duree(dur), preemptive(pree) {
        if(duree.getDureeEnHeures()>12 && !preemptive)
            throw CalendarException("Une tâche non préemptive ne peut durer plus de 12H");
        if(!verifCoherence(dispo, deadline, dur))
            throw CalendarException("La tâche dure trop longtemps par rapport à sa date d'échéance");
    }
public:
    bool verifCoherence(const QDate& dispo, const QDate& deadline, const Duree& dur);
    Duree getDuree() const { return duree; }
    void setDuree(Duree& dur);
    void setDureeRestante(Duree& dur);
    Duree getDureeRestante() const { return duree_restante; }
    Statut getStatut() const { return statut; }
    void setStatut(Statut stat) { statut = stat; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(const bool value);
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

    TacheComposite(const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline):
            Tache(id, desc, dispo, deadline) {}
public:
    const list<Tache*> getSousTaches() const { return soustaches; }
    bool estSousTache(const Tache& t);
    void ajouterSousTache(Tache& t);
    void retirerSousTache(Tache& t);
    virtual void affiche();
};

#endif // TACHES_H
