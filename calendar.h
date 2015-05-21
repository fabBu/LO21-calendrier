#ifndef CALENDAR_H
#define CALENDAR_H
#include <QString>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTextStream>
#include <iostream>
#include <iomanip>

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
    public:
        //! Constructeur à partir de heure et minute
        /*! \param h heure avec h>=0
            \param m minute avec 0<=m<=59
            */
        Duree(unsigned int h, unsigned int m):nb_minutes(h*60+m) {if (m>59) throw TimeException("erreur: initialisation duree invalide");}
        //! Constructeur à partir de minute
        /*! \param m minute avec m>=0
            */
        Duree(unsigned int m=0):nb_minutes(m) {}
        void setDuree(unsigned int heures, unsigned int minutes) { if (minutes>59) throw TimeException("erreur: initialisation duree invalide"); nb_minutes=heures*60+minutes; }

        unsigned int getDureeEnMinutes() const { return nb_minutes; } //<!Retourne la duree en minutes
        double getDureeEnHeures() const { return double(nb_minutes)/60; } //<!Retourne la duree en heures
        void afficher(std::ostream& f=std::cout) const { f<<std::setfill('0')<<std::setw(2)<<nb_minutes/60<<"H"<<std::setw(2)<<nb_minutes%60<<std::setfill(' '); } //<!Affiche la duree sous le format hhHmm
    private:
        unsigned int nb_minutes;
    };

    /*! \class Horaire
            \brief Classe permettant de manipuler des horaires
            L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
            Déclenchement d'exception dans le cas contraire
    */
    class Horaire{
    public:
        //! Constructeur à partir de heure et minute
        /*! \param h heure avec 0<=h<=23
            \param m minute avec 0<=m<=59
            */
        Horaire(unsigned short int  h, unsigned short int  m):heure(h),minute(m) {if (h>23||m>59) throw TimeException("erreur: initialisation horaire invalide");}
        void setHoraire(unsigned short int h, unsigned short int m) { if (h>23||m>59) throw TimeException("erreur: initialisation horaire invalide"); heure=h; minute=m; }
        void afficher(std::ostream& f=std::cout) const { f<<std::setfill('0')<<std::setw(2)<<heure<<"H"<<std::setfill('0')<<std::setw(2)<<minute<<std::setfill(' '); } //<!Affiche l'horaire sous le format hhHmm
        unsigned short int getHeure() const { return heure; } //<!Retourne l'heure de l'horaire
        unsigned short int getMinute() const { return minute; } //<!Retourne les minutes de l'horaire
        bool operator<(const Horaire& h) const; //<! h1<h2 retourne true si h1 est avant h2 dans le temps
    private:
        unsigned short int  heure;
        unsigned short int  minute;
    };


    /*! \class Intervalle
            \brief Classe permettant de manipuler des intervalles de dates
            L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
            Déclenchement d'exception dans le cas contraire
    */
    class Intervalle{
        public:
            //! Constructeur à partir de deux dates
            /*! \param d date de début de l'intervalle
                \param f date de fin de l'intervalle. On doit avoir d<=f
                */
            Intervalle(const QDate & d, const QDate & f);
            void afficher(std::ostream& f=std::cout) const; //<! Affiche l'intervalle de dates
            QDate getDebut() const { return debut; } //<! Retourne la date de début de l'intervalle
            QDate getFin() const { return fin; } //<! Retourne la date de fin de l'intervalle
            int getDuree() const {
                int annee = fin.year() - debut.year();
                if (annee != 0){
                    if (fin.daysInYear() < debut.daysInYear()){
                        annee--;
                    }
                }
               // int nbjours = (debut.isLeapYear()) ? 365 : 366;
                return fin.daysInYear()-debut.daysInYear()+annee*365;
            } //<! Retourne le nombre de jours s'écoulant entre le début et la fin de l'intervalle
            bool operator&&(const Intervalle & v) const; //<! I1&&I2 Retourne vrai si il y a intersection entre I1 et I2
            Intervalle operator + (const Intervalle & i) const; //<! I1+I2 Retourne un intervalle union des 2 intervalles I1 et I2 qui se touchent, ie I2.debut est le jour du lendemain de I1.fin
        private:
            QDate debut;
            QDate fin;
        };


std::ostream& operator<<(std::ostream& f, const Duree & d);
std::ostream& operator<<(std::ostream& f, const Horaire & h);
std::ostream& operator<<(std::ostream& f, const Intervalle & p);

std::istream& operator>>(std::istream&, Duree&); //lecture format hhHmm

//Horaire& operator+(const Horaire& h, const Duree& d);
//QDate& operator+(const QDate& h, const Duree& d);
QDateTime& operator+(const QDateTime& da, const QTime& du);

#endif // CALENDAR_H
