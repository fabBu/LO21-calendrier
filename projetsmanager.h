#ifndef PROJETSMANAGER_H
#define PROJETSMANAGER_H

#include <QString>
#include <QDate>
#include "xmlparsor.h"
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
    ProjetsManager(){}
    ~ProjetsManager() {}
    ProjetsManager(const ProjetsManager& p);
    ProjetsManager& operator=(const ProjetsManager& p);

    struct Handler
    {
        ProjetsManager *instance;

        Handler(): instance(0) {}
        ~Handler() {if (instance) delete instance;}
    };
    static Handler handler;
};

#endif // PROJETSMANAGER_H
