#include "activite.h"

void Activite::setType(const MetaEnum::Type& t){
    this->type = t;
}

void Activite::affiche(){
    Evenement::affiche();
    std::cout<< "type: " << type << std::endl;
    std::cout << "lieu: " << lieu.toStdString() << std::endl;
}

QString MetaEnum::enumToString(MetaEnum::Type t){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(t);
}

int MetaEnum::getNbEnum(){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.keyCount();
}

QString MetaEnum::enumToString(int j){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(j);
}

QString Activite::ActiviteToXML(QDomDocument& doc, QDomElement& elem)
{
    addXmlElement( doc, elem, "titre", titre );
    addXmlElement( doc, elem, "description", description );
    //addXmlElement( doc, elem, "type", type.enumToString(type) );
    addXmlElement( doc,  elem, "lieu", lieu );

    return doc.toString();
}
