#include "projetsmanager.h"

ProjetsManager::ProjetsManager()
{

}

TacheManager* ProjetsManager::trouverProjet(const QString& nom)
{
    for (std::list<TacheManager*>::const_iterator it = projets.begin(); it != projets.end(); it++)
    {
        if( (*it)->getNom() == nom )
            return *it;
    }
    return 0;
}
TacheManager& ProjetsManager::getProjet(const QString& nom){
    TacheManager* tm=trouverProjet(nom);
    if (!tm) throw CalendarException("Le projet "+nom+" n'existe pas");
    return *tm;
}

const TacheManager& ProjetsManager::getProjet(const QString& nom)const{
    TacheManager& tm = const_cast<ProjetsManager*>(this)->getProjet(nom);
    if(!&tm) throw CalendarException("Le projet "+nom+" n'existe pas");
    return tm;
}

void ProjetsManager::ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin)
{
    TacheManager* tmp=trouverProjet(nom);
    if( tmp )
        throw CalendarException("Le nom "+nom+" est déjà pris pour un autre projet");

    TacheManager* tm = new TacheManager(nom, debut, fin);
    projets.push_back(tm);
}

void ProjetsManager::retirerProjet(const QString& nom)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    projets.remove(tm);
}

void ProjetsManager::setNom(const QString& nom, const QString& nouveau)
{
    TacheManager* tm=trouverProjet(nouveau);
    if( tm )
        throw CalendarException("Le nom "+nouveau+" est déjà utilisé pour un autre projet");
    tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException(nom+" n'existe pas encore");

    tm->setNom(nouveau);
}

void ProjetsManager::setDebut(const QString &nom, const QDate &debut)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    tm->setDebut(debut);
}

void ProjetsManager::setFin(const QString &nom, const QDate &fin)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    tm->setFin(fin);
}

void ProjetsManager::setDates(const QString& nom, const QDate &debut, const QDate &fin)
{
    setDebut(nom, debut);
    setFin(nom, fin);
}
