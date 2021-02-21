#include "ConfigNote.h"

using namespace noteMPS;
using namespace fichierMPS;

bool configNote::creer(){
    doc_xml doc;
    auto iter = doc.begin();
    element_xml elt;
    elt.set_name("conf");
    *iter = elt;
    elt.set_name("directories");
    iter = doc.push_back(iter, elt);
    elt.set_name("default");
    elt.set_text(QDir::homePath()+"/note/");
    iter = doc.push_back(iter, elt);
    iter.to_parent(2);
    elt.set_name("files");
    elt.set_text("");
    iter = doc.push_back(iter, elt);
    elt.set_name("databases");
    doc.push_back(iter, elt);

    write(doc);
    return true;
}

//! Renvoie le chemin du dossier par default.
QString configNote::default_directory()
    {return get_vars(DEFAULT_DIRECTORY);}
