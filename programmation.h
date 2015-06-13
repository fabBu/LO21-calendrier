#ifndef PROGRAMMATION_H
#define PROGRAMMATION_H

#include <QDateTime>
#include "evenement.h"
#include "taches.h"
#include "activite.h"
#include "calendar.h"

/*!
 * \class Programmation
 * \brief Classe représentant une programmation
 *
 * La classe représente une programmation
 */
class Programmation
{
private:
    QDateTime date; /*!< Date de la programmation*/
    Duree duree; /*!< Durée de la programmation*/
    Evenement* evenement; /*!< Evenement programmé*/

    /* On ne souhaite pas que l'utilisateur de la classe puisse utiliser l'opérateur d'affectation ainsi que le constructeur par recopie d'une programmation */
    Programmation(const Programmation& p) {}
    Programmation& operator=(const Programmation& p)  {}

public:
    /*!
     * \brief Constructeur
     *
     * Constructeur de la classe Programmation
     *
     * \param da : date de la programmation
     * \param du : durée de la programmation
     * \param e : événement programmé
     *
     */
    Programmation(const QDateTime& da, const Duree& du, Evenement* e):date(da), duree(du), evenement(e){}

    /*!
     * \brief Constructeur
     *
     * Destructeur de la classe Programmation
     *
     */
    ~Programmation();

    /*!
     * \brief Getter sur date de début de la programmation
     * \return date de début
     */
    const QDateTime& getDate(){ return date;}

    /*!
     * \brief Getter sur date de fin de la programmation
     * \return date de fin
     */
    const QDateTime getDateFin(){return date+duree;}

    /*!
     * \brief Setter sur date de début de la programmation
     * \param d : date de début
     */
    void setDate(const QDateTime& d);

    /*!
     * \brief Getter sur la durée de la programmation
     * \return durée
     */
    const Duree& getDuree(){return duree;}

    /*!
     * \brief Setter sur la durée de la programmation
     * \param d : durée
     */
    void setDuree(const Duree& d);

    /*!
     * \brief Getter sur l'événement programmé
     * \return événement
     */
    Evenement& getEvenement(){return *evenement;}

    /*!
     * \brief Setter sur l'événement programmé
     * \param événement
     */
    void setEvenement(Evenement& e);

    /*!
     * \brief Méthode pour stocker une programmation en XML
     * \param document
     * \param element
     */
    QString ProgrammationToXML(QDomDocument& doc, QDomElement& elem);
};

#endif // PROGRAMMATION_H
