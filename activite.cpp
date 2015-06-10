#include "activite.h"

void Activite::setType(const MetaEnum::Type& t){
    this->type = t;
}

void Activite::setLieu(const QString& l){
    this->lieu = l;
}

void Activite::affiche(){
    Evenement::affiche();
    std::cout<< "type: " << type << std::endl;
    std::cout << "lieu: " << lieu.toStdString() << std::endl;
}

QString MetaEnum::typeToString(MetaEnum::Type t){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(t);
}

MetaEnum::Type MetaEnum::stringToType(const QString& type)
{
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    int value = metaEnum.keyToValue(type.toStdString().c_str());
    Type ty = static_cast<Type>(value);
    return ty;
}

int MetaEnum::getNbEnum(){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.keyCount();
}

QString MetaEnum::typeToString(int j){
    int index = metaObject()->indexOfEnumerator("Type");
    QMetaEnum metaEnum = metaObject()->enumerator(index);
    return metaEnum.valueToKey(j);
}

QString Activite::ActiviteToXML(QDomDocument& doc, QDomElement& elem)
{
    addXmlElement( doc, elem, "titre", titre );
    addXmlElement( doc, elem, "description", description );
    MetaEnum *m = new MetaEnum();
    addXmlElement( doc, elem, "type", m->typeToString(type) );
    addXmlElement( doc,  elem, "lieu", lieu );

    return doc.toString();
}
