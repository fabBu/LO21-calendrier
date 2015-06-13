#include <QDir>
#include <QStringList>
#include <QXmlStreamReader>
#include <QDebug>
//#include <dirent.h>


#include "xmlparsor.h"



/* Helper function to generate a DOM Element for the given DOM document

   and append it to the children of the given node. */

QDomElement addXmlElement( QDomDocument &doc, QDomNode &node,
                        const QString &tag,
                        const QString &value )
{

  QDomElement el = doc.createElement( tag );
  node.appendChild( el );

  if ( !value.isNull() ) {
    QDomText txt = doc.createTextNode( value );
    el.appendChild( txt );
  }

  return el;
}
