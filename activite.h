#ifndef ACTIVITE_H
#define ACTIVITE_H

#include "evenement.h"
#include <iostream>
#include <QMetaEnum>

/*!
 * \file activite.h
 * \brief Création des activités
 * \author Fabrice De Régibus
 * \version 1.0
 */

class MetaEnum : public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)
public:
    enum Type{
        Rendez_vous, /*!< Rendez-vous */
        Reunion, /*!< Réunion */
        Autre /*!< Autre */
    };
    QString enumToString(Type t);
    QString enumToString(int i);
    int getNbEnum();
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
    MetaEnum::Type type; /*!< Type d'événement*/
    QString lieu; /*!< Lieu de l'événement*/

public:
    /*!
     * \brief Constructeur
     *
     * Constructeur de la classe Activite
     *
     * \param ti : titre de l'événement
     * \param de : description de lévénement
     * \param ty : type de l'événement
     * \param l : lieu de l'événement
     */
    Activite(const QString& ti, const QString& de, const MetaEnum::Type& ty, const QString& l):Evenement(ti,de), type(ty), lieu(l) {}

    /*!
     * \brief Getter sur type
     * \return type
     */
    const MetaEnum::Type& getType() const{return type;}
    /*!
     * \brief Setter sur type
     * \param t : type
     */
    void setType(const MetaEnum::Type& t);
    /*!
     * \brief Getter sur lieu
     * \return lieu
     */
    const QString& getLieu() const { return lieu;}
    /*!
     * \brief Setter sur lieu
     * \param l : lieu
     */
    void setLieu(const QString& l);
    /*!
     * \brief Affichage en mode console
     */
    void affiche();
};

#endif // ACTIVITE_H
