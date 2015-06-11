#include "projetsmanager.h"

Handler<ProjetsManager> ProjetsManager::handler=Handler<ProjetsManager>();

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

void ProjetsManager::ajouterProjet(const QString& nom, const QDate& debut, const QDate& fin, const QColor& coul)
{
    TacheManager* tmp=trouverProjet(nom);
    if( tmp )
        throw CalendarException("Le nom "+nom+" est déjà pris pour un autre projet");

    TacheManager* tm = new TacheManager(nom, debut, fin, coul);
    projets.push_back(tm);
}

void ProjetsManager::retirerProjet(const QString& nom)
{
    TacheManager* tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException("Le projet "+nom+" n'existe pas");
    projets.remove(tm);
    delete tm;  
    QFile::remove("Projets\\"+nom+".xml");
}

void ProjetsManager::setNom(const QString nom, const QString& nouveau)
{
    TacheManager* tm=trouverProjet(nouveau);
    if( tm )
        throw CalendarException("Le nom "+nouveau+" est déjà utilisé pour un autre projet");
    tm=trouverProjet(nom);
    if( !tm )
        throw CalendarException(nom+" n'existe pas encore");

    tm->setNom(nouveau);
    QFile::rename("Projets\\"+nom+".xml", "Projets\\"+nouveau+".xml");
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
    if(debut > fin) throw CalendarException("La date de début dépasse la date de fin");
    setDebut(nom, debut);
    setFin(nom, fin);
}


void ProjetsManager::writeXML(const QString& dossier)
{
    QDir dir;
    dir.mkpath(dossier);
    dir.setPath(dossier);

    std::list<TacheManager*>::iterator it;
    for ( it = projets.begin(); it != projets.end(); ++it)
    {
        QDomDocument doc = (*it)->projetToXML();
        QFile file( dir.path()+"\\"+(*it)->getNom()+".xml" );
        file.open(QIODevice::WriteOnly);
        QTextStream ts(&file);
        ts<<doc.toString();
        file.close();
    }
}

void ProjetsManager::readXML(QFile& file)
{

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
    int r=255, g=255, b=255;
    QColor couleur=QColor(r, g, b);

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
            if ( proprietes.isElement() && proprietes.toElement().tagName() == "couleur" )
            {
                if( proprietes.toElement().hasAttribute("r") ) r= proprietes.toElement().attribute("r").toInt();
                if( proprietes.toElement().hasAttribute("g") ) g= proprietes.toElement().attribute("g").toInt();
                if( proprietes.toElement().hasAttribute("b") ) b= proprietes.toElement().attribute("b").toInt();
                couleur.setRgb(r,g,b);
            }
        }
        try
        {
            ajouterProjet(nomProjet,debut, fin, couleur);
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
         // Duree restant = Duree();

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

                        dur.setNbJour( e.toElement().attribute("jours").toInt() );
                        dur.setHeure( e.toElement().attribute("heures").toInt() );
                        dur.setMinute( e.toElement().attribute("minutes").toInt() );
                    }
                }

                if( type == "unaire" )
                    getProjet(nomProjet).ajouterTacheUnaire(titre,description,dispo, echeance,dur, preemp);

                if( type == "composite" )
                    getProjet(nomProjet).ajouterTacheComposite(titre,description,dispo, echeance);
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
