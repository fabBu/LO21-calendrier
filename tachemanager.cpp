#include "tachemanager.h"

TacheManager::TacheManager(const QString &name, const QDate& dispo, const QDate& deadline):nom(name), debut(dispo), fin(deadline){
    if( nom == "" ) throw CalendarException("Le projet doit avoir un nom !");
    if(debut > fin) throw CalendarException("La date de début dépasse la date de fin");
}

void TacheManager::setDebut(const QDate& d)
{
    for (std::list<Tache*>::const_iterator it = taches.begin(); it != taches.end(); it++)
    {
        if( (*it)->getDateDisponibilite() < d )
            throw CalendarException(getNom()+": la tâche "+(*it)->getTitre()+" possède une échéance inférieure");
    }
    debut=d;
}
void TacheManager::setFin(const QDate& f)
{
    for (std::list<Tache*>::const_iterator it = taches.begin(); it != taches.end(); it++)
    {
        if( (*it)->getDateEcheance() > f )
            throw CalendarException(getNom()+": la tâche "+(*it)->getTitre()+" possède une deadline supérieure");
    }
    fin=f;
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
        throw CalendarException("Ajout tâche : déjà existante dans le projet");
    taches.push_back(t);
}


Tache& TacheManager::ajouterTacheUnaire(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const Duree& dur, const Duree &durestante, bool preempt){
    if( dispo < debut ) throw CalendarException(t+" ne doit pas commencer avant le début du projet");
    if( deadline > fin ) throw CalendarException(t+" ne doit pas terminer après la fin du projet");
    if (trouverTache(t))
        throw CalendarException("Une tâche portant le même nom existe déjà dans le projet");

    Tache* newt=new TacheUnaire(t,desc,dispo,deadline,dur,durestante,preempt);
    addItem(newt);

    return *newt;
}

Tache& TacheManager::ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline)
{
    if( dispo < debut ) throw CalendarException(t+" ne doit pas commencer avant le début du projet");
    if( deadline > fin ) throw CalendarException(t+" ne doit pas terminer après la fin du projet");
    if (trouverTache(t))
        throw CalendarException("Une tâche portant le même nom existe déjà dans le projet");

    Tache* newt=new TacheComposite(t,desc,dispo,deadline);
    addItem(newt);

    return *newt;
}

void TacheManager::retirerTache(const QString& id)
{
    Tache* t=trouverTache(id);
    if( !t ) throw CalendarException("TM : Retrait d'une tache inexistante");

    const list<Tache*> l = t->getSucc();
    list<Tache*>::const_iterator it;
    for( it = l.begin() ; it != l.end() ; ++it )
    {
            (*it)->retirerPredecesseur(*t);
    }

    TacheComposite* tc;
    // Retirer la tâche de la sur-tache (composite) si elle existe
    Tache* st = t->getSurtache();
    if( st != 0 ) // La tache est une sous-tache d'une tacheComposite
    {
        tc = dynamic_cast<TacheComposite*>(st);
        tc->retirerSousTache(*t);
    }

    // Retirer toutes les sous-taches si la tache courante est composite
    if( tc = dynamic_cast<TacheComposite*>(t) )
    {
        const list<Tache*> l2 = tc->getSousTaches();
        for( it = l2.begin() ; it != l2.end() ; ++it )
        {
            tc->retirerSousTache((**it));
        }
    }

    taches.remove(t);
}

Tache& TacheManager::getTache(const QString& titre){
    Tache* t=trouverTache(titre);
    if (!t) throw CalendarException("TacheManager, tache inexistante");
    return *t;
}

const Tache& TacheManager::getTache(const QString& titre)const{
    Tache& t = const_cast<TacheManager*>(this)->getTache(titre);
    if(!&t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return t;
}

void TacheManager::ajouterSousTache(const QString& t, const QString& soust)
{
    TacheComposite* t1=  dynamic_cast<TacheComposite*>(trouverTache(t));
    if (!t1) throw CalendarException(t+" n'existe pas ou n'est pas composite");
    Tache* t2=trouverTache(soust);
    if (!t2) throw CalendarException("TacheManager, "+soust+" inexistant");

    t1->ajouterSousTache(*t2);
}

void TacheManager::ajouterPred(const QString& t, const QString& pred)
{
    Tache* t1=  trouverTache(t);
    if (!t1) throw CalendarException("TacheManager, "+t+" inexistant");
    Tache* t2=trouverTache(pred);
    if (!t2) throw CalendarException("TacheManager, "+pred+" inexistant");

    t1->ajouterPredecesseur(*t2);
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

QDomDocument TacheManager::projetToXML()
{
    QDomDocument doc;
    QDomProcessingInstruction instr = doc.createProcessingInstruction( "xml", "version='1.0' encoding='ISO-8859-1'");
    doc.appendChild(instr);

    QDomElement projetElement = addXmlElement( doc, doc, "projet" );

    //   Propriétés du projet
    QDomElement proprietesElement = addXmlElement(doc, projetElement, "proprietes" );
    addXmlElement( doc, proprietesElement, "nom", nom );
    addXmlElement( doc, proprietesElement, "debut", debut.toString("dd-MM-yyyy") );
    addXmlElement( doc, proprietesElement, "fin", fin.toString("dd-MM-yyyy") );

    //   Tâches appartenant au projet
    QDomElement tachesElement = addXmlElement(doc, projetElement, "taches");
    std::list<Tache*>::iterator it;
    for ( it = taches.begin(); it != taches.end(); ++it)
    {
        (*it)->TacheToXML(doc, tachesElement);
    }

    //   Contraintes entre les tâches du projet
    QDomElement contraintesElement = addXmlElement(doc, projetElement, "contraintes");
    std::list<Tache*>::iterator it2;
    for ( it2 = taches.begin(); it2 != taches.end(); ++it2)
    {
        (*it2)->contraintesToXML(doc, contraintesElement);
    }

    return doc;
}

