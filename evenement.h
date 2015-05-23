#ifndef EVENEMENT_H
#define EVENEMENT_H

/*!
  * \file evenement.h
  * \brief Création des événements
  * \author Fabrice De Régibus
  * \version 0.1
  */
#include <string.h>
#include <iostream>
#include <QString>
#include <iostream>
#include "calendar.h"

/*!
 * \class Evenement
 * \brief Classe abstraite représentant un événement
 *
 *  La classe représente un événement
 */
class Evenement
{
protected:
    QString titre; /*!< Nom de l'événement*/
    QString description; /*!< Description de l'événement*/
public:
    /*!
     * \brief Constructeur
     *
     * Constructeur de la classe Evenement
     *
     * \param t : titre
     * \param d : description
     */
    Evenement(const QString& t, const QString& d):titre(t), description(d) {
        if( titre == "") throw CalendarException("Tout événement doit posséder un nom !");
    }

    /*!
     * \brief Getter sur titre
     * \return titre
     */
    const QString& getTitre() { return titre;}

    /*!
     * \brief Setter sur titre
     * \param t : titre
     */
    void setTitre(const QString& t);

    /*!
     * \brief Getter sur description
     * \return description
     */
    const QString& getDescription() { return description;}

    /*!
     * \brief Setter sur description
     * \param d : description
     */
    void setDescription(const QString& d);

    /*!
     * \brief Affichage en mode console
     */
    virtual void affiche()=0;
};

#endif // EVENEMENT_H
