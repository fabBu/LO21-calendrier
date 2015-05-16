#include "tachemanager.h"

TacheManager::TacheManager(const QString &name):nom(name){}

Tache* TacheManager::trouverTache(const QString& id)const{

    for (std::list<Tache*>::const_iterator it = taches.begin(); it != taches.end(); it++)
    {
        if( (*it)->getTitre() == id )
            return *it;
    }
    return 0;
}

void TacheManager::addItem(Tache* t){
    if( trouverTache(t->getTitre()) )
        throw CalendarException("erreur : tache deja existante dans le projet");
    taches.push_back(t);
}

Tache& TacheManager::ajouterTacheUnaire(const QString& t, const QString& li, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool preempt){
    if (trouverTache(t))
        throw CalendarException("erreur, TacheManager, tache deja existante");

    Tache* newt=new TacheUnaire(t,li,desc,dispo,deadline,dur,preempt);
    addItem(newt);
    return *newt;
}

Tache& TacheManager::ajouterTacheComposite(const QString& t, const QString& li, const QString& desc, const QDate& dispo, const QDate& deadline)
{
    if (trouverTache(t))
        throw CalendarException("erreur, TacheManager, tache deja existante");

    Tache* newt=new TacheComposite(t,li,desc,dispo,deadline);
    addItem(newt);
    return *newt;
}

void TacheManager::retirerTache(const QString& id)
{
    Tache* t=trouverTache(id);
    if( !t ) throw CalendarException("Retrait d'une tache inexistante");
    taches.remove(t);
}

Tache& TacheManager::getTache(const QString& id){
    Tache* t=trouverTache(id);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}

const Tache& TacheManager::getTache(const QString& id)const{
    return const_cast<TacheManager*>(this)->getTache(id);
}

// TODO  a modifier
TacheManager::~TacheManager(){
   /*
    if (file!="") save(file);
    for(unsigned int i=0; i<nb; i++) delete taches[i];
    delete[] taches;
    file="";
    */
}
