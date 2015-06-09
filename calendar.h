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

class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};


/*! \class TimeException
    \brief Classe permettant de gérer les exceptions des classes du namespace TIME
*/
class TimeException{
public:
    //! Constructeur à partir d'une string
    TimeException(const std::string& m):info(m){}
    const std::string& GetInfo() const { return info; } //<! Retourne l'information stockée dans la classe
private:
    std::string info;
};

/*! \class Duree
        \brief Classe permettant de manipuler des durees
        L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
        Déclenchement d'exception dans le cas contraire
*/
class Duree{
    unsigned int nb_jour;
    QTime time;

public:
    Duree(){nb_jour=0; time=QTime(0,0);}
    Duree(int h, int m=0):nb_jour(h/24), time(h%24,m) {
        verifCoherence(h,m);
    }

    Duree(const Duree& d) {nb_jour=d.getNbJour(); time= QTime((int)d.getHeure(), (int)d.getMinute()); }

    void setDuree(int h, int m) {
        verifCoherence(h,m);
        nb_jour=h/24;
        time = QTime(h%24,m);
    }

    void verifCoherence(int h, int m) {
        if (m>59) throw TimeException("ERREUR: initialisation duree invalide, m supérieur à 59");
        if (h<0) throw TimeException("ERREUR: initialisation duree invalide, h négatif");
        if (m<0) throw TimeException("ERREUR: initialisation duree invalide, m négatif");
    }

    const QTime& getTime() const{ return time; }
    unsigned int getHeure() const{ return time.hour(); }
    unsigned int getMinute() const{ return time.minute(); }
    unsigned int getNbJour() const{ return nb_jour; }

    void setNbJour(unsigned int j){ nb_jour = j; }

    void setMinute(unsigned int m){
        if (m>59)
            throw TimeException("erreur: les minutes ne peuvent pas être supérieurs à 59");
        time = QTime(time.hour(),m);
    }

    void setHeure(unsigned int h){
        if(h>23)
            throw TimeException("erreur: les heures ne peuvent pas être supérieur à 23");
        time = QTime(h,time.minute());
    }

    unsigned int getDureeEnMinutes() const { return nb_jour*24*60+time.hour()*60+time.minute(); } //<!Retourne la duree en minutes

    double getDureeEnHeures() const {
        double res = nb_jour*24;
        res += time.hour();
        res += double(time.minute())/60;
        return res;
    } //<!Retourne la duree en heures

    unsigned int getDureeEnSeconde() const{
        unsigned int secondes = time.hour()*60*60 + time.minute()*60;
        return secondes;
    }

    void afficher(std::ostream& f=std::cout) const {
        f<<std::setfill('0')<<std::setw(2)<<nb_jour*24+time.hour()<<"H"<<std::setw(2)<<time.minute()<<std::setfill(' '); } //<!Affiche la duree sous le format hhHmm

    void writeXmlAttributes(QDomElement elem);
};

std::ostream& operator<<(std::ostream& f, const Duree & d);

std::istream& operator>>(std::istream&, Duree&); //lecture format hhHmm

QDateTime operator+(const QDateTime& da, const Duree& du);

#endif // CALENDAR_H
