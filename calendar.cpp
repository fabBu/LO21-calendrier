#include <iomanip>
#include <ctime>
#include "calendar.h"

void Duree::writeXmlAttributes(QDomElement elem)
{
    elem.setAttribute("jours", getNbJour());
    elem.setAttribute("heures", getHeure());
    elem.setAttribute("minutes", getMinute());
}



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

QDateTime operator+(const QDateTime& da, const Duree& du){
    QTime t = QTime(da.time().hour(),da.time().minute());
    QDate d = QDate(da.date().year(), da.date().month(), da.date().day());
    QDateTime ti(d.addDays(du.getNbJour()), t);
    QDateTime tmp = ti.addSecs(du.getDureeEnSeconde());
    return tmp;
}

Duree operator+(const Duree& d1, const Duree& d2)
{
    unsigned int nb_Jour=d1.getNbJour()+d2.getNbJour();
    unsigned int h=d1.getHeure()+d2.getHeure()+(nb_Jour*24);
    unsigned int m=d1.getMinute()+d2.getMinute();

    if(m>59)
    {
        h+=m/60;
        m=m%60;
    }

    Duree res(h, m);
    return res;
}
