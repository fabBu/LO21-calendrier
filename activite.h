#ifndef ACTIVITE_H
#define ACTIVITE_H

#include "evenement.h"
#include <QDate>
#include <iostream>

/*!
 * \file activite.h
 * \brief Création des activités
 * \author Fabrice De Régibus
 * \version 0.1
 */


/*!
 * \brief Enumeration du type de l'événement
 */
enum Type{
    rendez_vous, /*!< Rendez-vous */
    reunion, /*!< Réunion */
    autre /*!< Autre */
};


/*!
 * \class Activite
 * \brief Classe représentant une activité
 *
 * La classe représente une activité
 */
class Activite : public Evenement
{
private:
    Type type; /*!< Type d'événement*/

public:
    /*!
     * \brief Constructeur
     *
     * Constructeur de la classe Activite
     *
     * \param ti : titre de l'événement
     * \param l : lieu de l'événement
     * \param de : description de lévénement
     * \param ty : type de l'événement
     */
    Activite(const QString& ti, const QString& l, const QString& de, const Type& ty):Evenement(ti,l,de), type(ty) {}

    /*!
     * \brief Getter sur type
     * \return type
     */
    const Type& getType() const{return type;}
    /*!
     * \brief Setter sur type
     * \param t : type
     */
    void setType(const Type& t);
    /*!
     * \brief Affichage en mode console
     */
    void affiche();
};

#endif // ACTIVITE_H
