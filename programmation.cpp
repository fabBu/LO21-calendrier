#include "programmation.h"

void Programmation::setDate(const QDateTime& d){
    this->date = d;
}

void Programmation::setDuree(const Duree& d){
    this->duree = d;
}

void Programmation::setEvenement(Evenement& e){
    this->evenement = &e;
}

Programmation::~Programmation(){

    Activite* ac = dynamic_cast<Activite*>(evenement);
    if (ac){
        delete evenement;
    }
}



QString Programmation::ProgrammationToXML(QDomDocument& doc, QDomElement& elem)
{
    QDomElement programmationElement = addXmlElement( doc, elem, "programmation" );

    Activite* ac = dynamic_cast<Activite*>(evenement);
    Tache* t = dynamic_cast<Tache*>(evenement);

    QDomElement sousElement;
    if(ac)
    {
       sousElement = addXmlElement(doc, programmationElement, "activite");
       ac->ActiviteToXML(doc, sousElement);
       addXmlElement(doc, sousElement, "date", date.toString());
       QDomElement dureeElement = addXmlElement( doc, sousElement, "duree" );
       duree.writeXmlAttributes(dureeElement);
    }
    else if(t)
    {
        sousElement = addXmlElement(doc, programmationElement, "tache");
        addXmlElement(doc, sousElement, "projet", t->getParent()->getNom());
        addXmlElement(doc, sousElement, "titre", t->getTitre());
        addXmlElement(doc, sousElement, "date", date.toString());
        QDomElement dureeElement = addXmlElement( doc, sousElement, "duree" );
        duree.writeXmlAttributes(dureeElement);
    }
    else
    {
        qDebug()<<"Erreur: ProgrammationToXML";
    }



    return doc.toString();
}
