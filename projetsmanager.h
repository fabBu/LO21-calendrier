#ifndef PROJETSMANAGER_H
#define PROJETSMANAGER_H

#include <QString>
#include <QDate>
#include "xmlparsor.h"
#include "singleton.h"
#include "tachemanager.h"

class ProjetsManager
{
    std::list<TacheManager*> projets;

    TacheManager* trouverProjet(const QString& nom);

public:
    void ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin);
    void retirerProjet(const QString& nom);
    TacheManager& getProjet(const QString& nom);
    const TacheManager& getProjet(const QString& nom) const;
    const std::list<TacheManager*> getProjets() const {return projets;}
    void setNom(const QString& nom, const QString& nouveau);
    void setDebut(const QString& nom, const QDate& debut);
    void setFin(const QString& nom, const QDate& fin);
    void setDates(const QString& nom, const QDate& debut, const QDate& fin);

    static ProjetsManager& getInstance();
    static void libererInstance();

    void writeXML(const QString &dossier);
    void readXML(QFile &file);
private:
    static Handler<ProjetsManager> handler;
    friend class Handler<ProjetsManager>;

    ProjetsManager(){}
    ~ProjetsManager() {}
    ProjetsManager(const ProjetsManager& p);
    ProjetsManager& operator=(const ProjetsManager& p);
};

#endif // PROJETSMANAGER_H
