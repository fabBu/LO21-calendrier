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

    TacheManager* parent;

    Tache(TacheManager* p, const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline):
            Evenement(id, desc),parent(p),disponibilite(dispo),echeance(deadline), surtache(0), termine(false){ if(disponibilite>echeance) throw CalendarException("Echeance < Disponibilité"); }

    friend class TacheManager;

    virtual QString TacheToXML(QDomDocument &doc, QDomElement &elem) =0;
    virtual QString contraintesToXML(QDomDocument &doc, QDomElement &elem) =0;
public:
    QDate getDateDisponibilite() const {  return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);
    virtual void affiche();
    estTermine() const { return termine; }
    void setTermine(bool val);

    const list<Tache*> getPred() const { return predecesseurs; }
    const QString getPredString() const;
    const Tache *getLastPredecesseur() const;
    const Tache *getFirstSuccesseur() const;
    bool estPredecesseur(const Tache& t);
    void ajouterPredecesseur(Tache& t);
    void retirerPredecesseur(Tache& t);
    const list<Tache*> getSucc() const { return successeurs; }
    Tache* getSurtache() const { return surtache; }
    void setSurtache(Tache* t) { surtache=t; }   // TODO : Doit être TacheComposite*
    const TacheManager* const getParent() const { return parent; }

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
{
    friend class TacheManager;
    Duree duree; /*!< Duree de la tâche */
    Duree duree_restante;
    bool preemptive; /*!< Définit si la tâche peut être effectuée en plusieurs fois */

    TacheUnaire(TacheManager* p,const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool pree=false):
            Tache(p, id, desc, dispo, deadline),duree(dur), preemptive(pree), duree_restante(dur) {
        if(duree.getDureeEnHeures()>12 && !preemptive)
            throw CalendarException("Une tâche non préemptive ne peut durer plus de 12H");
        if(!verifCoherence(dispo, deadline, dur))
            throw CalendarException("La tâche dure trop longtemps par rapport à sa date d'échéance");
    }
public:
    bool verifCoherence(const QDate& dispo, const QDate& deadline, const Duree& dur);
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& dur);
    void setDureeRestante(const Duree& dur);
    Duree getDureeRestante() const { return duree_restante; }
    Statut getStatut() const { return statut; }
    void setStatut(Statut stat) { statut = stat; }
    bool isPreemptive() const { return preemptive; }
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
{friend class TacheManager;
    list<Tache*> soustaches; /*!< Ensemble des sous-tâches */

    TacheComposite(TacheManager* p, const QString& id, const QString& desc, const QDate& dispo, const QDate& deadline):
            Tache(p,id, desc, dispo, deadline) {}
public:
    void verifTermine();
    const list<Tache*> getSousTaches() const { return soustaches; }
    bool estSousTache(const Tache& t);
    void ajouterSousTache(Tache& t);
    void retirerSousTache(Tache& t);
    virtual void affiche();
private:
    TacheComposite(const TacheComposite& t);
    TacheComposite& operator=(const TacheComposite&);

    virtual QString TacheToXML(QDomDocument& doc, QDomElement& elem);
    virtual QString contraintesToXML(QDomDocument &doc, QDomElement &elem);
};

#endif // TACHES_H
