#include "FenPrincipaleNote.h"

using namespace noteMPS;

FenPrincipaleNote::FenPrincipaleNote(NoyauNote * noyau, BddNote *bdd, ZoneCentraleNote * centralZone,
                                   const QString &bddPathXML, const QString & configPath, QWidget *parent)
    : FenPrincipale (noyau,bdd,centralZone,bddPathXML,configPath,parent)
{

}
