#ifndef PROGRAMMATION_H
#define PROGRAMMATION_H

#include <QDate>
#include "evenement.h"
#include "calendar.h"

class Programmation
{
private:
    QDate date;
    Horaire horaire;
    Duree duree;
    const Evenement* evenement;

public:
    Programmation(const QDate& da, const Horaire& h, const Duree& du, const Evenement& e):date(da), horaire(h), duree(du), evenement(&e){}

    ~Programmation();

    const QDate& getDate(){ return date;}

    void setDate(const QDate& d);

    const Horaire& getHoraire(){return horaire;}

    void setHoraire(const Horaire& h);

    const Duree& getDuree(){return duree;}

    void setDuree(const Duree& d);

    const Evenement& getEvenement(){return *evenement;}

    void setEvenement(const Evenement& e);
};

#endif // PROGRAMMATION_H
