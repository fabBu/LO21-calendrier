#ifndef CALENDAR_H
#define CALENDAR_H
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTextStream>
#include <iostream>
#include <iomanip>

#include "xmlparsor.h"
using namespace std;

/*!
 * \class CalendarException
 * \brief Classe représentant une exception de type CALENDAR
 */
class CalendarException{
public:
    /*!
     * \brief Constructeur de la classe CalendarException
     * \param message : message d'erreur
     */
    CalendarException(const QString& message):info(message){}

    /*!
     * \brief Getter sur le message d'erreur
     * \return \a info : message d'erreur
     */
    QString getInfo() const { return info; }
private:
    QString info; /*!< Message d'erreur*/
};


/*! \class TimeException
    \brief Classe permettant de gérer les exceptions de type TIME
*/
class TimeException{
public:
    /*!
     * \brief Constructeur
     * \param m : message d'erreur
     */
    TimeException(const std::string& m):info(m){}
    /*!
     * \brief Getter sur le message d'erreur
     * \return \a info : message d'erreur
     */
    const std::string& GetInfo() const { return info; }
private:
    std::string info; /*!< Message d'erreur*/
};

/*!
 * \class Duree
 * \brief Classe permettant de manipuler des durees
 * L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
 * Déclenchement d'exception dans le cas contraire
*/
class Duree{
    unsigned int nb_jour; /*!< Nombre de jours*/
    QTime time; /*!< Nombre d'heure restante (sans les jours)*/

public:
    /*!
     * \brief Constructeur par défaut de la classe Durée
     */
    Duree(){nb_jour=0; time=QTime(0,0);}
    /*!
     * \brief Constructeur de la classe Duree
     * \param h : nombre d'heures
     * \param m : nombre de minutes
     */
    Duree(int h, int m=0):nb_jour(h/24), time(h%24,m) {
        verifCoherence(h,m);
    }

    /*!
     * \brief Constructeur par recopie de la classe Durée
     * \param d : durée à recopier
     *
     */
    Duree(const Duree& d) {nb_jour=d.getNbJour(); time= QTime((int)d.getHeure(), (int)d.getMinute()); }

    /*!
     * \brief Setter d'une durée
     * \param h : nombre d'heures
     * \param m : nombre de minutes
     */
    void setDuree(int h, int m) {
        verifCoherence(h,m);
        nb_jour=h/24;
        time = QTime(h%24,m);
    }

    /*!
     * \brief Vérificateur de cohérence pour les minutes et les heures
     * \param h : nombre d'heures
     * \param m : nombre de minutes
     * \throw <TimeException>["Il faut que les minutes soient comprises entre 0 et 59 et les heures doivent être supérieurs à 0."]
     */
    void verifCoherence(int h, int m) {
        if (m>59) throw TimeException("ERREUR: initialisation duree invalide, m supérieur à 59");
        if (h<0) throw TimeException("ERREUR: initialisation duree invalide, h négatif");
        if (m<0) throw TimeException("ERREUR: initialisation duree invalide, m négatif");
    }

    /*!
     * \brief Getter sur le nombre d'heure et minute restante
     * \return \a time
     */
    const QTime& getTime() const{ return time; }
    /*!
     * \brief Getter sur le nombre d'heure restante
     * \return heure
     */
    unsigned int getHeure() const{ return time.hour(); }
    /*!
     * \brief Getter sur le nombre de minute restante
     * \return minute
     */
    unsigned int getMinute() const{ return time.minute(); }
    /*!
     * \brief Getter sur le nombre de jour
     * \return \a nb_jour
     */
    unsigned int getNbJour() const{ return nb_jour; }

    /*!
     * \brief Setter sur le nombre de jour
     * \param j : jours
     */
    void setNbJour(unsigned int j){ nb_jour = j; }

    /*!
     * \brief Setter sur le nombre de minute
     * \param m : minutes
     * \throw <TimeException>["Les minutes ne peuvent pas être supérieurs à 59."]
     */
    void setMinute(unsigned int m){
        if (m>59)
            throw TimeException("ERREUR: les minutes ne peuvent pas être supérieurs à 59");
        time = QTime(time.hour(),m);
    }

    /*!
     * \brief Setter sur le nombre d'heure
     * \param h : heures
     * \throw <TimeException>["Les heures ne peuvent pas être supérieurs à 23."]
     */
    void setHeure(unsigned int h){
        if(h>23)
            throw TimeException("ERREUR: les heures ne peuvent pas être supérieur à 23");
        time = QTime(h,time.minute());
    }

    /*!
     * \brief Getter sur la durée en minute
     * \return minutes
     */
    unsigned int getDureeEnMinutes() const { return nb_jour*24*60+time.hour()*60+time.minute(); } //<!Retourne la duree en minutes

    /*!
     * \brief Getter sur la durée en heure
     * \return heures
     */
    double getDureeEnHeures() const {
        double res = nb_jour*24;
        res += time.hour();
        res += double(time.minute())/60;
        return res;
    }

    /*!
     * \brief Getter sur la durée en secondes
     * \return secondes
     */
    unsigned int getDureeEnSeconde() const{
        unsigned int secondes = time.hour()*60*60 + time.minute()*60;
        return secondes;
    }

    /*!
     * \brief Affichage en mode console
     * \param f : ostream
     */
    void afficher(std::ostream& f=std::cout) const {
        f<<std::setfill('0')<<std::setw(2)<<nb_jour*24+time.hour()<<"H"<<std::setw(2)<<time.minute()<<std::setfill(' '); } //<!Affiche la duree sous le format hhHmm

    /*!
     * \brief Création de code XML à partir des informations de durée
     * \param elem Element sous lequel insérer le code
     */
    void writeXmlAttributes(QDomElement elem);
};

std::ostream& operator<<(std::ostream& f, const Duree & d);

std::istream& operator>>(std::istream&, Duree&); //lecture format hhHmm

/*!
 * \brief Opérateur + pour additionner une date et une durée
 * \param da : date et horaire
 * \param du : durée
 * \return la nouvelle date et horaire
 */
QDateTime operator+(const QDateTime& da, const Duree& du);
/*!
 * \brief Opérateur + pour additionner deux durées
 * \param d1 : durée 1
 * \param d2 : durée 2
 * \return la nouvelle durée
 */
Duree operator+(const Duree& d1, const Duree& d2);

#endif // CALENDAR_H
