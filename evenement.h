#ifndef EVENEMENT_H
#define EVENEMENT_H

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
class Evenement : public QObject
{
    Q_OBJECT
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
    const QString& getTitre() const { return titre;}

    /*!
     * \brief Setter sur titre
     * \param t : titre
     */
    void setTitre(const QString& t);

    /*!
     * \brief Getter sur description
     * \return description
     */
    const QString& getDescription() const { return description;}

    /*!
     * \brief Setter sur description
     * \param d : description
     */
    void setDescription(const QString& d);

    /*!
     * \brief Affichage en mode console
     */
    virtual void affiche()=0;

signals:
    /*!
     * \brief Signal de suppression de l'événement
     * \param t Pointeur sur l'événement
     */
     void estSupprime(Evenement* t);
};

#endif // EVENEMENT_H
