#include "tachemanager.h"

TacheManager::TacheManager(const QString &name):nom(name){
    if( nom == "" ) throw CalendarException("Le projet doit avoir un nom !");
}

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

Tache& TacheManager::ajouterTacheUnaire(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, bool preempt){
    if (trouverTache(t))
        throw CalendarException("erreur, TacheManager, tache deja existante");

    Tache* newt=new TacheUnaire(t,desc,dispo,deadline,dur,preempt);
    addItem(newt);
    return *newt;
}

Tache& TacheManager::ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline)
{
    if (trouverTache(t))
        throw CalendarException("erreur, TacheManager, tache deja existante");

    Tache* newt=new TacheComposite(t,desc,dispo,deadline);
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

void TacheManager::setDatesDisponibiliteEcheance(Tache& t, const QDate& disp, const QDate& e)
{
    if (e<disp)
        throw CalendarException("erreur Tâche : date echéance < date disponibilité");

    const list<Tache*> l = this->getTaches();
    list<Tache*>::const_iterator it;
    for( it = l.begin() ; it != l.end() ; ++it )
    {
        if( (*it)->estPredecesseur(t) )
        {
            if( (*it)->getDateDisponibilite() > disp )
                throw CalendarException( "Le successeur "+(*it)->getTitre()+" possede une date de dispo supérieure" );

            if( (*it)->getDateEcheance() < e )
                throw CalendarException( "Le successeur "+(*it)->getTitre()+" possede une date d'échéance inférieure" );
        }

        TacheComposite* tc = dynamic_cast<TacheComposite*>(*it);
        if( tc && tc->estSousTache(t) )
        {
            if( (*it)->getDateDisponibilite() > disp )
                throw CalendarException( "La sur-tache "+tc->getTitre()+" possede une date de dispo supérieure" );

            if( (*it)->getDateEcheance() < e )
                throw CalendarException( "Le successeur "+tc->getTitre()+" possede une date d'échéance inférieure" );
        }
    }

    t.setDatesDisponibiliteEcheance(disp, e);
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
