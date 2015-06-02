#ifndef PROJETSMANAGER_H
#define PROJETSMANAGER_H

#include <QString>
#include <QDate>
#include "tachemanager.h"

class ProjetsManager
{
    std::list<TacheManager*> projets;

    TacheManager* trouverProjet(const QString& nom);
public:
    ProjetsManager();
    void ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin);
    void retirerProjet(const QString& nom);
    TacheManager& getProjet(const QString& nom);
    const TacheManager& getProjet(const QString& nom) const;
    const std::list<TacheManager*> getProjets() const {return projets;}
    void setNom(const QString& nom, const QString& nouveau);
    void setDebut(const QString& nom, const QDate& debut);
    void setFin(const QString& nom, const QDate& fin);
    void setDates(const QString& nom, const QDate& debut, const QDate& fin);
};

#endif // PROJETSMANAGER_H
