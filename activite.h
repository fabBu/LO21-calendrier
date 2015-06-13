#ifndef ACTIVITE_H
#define ACTIVITE_H

#include "xmlparsor.h"
#include "evenement.h"
#include <iostream>
#include <QMetaEnum>

/*!
 * \class MetaEnum
 * \brief Classe représentant un enum de type
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
    /*!
     * \brief Méthode pour passer d'un type à un QString
     * \param t : Type
     */
    QString typeToString(Type t);
    /*!
     * \brief Méthode pour passer d'un int à un type
     * \param i : int
     */
    QString typeToString(int i);
    /*!
     * \brief Méthode pour passer d'un QString à un type
     * \param type : QString
     */
    Type stringToType(const QString& type);
    /*!
     * \brief Getter sur le nombre de type
     * \return int
     */
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
     * \brief Getter sur \a type
     * \return \a type
     */
    const MetaEnum::Type& getType() const{return type;}
    /*!
     * \brief Setter sur \a type
     * \param t : nouveau type
     */
    void setType(const MetaEnum::Type& t);
    /*!
     * \brief Getter sur lieu
     * \return \a lieu
     */
    const QString& getLieu() const { return lieu;}
    /*!
     * \brief Setter sur \a lieu
     * \param l : nouveau lieu
     */
    void setLieu(const QString& l);
    /*!
     * \brief Affichage en mode console
     */
    void affiche();

    /*!
     * \brief Création de code XML à partir des informations de l'activité
     * \param doc Document XML dans lequel générer le code
     * \param elem Element sous lequel insérer le code
     * \return Chaîne de caractère généré et introduite dans le document XML
     */
    QString ActiviteToXML(QDomDocument& doc, QDomElement& elem);
};

#endif // ACTIVITE_H
