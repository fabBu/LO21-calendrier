#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H
#include <QString>
#include <QDate>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

#include "taches.h"

using namespace std;

class TacheManager {
private:
    friend class TacheEditeur;
    QString nom;
    QDate debut;
    QDate fin;

    list<Tache*> taches;
    void addItem(Tache* t);
    Tache* trouverTache(const QString& id) const;
    QString file;

public:
    TacheManager(const QString& name, const QDate& dispo, const QDate& deadline);
    ~TacheManager();

    const QString& getNom() const { return nom; }
    void setNom(const QString& name) { nom=name; }
    QDate getDebut() const {  return debut; }
    QDate getFin() const {  return fin; }
    void setDebut(const QDate& d);
    void setFin(const QDate& f);
    list<Tache*> getTaches() { return taches; }
    const list<Tache*> getTaches() const { return taches; }
    Tache& getTache(const QString& titre);
    const Tache& getTache(const QString& titre) const;
    Tache& ajouterTacheUnaire(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree &dur, bool preempt=false);
    Tache& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline);
    void retirerTache(const QString& id);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    void setDatesDisponibiliteEcheance(Tache& t, const QDate& disp, const QDate& e);
    void load(const QString& f);
    void save(const QString& f);

private:
    TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
};
#endif // TACHEMANAGER_H
