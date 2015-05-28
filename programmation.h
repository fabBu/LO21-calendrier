#ifndef PROGRAMMATION_H
#define PROGRAMMATION_H

#include <QDateTime>
#include "evenement.h"
#include "activite.h"
#include "calendar.h"

class Programmation
{
private:
    QDateTime date;
    Duree duree;
    Evenement* evenement;

public:
    Programmation(const QDateTime& da, const Duree& du, Evenement& e):date(da), duree(du), evenement(&e){}

    ~Programmation();

    const QDateTime& getDate(){ return date;}

    void setDate(const QDateTime& d);

    const Duree& getDuree(){return duree;}

    void setDuree(const Duree& d);

    Evenement& getEvenement(){return *evenement;}

    void setEvenement(Evenement& e);
};

#endif // PROGRAMMATION_H
