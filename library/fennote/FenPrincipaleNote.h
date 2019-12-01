/*Auteur: PERCIE DU SERT Maxime
 *Date: 20/08/2019
 */
#ifndef FENPRINCIPALENOTE_H
#define FENPRINCIPALENOTE_H

#include "NoyauNote.h"
#include "ZoneCentraleNote.h"
#include "FenPrincipale.h"

namespace noteMPS {
/*! \ingroup groupeFenNote
 * \brief Fenêtre principale de l'application note.
 *
 * Fenêtre principale de l'application note:
 *  + Crée les différents menus.
 *
 */
class FenPrincipaleNote : public fmps::FenPrincipale {
public:
    //! Constructeur.
    FenPrincipaleNote(NoyauNote * noyau = new NoyauNote,
                      std::unique_ptr<BddNote>  bdd = std::make_unique<BddNote>(),
                      ZoneCentraleNote * centralZone = new ZoneCentraleNote,
                      const QString &bddPathXML = DATA_BASE,
                      const QString & configPath = QDir::currentPath().append("/Config.xml"),
                      QWidget *parent = nullptr);
};
}
#endif // FENPRINCIPALENOTE_H
