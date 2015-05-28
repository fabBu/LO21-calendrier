#include <iomanip>
#include <ctime>
#include "calendar.h"

std::ostream& operator<<(std::ostream& f, const Duree & d){ d.afficher(f); return f; }


std::istream& operator>>(std::istream& flot, Duree& duree){
    unsigned int h,m;
    bool ok=true;
    flot>>h;
    if (flot) while (flot.peek()==' ') flot.ignore(); // passe les espaces
    else ok=false;

    if (!flot) ok=false;

    if(flot.peek()=='H') {
        flot.ignore();
        flot>>m;
        if (!flot) ok=false;
    }
    else {
        ok=false;
    }
    if (ok) duree=Duree(h,m); else flot.clear(std::ios::failbit);
    return flot;
}

//Horaire& operator+(const Horaire& h, const Duree& d){
//    int minutes = h.getMinute() + d.getDureeEnMinutes();
//    int heures = h.getHeure();

//    if (minutes > 60 ) {
//        heures += minutes/60;
//        minutes = minutes%60;
//        if (heures > 23) {
//            heures = heures % 24;
//        }
//    }

//    Horaire& res = Horaire(heures, minutes);
//    return res;
//}

//QDate& operator+(const QDate& h, const Duree& d){

//}

QDateTime& operator+(const QDateTime& da, const Duree& du){
    QTime t = QTime(da.time().hour(),da.time().minute());
    QDate d = QDate(da.date().year(), da.date().month(), da.date().day());
    QDateTime ti(d.addDays(du.getNbJour()), t);
    QDateTime tmp = ti.addSecs(du.getDureeEnSeconde());

    return tmp;

//    d.addDays(du.getNbJour());
//    int jour = 0;
//    if ((da.time().hour()+du.getHeure()) > 24 )
//        jour = (da.time().hour()+du.getHeure()) / 24;
//    QDateTime ti(d.addDays(jours), t);
//    QDateTime& res = ti;
//    return res;
}
