#include "projetsmanager.h"

ProjetsManager::Handler ProjetsManager::handler=ProjetsManager::Handler();

ProjetsManager& ProjetsManager::getInstance()
{
    if (!handler.instance)
        handler.instance = new ProjetsManager();

    return *handler.instance;
}

void ProjetsManager::libererInstance()
{
    if (handler.instance)
        delete handler.instance;

    handler.instance = 0;
}

TacheManager* ProjetsManager::trouverProjet(const QString& nom)
{
    for (std::list<TacheManager*>::const_iterator it = projets.begin(); it != projets.end(); it++)
    {
        if( (*it)->getNom() == nom )
            return *it;
    }
    return 0;
}
TacheManager& ProjetsManager::getProjet(const QString& nom){
    TacheManager* tm=trouverProjet(nom);
    if (!tm) throw CalendarException("Le projet "+nom+" n'existe pas");
    return *tm;
}

const TacheManager& ProjetsManager::getProjet(const QString& nom)const{
    TacheManager& tm = const_cast<ProjetsManager*>(this)->getProjet(nom);
    if(!&tm) throw CalendarException("Le projet "+nom+" n'existe pas");
    return tm;
}

void ProjetsManager::ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin)
{
    TacheManager* tmp=trouverProjet(nom);
    if( tmp )
        throw CalendarException("Le nom "+nom+" est déjà pris pour un autre projet");

    TacheManager* tm = new TacheManager(nom, debut, fin);
    projets.push_back(tm);
}

void ProjetsManager::retirerProjet(const QString& nom)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    projets.remove(tm);
}

void ProjetsManager::setNom(const QString& nom, const QString& nouveau)
{
    TacheManager* tm=trouverProjet(nouveau);
    if( tm )
        throw CalendarException("Le nom "+nouveau+" est déjà utilisé pour un autre projet");
    tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException(nom+" n'existe pas encore");

    tm->setNom(nouveau);
}

void ProjetsManager::setDebut(const QString &nom, const QDate &debut)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    tm->setDebut(debut);
}

void ProjetsManager::setFin(const QString &nom, const QDate &fin)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    tm->setFin(fin);
}

void ProjetsManager::setDates(const QString& nom, const QDate &debut, const QDate &fin)
{
    setDebut(nom, debut);
    setFin(nom, fin);
}


void ProjetsManager::writeXML()
{
    std::list<TacheManager*>::iterator it;
    for ( it = projets.begin(); it != projets.end(); ++it)
    {
        QDomDocument doc = (*it)->projetToXML();

        qDebug()<<doc.toString();
        QFile file( (*it)->getNom()+".xml" );
        file.open(QIODevice::WriteOnly);
        QTextStream ts(&file);
        ts<<doc.toString();
        file.close();
    }
}

void ProjetsManager::readXML()
{
    QString fileName = "Projet1.xml";
    QFile file(fileName);
    //Ouverture du fichier en lecture seul et en mode texte
    file.open(QFile::ReadOnly | QFile::Text);

    QDomDocument doc;
    //Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir coompte des namspaces
    doc.setContent(&file, false);

    //ici racine pointe sur l'element <root> de notre document
    QDomElement racine = doc.documentElement();

    //ici racine pointe sur une fils de <root> c'est à dire <site>
    racine = racine.firstChildElement();

    QString nomProjet;
    QDate debut;
    QDate fin;
    //Boucle permettant la navigation dans le fichier XML
    while(!racine.isNull())
    {
        //Si on pointe sur un element de type <nom>
        if(racine.tagName() == "nom")
        {
            //On recupère le text contenu entre les balise <nom> </nom>
            nomProjet = racine.text();
        }
        //Si on pointe sur un element de type <debut>
        if(racine.tagName() == "debut")
        {
            //On recupère la date depuis le texte
            debut = QDate::fromString( racine.text(), "dd-MM-yyyy" );
        }
        //Si on pointe sur un element de type <fin>
        if(racine.tagName() == "fin")
        {
            //On recupère la date depuis le texte
            fin = QDate::fromString( racine.text(), "dd-MM-yyyy" );
        }

        if(racine.tagName() == "taches")
        {
            ajouterProjet(nomProjet, debut, fin);

            QString titre;
            //On recupere le première enfant de l'element site c'est a dire <tache>
            QDomElement unElement = racine.firstChildElement();

            //On parcours tous les enfants de l'element <tache>
            while(!unElement.isNull())
            {
                //Si l'enfant de l'element tache est l'element <titre>
                if(unElement.tagName() == "titre")
                {
                    //On recupère le text contenu entre les balise <nom> </nom>
                    QString strNom = unElement.text();
                }
                //Si l'enfant de l'element <site> est <url>
                else if(unElement.tagName() == "url")
                {
                    //On recupère le text contenu entre les balise <url> </url>
                    QString strURL = unElement.text();
                }
                //Permet d'aller au prochaine enfant de <site> et de poursuivre la boucle
                unElement = unElement.nextSiblingElement();
            }
        }

        //On va a l'element fils de <root> suivant
        racine = racine.nextSiblingElement();
}
}
