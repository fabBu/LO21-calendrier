#include "taches.h"



//              ---------   Tache   ---------              //
void Tache::affiche()
{
    Evenement::affiche();
    std::cout<<" dispo="<<disponibilite.toString("yyyy-MM-dd").toStdString()<<" echeance="<<echeance.toString("yyyy-MM-dd").toStdString();
}

/*!
 *\brief Ajout d'une précédence
 *
 * Permet d'ajouter la tache t comme étant un prédécesseur
 *  de la tâche courante
 *
 *\param t : tache précédente
 */
void Tache::ajouterPredecesseur(Tache& t)
{
    // Verification de la coherence des donnees
    if( getDateDisponibilite() > t.getDateEcheance())
        predecesseurs.push_back(&t);
    else
        throw CalendarException(t.getTitre()+" se termine apres "+getTitre());

    // Verification que la tâche t ne fait pas déjà partie des prédécesseurs
    if( std::find(predecesseurs.begin(), predecesseurs.end(), &t) != predecesseurs.end() )
        throw CalendarException(t.getTitre()+" fait partie des predecesseurs de "+getTitre());
}

/*!
 *\brief Suppression d'une précédence
 *
 * Permet de retirer la tâche t précédant la tâche courante
 *
 *\param t : tache à retirer
 */
void Tache::retirerPredecesseur(Tache& t)
{
    // Verification que la tâche t fait bien partie des prédécesseurs
    if( std::find(predecesseurs.begin(), predecesseurs.end(), &t) == predecesseurs.end() )
        throw CalendarException(t.getTitre()+" ne fait pas partie des predecesseurs de "+getTitre());

    // Suppression de la tâche parmis les prédécesseurs
    predecesseurs.remove(&t);

}


//              ---------   TacheUnaire   ---------              //
void TacheUnaire::affiche()
{
    Tache::affiche();
    std::cout<<" duree="<<duree<<" preemptive="<<preemptive;
}






//              ---------   TacheComposite   ---------              //
/*!
 *\brief Ajout d'une sous-tâche
 *
 * Ajoute la tâche t comme étant une sous-tâche de
 * la tâche composite
 *
 *\param t : sous-tâche
 */
void TacheComposite::ajouterSousTache(Tache& t)
{
    // Verification que la tâche t n'est pas déjà une sous-tâche
    if( std::find(soustaches.begin(), soustaches.end(), &t) != soustaches.end() )
        throw CalendarException(t.getTitre()+" est deja une sous-tache de "+getTitre());

    soustaches.push_back(&t);

    // Cas où la sous-tâche se termine après la tâche composite
    if( getDateEcheance() < t.getDateEcheance() )
        setDatesDisponibiliteEcheance(getDateDisponibilite(), t.getDateEcheance());

    // Cas où la tâche débute avant la tâche composite
    if( getDateDisponibilite() > t.getDateDisponibilite() )
        setDatesDisponibiliteEcheance(t.getDateDisponibilite(), getDateEcheance());
}

/*!
 *\brief Retrait d'une sous-tâche
 *
 * Retire la tâche t du sous-ensemble de tâches
 * de la tâche composite
 *
 *\param t : sous-tâche
 */
void TacheComposite::retirerSousTache(Tache& t)
{
    // Verification que la tâche t fait bien partie des sous-tâches
    if( std::find(soustaches.begin(), soustaches.end(), &t) == soustaches.end() )
        throw CalendarException(t.getTitre()+" n\'est pas une sous-tache de "+getTitre());

    soustaches.remove(&t);

    // Mise à jour des dates de la tâche composite
    QDate min = (*soustaches.begin())->getDateDisponibilite();
    QDate max = (*soustaches.begin())->getDateEcheance();

    for (std::list<Tache*>::iterator it = soustaches.begin(); it != soustaches.end(); it++)
    {
        if( (*it)->getDateDisponibilite() < min ) min = (*it)->getDateDisponibilite();
        if( (*it)->getDateEcheance() > max ) max = (*it)->getDateEcheance();
    }

    setDatesDisponibiliteEcheance(min, max);;
}

void TacheComposite::affiche()
{
    Tache::affiche();
}
<<<<<<< HEAD

=======
>>>>>>> origin/master
