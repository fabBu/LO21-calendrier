#ifndef XMLPARSOR_H
#define XMLPARSOR_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QString>


/*! \class XMLparsor
   * \brief Lecture / Ecriture depuis fichiers XML
   *
   *  Classe permettant de lire et écrire un fichier XML.
   * Elle servira a charger puis sauvegarder l'ensemble des données (agenda et projets)
   *
   */
class XMLparsor
{
private:
    XMLparsor() {}
public:
    //static void loadProjets(list<TacheManager*> l);
    //static TacheManager* loadProjet(const QString& chemin);
    //static void loadContraintes(TacheManager* tm, QString chemin);
};

QDomElement addXmlElement( QDomDocument &doc, QDomNode &node, const QString &tag, const QString &value = QString::null );

#endif // XMLPARSOR_H
