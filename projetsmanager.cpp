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


void ProjetsManager::writeXML(const QString& dossier)
{
    QDir dir;
    dir.mkpath(dossier);
    dir.setCurrent(dossier);

    std::list<TacheManager*>::iterator it;
    for ( it = projets.begin(); it != projets.end(); ++it)
    {
        QDomDocument doc = (*it)->projetToXML();

        qDebug()<<doc.toString();
        QFile file( dir.path()+"\\"+(*it)->getNom()+".xml" );
        file.open(QIODevice::WriteOnly);
        QTextStream ts(&file);
        ts<<doc.toString();
        file.close();
    }
}

void ProjetsManager::readXML(QFile& file)
{

    //QFile file(dossier+"\\Projet1.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement projetElement = doc.namedItem("projet").toElement();
    if ( projetElement.isNull() ) {
      qWarning() << "No <projet> element found at the top-level "
                 << "of the XML file!";
      return;
    }

    QString nomProjet;
    QDate debut, fin;

    QDomNode nd = projetElement.firstChild();
    for ( ; !nd.isNull(); nd = nd.nextSibling() )
    {
        // Charger le projet lui-même avec ses propriétés
      if ( nd.isElement() && nd.toElement().tagName() == "proprietes" )
      {
        QDomNode proprietes = nd.firstChild();
        for ( ; !proprietes.isNull(); proprietes = proprietes.nextSibling() )
        {
            if ( proprietes.isElement() && proprietes.toElement().tagName() == "nom" )
                nomProjet=proprietes.toElement().text();
            if ( proprietes.isElement() && proprietes.toElement().tagName() == "debut" )
                debut= QDate::fromString(proprietes.toElement().text(), "dd-MM-yyyy");
            if ( proprietes.isElement() && proprietes.toElement().tagName() == "fin" )
                fin= QDate::fromString(proprietes.toElement().text(), "dd-MM-yyyy");
        }
        try
        {
            ajouterProjet(nomProjet,debut, fin);
        }
        catch(CalendarException e)
        { qDebug()<<e.getInfo().toStdString().c_str(); }
      }

      // Charger les tâches du projet
      if ( nd.isElement() && nd.toElement().tagName() == "taches" )
      {
          QString type;
          QString titre;
          QString description;
          QDate dispo, echeance;

          bool preemp, termine;
          Duree dur = Duree();
          Duree restant = Duree();

        QDomNode tache = nd.firstChild();
        for ( ; !tache.isNull(); tache = tache.nextSibling() )
        {
            if ( tache.isElement() && tache.toElement().tagName() == "tache" )
            {
                if( !tache.toElement().hasAttribute("type") ) continue;
                type = tache.toElement().attribute("type");

                QDomNode e = tache.firstChild();
                for ( ; !e.isNull(); e = e.nextSibling() )
                {
                    if ( e.isElement() && e.toElement().tagName() == "titre" )
                        titre=e.toElement().text();
                    if ( e.isElement() && e.toElement().tagName() == "description" )
                        description=e.toElement().text();
                    if ( e.isElement() && e.toElement().tagName() == "disponibilite" )
                        dispo= QDate::fromString(e.toElement().text(), "dd-MM-yyyy");
                    if ( e.isElement() && e.toElement().tagName() == "echeance" )
                        echeance= QDate::fromString(e.toElement().text(), "dd-MM-yyyy");

                    if ( e.isElement() && e.toElement().tagName() == "preemptive" )
                    {
                        if(e.toElement().text()=="oui") preemp=true;
                        else preemp=false;
                    }
                    if ( e.isElement() && e.toElement().tagName() == "termine" )
                    {
                        if(e.toElement().text()=="oui") termine=true;
                        else termine=false;
                    }
                    if ( e.isElement() && e.toElement().tagName() == "duree" )
                    {
                        if(!e.toElement().hasAttribute("jours")
                                || !e.toElement().hasAttribute("heures")
                                || !e.toElement().hasAttribute("minutes"))
                            break;

                        dur.setNbJour( e.toElement().attribute("jours").toUInt() );
                        dur.setHeure( e.toElement().attribute("heures").toUInt() );
                        dur.setMinute( e.toElement().attribute("minutes").toUInt() );
                    }
                    if ( e.isElement() && e.toElement().tagName() == "duree_restante" )
                    {
                        if(!e.toElement().hasAttribute("jours")
                                || !e.toElement().hasAttribute("heures")
                                || !e.toElement().hasAttribute("minutes"))
                            break;

                        restant.setNbJour( e.toElement().attribute("jours").toUInt() );
                        restant.setHeure( e.toElement().attribute("heures").toUInt() );
                        restant.setMinute( e.toElement().attribute("minutes").toUInt() );
                    }
                }

                if( type == "unaire" )
                {qDebug()<<"Ajout Unaire";
                    getProjet(nomProjet).ajouterTacheUnaire(titre,description,dispo, echeance,dur, restant, preemp);
                }

                if( type == "composite" )
                {qDebug()<<"Ajout Compo";
                    getProjet(nomProjet).ajouterTacheComposite(titre,description,dispo, echeance);
                }
            }
        }
      }

      // Charger les contraintes des tâches du projet
      if ( nd.isElement() && nd.toElement().tagName() == "contraintes" )
      {
          QString tache;

        QDomNode contrainte = nd.firstChild();
        for ( ; !contrainte.isNull(); contrainte = contrainte.nextSibling() )
        {
            if ( contrainte.isElement() && contrainte.toElement().tagName() == "contrainte" )
            {
                if( !contrainte.toElement().hasAttribute("tache") ) continue;
                tache = contrainte.toElement().attribute("tache");

                QDomNode e = contrainte.firstChild();
                for ( ; !e.isNull(); e = e.nextSibling() )
                {
                    if ( e.isElement() && e.toElement().tagName() == "composition" )
                    {
                        QDomNode compo = e.firstChild();
                        for ( ; !compo.isNull(); compo = compo.nextSibling() )
                        {
                            if( compo.isElement() && compo.toElement().tagName()=="sous-tache" )
                                getProjet(nomProjet).ajouterSousTache(tache, compo.toElement().text());
                        }
                    }
                    if ( e.isElement() && e.toElement().tagName() == "precedence" )
                    {
                        QDomNode pred = e.firstChild();
                        for ( ; !pred.isNull(); pred = pred.nextSibling() )
                        {
                            if( pred.isElement() && pred.toElement().tagName()=="predecesseur" )
                                getProjet(nomProjet).ajouterPred(tache, pred.toElement().text());
                        }
                    }
                }
            }
        }
      }
    }
}


/*
void ProjetsManager::readXML(const QString &dossier)
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
}*/
