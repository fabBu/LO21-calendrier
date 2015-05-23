#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H
#include <QString>
#include <QDate>
#include <QTextStream>
#include "taches.h"

using namespace std;

class TacheManager {
private:
    friend class TacheEditeur;
    QString nom;
    list<Tache*> taches;
    void addItem(Tache* t);
    Tache* trouverTache(const QString& id) const;
    QString file;

public:
    TacheManager(const QString& name);
    ~TacheManager();

    const QString& getNom() const { return nom; }
    const list<Tache*> getTaches() const { return taches; }
    Tache& getTache(const QString& id);
    const Tache& getTache(const QString& code) const;
    Tache& ajouterTacheUnaire(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool preempt=false);
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
