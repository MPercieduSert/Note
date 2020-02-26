#include "ConfigNote.h"

using namespace noteMPS;
using namespace fichierMPS;

bool ConfigNote::creer(){
    XmlDoc doc;
    auto iter = doc.begin();
    XmlElement elt;
    elt.setName("conf");
    *iter = elt;
    elt.setName("directories");
    iter = doc.push_back(iter, elt);
    elt.setName("default");
    elt.setText(QDir::homePath()+"/note/");
    iter = doc.push_back(iter, elt);
    iter.toParent(2);
    elt.setName("files");
    elt.setText("");
    iter = doc.push_back(iter, elt);
    elt.setName("databases");
    doc.push_back(iter, elt);

    writeConf(doc);
    return true;
}

//! Renvoie le chemin du dossier par default.
QString ConfigNote::defaultDirectory()
    {return getVars(DEFAULT_DIRECTORY);}
