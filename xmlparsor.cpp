#include <QDir>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDebug>
//#include <dirent.h>


#include "xmlparsor.h"




void XMLparsor::loadProjets(list<TacheManager*> l)
{
    /*TacheManager* projet;

    QString chemin = "./Projets/";

    //DIR* dossier = opendir(chemin.toStdString().c_str());
    struct dirent* courant;

    //if( dossier==NULL)
    //    throw CalendarException("Le dossier \'Projets\' n\'existe pas");

    while( courant=readdir(dossier) )
    {
        // Ignorer les fichiers cachés
        if( courant->d_name[0] == '.' )
            continue;

        if( string(courant->d_name).find("chargement.xml") != std::string::npos )
        {
 qDebug()<<"COURANT="<<courant->d_name<<"\n";
            projet = loadProjet(chemin+courant->d_name);
            l.push_back(projet);
            std::cout<<"TacheMan="<<projet->getNom().toStdString();
        }

        if( string(courant->d_name).find("contraintes.xml") != std::string::npos )
        {
 qDebug()<<"COURANT="<<courant->d_name<<"\n";
            loadContraintes(projet, chemin+courant->d_name);
        }
    }


/*
    QDirIterator dossier("./Projets", QStringList() << "*.xml", QDir::Files);
    QStringList list = dossier.entryList();

    QDir dossier("./Projets");


    foreach (QDir dir, dossier.entryList(QDir::AllDirs)) {

        TacheManager* projet;
        QStringList filters;
        filters << "*.xml";

        foreach ( QFileInfo file, dir.entryList(filters, QDir::Files) )
        {
qDebug()<<file.fileName();
            if( file.fileName().contains(QRegExp("*projet*")) ) {
                projet = loadProjet(file.filePath());
                l.push_back(projet);
            }
            if( file.fileName().contains(QRegExp("*contrainte*")) )
                loadContraintes(projet, file.filePath());
        }
    }*/
}

 TacheManager* XMLparsor::loadProjet(const QString& file)
{
 /*   TacheManager* proj;

    QFile fin(file);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CalendarException("Erreur ouverture fichier tâches");
    }



    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "projet" )
            {
                qDebug()<<"Nouveau projet\n";
                QString nom;

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "projet")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        if(xml.name() == "nom") {
                            xml.readNext(); nom=xml.text().toString();
                            qDebug()<<"id="<<nom<<"\n";
                        }
                    }
                }
                proj = new TacheManager(nom);
            }

            // If it's named taches, we'll go to the next.
            if(xml.name() == "taches") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "tacheUnaire") {
                //qDebug()<<"new tacheUnaire\n";
                QString titre;
                QString description;
                QString lieu;
                QDate disponibilite;
                QDate echeance;
                Duree duree;
                bool preemptive;

                QXmlStreamAttributes attributes = xml.attributes();*/
                /* Let's check that Task has attribute. */
                /*if(attributes.hasAttribute("preemptive")) {
                    QString val =attributes.value("preemptive").toString();
                    preemptive=(val == "true" ? true : false);
                }
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tacheUnaire")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found titre.
                        if(xml.name() == "titre") {
                            xml.readNext(); titre=xml.text().toString();
                            //qDebug()<<"titre="<<titre<<"\n";
                        }
                        // We've found description.
                        if(xml.name() == "description") {
                            xml.readNext(); description=xml.text().toString();
                            //qDebug()<<"description="<<description<<"\n";
                        }

                        // We've found disponibilite
                        if(xml.name() == "disponibilite") {
                            xml.readNext();
                            disponibilite=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"disp="<<disponibilite.toString()<<"\n";
                        }
                        // We've found echeance
                        if(xml.name() == "echeance") {
                            xml.readNext();
                            echeance=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"echeance="<<echeance.toString()<<"\n";
                        }
                        // We've found duree
                        if(xml.name() == "duree") {
                            xml.readNext();  duree = Duree(xml.text().toString().toUInt());
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
                //qDebug()<<"ajout tache "<<identificateur<<"\n";
                proj->ajouterTacheUnaire(titre,description,disponibilite,echeance,duree,preemptive);
            }
        }
    }*/return 0;
}

void XMLparsor::loadContraintes(TacheManager* tm, QString chemin)
{
/*
    QFile fin(chemin);
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CalendarException("Erreur ouverture fichier contraintes");
    }
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()) {
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {

            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "predecesseurs") {
                //qDebug()<<"new tacheUnaire\n";
                QString tache;
                QString pred;

                QXmlStreamAttributes attributes = xml.attributes();*/
                /* Let's check that Task has attribute. */
                /*if(attributes.hasAttribute("tache")) {
                    tache =attributes.value("tache").toString();
                }
                else
                    continue;
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.

                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "predecesseurs")) {
                    if(xml.tokenType() == QXmlStreamReader::StartElement) {
                        // We've found titre.
                        if(xml.name() == "predecesseur") {
                            xml.readNext(); pred=xml.text().toString();

                            Tache& t = tm->getTache(tache);
                            Tache& p = tm->getTache(pred);
                            t.ajouterPredecesseur(p);
                            //qDebug()<<"titre="<<titre<<"\n";
                        }
                    }
                    // ...and next...
                    xml.readNext();
                }
            }
        }
    }*/
}
