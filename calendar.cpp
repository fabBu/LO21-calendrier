#include <iomanip>
#include <ctime>
#include "calendar.h"

bool Horaire::operator<(const Horaire& h) const{
    if (heure<h.heure) return true;
    if (heure>h.heure) return false;
    if (minute<h.minute) return true;
    if (minute>h.minute) return false;
    return true;
}


std::ostream& operator<<(std::ostream& f, const Horaire& h){ h.afficher(f); return f; }
std::ostream& operator<<(std::ostream& f, const Intervalle& x){ x.afficher(f); return f;}
std::ostream& operator<<(std::ostream& f, const Duree & d){ d.afficher(f); return f; }


Intervalle::Intervalle(const QDate & d, const QDate & f):debut(d),fin(f){
    if (fin<debut) throw TimeException("Erreur dans la creation d'un intervalle: fin<debut");
}

bool Intervalle::operator&&(const Intervalle & v) const {
    if (debut<v.debut){
        if (fin<v.debut) return false; else return true;
    }
    if (v.fin<fin){
        if (v.fin<debut) return false; else return true;
    }
    return true;
}

Intervalle Intervalle::operator+(const Intervalle & i) const {
    QDate d=fin.addDays(1);
    fin.addDays(-1);
    if (d==i.debut){
        return Intervalle(debut,i.fin);
    }else throw TimeException("Ne peut pas faire l'union de 2 intervalles (ils ne se touchent pas...");
}

void Intervalle::afficher(std::ostream& f) const {
    f<<"["<<debut.toString("yyyy.MM.dd").toStdString()<<" ; "<<fin.toString("yyyy.MM.dd").toStdString()<<"]";
}


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

QDateTime& operator+(const QDateTime& da, const QTime& du){
    QTime t = QTime(da.time().hour() + du.hour(),da.time().minute() + du.minute());
    QDate d = QDate(da.date().year(), da.date().month(), da.date().day());
    int jours = 0;
    if ((da.time().hour()+du.hour()) > 24 )
        jours = (da.time().hour()+du.hour()) / 24;
    QDateTime& res = QDateTime(d.addDays(jours), t);
    return res;
}
