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
 */
class Evenement : public QObject
{
    Q_OBJECT
protected:

    QString titre; /*!< Nom de l'événement*/
    QString description; /*!< Description de l'événement*/
public:
    /*!
     * \brief Constructeur de la classe Evenement
     * \param t : \a titre
     * \param d : \a description
     */
    Evenement(const QString& t, const QString& d):titre(t), description(d) {
        if( titre == "") throw CalendarException("Tout événement doit posséder un nom !");
    }

    /*!
     * \brief Getter sur \a titre
     * \return \a titre
     */
    const QString& getTitre() const { return titre;}

    /*!
     * \brief Setter sur \a titre
     * \param t : \a titre
     */
    void setTitre(const QString& t);

    /*!
     * \brief Getter sur \a description
     * \return \a description
     */
    const QString& getDescription() const { return description;}

    /*!
     * \brief Setter sur \a description
     * \param d : \a description
     */
    void setDescription(const QString& d);

    /*!
     * \brief Affichage en mode console
     */
    virtual void affiche()=0;

signals:
    /*!
     * \brief Signal de suppression de l'événement
     * \param t : Pointeur sur l'événement
     */
     void estSupprime(Evenement* t);
};

#endif // EVENEMENT_H
